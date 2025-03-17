/*! **************************************************************************
 * @file railos_packager.hxx
 * @author K. Zarebski
 * @date 2022-01-05
 * @brief Main application definition file
 *
 * This file contains code for the construction of the main GUI application
 * which forms the central interface for the install/uninstall and creation
 * of padckages for Railway Operation Simulator
 *
 ****************************************************************************/
#ifndef __RAILOSPKGMANAGER_HXX__
#define __RAILOSPKGMANAGER_HXX__

#include <QWidget>
#include <QDesktopServices>
#include <QProcess>
#include <QFileDialog>
#include <QTableWidget>
#include <QtMath>
#include <QDebug>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QtCore>
#include <QLabel>
#include <QComboBox>
#include <QMap>
#include <QLabel>
#include <QPushButton>
#include <iostream>

#include "railos_system.hxx"
#include "railostools/country_codes.hxx"

#ifndef RAILOSPKGMANAGER_VERSION
    #error "Version not specified during build"
#endif

/*! **********************************************************************
 * @namespace RailOSPkg
 * @brief Contains all methods and classes relating to the RailOSPkg::Manager
 ************************************************************************/
namespace RailOSPkg {
/*! **********************************************************************
 * @class Manager
 * @brief Constructs the main GUI interface for the package manager
 *
 * Creates the windows and widgets with which the user interacts acting
 * as the central interface for the install/uninstall and creation of
 * packages. GUI is built using Qt widgets consisting of Buttons,
 * Text Entry objects and windows
 * **********************************************************************/
class Manager : public QWidget
{
    Q_OBJECT

private:
    const int WINDOW_WIDTH = 600;
    const int WINDOW_HEIGHT = 500;
    const int TABLE_WIDTH = 510;
    const int TABLE_HEIGHT = 330;
    const int TABLE_NCOLS = 4;
    const int BUTTON_WIDTH = 150;
    const int BUTTON_HEIGHT = 30;
    QString cache_str_ = "";
    System* system_ = new System(this);
    QWidget* package_form_ = new QWidget(this, Qt::Tool);
    QWidget* url_form_ = new QWidget(this, Qt::Tool);
    QSpinBox* year_box_;
    QCheckBox* factual_box_;
    QCheckBox* advanced_ = new QCheckBox(this);
    QLabel* factual_box_label_;
    QComboBox* country_code_;
    QLabel* advanced_str_ = new QLabel(this);
    QLabel* year_box_label_;
    QLabel* country_code_label_;
    QLabel* url_label_;
    QLabel* branch_label_;
    QLineEdit* url_entry_;
    QLineEdit* branch_entry_;
    QMap<QString, QLineEdit*> package_form_entry_;
    QMap<QString, QLabel*> package_form_labels_;
    QMap<QString, QPushButton*> buttons_ = {
        {"install", new QPushButton(QPushButton::tr("Install Package"), this)},
        {"create", new QPushButton(QPushButton::tr("Create Package"), this)},
        {"uninstall", new QPushButton(QPushButton::tr("Uninstall Package"), this)},
        {"github", new QPushButton(QPushButton::tr("GitHub"), this)},
        {"railospath", new QPushButton(QPushButton::tr("Set RailOS"), this)},
        {"open_railos_dir", new QPushButton(QPushButton::tr("Open RailOS Folder"), this)},
        {"launch_railos", new QPushButton(QPushButton::tr("Launch RailOS"), this)}
    };
    QLabel* info_str_ = new QLabel(this);
    QTableWidget* installed_ = new QTableWidget(this);

/*! **************************************************************************
 * @brief Populate the table display with all installed packages
 * **************************************************************************/
    void populateTable_();

/*! **************************************************************************
 * @brief Construct the package creation form for metadata entry
 * **************************************************************************/
    void buildPackageForm_();

/*! **************************************************************************
 * @brief Construct the GitHub project form for zip retrieval
 * **************************************************************************/
    void buildURLForm_();

/*! **************************************************************************
 * @brief Wipe all inputs from the package creation form
 * **************************************************************************/
    void clearPackageForm_();

/*! **************************************************************************
 * @brief Validate information provided in the package creation form
 *
 * @return A mapping of the recorded form information
 * **************************************************************************/
    QMap<QString,QString> checkPackageForm_();
private slots:
/*! **************************************************************************
 * @brief Action associated with the "Install Package" button
 * **************************************************************************/
    void on_InstallButtonClicked();

/*! **************************************************************************
 * @brief Action associated with the "Uninstall Package" button
 * **************************************************************************/
    void on_UninstallButtonClicked();

/*! **************************************************************************
 * @brief Action associated with the "Create Package" button
 * **************************************************************************/
    void on_CreateButtonClicked();

/*! **************************************************************************
 * @brief Action associated with the "Create Package/Confirm" button
 * **************************************************************************/
    void on_CreateConfirmClicked();

/*! **************************************************************************
 * @brief Action associated with the "Create Package/Cancel" button
 * **************************************************************************/
    void on_CreateCancelClicked();

/*! **************************************************************************
 * @brief Action associated with the "Create Package/RLY Browse" button
 * **************************************************************************/
    void on_BrowseRlyFilesClicked();

/*! **************************************************************************
 * @brief Action associated with the "Create Package/TTB Browse" button
 * **************************************************************************/
    void on_BrowseTTBFilesClicked();

/*! **************************************************************************
 * @brief Action associated with the "Create Package/SSN Browse" button
 * **************************************************************************/
    void on_BrowseSSNFilesClicked();

/*! **************************************************************************
 * @brief Action associated with the "Create Package/Docs Browse" button
 * **************************************************************************/
    void on_BrowseDocFilesClicked();

/*! **************************************************************************
 * @brief Action associated with the "Create Package/Imgs Browse" button
 * **************************************************************************/
    void on_BrowseImgFilesClicked();

/*! **************************************************************************
 * @brief Action associated with the "Create Package/Graphics Browse" button
 * **************************************************************************/
    void on_BrowseGraphicFilesClicked();

/*! **************************************************************************
 * @brief Action associated with the clicking advanced options checkbox
 * **************************************************************************/
    void on_CheckBoxClicked();

/*! **************************************************************************
 * @brief Action associated with the "GitHub" button
 * **************************************************************************/
    void on_GitHubClicked();

/*! **************************************************************************
 * @brief Action associated with the "GitHub/Ok" button
 * **************************************************************************/
    void on_GitHubOkClicked();

/*! **************************************************************************
 * @brief Action associated with the "GitHub/Cancel" button
 * **************************************************************************/
    void on_GitHubCancelClicked();

/*! **************************************************************************
 * @brief Action associated with the "RailOS Path" button
 * **************************************************************************/
    void on_RailOSPathClicked();

/*! **************************************************************************
 * @brief Action associated with the "RailOS Directory Open" button
 * **************************************************************************/
    void on_RailOSDirOpenClicked();

/*! **************************************************************************
 * @brief Action associated with the "RailOS Launch" button
 * **************************************************************************/
    void on_RailOSLaunchClicked();

public:
/*! **************************************************************************
 * @brief Initialise an instance of the application GUI definition
 * **************************************************************************/
    Manager();

/*! **************************************************************************
 * @brief Destructor for application GUI definition
 * **************************************************************************/
    ~Manager();
};
};
#endif // RailOSPkg::Manager_HXX
