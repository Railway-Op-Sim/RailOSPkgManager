#ifndef __ROSSYSTEM_HXX__
#define __ROSSYSTEM_HXX__

#include <QString>
#include <QWidget>
#include <QFileInfo>
#include <QDir>
#include <QTemporaryDir>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStandardPaths>
#include <QTableWidgetItem>

#include "toml.hpp"
#include "digestpp.hpp"
#include "AbZip/AbZip.h"

namespace ROSPkg {
    class System {
        private:
            QWidget* parent_ = nullptr;
            QMap<QString, toml::table> installed_;
            QString ros_loc_ = "";
            const QString cache_file_ = QDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)).filePath("ros_cfg");
            void createCache_();
            void parseMetaFile_(const QString& file_name);
            void populateInstalled_();
        public:
            System(QWidget* parent);
            QList<QList<QTableWidgetItem*>> getTableInfo() const;
            void unzipFile(const QString& file_name) const;
    };
}; 

#endif