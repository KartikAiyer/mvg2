find_package(fmt)
find_package(spdlog)
find_package(docopt)
find_package(opencv)
find_package(libheif)

# Generic test that uses conan libs
add_executable(mvg2 main.cpp)
target_link_libraries(
  mvg2
  PRIVATE project_options
          project_warnings
          docopt::docopt
          fmt::fmt
          spdlog::spdlog
          opencv::opencv
          libheif::libheif
          assets_lib)
