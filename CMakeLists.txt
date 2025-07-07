cmake_minimum_required(VERSION 3.10)
project(con_queue)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)

add_executable(con_queue src/main.cc)

target_include_directories(con_queue PRIVATE
    ${GTKMM_INCLUDE_DIRS}
    ${GSTREAMER_INCLUDE_DIRS})

target_link_libraries(con_queue
    ${GTKMM_LIBRARIES}
    ${GSTREAMER_LIBRARIES})

target_compile_options(con_queue PRIVATE
    ${GTKMM_CFLAGS_OTHER}
    ${GSTREAMER_CFLAGS_OTHER})
