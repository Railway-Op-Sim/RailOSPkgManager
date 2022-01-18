/*! **************************************************************************
 * @file ros_packager.hxx
 * @author K. Zarebski
 * @date 2022-01-05
 * @brief File containing method related to ROS package creation
 *
 * This file contains the Packager class used for handling and creating
 * packages for Railway Operation Simulator. A package is defined as a single
 * route simulation consisting of:
 * 
 * - A single railway definition (.rly) file
 * - One or more program timetable files (.ttb)
 * - Optionally, session files (.ssn)
 * - Documentation files (.pdf, .md)
 * - A metadata definition file (.toml)
 * 
 * A metadata file following the standard is created and used as a package
 * definition file.
 * 
 ****************************************************************************/
#ifndef __ROSPACKAGER_HXX__
#define __ROSPACKAGER_HXX__

#include <QDir>
#include <QString>
#include <QTextStream>
#include <QTemporaryDir>
#include <QWidget>
#include <QtCore>

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QStandardPaths>
#include <QList>

#include "elzip.hpp"

#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
namespace Qt {
	static auto endl = ::endl;
};
#endif

namespace ROSPkg {
/*! **********************************************************************
 * @class Packager
 * @brief Performs assembly of files into a single package directory
 * 
 * Files are assembled into a single directory and metadata obtained. 
 * A metadata TOML file is automatically generated for the given file 
 * set and information provided by the user.
 * **********************************************************************/
class Packager {
    private:
        QWidget* parent_ = nullptr;
        const QDir output_dir_{QStandardPaths::writableLocation(QStandardPaths::CacheLocation)};
        const QString package_name_ = "";
        const QString ros_loc_ = "";
        QString display_name_ = "";
        QString author_ = "Unknown";
        QList<QString> contributors_ = {};
        QString version_ = "";
        QString description_ = "";
        int year_ = -1;
        QString factual_ = "false";
        QString country_code_ = "UN";
        QString rly_file_ = "";
        QList<QString> ssn_files_;
        QList<QString> doc_files_;
        QList<QString> ttb_files_;
        QString toml_file_ = "";
    public:
/*! **************************************************************************
 * @brief Initialise a new packager for a given parent widget
 *
 * @param parent pointer of parent widget for application
 * @param ros_loc full path to directory containing railway.exe
 * @param package_name name to give final package
 * **************************************************************************/
    Packager(QWidget* parent, const QString& ros_loc, const QString& package_name) : 
        parent_(parent), ros_loc_(ros_loc), package_name_(package_name) {}

/*! *************************************************************************
 * @brief set main railway definition (.rly) file for package
 *
 * @param rly_path full path to the RLY file
 * *************************************************************************/
    void setRLYFile(const QString& rly_path) {rly_file_ = rly_path;}

/*! *************************************************************************
 * @brief append a program timetable file (.ttb) to the package
 * 
 * @param ttb_path full path to the TTB file
 * *************************************************************************/
    void addTTBFile(const QString& ttb_path) {ttb_files_.push_back(ttb_path);}

/*! *************************************************************************
 * @brief append a session file (.ssn) to the package
 *
 * @param ssn_path full path to the SSN file
 * *************************************************************************/
    void addSSNFile(const QString& ssn_path) {ssn_files_.push_back(ssn_path);}

/*! *************************************************************************
 * @brief append a documentation file (.pdf, .md) to the package
 *
 * @param doc_path full path to the documentation file
 * *************************************************************************/
    void addDocFile(const QString& doc_path) {doc_files_.push_back(doc_path);}

/*! *************************************************************************
 * @brief add a contributor to the package, this is an additional author.
 *
 * @param contributor name of contributor
 * *************************************************************************/  
    void addContributor(const QString& contributor) {contributors_.push_back(contributor);}

/*! *************************************************************************
 * @brief set name of package author
 *
 * @param author name of package author
 * *************************************************************************/
    void setAuthor(const QString& author) {author_ = author;}

/*! *************************************************************************
 * @brief set package description
 *
 * @param description package description
 * *************************************************************************/
    void setDescription(const QString& description) {description_ = description;}

/*! *************************************************************************
 * @brief set whether package contains a factual or fictional simulation
 *
 * @param is_true if simulation is factual
 * *************************************************************************/
    void setFactual(bool is_true) {factual_ = (is_true) ? "true" : "false";}

/*! *************************************************************************
 * @brief set the year the simulation in the package is set
 *
 * @param year year of simulation (if fictional this can be any positive int)
 * *************************************************************************/
    void setYear(int year) {year_ = year;}

/*! *************************************************************************
 * @brief set semantic version for package in the form MAJOR.MINOR.PATCH
 *
 * @param version semantic version
 * *************************************************************************/
    void setVersion(const QString& version) {version_ = version;}

/*! *************************************************************************
 * @brief set the two letter ISO country code for the represented region
 *
 * @param country_code ISO code
 * *************************************************************************/
    void setCountryCode(const QString& country_code) {country_code_ = country_code;}

/*! *************************************************************************
 * @brief set package display name
 *
 * @param author name of package as to be displayed
 * *************************************************************************/
    void setDisplayName(const QString& display_name) {display_name_ = display_name;}

/*! *************************************************************************
 * @brief construct a TOML metadata file for the package
 *
 * @return full path of created TOML file
 * *************************************************************************/
    QString buildTOML();

/*! *************************************************************************
 * @brief display package assembly failure dialog
 * *************************************************************************/
    void packageFailure();

/*! *************************************************************************
 * @brief create the directory for the package and then an archive
 *
 * @return the full path of the created ZIP file
 * *************************************************************************/
    QString createPackage();
    };
};

#endif
