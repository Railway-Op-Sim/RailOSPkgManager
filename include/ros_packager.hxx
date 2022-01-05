#ifndef __ROSPACKAGER_HXX__
#define __ROSPACKAGER_HXX__

#include <QDir>
#include <QString>
#include <QTemporaryDir>
#include <QWidget>
#include <QtCore>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QStandardPaths>
#include <QList>

#include "elzip.hpp"

namespace ROSPkg {
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
            Packager(QWidget* parent, const QString& ros_loc, const QString& package_name) : 
                parent_(parent), ros_loc_(ros_loc), package_name_(package_name) {}
            void setRLYFile(const QString& rly_path) {rly_file_ = rly_path;}
            void addTTBFile(const QString& ttb_path) {ttb_files_.push_back(ttb_path);}
            void addSSNFile(const QString& ssn_path) {ssn_files_.push_back(ssn_path);}
            void addDocFile(const QString& doc_path) {doc_files_.push_back(doc_path);}
            void addContributor(const QString& contributor) {contributors_.push_back(contributor);}
            void setAuthor(const QString& author) {author_ = author;}
            void setDescription(const QString& description) {description_ = description;}
            void setFactual(bool is_true) {factual_ = (is_true) ? "true" : "false";}
            void setYear(int year) {year_ = year;}
            void setVersion(const QString& version) {version_ = version;}
            void setCountryCode(const QString& country_code) {country_code_ = country_code;}
            void setDisplayName(const QString& display_name) {display_name_ = display_name;}
            QString buildTOML();
            void packageFailure();
            QString createPackage();
    };
};

#endif