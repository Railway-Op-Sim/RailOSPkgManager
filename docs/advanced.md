# Advanced Features
ROS Package Manager comes with some advanced features which are hidden by default but can be enabled with the "Advanced" checkbox on the main screen.

## GitHub Repository Install
ROS Package Manager supports the ability to install packages directly from a repository. For example you can install
projects from the Railway Operation Simulator Developer GitHub [ROSDGH](https://github.com/Railway-Op-Sim) by clicking the "GitHub" button after enabling advanced features.

To install from a repository you must provide the name in the form `<USER>/<REPOSITORY>`, by default the `USER` is set to be the ROSDGH `Railway-Op-Sim` username. Optionally you may specify the branch to install from if this is not `master`.

!!! important "URL Download"
    The URL `https://github.com/<USER>/<PROJECT>/archive/refs/heads/<BRANCH>.zip` is used to retrieve the project files.

!!! important "Error 22"
    If you perform above a certain number of download requests in a given time you are likely to receive an Error 22. This is
    from the GitHub server itself. This feature is for developer use only, all normal add-on installations should be performed
    from a zip file downloaded from the main Railway Operation Simulator website. 

<figure markdown>
![gh_install](media/gh_install.png)
</figure>
<figure markdown>
![gh_success](media/gh_success.png)
</figure>

## Setting ROS Location
When the program is first launched you will be asked for the location of your local Railway Operation Simulator installation. If however you want to change this at a later point you can do so using the "Set ROS" button under the advanced options.