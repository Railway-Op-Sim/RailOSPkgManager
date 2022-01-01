#include "ros_system.hxx"

void ROSPkg::System::createCache_() {
    const QString cache_dir_ = QFileInfo(cache_file_).absolutePath();
    if(!QDir(cache_dir_).exists()) {
        QDir().mkdir(cache_dir_);
    }
    ros_loc_ = QFileDialog::getOpenFileName(
        parent_,
        QFileDialog::tr("Locate") + QString(" Railway Operation Simulator"),
        QString(QStandardPaths::HomeLocation), QFileDialog::tr("ROS Exe (railway.exe)")
    );
    QFile file_(cache_file_);
    if(file_.open(QIODevice::WriteOnly)) {
        QTextStream stream_(&file_);
        stream_ << QFileInfo(ros_loc_).absolutePath();
    }
}

ROSPkg::System::System(QWidget* parent) {
    parent_ = parent;

    if(ros_loc_.isEmpty() && QFile::exists(cache_file_)) {
        QFile file_(cache_file_);
        if(file_.open(QIODevice::ReadOnly)) {
            ros_loc_ = file_.readLine();
        }
    }

    while(!QFile::exists(ros_loc_)) {
        createCache_();
        QFile file_(cache_file_);
        if(file_.open(QIODevice::ReadOnly)) {
            ros_loc_ = file_.readLine();
        }
    }

    populateInstalled_();
}

void ROSPkg::System::parseMetaFile_(const QString& file_name) {
    //TODO: Need a better way of handling missing informartion

    if(!QFile::exists(file_name)) {
        return;
    }

    toml::table meta_data_;

    try
    {
        meta_data_ = toml::parse_file(file_name.toStdString());
    }
    catch ( const toml::parse_error& err ) {
        return;
    }

    QFile file_(file_name);
    QString hash_ = "";
    if(file_.open(QIODevice::ReadOnly)) {
        const QString toml_dat_ = file_.readLine();
        hash_ = QString::fromStdString(digestpp::sha256().absorb(toml_dat_.toStdString()).hexdigest());
    }

    qDebug() << "Registering add-on: " << hash_;

    installed_[hash_] = meta_data_;

}

void ROSPkg::System::populateInstalled_() {
    const QString ros_dir_ = QFileInfo(ros_loc_).absolutePath();
    QDir meta_dir_(QDir(ros_dir_).filePath("Metadata"));

    QList<QString> filters_ = {"*.toml"};
    meta_dir_.setNameFilters(filters_);

    QFileInfoList file_info_ = meta_dir_.entryInfoList();

    for(QFileInfo& info_ : file_info_) {
        parseMetaFile_(info_.absoluteFilePath());
    }
}

QList<QList<QTableWidgetItem*>> ROSPkg::System::getTableInfo() const {
    QList<QList<QTableWidgetItem*>> info_;

    for(auto& table : installed_.toStdMap()) {
        QList<QTableWidgetItem*> row_;
        std::optional<std::string> name = table.second["name"].value<std::string>();
        std::optional<std::string> version = table.second["version"].value<std::string>();
        std::optional<std::string> author = table.second["author"].value<std::string>();
        row_.push_back(new QTableWidgetItem(QString::fromStdString((name.value_or("Unknown")))));
        row_.push_back(new QTableWidgetItem(QString::fromStdString((version.value_or("Latest")))));
        row_.push_back(new QTableWidgetItem(QString::fromStdString((author.value_or("")))));
        row_.push_back(new QTableWidgetItem(table.first));
        info_.push_back(row_);
    }
    return info_;
}

void ROSPkg::System::unzipFile(const QString& file_name) const {
    AbZip zip_(file_name);
    QTemporaryDir temp_dir_;
    if(!zip_.extractAll(temp_dir_.path())) {
        qDebug() << zip_.errorCount() << "errors occurred:" << zip_.errorString();
    }
    QList<QString> filter_ssn_{"*.ssn"};
    QList<QString> filter_rly_{"*.rly"};
    QList<QString> filter_ttb_{"*.ttb"};
    QList<QString> filter_toml_{"*.toml"};
    QList<QString> files_ssn_, files_rly_, files_ttb_, files_toml_;
    QDirIterator it_ssn(temp_dir_.path(), filter_ssn_, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QDirIterator it_rly(temp_dir_.path(), filter_rly_, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QDirIterator it_ttb(temp_dir_.path(), filter_ttb_, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QDirIterator it_toml(temp_dir_.path(), filter_toml_, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    while(it_ssn.hasNext()) {
        files_ssn_ << it_ssn.next();
    }
    while(it_rly.hasNext()) {
        files_rly_ << it_rly.next();
    }
    while(it_ttb.hasNext()) {
        files_ttb_ << it_ttb.next();
    }
    while(it_toml.hasNext()) {
        files_toml_ << it_toml.next();
    }

    for(const QString& rly_file : files_rly_ ) {
        QString base_name_ = QFileInfo(rly_file).fileName();
        QString new_path_ = ros_loc_ + QDir::separator() + "Railways" + QDir::separator() + base_name_;
        qDebug() << "Copying: " << rly_file << " to " << new_path_;
        QFile(rly_file).copy(new_path_);
    }

    for(const QString& ttb_file : files_ttb_ ) {
        QString base_name_ = QFileInfo(ttb_file).fileName();
        QString new_path_ = ros_loc_ + QDir::separator() + "Program timetables" + QDir::separator() + base_name_;
        qDebug() << "Copying: " << ttb_file << " to " << new_path_;
        QFile(ttb_file).copy(new_path_);
    }

    for(const QString& ssn_file : files_ssn_ ) {
        QString base_name_ = QFileInfo(ssn_file).fileName();
        QString new_path_ = ros_loc_ + QDir::separator() + "Sessions" + QDir::separator() + base_name_;
        qDebug() << "Copying: " << ssn_file << " to " << new_path_;
        QFile(ssn_file).copy(new_path_);
    }

    zip_.close();
}
