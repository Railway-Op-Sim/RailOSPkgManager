#ifndef __ROSSYSTEM_HXX__
#define __ROSSYSTEM_HXX__

#include <QString>
#include <QWidget>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include <QStandardPaths>

namespace ROSPkg {
    class System {
        private:
            QWidget* parent_ = nullptr;
            const QString cache_file_ = QDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)).filePath("ros_cfg");
        public:
            System(QWidget* parent);
    };
}; 

#endif