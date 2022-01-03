#ifndef __ROSPKGMANAGER_HXX__
#define __ROSPKGMANAGER_HXX__

#include <QWidget>
#include <QFileDialog>
#include <QTableWidget>
#include <QtMath>
#include <QDebug>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QtCore>
#include <QLabel>
#include <QMap>
#include <QLabel>
#include <QPushButton>

#include "ros_system.hxx"

namespace ROSPkg {
    class Manager : public QWidget
    {
        Q_OBJECT

    private:
        const int WINDOW_WIDTH = 600;
        const int WINDOW_HEIGHT = 480;
        const int TABLE_WIDTH = 510;
        const int TABLE_HEIGHT = 330;
        const int TABLE_NCOLS = 4;
        const int BUTTON_WIDTH = 150;
        const int BUTTON_HEIGHT = 30;
        QString cache_str_ = "";
        System* system_ = new System(this);
        QWidget* subwindow_ = new QWidget(this, Qt::Tool);
        QSpinBox* year_box_;
        QCheckBox* factual_box_;
        QLabel* factual_box_label_;
        QLabel* year_box_label_;
        QMap<QString, QLineEdit*> package_form_entry_;
        QMap<QString, QLabel*> package_form_labels_;
        QMap<QString, QPushButton*> buttons_ = {
            {"install", new QPushButton(QPushButton::tr("Install Package"), this)},
            {"create", new QPushButton(QPushButton::tr("Create Package"), this)},
            {"uninstall", new QPushButton(QPushButton::tr("Uninstall Package"), this)},
        };
        QLabel* info_str_ = new QLabel(this);
        QTableWidget* installed_ = new QTableWidget(this);
        void populateTable_();
        void buildPackageForm_();
        void clearPackageForm_();
    private slots:
        void on_InstallButtonClicked();
        void on_UninstallButtonClicked();
        void on_CreateButtonClicked();
        void on_CreateConfirmClicked();
        void on_CreateCancelClicked();
        void on_BrowseRlyFilesClicked();
        void on_BrowseTTBFilesClicked();
        void on_BrowseSSNFilesClicked();
        void on_BrowseDocFilesClicked();
    public:
        Manager();
        ~Manager();
    };
};
#endif // ROSPKGMANAGER_HXX
