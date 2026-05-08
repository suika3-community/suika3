find_path(NOCT_INCLUDE_DIR noct/noct.h)

find_library(NOCT_LIBRARY NAMES noct noctapi)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Noct DEFAULT_MSG NOCT_LIBRARY NOCT_INCLUDE_DIR)

set(NOCT_LIBRARIES ${NOCT_LIBRARY})
set(NOCT_INCLUDE_DIRS ${NOCT_INCLUDE_DIR})
