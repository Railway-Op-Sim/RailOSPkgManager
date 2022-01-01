#ifndef ROSPKGMANAGER_HXX
#define ROSPKGMANAGER_HXX

#include <QWidget>
#include <QFileDialog>
#include <QTableWidget>

namespace ROSPkg {
    class Manager : public QWidget
    {
        Q_OBJECT

    private:
        QTableWidget* installed_ = new QTableWidget();
    public:
        Manager();
        ~Manager();
    };
};
#endif // ROSPKGMANAGER_HXX
