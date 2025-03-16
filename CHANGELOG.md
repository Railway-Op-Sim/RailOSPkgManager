# Unreleased
- Switch from 11zip to Quazip for package creation backend.

# 2023-01-29 [v1.1.2](https://github.com/Railway-Op-Sim/RailOSPkgManager/releases/tag/v1.1.2)
- Fix graphics files not being copied
# 2022-03-24 [v1.1.1](https://github.com/Railway-Op-Sim/RailOSPkgManager/releases/tag/v1.1.1)
- Fixes an issue whereby a file may be copied to itself when building a package, also fixes tabbing through entries order.
# 2021-02-10 [v1.1.0](https://github.com/Railway-Op-Sim/RailOSPkgManager/releases/tag/v1.1.0)
- Add ability to change ROS location at a later point.
# 2021-01-19 [v0.5.5](https://github.com/Railway-Op-Sim/RailOSPkgManager/releases/tag/v0.5.5)
- Fixed curl issues when retrieving add ons from GitHub.
- Fixed bad file type in browser windows listing during package creation.
- Uses latest `rostools` with validation removed on import to prevent error throws.
- Adds icon.
# 2021-01-19 [v0.5.0](https://github.com/Railway-Op-Sim/RailOSPkgManager/releases/tag/v0.5.0)
- Add ability to upgrade base program version from the package manager.
# 2021-01-18 [v0.4.2](https://github.com/Railway-Op-Sim/RailOSPkgManager/releases/tag/v0.4.2)
- Working release for Windows
# 2021-01-06 [v0.4.1](https://github.com/Railway-Op-Sim/RailOSPkgManager/releases/tag/v0.4.1)
- Fix vulnerability under CURL.
- Fix bug in file closure after download.

# 2021-01-06 [v0.4.0](https://github.com/Railway-Op-Sim/RailOSPkgManager/releases/tag/v0.4.0)
- Added ability to install from a github project in the form "user/project-name".
- Few tidy ups in information extraction.
- Fixed documentation install/uninstall.
# 2021-01-05 [v0.3.0](https://github.com/Railway-Op-Sim/RailOSPkgManager/releases/tag/v0.3.0)
- Switch from AbZip to 11Zip library for archives
- Now builds under Windows
# 2021-01-03 [v0.2.0](https://github.com/Railway-Op-Sim/RailOSPkgManager/releases/tag/v0.2.0)
- Added package creator. User can create packages in the standard form, this also builds the new standard metadata TOML file.

# 2021-01-03 [v0.1.0](https://github.com/Railway-Op-Sim/RailOSPkgManager/releases/tag/v0.1.0)
- Install ROS add-ons from a zip archive.
- Remove add-ons, relevant files being tracked by the created package metadata file which follows the new ROS standard metadata TOML format.
