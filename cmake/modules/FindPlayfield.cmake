find_path(PLAYFIELD_INCLUDE_DIR playfield/playfield.h)

find_library(PLAYFIELD_LIBRARY NAMES playfield)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PLAYFIELD DEFAULT_MSG PLAYFIELD_INCLUDE_DIR)

set(PLAYFIELD_INCLUDE_DIRS ${PLAYFIELD_INCLUDE_DIR})
set(PLAYFIELD_LIBRARIES ${PLAYFIELD_LIBRARY})
