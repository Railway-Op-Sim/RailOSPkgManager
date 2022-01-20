<p align="center">
<img 
    style="display: block; 
           margin-left: auto;
           margin-right: auto;
           width: 30%;"
    src="https://raw.githubusercontent.com/Railway-Op-Sim/ROSPkgManager/main/img/rospkgmanager.svg" 
    alt="Our logo">
</img>
</p>

# Railway Operation Simulator Package Manager

[![Build & Test](https://github.com/Railway-Op-Sim/ROSPkgManager/actions/workflows/build_packager.yaml/badge.svg)](https://github.com/Railway-Op-Sim/ROSPkgManager/actions/workflows/build_packager.yaml) [![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=Railway-Op-Sim_ROSPkgManager&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=Railway-Op-Sim_ROSPkgManager)

This application aims to simplify the process of adding and removing route simulations to the local installation of [Railway Operation Simulator](https://railwayoperationsimulator.com/) (ROS), a railway signalling simulation by Albert Ball.

Full documentation can be found on the [project website](https://railway-op-sim.github.io/ROSPkgManager/).

## Getting Started
On first launch you will need to specify the location of the `railway.exe` ROS executable on your system, this location is used to determine where add-ons should be installed. Once this is set the location will be cached and so no longer required. Functionality for altering the installation will be added at a later date. Within this manual `${ROS_HOME}` refers to the directory containing `railway.exe`.

## Installing Packages
Packages are installed by selecting "Install Package" and pointing to a downloaded archive containing the project files. These files will be extracted to the relevant locations. If a package/route metadata TOML file is not present within the archive one will be created and saved in `${ROS_HOME}/Metadata`, this file is formatted in the standard described on the ROS project template GitHub repository [here](https://github.com/Railway-Op-Sim/UN-Template/blob/master/README.md). Only add-ons installed using this application, and so having a metadata file, will be listed in the package manager.

## Uninstall Packages
Uninstalling packages is simple, select the entry from the list of installed add-ons and click "Uninstall Package".

## Creating Packages
A new package can be created by selecting "Create Package" and filling in the relevant information. All fields are mandatory apart from "Sessions" and "Contributors". Note documentation must be either PDF or markdown, Microsoft Office files will not be accepted as they are not easily cross-platform compatible. Documentation for a package is mandatory (a simulation should have a README!).

The buttons next to the file declaration fields allow you to browse for files to add, if more than one file is included the paths are comma-separated values.

|**Field**|**Description**|
|---|---|
|Package Name|Name of the package as it would appear on a file system.|
|Display Name|Name of the package as it would appear on a listing.|
|Description|A brief description of the contained simulation.|
|Version|A semantic version of the form `MAJOR.MINOR.PATCH` for this package.|
|Author|The leading author of this project.|
|Contributors|(Optional) Additional authors (separated by `,`).|
|Railway File|Single `.rly` for this simulation.|
|Timetable Files|All `.ttb` files for this simulation (separated by `,`).|
|Session Files|(Optional) All `.ssn` files for this simulation (separated by `,`).|
|Manual Files|Documentation files (`.pdf`, `.md`) for this simulation (separated by `,`).|
|Country Code|Two letter country code of route origin in this simulation (`FN` if fictional).|
|Factual|If simulation is a factual representation.|
|Year|Year for which this simulation is a representation (can be any value if fictional).|

## Upgrading Railway Operation Simulator
If select a zip file containing a `railway.exe` executable the program will assume it to be a copy of the main ROS program and ask if you wish to
upgrade your local installation with the files contained.

## Advanced
Clicking the "Advanced" checkbox shows additional options for package installation.

### Installing from a GitHub Repository
Installations can be performed from a GitHub repository containing the required files to define a project. Clicking the
"GitHub" button under the advanced options opens a dialog in which the user can enter the relevant GitHub project. 
The project is specified in the form "github-user/project-name".
By default the user is set to "Railway-Op-Sim" as authors are encouraged to develop under this organisation which offers templates to ensure projects are in the correct form. 
There is also the option to specify an alternative branch to "master". Clicking "Ok" will download an archive of the
project and install it.

