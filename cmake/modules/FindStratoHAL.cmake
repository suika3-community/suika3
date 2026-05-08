find_path(STRATO_INCLUDE_DIR stratohal/platform.h)

find_library(STRATO_LIBRARY NAMES strato stratopack)
find_library(STRATOPACK_LIBRARY NAMES stratopack)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(StratoHAL DEFAULT_MSG STRATO_LIBRARY STRATOPACK_LIBRARY STRATO_INCLUDE_DIR)

set(STRATO_LIBRARIES ${STRATO_LIBRARY} ${STRATOPACK_LIBRAY})
set(STRATO_INCLUDE_DIRS ${STRATO_INCLUDE_DIR})
