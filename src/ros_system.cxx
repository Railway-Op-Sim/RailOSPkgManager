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
        stream_ << ros_loc_;
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

    QStringList filters_ = {"*.toml"};
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
    const QString home_dir_ = QDir::homePath();
    const QString out_dir_ = home_dir_ + QDir::separator() + "temp_dir";
    qDebug() << out_dir_;
    if(!zip_.extractAll(out_dir_)) {
        qDebug() << zip_.errorCount() << "errors occurred:" << zip_.errorString();
    }
    zip_.close();
}
