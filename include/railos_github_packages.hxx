/*! **************************************************************************
 * @file railos_github.hxx
 * @author K. Zarebski
 * @date 2026-07-15
 * @brief File containing methods relating to RailOS GitHub repositories
 *
 * This file contains the Packager class used for handling and creating
 * packages for Railway Operation Simulator. A package is defined as a single
 * route simulation consisting of:
 *
 * - A single railway definition (.rly) file
 * - One or more program timetable files (.ttb)
 * - Optionally, session files (.ssn)
 * - Documentation files (.pdf, .md)
 * - Image files (.png, .bmp)
 * - A metadata definition file (.toml)
 *
 * A metadata file following the standard is created and used as a package
 * definition file.
 *
 ****************************************************************************/
#ifndef __RAILOS_GITHUB_REPOS_HXX__
#define __RAILOS_GITHUB_REPOS_HXX__
#include "curl/curl.h"
#include <QMessageBox>
#include <QWidget>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

const std::string GITHUB_RAILOS_PKG_REGISTRY_ =
    "https://raw.githubusercontent.com/Railway-Op-Sim/RailOSPkgManager/refs/"
    "heads/registry-data/data/github_registry.dat";

namespace RailOSPkg {
size_t write_to_string_(void *contents, size_t size, size_t nmemb,
                        std::string *output);
std::vector<std::string> get_repositories(QWidget *parent);
}; // namespace RailOSPkg

#endif