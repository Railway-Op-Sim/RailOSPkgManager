#include "ros_packager.hxx"

void ROSPkg::Packager::packageFailure() {
    QMessageBox::critical(parent_, QMessageBox::tr("Package build failure"), QMessageBox::tr("Failed to build package due to missing data."));
}

QString ROSPkg::Packager::buildTOML() {
    QString file_name_ = QString(package_name_);
    file_name_.replace(" ", "_");
    toml_file_ = ros_loc_ + QDir::separator() + "Metadata" + QDir::separator() + file_name_ + ".toml";

    if(version_.isEmpty()) {
        const QDate now_ = QDate::currentDate();
        version_ = now_.toString(Qt::ISODate);
    }

    // Builds a TOML file manually using strings
    QFile file_(toml_file_);
    if(file_.open(QIODevice::WriteOnly)) {
        QTextStream stream_(&file_);
        stream_ << "name = \"" << package_name_  << "\"" << Qt::endl;
        if(!description_.isEmpty()) stream_ << "description = \"" << description_ << "\"" << Qt::endl;
        if(!display_name_.isEmpty()) stream_ << "display_name = \"" << display_name_ << "\"" << Qt::endl;
        if(rly_file_.isEmpty()) {packageFailure(); return "";}
        if(ttb_files_.empty()) {packageFailure(); return "";}
        stream_ << "rly_file = \"" << QFileInfo(rly_file_).fileName() << "\"" << Qt::endl;
        stream_ << "ttb_files = [";
        for(const QString& ttb_file : ttb_files_) {
            stream_ << "\"" << QFileInfo(ttb_file).fileName() << "\",";
        }
        stream_ << "]" << Qt::endl;
        if(!ssn_files_.empty()) {
            stream_ << "ssn_files = [";
            for(const QString& ssn_file : ssn_files_) {
                stream_ << "\"" << QFileInfo(ssn_file).fileName() << "\",";
            }
            stream_ << "]" << Qt::endl;
        }
        stream_ << "doc_files = [";
        if(!doc_files_.empty()) {
            for(const QString& doc_file : doc_files_) {
                stream_ << "\"" << QFileInfo(doc_file).fileName() << "\",";
            }
        }
        stream_ << "]" << Qt::endl;
        if(year_ > 0) stream_ << "year = \"" << year_ << "\"" << Qt::endl;
        if(!factual_.isEmpty()) stream_ << "factual = \"" << factual_ << "\"" << Qt::endl;
        stream_ << "author = \"" << author_ << "\"" << Qt::endl;
        if(!contributors_.empty()) {
            stream_ << "contributors = [";
            for(const QString& contrib : contributors_) {
                stream_ << "\"" << QFileInfo(contrib).fileName() << "\",";
            }
            stream_ << "]" << Qt::endl;
        }
        stream_ << "country_code = \"" << country_code_ << "\"" << Qt::endl;
    }

    return toml_file_;
}

QString ROSPkg::Packager::createPackage() {
    buildTOML();
    const QString build_dir_ = ros_loc_ + QDir::separator() + "Packages";
    if(!QDir(build_dir_).exists()) {
        QDir().mkdir(build_dir_);
    }
    QString package_file_name_ = package_name_;
    QString package_name_version_ = version_;
    version_.replace(".", "_");
    package_file_name_ += version_;
    package_file_name_.replace(" ", "_");

    const QString out_dir_ = build_dir_ + QDir::separator() + package_file_name_;

    QDir().mkdir(out_dir_);
    QDir().mkdir(out_dir_+ QDir::separator() + "Railway");
    QDir().mkdir(out_dir_+ QDir::separator() + "Program_Timetables");
    QDir().mkdir(out_dir_+ QDir::separator() + "Sessions");
    QDir().mkdir(out_dir_+ QDir::separator() + "Documentation");
    QDir().mkdir(out_dir_+ QDir::separator() + "Images");
    QDir().mkdir(out_dir_+ QDir::separator() + "Metadata");

    QFile(rly_file_).copy(
        out_dir_+
        QDir::separator() + "Railway" + QDir::separator() + QFileInfo(rly_file_).fileName()
    );

    QFile(toml_file_).copy(
        out_dir_+
        QDir::separator() + "Metadata" + QDir::separator() + QFileInfo(toml_file_).fileName()
    );

    for(const QString& ttb_file : ttb_files_) {
        QFile(ttb_file).copy(
            out_dir_+
            QDir::separator() + "Program_Timetables" + QDir::separator() + QFileInfo(ttb_file).fileName()
        );
    }

    for(const QString& ssn_file : ssn_files_) {
        QFile(ssn_file).copy(
            out_dir_+
            QDir::separator() + "Sessions" + QDir::separator() + QFileInfo(ssn_file).fileName()
        );
    }

    for(const QString& doc_file : doc_files_) {
        QFile(doc_file).copy(
            out_dir_+
            QDir::separator() + "Documentation" + QDir::separator() + QFileInfo(doc_file).fileName()
        );
    }

    const QString zip_file_ = out_dir_ + ".zip";

    elz::zipFolder(out_dir_.toStdString(), zip_file_.toStdString());

    if(!QFile::exists(zip_file_)) {
        QMessageBox::critical(parent_, QMessageBox::tr("Archive creation failure"), QMessageBox::tr("Failed to create package archive"));
        throw std::runtime_error("");
    }

    QDir(out_dir_).removeRecursively();

    return zip_file_;
}
