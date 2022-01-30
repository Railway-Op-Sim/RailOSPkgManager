#include "ros_packager.hxx"

void ROSPkg::Packager::packageFailure() {
    QMessageBox::critical(parent_, QMessageBox::tr("Package build failure"), QMessageBox::tr("Failed to build package due to missing data."));
}

QString ROSPkg::Packager::buildTOML() {
    QString file_name_ = QString(package_name_);
    file_name_.replace(" ", "_");

    QString metadata_dir_ = ros_loc_ + QDir::separator() + "Metadata";

    if(!QDir(metadata_dir_).exists()) {
        QDir().mkpath(metadata_dir_);
    }

    toml_file_ = metadata_dir_ + QDir::separator() + file_name_ + ".toml";

    if(version_.isEmpty()) {
        const QDate now_ = QDate::currentDate();
        version_ = now_.toString(Qt::ISODate);
    }


    ROSTools::Metadata new_meta_;
    if(!description_.isEmpty()) new_meta_.setDescription(description_.toStdString());
    if(package_name_.isEmpty()) {packageFailure(); return "";}
    if(display_name_.isEmpty()) {
        display_name_ = QString(package_name_);
    }
    new_meta_.setDisplayName(display_name_.toStdString());
    new_meta_.setName(package_name_.toStdString());
    new_meta_.setFactual(factual_);
    new_meta_.setCountryCode(country_code_.toStdString());
    if(rly_file_.isEmpty()) {packageFailure(); return "";}
    if(ttb_files_.empty()) {packageFailure(); return "";}
    if(doc_files_.empty()) {packageFailure(); return "";}
    new_meta_.setAuthor(author_.toStdString());
    new_meta_.setRLYFile(QFileInfo(rly_file_).fileName().toStdString());
    for(const QString& ttb_file : ttb_files_) {
        new_meta_.addTTBFile(QFileInfo(ttb_file).fileName().toStdString());
    }
    for(const QString& doc_file : doc_files_) {
        new_meta_.addDocFile(QFileInfo(doc_file).fileName().toStdString());
    }
    if(!ssn_files_.empty()) {
        for(const QString& ssn_file : ssn_files_) {
            new_meta_.addSSNFile(QFileInfo(ssn_file).fileName().toStdString());
        }
    }
    if(!img_files_.empty()) {
        for(const QString& img_file : img_files_) {
            new_meta_.addImgFile(QFileInfo(img_file).fileName().toStdString());
        }
    }
    if(!graphic_files_.empty()) {
        for(const QString& graphic_file : graphic_files_) {
            new_meta_.addGraphicFile(QFileInfo(graphic_file).fileName().toStdString());
        }
    }
    if(!contributors_.empty()) {
        for(const QString& contributor : contributors_) {
            new_meta_.addContributor(contributor.toStdString());
        }
    }

    try {
        new_meta_.validate();
    } catch(std::runtime_error& e) {
        const QString err_ = QString("Package creation was unsuccessful due to validation failure:\n") + QString(e.what());
        QMessageBox::critical(
            parent_,
            QMessageBox::tr("Package Creation Failed"),
            QMessageBox::tr(err_.toStdString().c_str())
        );
        return "";
    }

    new_meta_.write(toml_file_.toStdString());

    return toml_file_;
}

QString ROSPkg::Packager::createPackage() {
    buildTOML();
    const QString build_dir_ = ros_loc_ + QDir::separator() + "Packages";
    if(!QDir(build_dir_).exists()) {
        QDir().mkpath(build_dir_);
    }
    QString package_file_name_ = package_name_;
    QString package_name_version_ = version_;
    version_.replace(".", "_");
    package_file_name_ += version_;
    package_file_name_.replace(" ", "_");

    const QString out_dir_ = build_dir_ + QDir::separator() + package_file_name_;

    QDir().mkpath(out_dir_);
    QDir().mkpath(out_dir_+ QDir::separator() + "Railway");
    QDir().mkpath(out_dir_+ QDir::separator() + "Program_Timetables");
    QDir().mkpath(out_dir_+ QDir::separator() + "Sessions");
    QDir().mkpath(out_dir_+ QDir::separator() + "Documentation");
    QDir().mkpath(out_dir_+ QDir::separator() + "Images");
    QDir().mkpath(out_dir_+ QDir::separator() + "Metadata");

    const QString out_rly_file_ = out_dir_ +
        QDir::separator() + "Railway" + QDir::separator() + QFileInfo(rly_file_).fileName();
    const QString out_toml_file_ = out_dir_+
        QDir::separator() + "Metadata" + QDir::separator() + QFileInfo(toml_file_).fileName();

    if(QFile(out_rly_file_).exists()) QFile(out_rly_file_).remove();
    if(QFile(out_toml_file_).exists()) QFile(out_toml_file_).remove();

    QFile(rly_file_).copy(out_rly_file_);
    QFile(toml_file_).copy(out_toml_file_);

    for(const QString& ttb_file : ttb_files_) {
        const QString out_ttb_file_ = out_dir_+
            QDir::separator() + "Program_Timetables" +
            QDir::separator() + QFileInfo(ttb_file).fileName();
        if(QFile(out_ttb_file_).exists()) QFile(out_ttb_file_).remove();
        QFile(ttb_file).copy(out_ttb_file_);
    }

    for(const QString& ssn_file : ssn_files_) {
        const QString out_ssn_file_ = out_dir_+
            QDir::separator() + "Sessions" +
            QDir::separator() + QFileInfo(ssn_file).fileName();
        if(QFile(out_ssn_file_).exists()) QFile(out_ssn_file_).remove();
        QFile(ssn_file).copy(out_ssn_file_);
    }

    for(const QString& doc_file : doc_files_) {
        const QString out_doc_file_ = out_dir_+
            QDir::separator() + "Documentation" +
            QDir::separator() + QFileInfo(doc_file).fileName();
        if(QFile(out_doc_file_).exists()) QFile(out_doc_file_).remove();
        QFile(doc_file).copy(out_doc_file_);
    }

    for(const QString& img_file : img_files_) {
        const QString out_img_file_ = out_dir_+
            QDir::separator() + "Images" +
            QDir::separator() + QFileInfo(img_file).fileName();
        if(QFile(out_img_file_).exists()) QFile(out_img_file_).remove();
        QFile(img_file).copy(out_img_file_);
    }

    for(const QString& graphic_file : graphic_files_) {
        const QString out_graphic_file_ = out_dir_+
            QDir::separator() + "Graphics" +
            QDir::separator() + QFileInfo(graphic_file).fileName();
        if(QFile(out_graphic_file_).exists()) QFile(out_graphic_file_).remove();
        QFile(graphic_file).copy(out_graphic_file_);
    }

    const QString zip_file_ = out_dir_ + ".zip";

    elz::zipFolder(out_dir_.toStdString(), zip_file_.toStdString());

    if(!QFile::exists(zip_file_)) {
        QMessageBox::critical(parent_, QMessageBox::tr("Archive creation failure"), QMessageBox::tr("Failed to create package archive"));
        return "";
    }

    QDir(out_dir_).removeRecursively();

    return zip_file_;
}
