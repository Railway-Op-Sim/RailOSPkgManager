cmake_minimum_required( VERSION 3.20 )

project( ROSPkgManager VERSION 0.1.0 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC ON )
set( LIBRARY_NAME rospkg )

find_package( QT NAMES Qt6 Qt5 )
find_package( Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools Core Gui REQUIRED )

set( MAIN_SRC ${CMAKE_SOURCE_DIR}/application.cxx )
set( EXTERNAL_SOURCES ${CMAKE_SOURCE_DIR}/external )

# ------------------------- AbZip ---------------------------- #
file( GLOB_RECURSE ABZIP_SRCS ${EXTERNAL_SOURCES}/AbZip/*.cpp )
file( GLOB_RECURSE ABZIP_HEADERS ${EXTERNAL_SOURCES}/AbZip/*.h )
find_package( ZLIB REQUIRED )
set( ABZIP abzip )
set( ABZIP_INCLUDE_DIR ${EXTERNAL_SOURCES}/AbZip )
add_library( ${ABZIP} ${ABZIP_SRCS} ${ABZIP_HEADERS} )
target_include_directories( ${ABZIP} PRIVATE ${ABZIP_INCLUDE_DIR} )
target_include_directories( ${ABZIP} PRIVATE ${ABZIP_INCLUDE_DIR}/Compressors )
target_include_directories( ${ABZIP} PRIVATE ${ABZIP_INCLUDE_DIR}/Encryption )
target_link_libraries( ${ABZIP} PRIVATE ${ZLIB_LIBRARIES} )
target_link_libraries( ${ABZIP} PUBLIC Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui )

# ------------------------------------------------------------ #

file( GLOB SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cxx )
file( GLOB TS_FILES ${CMAKE_SOURCE_DIR}/lang/*.ts )

# Important to add headers so MOC can process them for Q_OBJECT macro
file( GLOB HEADERS ${CMAKE_SOURCE_DIR}/include/*.hxx )

set( DIGESTPP digestpp )

set( ROSTOOLS rostools )
find_package( ${ROSTOOLS} 0.4.0 REQUIRED )

add_library( ${LIBRARY_NAME} STATIC ${SRC_FILES} ${HEADERS} )

include(FetchContent)
FetchContent_Declare(
    ${DIGESTPP}
    GIT_REPOSITORY https://github.com/kerukuro/digestpp.git
    GIT_TAG        "34ff2eeae397ed744d972d86b5a20f603b029fbd"
)
FetchContent_MakeAvailable( ${DIGESTPP} )

target_link_libraries( ${LIBRARY_NAME} PUBLIC Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui ${ABZIP} ${ROSTOOLS_LIBRARIES} )
target_include_directories( ${LIBRARY_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include )
target_include_directories( ${LIBRARY_NAME} PUBLIC ${tomlplusplus_SOURCE_DIR} )
target_include_directories( ${LIBRARY_NAME} PUBLIC ${EXTERNAL_SOURCES} )
target_link_libraries( ${LIBRARY_NAME} PUBLIC ${ROSTOOLS} )
target_include_directories( ${LIBRARY_NAME} PUBLIC ${digestpp_SOURCE_DIR} )

add_executable( ${PROJECT_NAME} ${MAIN_SRC} )
target_link_libraries( ${PROJECT_NAME} PRIVATE ${LIBRARY_NAME} )
target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include )

set_target_properties( ${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
 )
