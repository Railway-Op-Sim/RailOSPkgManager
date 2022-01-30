# Creating Packages
## The Editor Form
In order create a package there is a minimum requirement for files and metadata that must be provided
to meet the standard. The built in editor form contains all the required fields for creating a
metadata file to be included within the package. This file provides the means of linking together
all the components of a package, as well as defines extra information such as version number.

Those fields requiring one or more file path have buttons for browsing the local file system.

<figure markdown>
![pkg_editor](media/package_editor.png)
</figure>

## Mandatory Requirements
Requirements for package creation are quite strict, the idea being a package should be accessible to
all using ROS where possible:

- Documentation MUST be provided and should be either a markdown file `.md` or a PDF `.pdf`, formats like `.docx` are not accepted due to them not being universal.
- You must provide only a single `.rly` file per package.
- Do NOT add a file to a package that is used by another simulation, this file will be removed whenever the package is uninstalled breaking any other simulations which require it.

!!! important "Image Inclusion"
    In future releases image files will also be included as an additional field. Currently this is not supported.

### Editor Form Fields

|**Field**|**Description**|
|---|---|
|Package Name|Name of the package as it would appear on a file system.|
|Display Name|Name of the package as it would appear on a listing.|
|Description|A brief description of the contained simulation.|
|Version|A [semantic version](https://semver.org) of the form `MAJOR.MINOR.PATCH` for this package.|
|Author|The leading author of this project.|
|Contributors|(Optional) Additional authors (separated by `,`).|
|Railway File|Single `.rly` for this simulation.|
|Timetable Files|All `.ttb` files for this simulation (separated by `,`).|
|Session Files|(Optional) All `.ssn` files for this simulation (separated by `,`).|
|Documentation Files|Documentation files (`.pdf`, `.md`) for this simulation (separated by `,`).|
|Graphic Files|(Optional) Graphics files used in map (separated by `,`).|
|Image Files|Screenshot image files to include (separated by `,`).|
|Country Code|Two letter country code of route origin in this simulation (`FN` if fictional).|
|Factual|If simulation is a factual representation.|
|Year|Year for which this simulation is a representation (can be any value if fictional).|
