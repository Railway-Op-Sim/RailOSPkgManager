#!/usr/bin/bash
if [ ! -f "$PWD/src/railos_packager.cxx" ]; then
    echo "ERROR: This build script must be run from the source directory"
    exit 1
fi
QT_MINGW_CMAKE_DIR=$(ls C:/Qt/*/mingw_*/lib/cmake -d | cut -d ' ' -f 1 | xargs )
QT_TOOLS_BIN=$(ls C:/Qt/Tools/mingw*/bin -d | cut -d ' ' -f 1 | xargs)
QT_MINGW_BIN=$(ls C:/Qt/*/mingw_*/bin -d | cut -d ' ' -f 1 | xargs )
echo "Using $QT_MINGW_CMAKE_DIR for Cmake prefix..."
echo "Using $QT_TOOLS_BIN for compilers..."
echo "Using $QT_MINGW_BIN for deployment..."
rm -rf build && rm -rf RailosPkgManager_Release/ && 
PATH=$QT_TOOLS_BIN:$PATH CXX=$QT_TOOLS_BIN/g++.exe CC=$QT_TOOLS_BIN/gcc.exe cmake -Bbuild -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=$QT_MINGW_CMAKE_DIR -DCMAKE_BUILD_TYPE=Release &&
cmake --build build &&
mkdir -p RailOSPkgManager_Release && 
cp build/RailOSPkgManager.exe RailOSPkgManager_Release/ &&
$QT_MINGW_BIN/windeployqt.exe RailOSPkgManager_Release/RailOSPkgManager.exe
cp $QT_MINGW_BIN/libstdc++*.dll RailosPkgManager_Release/
cp $QT_MINGW_BIN/libwinpthread*.dll RailosPkgManager_Release/
cp $PWD/release/libcurl*.dll RailosPkgManager_Release/
