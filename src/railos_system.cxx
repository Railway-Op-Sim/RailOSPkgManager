#include "railos_system.hxx"

size_t ROSPkg::download_write_file_(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

void ROSPkg::System::createCache(bool startup) {
    const QString cache_dir_ = QFileInfo(cache_file_).absolutePath();
    if(!QDir(cache_dir_).exists()) {
        QDir().mkpath(cache_dir_);
    }
    const QString railosexe_ = QFileDialog::getOpenFileName(
        parent_,
        QFileDialog::tr("Locate") + QString(" Railway Operation Simulator"),
        QString(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)), QFileDialog::tr("ROS Exe (railway.exe)")
    );

    if(railosexe_.isNull())
    {
        if(!startup) return;
        QMessageBox::critical(
	    parent_,
	    QMessageBox::tr("Missing ROS Location"),
	    QMessageBox::tr("Railway Operation Simulator executable 'railway.exe' location is required, aborting.")
	);
	throw std::runtime_error("No ROS location set");
    }

    railos_loc_ = QFileInfo(railosexe_).absolutePath();

	if(railos_loc_.isEmpty()) return;
    QFile file_(cache_file_);
    if(file_.open(QIODevice::WriteOnly)) {
        QTextStream stream_(&file_);
        stream_ << QFileInfo(railos_loc_).absolutePath();
    }
}

ROSPkg::System::System(QWidget* parent) {
    parent_ = parent;

    if(railos_loc_.isEmpty() && QFile::exists(cache_file_)) {
        QFile file_(cache_file_);
        if(file_.open(QIODevice::ReadOnly)) {
            railos_loc_ = file_.readLine();
        }
    }

    QFileInfo loc_info_(railos_loc_ + QDir::separator() + "Railway" + QDir::separator() + "railway.exe");

    if(!loc_info_.exists() || !loc_info_.isFile()) {
        createCache(true);
	if(railos_loc_.isEmpty()) {
	    QMessageBox::critical(
                parent_,
		QMessageBox::tr("Railway Operation Simulator EXE unset"),
		QMessageBox::tr("Cannot load package manager without setting Railway Operation Simulator executable path")
	    );
	    throw std::runtime_error("No ROS location set");
	}
        QFile file_(cache_file_);
        if(file_.open(QIODevice::ReadOnly)) {
            railos_loc_ = file_.readLine();
        }
    }

    populateInstalled();
}

void ROSPkg::System::parseMetaFile_(const QString& file_name) {
    if(!QFile::exists(file_name)) {
        return;
    }

    RailOSTools::Metadata meta_data_;

    try {
        meta_data_ = RailOSTools::Metadata(std::filesystem::path(file_name.toStdString()), false);
    } catch(std::runtime_error& e) {
        const QString err_ = QString("Cannot import package from '") +
        file_name +
        QString("' due to missing content:\n") + 
        QString(e.what()) +
        QString(" in metadata.");

        QMessageBox::critical(
            parent_,
            QMessageBox::tr("Missing Package Content"),
            QMessageBox::tr(err_.toStdString().c_str()));
        return;
    }

    QFile file_(file_name);
    QString hash_ = "";
    if(file_.open(QIODevice::ReadOnly)) {
        QString toml_dat_;
        while(!file_.atEnd()) {
            toml_dat_ += file_.readLine();
        }
        hash_ = QString::fromStdString(digestpp::sha256().absorb(toml_dat_.toStdString()).hexdigest());
    }

    qDebug() << "Registering add-on: " << hash_;

    installed_.insert(hash_, meta_data_);

}

void ROSPkg::System::populateInstalled() {
    QDir meta_dir_(QDir(railos_loc_).filePath("Railway/Metadata"));
    QList<QString> filters_ = {"*.toml"};
    QList<QString> toml_files_;
    QDirIterator iter_toml_(meta_dir_.path(), filters_, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    while(iter_toml_.hasNext()) {
        toml_files_ << iter_toml_.next();
    }

    for(const QString& file_ : toml_files_) {
        parseMetaFile_(file_);
    }
}

QList<QList<QTableWidgetItem*>> ROSPkg::System::getTableInfo() const {
    QList<QList<QTableWidgetItem*>> info_;

    for(auto& table : installed_.toStdMap()) {
        std::stringstream ss;
        ss << table.second.version();
        QList<QTableWidgetItem*> row_;
        row_.push_back(new QTableWidgetItem(QString::fromStdString(table.second.name())));
        row_[row_.size()-1]->setFlags(row_[row_.size()-1]->flags() & ~Qt::ItemIsEditable);
        row_.push_back(new QTableWidgetItem(QString::fromStdString(ss.str())));
        row_[row_.size()-1]->setFlags(row_[row_.size()-1]->flags() & ~Qt::ItemIsEditable);
        row_.push_back(new QTableWidgetItem(QString::fromStdString(table.second.author())));
        row_[row_.size()-1]->setFlags(row_[row_.size()-1]->flags() & ~Qt::ItemIsEditable);
        row_.push_back(new QTableWidgetItem(table.first));
        row_[row_.size()-1]->setFlags(row_[row_.size()-1]->flags() & ~Qt::ItemIsEditable);
        info_.push_back(row_);
    }
    return info_;
}

QMap<QString, QList<QString>> ROSPkg::System::getZipFileListing_(const QString& unzip_directory) const {
    // File path filters
    QList<QString> filter_ssn_{"*.ssn", "*.SSN"};
    QList<QString> filter_rly_{"*.rly", "*.RLY"};
    QList<QString> filter_ttb_{"*.ttb", "*.TTB"};
    QList<QString> filter_toml_{"*.toml"};
    QList<QString> filter_docs_{"*.md", "*.pdf"};
    QList<QString> filter_imgs_{"*.bmp"};
    QList<QString> filter_graphics_{"*.jpg", "*.png", "*.gif"};
    QList<QString> filter_railosexe_{"railway.exe"};
    QList<QString> filter_railosfiles_{"*.dll", "*.chm", "*.bpl", "*.txt", "*.pdf"};

    // File result containers
    QMap<QString, QList<QString>> files_ ={
        {"ssn", {}},
        {"toml", {}},
        {"docs", {}},
        {"ttb", {}},
        {"rly", {}},
        {"imgs", {}},
        {"graphics", {}},
    };


    // Directory iterators for each file type
    QDirIterator it_ssn(unzip_directory, filter_ssn_, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QDirIterator it_rly(unzip_directory, filter_rly_, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QDirIterator it_ttb(unzip_directory, filter_ttb_, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QDirIterator it_toml(unzip_directory, filter_toml_, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QDirIterator it_docs(unzip_directory, filter_docs_, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QDirIterator it_railosexe(unzip_directory, filter_railosexe_, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QDirIterator it_imgs(unzip_directory, filter_imgs_, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QDirIterator it_graphics(unzip_directory, filter_graphics_, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QDirIterator it_railosfiles(unzip_directory, filter_railosfiles_, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    // Check if package is an ROS upgrade
    if(it_railosexe.hasNext()) {
        QMap<QString, QList<QString>>railosfiles_{{"ros", {it_railosexe.next()}}};
        QDir unzip_dir(unzip_directory);
        railosfiles_["source_loc"] = {unzip_dir.path()};
        
        while(it_railosfiles.hasNext()) {
            const QString railosfile_{it_railosfiles.next()};
            railosfiles_["railosfiles"] << railosfile_;
        }

        return railosfiles_;
    }

    // For each file type search recursively for the needed file types
    while(it_toml.hasNext()) {
        const QString toml_file_{it_toml.next()};
        files_["toml"] << toml_file_;
    }

    while(it_ssn.hasNext()) {
        const QString ssn_file_{it_ssn.next()};
        files_["ssn"] << ssn_file_;
    }

    while(it_rly.hasNext()) {
        const QString rly_file_{it_rly.next()};
        files_["rly"] << rly_file_;
    }

    while(it_ttb.hasNext()) {
        const QString ttb_file_{it_ttb.next()};
        files_["ttb"] << ttb_file_;
    }

    while(it_docs.hasNext()) {
        const QString docs_file_{it_docs.next()};
        files_["docs"] << docs_file_;
    }

    while(it_imgs.hasNext()) {
        const QString img_file_{it_imgs.next()};
        files_["imgs"] << img_file_;
    }

    while(it_graphics.hasNext()) {
        const QString graphic_file_{it_graphics.next()};
        files_["graphics"] << graphic_file_;
    }

    return files_;
}

void ROSPkg::System::upgradeROS_(const QMap<QString, QList<QString>>& files_list) const {
    QMessageBox railosinstall_dialog(parent_);

    railosinstall_dialog.setWindowTitle("Upgrade Railway Operation Simulator?");
    railosinstall_dialog.setText("A Railway Operation Simulator application package was detected.");
    railosinstall_dialog.setInformativeText("Do you want to upgrade your local installation?");
    railosinstall_dialog.setStandardButtons(QMessageBox::Abort | QMessageBox::Ok);
    railosinstall_dialog.setDefaultButton(QMessageBox::Ok);

    const int return_code_ = railosinstall_dialog.exec();
    QStringList installed_ = {};

    if(return_code_ == QMessageBox::Abort) return;

    const QString railosexe_ = files_list["ros"][0];
    const QString railosexe_out_ = QDir(railos_loc_).filePath("Railway/"+QFileInfo(railosexe_).fileName());

    if(QFile(railosexe_out_).exists()) {
        QFile(railosexe_out_).remove();
    }

    copy_check(parent_, railosexe_, railosexe_out_);

    installed_.push_back(railosexe_out_);

    const QString archive_loc_ = files_list["source_loc"][0];

    for(const QString railosfile : files_list["railosfiles"]) {
        const QString file_ = QDir(railos_loc_).filePath(QDir(archive_loc_).relativeFilePath(railosfile));
        installed_.push_back(file_);
        if(QFile(file_).exists()) {
            QFile(file_).remove();
        }
        copy_check(parent_, railosfile, file_);
    }

    const QString info_str_ = "The following files were installed:\n"+installed_.join("\n");

    QMessageBox::information(
        parent_,
        QMessageBox::tr("Upgrade successful"),
        QMessageBox::tr(info_str_.toStdString().c_str())
    );
}

void ROSPkg::System::unpackZip_(const QMap<QString, QList<QString>>& files_list, bool legacy_package) const {
    QString info_text_ = "";
    const QDir railos_loc_dir_(railos_loc_);
    for(const QString& rly_file : files_list["rly"] ) {
        QString base_name_ = QFileInfo(rly_file).fileName();
        QString new_path_ = railos_loc_dir_.filePath("Railway/Railways/" + base_name_);
        info_text_ += "Added " + new_path_;
        qDebug() << "Copying: " << rly_file << " to " << new_path_;
        copy_check(parent_, rly_file, new_path_);
    }

    for(const QString& ttb_file : files_list["ttb"] ) {
        QString base_name_ = QFileInfo(ttb_file).fileName();
        QString new_path_ = railos_loc_dir_.filePath("Railway/Program timetables/" + base_name_);
        info_text_ += "\n\nAdded " + new_path_;
        qDebug() << "Copying: " << ttb_file << " to " << new_path_;
        copy_check(parent_, ttb_file, new_path_);
    }

    for(const QString& graphic_file : files_list["graphics"] ) {
        QString base_name_ = QFileInfo(graphic_file).fileName();
        QString new_path_ = railos_loc_dir_.filePath("Railway/Graphics/" + base_name_);
        info_text_ += "\n\nAdded " + new_path_;
        qDebug() << "Copying: " << graphic_file << " to " << new_path_;
        copy_check(parent_, graphic_file, new_path_);
    }

    for(const QString& img_file : files_list["imgs"] ) {
        QString base_name_ = QFileInfo(img_file).fileName();
        QString new_path_ = railos_loc_dir_.filePath("Railway/Images/" + base_name_);
        info_text_ += "\n\nAdded " + new_path_;
        qDebug() << "Copying: " << img_file << " to " << new_path_;
        copy_check(parent_, img_file, new_path_);
    }

    for(const QString& ssn_file : files_list["ssn"] ) {
        QString base_name_ = QFileInfo(ssn_file).fileName();
        QString new_path_ = railos_loc_dir_.filePath("Railway/Sessions/" + base_name_);
        info_text_ += "\n\nAdded " + new_path_;
        qDebug() << "Copying: " << ssn_file << " to " << new_path_;
        copy_check(parent_, ssn_file, new_path_);
    }

    QDir doc_dir_(railos_loc_dir_.filePath("Railway/Documentation"));

    // If the Documentation directory does not yet exist create it
    if(!doc_dir_.exists()) {
        QDir().mkpath(doc_dir_.path());
    }

    QString meta_dir_ = railos_loc_dir_.filePath("Railway/Metadata");

    RailOSTools::Metadata package_data_;

    try {
        package_data_ = RailOSTools::Metadata(std::filesystem::path(files_list["toml"][0].toStdString()), false);
    }
    catch(const std::invalid_argument&) {
        QMessageBox::critical(
            parent_,
            QMessageBox::tr("Missing Package Metadata"),
            QMessageBox::tr("Cannot install selected package, missing package metadata.")
        );

        return;
    } catch(std::runtime_error& e) {
        const QString err_ = QString("Cannot import package due to missing content:\n") +
        QString(e.what()) +
        QString(" in metadata.");

        QMessageBox::critical(
            parent_,
            QMessageBox::tr("Missing Package Content"),
            QMessageBox::tr(err_.toStdString().c_str()));

        return;
    }

    const QString pkg_doc_dir_ = doc_dir_.filePath(QString::fromStdString(package_data_.display_name()).replace(" ", "_"));

    if(!files_list["docs"].empty()) {
        // Create directory for add-on docs
        QDir().mkpath(pkg_doc_dir_);
    }

    if(!legacy_package) {
        for(const QString& toml_file : files_list["toml"]) {
            QString base_name_ = QFileInfo(toml_file).fileName();
            QString new_path_ = railos_loc_dir_.filePath("Railway/Metadata/" + base_name_);
            info_text_ += "\n\nAdded " + new_path_;
            qDebug() << "Copying: " << toml_file << " to " << new_path_;
            copy_check(parent_, toml_file, new_path_);
        }
    }

    for(const QString& doc_file : files_list["docs"] ) {
        QString base_name_ = QFileInfo(doc_file).fileName();
        QString new_path_ = QDir(pkg_doc_dir_).filePath(base_name_);
        info_text_ += "\n\nAdded " + new_path_;
        copy_check(parent_, doc_file, new_path_);
    }

    QMessageBox::information(parent_, QMessageBox::tr("Add-on installed successfully"), QMessageBox::tr(info_text_.toStdString().c_str()));
}

void ROSPkg::System::unzipFile(const QString& file_name, const QString& author, const QString& pkg_name, const QString& country_code) const {
    QTemporaryDir temp_dir_;
    QDir().mkpath(temp_dir_.path());
    bool no_toml_ = false;
    elz::extractZip(file_name.toStdString(), temp_dir_.path().toStdString());
    QMap<QString, QList<QString>> files_list_ = getZipFileListing_(temp_dir_.path());

    qDebug() << "Zip File Contents: " << files_list_ << Qt::endl;

    // If there is an ROS executable in the zip file assume that it is
    // an upgrade to ROS to be installed
    if(files_list_.contains("ros")) {
        upgradeROS_(files_list_);
        return;
    }

    // If no TOML file is found then archive is not an ROS package yet so one needs to be created
    if(files_list_["toml"].empty()) {

        const QString package_name_ = (!pkg_name.isEmpty()) ? pkg_name : QString(QFileInfo(file_name).baseName()).replace("_", " ");

        ROSPkg::Packager packager_{parent_, railos_loc_, package_name_};

        if(!author.isEmpty()) packager_.setAuthor(author);
        if(!country_code.isEmpty()) packager_.setCountryCode(country_code);

        if(files_list_["rly"].size() != 1) {
            QMessageBox::critical(
                parent_,
                QMessageBox::tr("Package definition ambiguous"),
                QMessageBox::tr("Expected single RLY file from archive.")
            );

            return;
        }

        if(files_list_["ttb"].empty()) {
            QMessageBox::critical(
                parent_,
                QMessageBox::tr("Missing timetables"),
                QMessageBox::tr("Expected one or more TTB files within archive.")
            );

            return;
        }

        for(const QString& ttb_file : files_list_["ttb"]) packager_.addTTBFile(ttb_file);

        if(!files_list_["ssn"].empty()) {
            for(const QString& ssn_file : files_list_["ssn"]) packager_.addSSNFile(ssn_file);
        }

        if(!files_list_["docs"].empty()) {
            for(const QString& doc_file : files_list_["docs"]) packager_.addDocFile(doc_file);
        }

        if(!files_list_["imgs"].empty()) {
            for(const QString& img_file : files_list_["imgs"]) packager_.addImgFile(img_file);
        }

        if(!files_list_["graphics"].empty()) {
            for(const QString& graphic_file : files_list_["graphics"]) packager_.addGraphicsFile(graphic_file);
        }

        packager_.setRLYFile(QFileInfo(files_list_["rly"][0]).fileName());

        const QString new_toml_ = packager_.buildTOML(true);

        if(new_toml_.isEmpty()) {
            QMessageBox::critical(
                parent_,
                QMessageBox::tr("TOML creation failure"),
                QMessageBox::tr("TOML creation for non-project archive failed.")
            );

            return;
        }

        const QString message_ = QString("Created metadata file for legacy package import '") + new_toml_ + QString("'.");

        QMessageBox::information(
            parent_,
            QMessageBox::tr("New Metadata File Created"),
            QMessageBox::tr(message_.toStdString().c_str())
        );

        files_list_["toml"].push_back(new_toml_);

        no_toml_ = true;

    }

    else if(files_list_["toml"].size() > 1) {
        QMessageBox::critical(
            parent_,
            QMessageBox::tr("Package Definition Ambiguous"),
            QMessageBox::tr("Expected single metadata (TOML) file in package, but multiple candidates found.")
        );

        return;
    }

    unpackZip_(files_list_, no_toml_);
}

void ROSPkg::System::uninstall(const QString& sha) {
    QString info_text_ = "";
    const RailOSTools::Metadata meta_data_ = installed_[sha];
    const QDir data_dir_(railos_loc_);
    const QString toml_file_ = QFileInfo(QString::fromStdString(meta_data_.toml_file().string())).fileName();

    const QString rly_file_path_ = data_dir_.filePath("Railway/Railways/" + QString::fromStdString(meta_data_.rly_file().string()));
    info_text_ += "Removed '" + rly_file_path_ + "'";
    QFile(rly_file_path_).remove();

    const QString toml_file_path_ = data_dir_.filePath("Railway/Metadata/" + toml_file_);
    info_text_ += "\n\nRemoved '" + toml_file_path_  + "'";
    QFile(toml_file_path_).remove();

    const QString doc_dir_path_ = data_dir_.filePath("Railway/Documentation/" + QString::fromStdString(meta_data_.display_name()).replace(" ", "_"));

    const QList<QString> ttb_files_;

    for(const std::filesystem::path& ttb_file: meta_data_.ttb_files()){
        const QString ttb_file_path_ = data_dir_.filePath("Railway/Program timetables/" + QString::fromStdString(ttb_file.string()));
        info_text_ += "\n\nRemoved '" + ttb_file_path_ + "'";

        QFile(ttb_file_path_).remove();
    }

    for(const std::filesystem::path& ssn_file: meta_data_.ssn_files()){
        const QString ssn_file_path_ = data_dir_.filePath("Railway/Sessions/" + QString::fromStdString(ssn_file.string()));
        info_text_ += "\n\nRemoved '" + ssn_file_path_ + "'";

        QFile(ssn_file_path_).remove();
    }

    for(const std::filesystem::path& img_file: meta_data_.img_files()){
        const QString img_file_path_ = data_dir_.filePath("Railway/Images/" + QString::fromStdString(img_file.string()));
        info_text_ += "\n\nRemoved '" + img_file_path_ + "'";

        QFile(img_file_path_).remove();
    }

    for(const std::filesystem::path& graphic_file: meta_data_.graphic_files()){
        const QString graphic_file_path_ = data_dir_.filePath("Railway/Images/" + QString::fromStdString(graphic_file.string()));
        info_text_ += "\n\nRemoved '" + graphic_file_path_ + "'";

        QFile(graphic_file_path_).remove();
    }

    if(QDir doc_dir_(doc_dir_path_); doc_dir_.exists()){
        info_text_ += "\n\nRemoved '" + doc_dir_path_;
        doc_dir_.removeRecursively();
    }

    installed_.remove(sha);
    QMessageBox::information(parent_, QMessageBox::tr("Add-on uninstalled successfully"), QMessageBox::tr(info_text_.toStdString().c_str()));
}

void ROSPkg::System::fetchGitHub(const QString& repository_path, const QString& branch) const {
    const QString GitHub_URL_ = "https://github.com/" + repository_path + "/archive/refs/heads/" + branch + ".zip";
    QTemporaryDir temp_dir_;
    QDir().mkpath(temp_dir_.path());
    const QList<QString> gh_path_ = repository_path.split("/");

    QString zip_file_name_ = "download";
    QString author_ = gh_path_[0];
    QString country_code_ = "";

    if(gh_path_[1].contains("-")) {
        // Handle the case of repositories starting with two letter country code
        // for these cases skip the prefix in the file name
        const QList<QString> hyphenated_ = gh_path_[1].split("-");
        zip_file_name_ = "";
        qDebug() << hyphenated_ << Qt::endl;
        if(RailOSTools::COUNTRY_CODES.find(hyphenated_[0].toStdString()) == RailOSTools::COUNTRY_CODES.end()) {
            zip_file_name_ += hyphenated_[0];
        }
        else {
            country_code_ = hyphenated_[0];
        }
        for(int i{1}; i < hyphenated_.size()-1; ++i) zip_file_name_ += hyphenated_[i] + " ";
        zip_file_name_ += hyphenated_[hyphenated_.size()-1];
    }
    else {
        zip_file_name_ = gh_path_[1];
    }

    if(gh_path_[0].contains("-")) {
        author_ = (gh_path_[0].split("-")).join(" ");
    }

    const QString download_path_ = temp_dir_.path() + QDir::separator() + zip_file_name_ + ".zip";

    CURL* curl_ = curl_easy_init();
    CURLcode res;
    FILE* file_ = fopen(download_path_.toStdString().c_str(), "wb");

    curl_easy_setopt(curl_, CURLOPT_URL, GitHub_URL_.toStdString().c_str());
    curl_easy_setopt(curl_, CURLOPT_FAILONERROR, true);
    curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, false);
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, download_write_file_);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, file_);
    curl_easy_setopt(curl_, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl_, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);

    res = curl_easy_perform(curl_);
    fclose(file_);

    if(!QFile::exists(download_path_) || res != CURLE_OK) {
        const QString alert_ = "Failed to retrieve project using URL:\n"+GitHub_URL_+"\nwith error code "+QString::number(res);
        QMessageBox::critical(
            parent_,
            QMessageBox::tr("Invalid URL"),
            QMessageBox::tr(alert_.toStdString().c_str())
        );
        res = curl_easy_perform(curl_);
        return;
    }

    unzipFile(download_path_, author_, zip_file_name_, country_code_);

}
