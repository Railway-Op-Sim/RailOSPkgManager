#include "ros_packager.hxx"

void ROSPkg::Packager::packageFailure() {
    QMessageBox::critical(parent_, QMessageBox::tr("Package build failure"), QMessageBox::tr("Failed to build package due to missing data."));
}

QString ROSPkg::Packager::buildTOML() {
    QString file_name_ = QString(package_name_);
    file_name_.replace(" ", "_");
    const QString toml_file_ = ros_loc_ + QDir::separator() + "Metadata" + QDir::separator() + file_name_ + ".toml";

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
        stream_ << "display_name = \"" << display_name_ << "\"" << Qt::endl;
        stream_ << "rly_file = \"" << rly_file_ << "\"" << Qt::endl;
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
