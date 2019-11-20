# Try to find the sqlite3 library.
# This will define:
#
# sqlite3_FOUND       - sqlite3 library is available
# sqlite3_INCLUDE_DIR - Where the sqlite3.h header file is
# sqlite_LIBRARIES   - The libraries to link in.

find_library(sqlite3_LIBRARIES NAMES sqlite3 PATHS
        /usr
        /usr/local
        /opt
        )

find_path(sqlite3_INCLUDE_DIR sqlite3.h PATHS
        /usr
        /usr/local
        /opt
        PATH_SUFFIXES
        )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(sqlite3 DEFAULT_MSG
        sqlite3_LIBRARIES
        sqlite3_INCLUDE_DIR
        )

mark_as_advanced(
        sqlite3_LIBRARIES
        sqlite3_INCLUDE_DIR
)
