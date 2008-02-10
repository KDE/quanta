
set(QUANTA_DATA_INSTALL_DIR ${DATA_INSTALL_DIR}/quanta CACHE STRING "The Quanta data dir")
project(quanta)
add_definitions ( -DKDE_DEFAULT_DEBUG_AREA=24000 )


add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(quantacore)
add_subdirectory(plugins)
add_subdirectory(data)
add_subdirectory(scripts)


