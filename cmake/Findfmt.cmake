find_path(FMT_INCLUDE_DIR NAMES fmt/format.h)
find_file(FMT_SRC fmt/format.h)
find_library(FMT_LIBRARY NAMES /cmake/fmt/)
find_file(CONFIG /cmake/fmt/fmt-config.cmake)


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FMT DEFAULT_MSG
FMT_INCLUDE_DIR
FMT_SRC
CONFIG
)
