cmake_minimum_required( VERSION 3.20 )

project( ROSPkgManager VERSION 1.0.0 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( ROSPKGMANAGER_LIBRARY rospkg )

find_package( QT NAMES Qt6 Qt5 )
find_package( Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools Core Gui REQUIRED )

set( MAIN_SRC ${CMAKE_SOURCE_DIR}/application.cxx )
set( EXTERNAL_SOURCES ${CMAKE_SOURCE_DIR}/external )
set( APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/img/appicon.rc" )

option( BUILD_ROSPKGMANAGER_TESTS "Build the available ROSPkgManager tests" OFF )

file( GLOB SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cxx )
file( GLOB TS_FILES ${CMAKE_SOURCE_DIR}/lang/*.ts )

# Important to add headers so MOC can process them for Q_OBJECT macro
file( GLOB HEADERS ${CMAKE_SOURCE_DIR}/include/*.hxx )

set( DIGESTPP digestpp )
set( 11ZIP elzip )
set( ROSTOOLS rostools )

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DROSPKGMANAGER_VERSION=\\\"${CMAKE_PROJECT_VERSION}\\\" " )

add_library( ${ROSPKGMANAGER_LIBRARY} STATIC ${SRC_FILES} ${HEADERS} )
set_target_properties( ${ROSPKGMANAGER_LIBRARY} PROPERTIES AUTOMOC ON )
add_subdirectory( ${CMAKE_SOURCE_DIR}/external/11Zip )

find_package( CURL )
set( ELZIP_TESTS OFF )

if( NOT CURL_FOUND )
    add_subdirectory( ${CMAKE_SOURCE_DIR}/external/curl )
    set_target_properties( curl PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1 )
    set( CURL_LIBRARIES libcurl )
    set( CURL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/curl/lib/include )
else()
    set( CURL_LIBRARIES curl )
endif()

add_subdirectory( ${CMAKE_SOURCE_DIR}/external/rostools/cpp )

include(FetchContent)
FetchContent_Declare(
    ${DIGESTPP}
    GIT_REPOSITORY https://github.com/kerukuro/digestpp.git
    GIT_TAG        "34ff2eeae397ed744d972d86b5a20f603b029fbd"
)
FetchContent_MakeAvailable( ${DIGESTPP} )

target_link_libraries( ${ROSPKGMANAGER_LIBRARY} PUBLIC ${11ZIP} )
target_link_libraries( ${ROSPKGMANAGER_LIBRARY} PUBLIC ${CURL_LIBRARIES} )
target_link_libraries( ${ROSPKGMANAGER_LIBRARY} PUBLIC Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui )
target_link_libraries( ${ROSPKGMANAGER_LIBRARY} PUBLIC ${ROSTOOLS} )
target_include_directories( ${ROSPKGMANAGER_LIBRARY} PRIVATE ${CMAKE_SOURCE_DIR}/include )
target_include_directories( ${ROSPKGMANAGER_LIBRARY} PUBLIC ${CURL_INCLUDE_DIR} )
target_include_directories( ${ROSPKGMANAGER_LIBRARY} PUBLIC ${tomlplusplus_SOURCE_DIR} )
target_include_directories( ${ROSPKGMANAGER_LIBRARY} PUBLIC ${digestpp_SOURCE_DIR} )

add_executable( ${PROJECT_NAME} ${MAIN_SRC} ${APP_ICON_RESOURCE_WINDOWS} )
set_target_properties( ${PROJECT_NAME} PROPERTIES AUTOMOC ON )
target_link_libraries( ${PROJECT_NAME} PRIVATE ${ROSPKGMANAGER_LIBRARY} )
target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include )

if( BUILD_ROSPKGMANAGER_TESTS )
    add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/tests )
endif()

set_target_properties( ${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
 )
