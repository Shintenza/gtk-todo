cmake_minimum_required(VERSION 3.20)

project(c_todo)
set(SOURCE_FILES main.c add_data.c connect_db.c)
find_package(PkgConfig REQUIRED)
find_package (mongoc-1.0 1.7 REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)

include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${GTK_LIBRARIES} mongo::mongoc_shared)
