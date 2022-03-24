cmake_minimum_required( VERSION 3.20 )

project( RailOSPkgManager VERSION 1.1.1 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( RailOSPkgManager_LIBRARY rospkg )
set( ROSTOOLS_STATIC_LIBRARY ON )

find_package( QT NAMES Qt6 Qt5 )
find_package( Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools Core Gui REQUIRED )

set( MAIN_SRC ${CMAKE_SOURCE_DIR}/application.cxx )
set( EXTERNAL_SOURCES ${CMAKE_SOURCE_DIR}/external )
set( APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/img/appicon.rc" )

option( BUILD_RailOSPkgManager_TESTS "Build the available RailOSPkgManager tests" OFF )

include( ${CMAKE_CURRENT_SOURCE_DIR}/cmake/find_choco.cmake )

file( GLOB SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cxx )
file( GLOB TS_FILES ${CMAKE_SOURCE_DIR}/lang/*.ts )

# Important to add headers so MOC can process them for Q_OBJECT macro
file( GLOB HEADERS ${CMAKE_SOURCE_DIR}/include/*.hxx )

set( DIGESTPP digestpp )
set( 11ZIP elzip )
set( ROSTOOLS rostools )

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DRailOSPkgManager_VERSION=\\\"${CMAKE_PROJECT_VERSION}\\\" " )

add_library( ${RailOSPkgManager_LIBRARY} STATIC ${SRC_FILES} ${HEADERS} )
set_target_properties( ${RailOSPkgManager_LIBRARY} PROPERTIES AUTOMOC ON )
add_subdirectory( ${CMAKE_SOURCE_DIR}/external/11Zip )

set( ELZIP_TESTS OFF )

if( CHOCO_FOUND )
    ChocoFind_CURL()
endif()

find_package( CURL REQUIRED )

set( CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++ -lwinpthread" )
SET( CMAKE_FIND_LIBRARY_SUFFIXES ".a;.dll.a" )
set( BUILD_SHARED_LIBS OFF )

add_subdirectory( ${CMAKE_SOURCE_DIR}/external/rostools/cpp )

include(FetchContent)
FetchContent_Declare(
    ${DIGESTPP}
    GIT_REPOSITORY https://github.com/kerukuro/digestpp.git
    GIT_TAG        "34ff2eeae397ed744d972d86b5a20f603b029fbd"
)
FetchContent_MakeAvailable( ${DIGESTPP} )

target_link_libraries( ${RailOSPkgManager_LIBRARY} PUBLIC ${11ZIP} )
target_link_libraries( ${RailOSPkgManager_LIBRARY} PUBLIC ${CURL_LIBRARIES} )
target_link_libraries( ${RailOSPkgManager_LIBRARY} PUBLIC -static )
target_link_libraries( ${RailOSPkgManager_LIBRARY} PUBLIC Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui )
target_link_libraries( ${RailOSPkgManager_LIBRARY} PUBLIC ${ROSTOOLS} )
target_include_directories( ${RailOSPkgManager_LIBRARY} PRIVATE ${CMAKE_SOURCE_DIR}/include )
target_include_directories( ${RailOSPkgManager_LIBRARY} PUBLIC ${CURL_INCLUDE_DIR} )
target_include_directories( ${RailOSPkgManager_LIBRARY} PUBLIC ${tomlplusplus_SOURCE_DIR} )
target_include_directories( ${RailOSPkgManager_LIBRARY} PUBLIC ${digestpp_SOURCE_DIR} )

add_executable( ${PROJECT_NAME} ${MAIN_SRC} ${APP_ICON_RESOURCE_WINDOWS} )
set_target_properties( ${PROJECT_NAME} PROPERTIES AUTOMOC ON )
target_link_libraries( ${PROJECT_NAME} PRIVATE ${RailOSPkgManager_LIBRARY} )
target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include )

if( BUILD_RailOSPkgManager_TESTS )
    add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/tests )
endif()

set_target_properties( ${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
 )
