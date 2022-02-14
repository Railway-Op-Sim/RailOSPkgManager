#include "ros_pkgmanager.hxx"

void ROSPkg::Manager::buildPackageForm_() {

    const int window_x_ = this->geometry().x()+this->width()/4;
    const int window_y_ = this->geometry().y()+this->height()/4;

    factual_box_ = new QCheckBox(package_form_);
    factual_box_label_ = new QLabel(package_form_);
    factual_box_label_->setText("Factual");

    year_box_ = new QSpinBox(package_form_);
    year_box_->clear();
    year_box_->setMinimum(1800);
    year_box_->setMaximum(2200);
    year_box_label_ = new QLabel(package_form_);
    year_box_label_->setText("Year");
    year_box_->setValue(2022);

    country_code_ = new QComboBox(package_form_);

    for(std::map<std::string, std::string>::const_iterator it = ROSTools::COUNTRY_CODES.begin(); it != ROSTools::COUNTRY_CODES.end(); it++) {
        country_code_->addItem(QString::fromStdString(it->first));
    }

    country_code_->setCurrentText("GB");

    country_code_label_= new QLabel(package_form_);
    country_code_label_->setText("Country Code");

    buttons_["confirm_create"] = new QPushButton(QPushButton::tr("Ok"), package_form_);
    buttons_["confirm_cancel"] = new QPushButton(QPushButton::tr("Cancel"), package_form_);
    buttons_["browse_rly"] = new QPushButton(QPushButton::tr("..."), package_form_);
    buttons_["browse_ttb"] = new QPushButton(QPushButton::tr("..."), package_form_);
    buttons_["browse_ssn"] = new QPushButton(QPushButton::tr("..."), package_form_);
    buttons_["browse_doc"] = new QPushButton(QPushButton::tr("..."), package_form_);
    buttons_["browse_img"] = new QPushButton(QPushButton::tr("..."), package_form_);
    buttons_["browse_graphic"] = new QPushButton(QPushButton::tr("..."), package_form_);

    connect(buttons_["confirm_cancel"], &QPushButton::clicked, this, &Manager::on_CreateCancelClicked);
    connect(buttons_["confirm_create"], &QPushButton::clicked, this, &Manager::on_CreateConfirmClicked);
    connect(buttons_["browse_rly"], &QPushButton::clicked, this, &Manager::on_BrowseRlyFilesClicked);
    connect(buttons_["browse_ttb"], &QPushButton::clicked, this, &Manager::on_BrowseTTBFilesClicked);
    connect(buttons_["browse_ssn"], &QPushButton::clicked, this, &Manager::on_BrowseSSNFilesClicked);
    connect(buttons_["browse_doc"], &QPushButton::clicked, this, &Manager::on_BrowseDocFilesClicked);
    connect(buttons_["browse_img"], &QPushButton::clicked, this, &Manager::on_BrowseImgFilesClicked);
    connect(buttons_["browse_graphic"], &QPushButton::clicked, this, &Manager::on_BrowseGraphicFilesClicked);

    package_form_->setGeometry(window_x_, window_y_, 700, 700);
    package_form_->setWindowTitle("Create Package");
    package_form_->hide();

    buttons_["confirm_create"]->move(package_form_->width()/2-120, package_form_->height()-40);
    buttons_["confirm_cancel"]->move(package_form_->width()/2+30, package_form_->height()-40);

    const QMap<QString, QString> entries_ = {
        {"package_name", "Name of package"},
        {"version", "Semantic version (x.y.z)"},
        {"author", "Author"},
        {"description", "Description"},
        {"display_name", "Display name/Title"},
        {"contributors", "Contributors"},
        {"rly_file_path", "Railway file (*.rly)"},
        {"ttb_file_paths", "Timetable files (*.ttb)"},
        {"ssn_file_paths", "Session files (*.ssn)"},
        {"doc_file_paths", "Documentation files (*.pdf,*.md)"},
        {"img_file_paths", "Image files"},
        {"graphic_file_paths", "Graphics files"}
    };
    for(const auto& entry : entries_.toStdMap()) {
        package_form_entry_[entry.first] = new QLineEdit(package_form_);
        package_form_entry_[entry.first]->setFixedSize(300, package_form_entry_[entry.first]->height());
        package_form_labels_[entry.first] = new QLabel(package_form_);
        package_form_labels_[entry.first]->setText(entry.second);
    }

    package_form_labels_["package_name"]->move(
        package_form_->width()/11,
        package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_entry_["package_name"]->move(
        package_form_->width()/3,
        package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_labels_["display_name"]->move(
        package_form_->width()/11,
        2*package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_entry_["display_name"]->move(
        package_form_->width()/3,
        2*package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_labels_["version"]->move(
        package_form_->width()/11,
        3*package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_entry_["version"]->setText("1.0.0");
    package_form_entry_["version"]->move(
        package_form_->width()/3,
        3*package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_labels_["description"]->move(
        package_form_->width()/11,
        4*package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_entry_["description"]->move(
        package_form_->width()/3,
        4*package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_labels_["author"]->move(
        package_form_->width()/11,
        5*package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_entry_["author"]->move(
        package_form_->width()/3,
        5*package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_labels_["contributors"]->move(
        package_form_->width()/11,
        6*package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_entry_["contributors"]->move(
        package_form_->width()/3,
        6*package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_labels_["rly_file_path"]->move(
        package_form_->width()/11,
        7*package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_entry_["rly_file_path"]->move(
        package_form_->width()/3,
        7*package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_labels_["ttb_file_paths"]->move(
        package_form_->width()/11,
        8*package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_entry_["ttb_file_paths"]->move(
        package_form_->width()/3,
        8*package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_labels_["ssn_file_paths"]->move(
        package_form_->width()/11,
        9*package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_entry_["ssn_file_paths"]->move(
        package_form_->width()/3,
        9*package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_labels_["doc_file_paths"]->move(
        package_form_->width()/11,
        10*package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_entry_["doc_file_paths"]->move(
        package_form_->width()/3,
        10*package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_labels_["img_file_paths"]->move(
        package_form_->width()/11,
        11*package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_entry_["img_file_paths"]->move(
        package_form_->width()/3,
        11*package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_labels_["graphic_file_paths"]->move(
        package_form_->width()/11,
        12*package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_entry_["graphic_file_paths"]->move(
        package_form_->width()/3,
        12*package_form_->height()/(package_form_entry_.size()+7)
    );

    buttons_["browse_rly"]->move(
        9*package_form_->width()/11,
        7*package_form_->height()/(package_form_entry_.size()+7)
    );

    buttons_["browse_ttb"]->move(
        9*package_form_->width()/11,
        8*package_form_->height()/(package_form_entry_.size()+7)
    );

    buttons_["browse_ssn"]->move(
        9*package_form_->width()/11,
        9*package_form_->height()/(package_form_entry_.size()+7)
    );

    buttons_["browse_doc"]->move(
        9*package_form_->width()/11,
        10*package_form_->height()/(package_form_entry_.size()+7)
    );

    buttons_["browse_img"]->move(
        9*package_form_->width()/11,
        11*package_form_->height()/(package_form_entry_.size()+7)
    );

    buttons_["browse_graphic"]->move(
        9*package_form_->width()/11,
        12*package_form_->height()/(package_form_entry_.size()+7)
    );

    country_code_->move(
        package_form_->width()/3,
        (package_form_entry_.size()+1)*package_form_->height()/(package_form_entry_.size()+7)
    );

    country_code_label_->move(
        package_form_->width()/11,
        (package_form_entry_.size()+1)*package_form_->height()/(package_form_entry_.size()+7)
    );

    factual_box_->move(
        package_form_->width()/3,
        (package_form_entry_.size()+2)*package_form_->height()/(package_form_entry_.size()+7)
    );
    factual_box_label_->move(
        package_form_->width()/11,
        (package_form_entry_.size()+2)*package_form_->height()/(package_form_entry_.size()+7)
    );

    year_box_->move(
        package_form_->width()/3,
        (package_form_entry_.size()+3)*package_form_->height()/(package_form_entry_.size()+7)
    );

    year_box_label_->move(
        package_form_->width()/11,
        (package_form_entry_.size()+3)*package_form_->height()/(package_form_entry_.size()+7)
    );

    package_form_entry_["package_name"]->setMaxLength(100);
}

void ROSPkg::Manager::buildURLForm_() {
    const int window_x_ = this->geometry().x()+this->width()/4;
    const int window_y_ = this->geometry().y()+this->height()/4;

    url_form_->setGeometry(window_x_, window_y_, 450, 250);
    url_form_->setWindowTitle("Fetch GitHub Project");

    url_label_ = new QLabel(url_form_);
    url_label_->setText("Enter GitHub project in the form 'user/repository'");

    branch_label_ = new QLabel(url_form_);
    branch_label_->setText("Branch");
    branch_entry_ = new QLineEdit(url_form_);

    buttons_["confirm_fetch"] = new QPushButton(QPushButton::tr("Ok"), url_form_);
    buttons_["cancel_fetch"] = new QPushButton(QPushButton::tr("Cancel"), url_form_);

    buttons_["confirm_fetch"]->move(200, 200);
    buttons_["cancel_fetch"]->move(300, 200);

    connect(buttons_["cancel_fetch"], &QPushButton::clicked, this, &Manager::on_GitHubCancelClicked);
    connect(buttons_["confirm_fetch"], &QPushButton::clicked, this, &Manager::on_GitHubOkClicked);

    url_entry_ = new QLineEdit(url_form_);
    url_entry_->setText("Railway-Op-Sim/");

    url_label_->move(20, 20);
    url_entry_->setGeometry(20, 50, 350, url_entry_->height());

    branch_label_->move(20, 100);
    branch_entry_->setText("master");
    branch_entry_->setGeometry(20, 130, 200, branch_entry_->height());

    url_form_->hide();
}


void ROSPkg::Manager::clearPackageForm_() {
    for(const auto& entry : package_form_entry_.keys()) {
        if(entry == "version") continue; // Keep default version in form
        package_form_entry_[entry]->clear();
    }
}

ROSPkg::Manager::Manager()
{

    // Set the Window Dimensions and Properties
    this->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    const QString title_ = QString("Railway Operation Simulator Package Manager v")+QString(ROSPKGMANAGER_VERSION);
    this->setWindowTitle(title_);

    // Define the installed add-on table offsets
    const int table_x_ = (WINDOW_WIDTH-TABLE_WIDTH)/2;
    const int table_y_ = (WINDOW_HEIGHT-TABLE_HEIGHT)/3;

    // Title string setup
    info_str_->setText("Installed Packages");
    info_str_->setGeometry(table_x_, table_y_-50, TABLE_WIDTH, 50);

    installed_->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Construct the table for displaying installed packages
    installed_->setGeometry(table_x_, table_y_, TABLE_WIDTH, TABLE_HEIGHT);
    installed_->setColumnCount(TABLE_NCOLS);
    for(int i{0}; i < TABLE_NCOLS; ++i) {
        installed_->setColumnWidth(i, TABLE_WIDTH/TABLE_NCOLS);
    }
    installed_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    installed_->setFont(QFont("Arial"));
    installed_->setHorizontalHeaderLabels({"Name", "Version", "Author", "SHA"});
    installed_->setGridStyle(Qt::NoPen);
    installed_->setAlternatingRowColors(true);

    // Arrange buttons
    buttons_["install"]->setGeometry(table_x_, table_y_+TABLE_HEIGHT+20, BUTTON_WIDTH, BUTTON_HEIGHT);
    buttons_["create"]->setGeometry(table_x_ + (BUTTON_WIDTH/3+TABLE_WIDTH/TABLE_NCOLS), table_y_+TABLE_HEIGHT+20, BUTTON_WIDTH, BUTTON_HEIGHT);
    buttons_["uninstall"]->setGeometry(table_x_ + 2*(BUTTON_WIDTH/3+TABLE_WIDTH/TABLE_NCOLS), table_y_+TABLE_HEIGHT+20, BUTTON_WIDTH, BUTTON_HEIGHT);
    buttons_["github"]->setGeometry(table_x_+100, table_y_+TABLE_HEIGHT+65, BUTTON_WIDTH, BUTTON_HEIGHT);
    buttons_["ros_path"]->setGeometry(table_x_+300, table_y_+TABLE_HEIGHT+65, BUTTON_WIDTH, BUTTON_HEIGHT);



    // Enable Advanced Features
    advanced_->move(table_x_, table_y_+TABLE_HEIGHT+70);
    advanced_str_->move(table_x_+20, table_y_+TABLE_HEIGHT+70);
    advanced_str_->setText("Advanced");
    advanced_->setChecked(false);
    buttons_["github"]->hide();
    buttons_["ros_path"]->hide();

    connect(buttons_["install"], &QPushButton::clicked, this, &Manager::on_InstallButtonClicked);
    connect(buttons_["uninstall"], &QPushButton::clicked, this, &Manager::on_UninstallButtonClicked);
    connect(buttons_["create"], &QPushButton::clicked, this, &Manager::on_CreateButtonClicked);
    connect(buttons_["github"], &QPushButton::clicked, this, &Manager::on_GitHubClicked);
    connect(buttons_["ros_path"], &QPushButton::clicked, this, &Manager::on_ROSPathClicked);
    connect(advanced_, &QCheckBox::clicked, this, &Manager::on_CheckBoxClicked);

	if (system_->getROSLocation().isEmpty())
	{
		 buttons_["install"]->setDisabled(true);
		 buttons_["create"]->setDisabled(true);
		 buttons_["uninstall"]->setDisabled(true);
		 buttons_["github"]->setDisabled(true);
	}

    installed_->update();
    populateTable_();
    installed_->show();
    info_str_->update();
    info_str_->show();

    buildPackageForm_();
    buildURLForm_();

}

void ROSPkg::Manager::populateTable_() {
    const QList<QList<QTableWidgetItem*>> table_items_ = system_->getTableInfo();
    installed_->setRowCount(0);
    installed_->setRowCount(table_items_.size());
    for(int i{0}; i < table_items_.size(); ++i) {
        for(int j{0}; j < table_items_[i].size(); ++j) {
            installed_->setItem(i, j, table_items_[i][j]);
        }
    }
    installed_->update();
    this->update();
}

ROSPkg::Manager::~Manager()
{
    for(auto& button: buttons_) {
        delete button;
    }
    delete info_str_;
    delete installed_;
    delete system_;
}

void ROSPkg::Manager::on_InstallButtonClicked() {
    const QString zip_file_ = QFileDialog::getOpenFileName(
        this,
        QFileDialog::tr("Open Zip Archive"),
        QString(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)), QFileDialog::tr("Packages (*.zip)")
    );
	if(zip_file_.isEmpty()) return;
    system_->unzipFile(zip_file_);
    system_->populateInstalled();
    populateTable_();
}

void ROSPkg::Manager::on_UninstallButtonClicked() {
    QItemSelectionModel *select_ = installed_->selectionModel();
    if(!select_->hasSelection()) return;
    QModelIndexList selected_rows_ = select_->selectedRows();

    const int row_ = selected_rows_.at(0).row();
    const QString sha_ = installed_->item(row_, 3)->text();
    system_->uninstall(sha_);
    system_->populateInstalled();
    populateTable_();
}

void ROSPkg::Manager::on_CreateButtonClicked() {
    package_form_->show();
}

void ROSPkg::Manager::on_CreateCancelClicked() {
    clearPackageForm_();
    package_form_->hide();
}

void ROSPkg::Manager::on_CreateConfirmClicked() {
    QMap<QString,QString> data_;
    try {
        data_ = checkPackageForm_();
    }
    catch(const std::invalid_argument&) {
        return;
    }
    ROSPkg::Packager* package_ = new ROSPkg::Packager(this, system_->getROSLocation(), data_["package_name"]);
    package_->setAuthor(data_["author"]);
    package_->setCountryCode(data_["country_code"]);
    package_->setDisplayName(data_["display_name"]);
    package_->setFactual(data_["factual"] == "true");
    package_->setYear(data_["year"].toInt());
    package_->setVersion(data_["version"]);
    package_->setDescription(data_["description"]);

    try {
        package_->setRLYFile(data_["rly_file_path"]);
    } catch(const std::runtime_error&) {
        return;
    }

    QList<QString> ttb_files_ = data_["ttb_file_paths"].split(",");
    QList<QString> doc_files_ = data_["doc_file_paths"].split(",");
    QList<QString> img_files_ = data_["img_file_paths"].split(",");

    if(!data_["ssn_files_paths"].isEmpty()) {
        QList<QString> ssn_files_ = data_["ssn_file_paths"].split(",");
        try {
            for(const QString& ssn_file : ssn_files_) {
                package_->addSSNFile(ssn_file);
            }
        } catch(const std::runtime_error&) {
            return;
        }
    }

    if(!data_["graphic_files_paths"].isEmpty()) {
        QList<QString> graphic_files_ = data_["graphic_files_paths"].split(",");
        try {
            for(const QString& graphic_file : graphic_files_) {
                package_->addGraphicsFile(graphic_file);
            }
        } catch(const std::runtime_error&) {
            return;
        }
    }

    if(!data_["contributors"].isEmpty()) {
        QList<QString> contribs_ = data_["contributors"].split(",");
        for(const QString& contrib : contribs_) {
            package_->addContributor(contrib);
        }
    }

    for(const QString& ttb_file : ttb_files_) {
        try {
            package_->addTTBFile(ttb_file);
        } catch(std::runtime_error&) {
            return;
        }
    }

    for(const QString& doc_file : doc_files_) {
        try {
            package_->addDocFile(doc_file);
        } catch(std::runtime_error&) {
            return;
        }
    }

    for(const QString& img_file : img_files_) {
        try {
            package_->addImgFile(img_file);
        } catch(std::runtime_error&) {
            return;
        }
    }


    package_->createPackage();
    package_form_->close();
    clearPackageForm_();
    system_->populateInstalled();
    populateTable_();
}

void ROSPkg::Manager::on_BrowseRlyFilesClicked() {
    const QString rly_file_ = QFileDialog::getOpenFileName(
        this,
        QFileDialog::tr("Find Railway File"),
        QString(QDir(system_->getROSLocation()).filePath("Railways")), QFileDialog::tr("Railway Files (*.rly)")
    );
	if(rly_file_.isEmpty()) return;
    package_form_entry_["rly_file_path"]->setText(rly_file_);
    package_form_entry_["rly_file_path"]->update();
}

void ROSPkg::Manager::on_BrowseSSNFilesClicked() {
    QString ssn_file_path_ = QFileDialog::getOpenFileName(
        this,
        QFileDialog::tr("Find Session File"),
        QString(QDir(system_->getROSLocation()).filePath("Sessions")), QFileDialog::tr("Session Files (*.ssn)")
    );
	if(ssn_file_path_.isEmpty()) return;
    if(!package_form_entry_["ssn_file_paths"]->text().isEmpty()) {
        ssn_file_path_ = package_form_entry_["ttb_file_paths"]->text() + "," + ssn_file_path_;
    }
    package_form_entry_["ssn_file_paths"]->setText(ssn_file_path_);
    package_form_entry_["ssn_file_paths"]->update();
}

void ROSPkg::Manager::on_BrowseImgFilesClicked() {
    QString img_file_path_ = QFileDialog::getOpenFileName(
        this,
        QFileDialog::tr("Find Image File"),
        QString(QDir(system_->getROSLocation()).filePath("Images")), QFileDialog::tr("Image Files (*.bmp *.png *.jpg *.pdf)")
    );
	if(img_file_path_.isEmpty()) return;
    if(!package_form_entry_["img_file_paths"]->text().isEmpty()) {
        img_file_path_ = package_form_entry_["img_file_paths"]->text() + "," + img_file_path_;
    }
    package_form_entry_["img_file_paths"]->setText(img_file_path_);
    package_form_entry_["img_file_paths"]->update();
}

void ROSPkg::Manager::on_BrowseGraphicFilesClicked() {
    QString graphic_file_path_ = QFileDialog::getOpenFileName(
        this,
        QFileDialog::tr("Find Graphics File"),
        QString(QDir(system_->getROSLocation()).filePath("Graphics")), QFileDialog::tr("Graphic Files (*.bmp *.png *.jpg)")
    );
	if(graphic_file_path_.isEmpty()) return;
    if(!package_form_entry_["graphic_file_paths"]->text().isEmpty()) {
        graphic_file_path_ = package_form_entry_["graphic_file_paths"]->text() + "," + graphic_file_path_;
    }
    package_form_entry_["graphic_file_paths"]->setText(graphic_file_path_);
    package_form_entry_["graphic_file_paths"]->update();
}

void ROSPkg::Manager::on_BrowseTTBFilesClicked() {
    QString ttb_file_path_ = QFileDialog::getOpenFileName(
        this,
        QFileDialog::tr("Find Timetable File"),
        QString(QDir(system_->getROSLocation()).filePath("Program timetables")), QFileDialog::tr("Timetable Files (*.ttb)")
    );
	if(ttb_file_path_.isEmpty()) return;
    if(!package_form_entry_["ttb_file_paths"]->text().isEmpty()) {
        ttb_file_path_ = package_form_entry_["ttb_file_paths"]->text() + "," + ttb_file_path_;
    }
    package_form_entry_["ttb_file_paths"]->setText(ttb_file_path_);
    package_form_entry_["ttb_file_paths"]->update();
}

void ROSPkg::Manager::on_BrowseDocFilesClicked() {
    QString doc_file_path_ = QFileDialog::getOpenFileName(
        this,
        QFileDialog::tr("Find Documentation File"),
        QString(QDir(system_->getROSLocation()).filePath("Documentation")), QFileDialog::tr("Documentation Files (*.pdf *.md)")
    );
	if(doc_file_path_.isEmpty()) return;
    if(!package_form_entry_["doc_file_paths"]->text().isEmpty()) {
        doc_file_path_ = package_form_entry_["doc_file_paths"]->text() + "," + doc_file_path_;
    }
    package_form_entry_["doc_file_paths"]->setText(doc_file_path_);
    package_form_entry_["doc_file_paths"]->update();
}

QMap<QString,QString>  ROSPkg::Manager::checkPackageForm_() {
    QMap<QString,QString> data_;
    for(const auto& e : package_form_entry_.toStdMap()) {
        // Session files are not mandatory
        if(e.first == "ssn_file_paths") continue;

        // Contributor list not mandatory
        if(e.first == "contributors") continue;

        // Graphic files are optional
        if(e.first == "graphic_file_paths") continue;

        if(e.second->text().isEmpty()) {
            const QString err_ = QString("Package is missing required information '") + QString(e.first) + QString("'.");
            QMessageBox::critical(
                this,
                QMessageBox::tr("Incomplete Package"),
                QMessageBox::tr(err_.toStdString().c_str())
                );
            throw std::invalid_argument("Invalid package form data specified");
        }
    }
    for(const auto& le : package_form_entry_.toStdMap()) {
        data_[le.first] = le.second->text();
    }
    data_["factual"] = (factual_box_->isChecked()) ? "true" : "false";
    data_["year"] = QString::number(year_box_->value());
    data_["country_code"] = country_code_->currentText();

    return data_;
}

void ROSPkg::Manager::on_CheckBoxClicked() {
    buttons_["github"]->setVisible(advanced_->isChecked());
    buttons_["github"]->setEnabled(advanced_->isChecked());
    buttons_["ros_path"]->setVisible(advanced_->isChecked());
    buttons_["ros_path"]->setEnabled(advanced_->isChecked());
}

void ROSPkg::Manager::on_GitHubClicked() {
    url_form_->show();
}

void ROSPkg::Manager::on_GitHubCancelClicked() {
    url_entry_->clear();
    url_form_->hide();
}

void ROSPkg::Manager::on_ROSPathClicked() {
	 if (system_->createCache(false)) {
		 buttons_["install"]->setDisabled(false);
		 buttons_["create"]->setDisabled(false);
		 buttons_["uninstall"]->setDisabled(false);
		 buttons_["github"]->setDisabled(false);
		 system_->populateInstalled();
	 }

}

void ROSPkg::Manager::on_GitHubOkClicked() {
    const QString path_ = url_entry_->text();
    const QString branch_ = branch_entry_->text();
    url_entry_->setText("Railway-Op-Sim/");
    url_form_->close();
    system_->fetchGitHub(path_, branch_);
    system_->populateInstalled();
    populateTable_();
}
