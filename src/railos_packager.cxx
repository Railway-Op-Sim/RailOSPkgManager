#include "railos_packager.hxx"

void RailOSPkg::Packager::packageFailure() {
    QMessageBox::critical(parent_, QMessageBox::tr("Package build failure"), QMessageBox::tr("Failed to build package due to missing data."));
}

QString RailOSPkg::Packager::buildTOML(bool imported) {
    QString file_name_ = QString(package_name_);
    file_name_.replace(" ", "_");

    QDir railos_loc_dir_(railos_loc_);

    QDir metadata_dir_(railos_loc_dir_.filePath("Railway/Metadata"));

    if(!metadata_dir_.exists()) {
        QDir().mkpath(metadata_dir_.path());
    }

    toml_file_ = metadata_dir_.filePath(file_name_ + ".toml");

    if(version_.isEmpty()) {
        const QDate now_ = QDate::currentDate();
        version_ = now_.toString("yy.M.d");
    }

    RailOSTools::Metadata new_meta_;

    try {
        if(!description_.isEmpty()) new_meta_.setDescription(description_.toStdString());
        if(package_name_.isEmpty()) {packageFailure(); return "";}
        if(display_name_.isEmpty()) {
            display_name_ = QString(package_name_);
        }

        if(!imported) {
            if(rly_file_.isEmpty()) {packageFailure(); return "";}
            if(ttb_files_.empty()) {packageFailure(); return "";}
            if(doc_files_.empty()) {packageFailure(); return "";}
            if(author_.isEmpty()) {packageFailure(); return "";}
            if(version_.isEmpty()) {packageFailure(); return "";}
        }

        new_meta_.setReleaseNow();
        new_meta_.setDisplayName(display_name_.toStdString());
        new_meta_.setName(package_name_.toStdString());
        new_meta_.setFactual(factual_);
        new_meta_.setCountryCode(country_code_.toStdString());
        new_meta_.setAuthor(author_.toStdString());
        new_meta_.setVersion(version_.toStdString());

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
    } catch(std::runtime_error& e) {
        const QString err_ = QString("Package creation was unsuccessful with the following error:\n") + QString(e.what());
        QMessageBox::critical(
            parent_,
            QMessageBox::tr("Package Creation Failed"),
            QMessageBox::tr(err_.toStdString().c_str())
        );
        return "";
    }

    try {
        if(!imported) new_meta_.validate();
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

QString RailOSPkg::Packager::createPackage() {
    const QString new_toml_ = buildTOML();
    const QString build_dir_ = QDir(railos_loc_).filePath("Railway/Packages");
    if(!QDir(build_dir_).exists()) {
        QDir().mkpath(build_dir_);
    }
    QString package_file_name_ = package_name_;
    QString package_name_version_ = version_;
    version_.replace(".", "_");
    package_file_name_ += version_;
    package_file_name_.replace(" ", "_");

    const QString out_dir_ = QDir(build_dir_).filePath(package_file_name_);

    QDir().mkpath(out_dir_);
    QDir().mkpath(QDir(out_dir_).filePath("Railway"));
    QDir().mkpath(QDir(out_dir_).filePath("Program_Timetables"));
    QDir().mkpath(QDir(out_dir_).filePath("Sessions"));
    QDir().mkpath(QDir(out_dir_).filePath("Documentation"));
    QDir().mkpath(QDir(out_dir_).filePath("Images"));
    QDir().mkpath(QDir(out_dir_).filePath("Metadata"));

    const QString out_rly_file_ = QDir(out_dir_).filePath(
        "Railway/" + QFileInfo(rly_file_).fileName()
    );
    const QString out_toml_file_ = QDir(out_dir_).filePath(
        "Metadata/" + QFileInfo(toml_file_).fileName()
    );
    const QString loc_rly_file_ = QDir(railos_loc_).filePath(
        "Railway/Railways/" + QFileInfo(rly_file_).fileName()
    );

    try {

        copy_check(parent_, rly_file_, out_rly_file_);
        copy_check(parent_, toml_file_, out_toml_file_);
        copy_check(parent_, rly_file_, loc_rly_file_);

        for(const QString& ttb_file : ttb_files_) {
            const QString out_ttb_file_ = QDir(out_dir_).filePath(
                "Program_Timetables/" + QFileInfo(ttb_file).fileName()
            );
            const QString loc_ttb_file_ = QDir(railos_loc_).filePath(
                "Railway/Program timetables/" + QFileInfo(ttb_file).fileName()
            );
            copy_check(parent_, ttb_file, out_ttb_file_);
            copy_check(parent_, ttb_file, loc_ttb_file_);
        }

        for(const QString& ssn_file : ssn_files_) {
            const QString out_ssn_file_ = QDir(out_dir_).filePath(
                "Sessions/" + QFileInfo(ssn_file).fileName()
            );
            const QString loc_ssn_file_ = QDir(railos_loc_).filePath(
                "Railway/Sessions/" + QFileInfo(ssn_file).fileName()
            );
            copy_check(parent_, ssn_file, out_ssn_file_);
            copy_check(parent_, ssn_file, loc_ssn_file_);
        }

        const QDir loc_doc_dir_(QDir(railos_loc_).filePath("Railway/Documentation/"+display_name_.replace(" ", "_")));
        if(!loc_doc_dir_.exists()) QDir().mkpath(loc_doc_dir_.path());

        for(const QString& doc_file : doc_files_) {
            const QString out_doc_file_ = QDir(out_dir_).filePath(
                "Documentation/" + QFileInfo(doc_file).fileName()
            );
            const QString loc_doc_file_ = QDir(loc_doc_dir_).filePath(
                QFileInfo(doc_file).fileName()
            );
            copy_check(parent_, doc_file, out_doc_file_);
            copy_check(parent_, doc_file, loc_doc_file_);
        }

        for(const QString& img_file : img_files_) {
            const QString out_img_file_ = QDir(out_dir_).filePath(
                "Images/" + QFileInfo(img_file).fileName()
            );
            const QString loc_img_file_ = QDir(railos_loc_).filePath(
                "Railway/Images/" + QFileInfo(img_file).fileName()
            );
            copy_check(parent_, img_file, out_img_file_);
            copy_check(parent_, img_file, loc_img_file_);
        }

        for(const QString& graphic_file : graphic_files_) {
            const QString out_graphic_file_ = QDir(out_dir_).filePath(
                "Graphics/" + QFileInfo(graphic_file).fileName()
            );
            const QString loc_graphic_file_ = QDir(railos_loc_).filePath(
                "Railway/Graphics/" + QFileInfo(graphic_file).fileName()
            );
            copy_check(parent_, graphic_file, out_graphic_file_);
            copy_check(parent_, graphic_file, loc_graphic_file_);
        }
    } catch(std::runtime_error& e) {
        QMessageBox::critical(
            parent_,
            QMessageBox::tr("File not found"),
            QMessageBox::tr("Failed to create package from specified sources.")
        );
        QFile(new_toml_).remove();
        return "";
    }

    const QString zip_file_ = out_dir_ + ".zip";

    JlCompress::compressDir(zip_file_, out_dir_);

    if(!QFile::exists(zip_file_)) {
        QMessageBox::critical(
            parent_,
            QMessageBox::tr("Archive creation failure"),
            QMessageBox::tr("Failed to create package archive")
        );
        return "";
    }

    QDir(out_dir_).removeRecursively();

    const QString success_msg_ = "Package saved to '"+zip_file_+"'";

    QMessageBox::information(
        parent_,
        QMessageBox::tr("Package Creation Successful"),
        QMessageBox::tr(success_msg_.toStdString().c_str())
    );

    return zip_file_;
}

void RailOSPkg::copy_check(QWidget* parent, const QString in_file, const QString out_file) {
    // Do not try to copy a file to itself!
    if(in_file == out_file) return;
    const QString in_file_ = QDir::fromNativeSeparators(in_file);
    const QString out_file_ = QDir::fromNativeSeparators(out_file);
    if(QFile(out_file_).exists()) QFile(out_file_).remove();
    if(!QFile(in_file_).copy(out_file_)) {
        const QString message_ = "Failed to copy file '"+in_file_+"' to '"+out_file_+"'";
        QMessageBox::critical(
            parent,
            QMessageBox::tr("File Copy Failure"),
            QMessageBox::tr(message_.toStdString().c_str())
        );
        throw std::runtime_error("File copy failed");
    }
}
