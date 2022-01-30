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
#include "elzip.hpp"
#include "curl/curl.h"

#include "ros_packager.hxx"

namespace ROSPkg {
    size_t download_write_file_(void *ptr, size_t size, size_t nmemb, FILE *stream); 
/*! **********************************************************************
 * @class System
 * @brief Procedures relating to the files on the file system
 * 
 * Handles the creation/removal of files on the file system including
 * the building of packages and metadata files. Also determines which
 * packages are installed. Packages are stored under an SHA256 hash
 * calculated from the relevant metadata file.
 * **********************************************************************/
class System {
private:
    QWidget* parent_ = nullptr;
    QMap<QString, ROSTools::Metadata> installed_;
    QString ros_loc_ = "";
    const QString cache_file_ = QDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)).filePath("ros_cfg");

/*! **************************************************************************
* @brief Locate the ROS installation on the local system and cache it
*
* Opens a user dialog requesting the location of railway.exe on the system
* and stores the location for future use.
* **************************************************************************/
    void createCache_();

/*! **************************************************************************
 * @brief Parse an existing metadata file for information on a package
 *
 * If a package contains a metadata TOML file this is read and information
 * extracted to be displayed on the installed packages listing
 * **************************************************************************/
    void parseMetaFile_(const QString& file_name);

/*! **************************************************************************
 * @brief Get all relevant files from the specified zip archive
 * **************************************************************************/
    QMap<QString, QList<QString>> getZipFileListing_(const QString& directory) const;

/*! **************************************************************************
 * @brief Copy files from zip to ROS directories
 * **************************************************************************/
    void unpackZip_(const QMap<QString, QList<QString>>& file_listing) const;

/*! **************************************************************************
 * @brief Upgrade the local ROS installation from the contents of a zip file
 * **************************************************************************/
    void upgradeROS_(const QMap<QString, QList<QString>>& files_list) const;

public:
/*! **************************************************************************
 * @brief Initialise a new instance of the System class
 *
 * @param parent pointer to the parent widget
 * **************************************************************************/
    System(QWidget* parent);

/*! **************************************************************************
 * @brief Retrieve the listing of installed add-ons for display
 *
 * Converts the data of the installed packages into table objects to be
 * displayed on the main application interface.
 * 
 * @return a 2D array of information for display on the table widget
 * **************************************************************************/
    QList<QList<QTableWidgetItem*>> getTableInfo() const;

/*! **************************************************************************
* @brief Extract contents from add-on archive
*
* Although all contents is extracted, only files stated in the relevant
* metadata file will be kept.
* 
* @param file_name path of archive to be extracted.
* @param author optional name of the author if known
* @param pkg_name optional name of the package if known
* ***************************************************************************/
    void unzipFile(const QString& file_name, const QString& author="", const QString& pkg_name="", const QString& country_code="") const;

/*! **************************************************************************
 * @brief Find the list of packages installed on the local system
 * **************************************************************************/
    void populateInstalled();

/*! **************************************************************************
 * @brief Uninstall a package given its SHA256 ID
 *
 * Packages are stored under an SHA256 ID calculated from their metadata TOML
 * file contents.
 * 
 * @param sha the SHA256 hash of the package to remove
 * **************************************************************************/
    void uninstall(const QString& sha);

/*! **************************************************************************
 * @brief Retrieve the location of railway.exe on the file system
 * **************************************************************************/
    QString getROSLocation() const {return ros_loc_;}

/*! **************************************************************************
 * @brief Retrieve archive from a GitHub repository
 *
 * @param repository_path GitHub repository path in the form "user/repo"
 * **************************************************************************/
    void fetchGitHub(const QString& repository_path, const QString& branch = "master") const;
};
};

#endif
