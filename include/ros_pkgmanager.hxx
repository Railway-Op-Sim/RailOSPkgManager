#ifndef ROSPKGMANAGER_HXX
#define ROSPKGMANAGER_HXX

#include <QWidget>
#include <QFileDialog>
#include <QTableWidget>
#include <QtMath>
#include <QDebug>
#include <QtCore>
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
        const System* system_ = new System(this);
        QMap<QString, QPushButton*> buttons_ = {
            {"install", new QPushButton(QPushButton::tr("Install Package"), this)},
            {"create", new QPushButton(QPushButton::tr("Create Package"), this)},
            {"uninstall", new QPushButton(QPushButton::tr("Uninstall Package"), this)}
        };
        QLabel* info_str_ = new QLabel(this);
        QTableWidget* installed_ = new QTableWidget(this);
        void populateTable_();
    private slots:
        void on_InstallButtonClicked();
    public:
        Manager();
        ~Manager();
    };
};
#endif // ROSPKGMANAGER_HXX
