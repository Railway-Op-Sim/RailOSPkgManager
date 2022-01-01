#include "ros_pkgmanager.hxx"

ROSPkg::Manager::Manager()
{    
    // Set the Window Dimensions and Properties
    this->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    this->setWindowTitle("Railway Operation Simulator Package Manager");

    // Define the installed add-on table offsets
    const int table_x_ = (WINDOW_WIDTH-TABLE_WIDTH)/2;
    const int table_y_ = 1.1*(WINDOW_HEIGHT-TABLE_HEIGHT)/2;

    // Title string setup
    info_str_->setText("Installed Packages");
    info_str_->setGeometry(table_x_, table_y_-50, TABLE_WIDTH, 50);

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
    buttons_["install"]->setGeometry(table_x_, WINDOW_HEIGHT-50, BUTTON_WIDTH, BUTTON_HEIGHT);
    buttons_["create"]->setGeometry(table_x_ + (TABLE_WIDTH/TABLE_NCOLS), WINDOW_HEIGHT-50, BUTTON_WIDTH, BUTTON_HEIGHT);
    buttons_["create"]->setVisible(false);
    buttons_["create"]->setEnabled(false);
    buttons_["uninstall"]->setGeometry(table_x_ + 2*(TABLE_WIDTH/TABLE_NCOLS), WINDOW_HEIGHT-50, BUTTON_WIDTH, BUTTON_HEIGHT);

    connect(buttons_["install"], &QPushButton::clicked, this, &Manager::on_InstallButtonClicked);

    installed_->update();
    populateTable_();
    installed_->show();
    info_str_->update();
    info_str_->show();
}

void ROSPkg::Manager::populateTable_() {
    const QList<QList<QTableWidgetItem*>> table_items_ = system_->getTableInfo();
    installed_->setRowCount(table_items_.size());
    for(int i{0}; i < table_items_.size(); ++i) {
        for(int j{0}; j < table_items_[i].size(); ++j) {
            installed_->setItem(i, j, table_items_[i][j]);
        }
    }
    installed_->update();
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
        QString(QStandardPaths::HomeLocation), QFileDialog::tr("Add-ons (*.zip)")
    );
    system_->unzipFile(zip_file_);
    populateTable_();
}