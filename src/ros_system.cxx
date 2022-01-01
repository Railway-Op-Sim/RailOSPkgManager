#include "ros_system.hxx"

ROSPkg::System::System(QWidget* parent) {
    parent_ = parent;
    const QString cache_dir_ = QFileInfo(cache_file_).absolutePath();
    if(!QDir(cache_file_).exists()) {
        const QString ros_loc_ = QFileDialog::getOpenFileName(
            parent_,
            QFileDialog::tr("Find ROS"),
            QString(QStandardPaths::HomeLocation), QFileDialog::tr("ROS Exe (railway.exe)")
        );
    }
}
