kde4_header()

set(QUANTA_DATA_INSTALL_DIR ${DATA_INSTALL_DIR}/quanta CACHE STRING "The Quanta data dir")
project(quanta)

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(quantacore)
add_subdirectory(plugins)
add_subdirectory(data)

kde4_footer()

