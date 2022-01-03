#ifndef __ROSSYSTEM_HXX__
#define __ROSSYSTEM_HXX__

#include <QString>
#include <QWidget>
#include <QFileInfo>
#include <QWindow>
#include <QDir>
#include <QTemporaryDir>
#include <QFileDialog>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QStandardPaths>
#include <QTableWidgetItem>
#include <QDirIterator>

#include <filesystem>
#include <sstream>

#include "rostools/metadata.hxx"
#include "digestpp.hpp"
#include "AbZip/AbZip.h"

#include "ros_packager.hxx"

namespace ROSPkg {
    class System {
        private:
            QWidget* parent_ = nullptr;
            QMap<QString, ROSTools::Metadata> installed_;
            QString ros_loc_ = "";
            const QString cache_file_ = QDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)).filePath("ros_cfg");
            void createCache_();
            void parseMetaFile_(const QString& file_name);
        public:
            System(QWidget* parent);
            QList<QList<QTableWidgetItem*>> getTableInfo() const;

            /*!
            * @brief Extract contents from add-on archive
            *
            * Although all contents is extracted, only files stated in the relevant
            * metadata file will be kept.
            * 
            * @param file_name path of archive to be extracted.
            * 
            */
            void unzipFile(const QString& file_name) const;
            void populateInstalled();
            void uninstall(const QString& sha);
    };
}; 

#endif