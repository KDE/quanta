PROJECT(sql)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${sql_SOURCE_DIR}/cmake)

enable_testing()

find_package(KDE4 REQUIRED)
find_package(KDevPlatform REQUIRED)

include_directories(
    ${KDEVPLATFORM_INCLUDE_DIR}
    ${KDE4_INCLUDES}
    ${QT4_INCLUDES}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(kdevsqllanguagesupport_PART_SRCS resulttablewidget.cpp
    sqllanguagesupport.cpp
)

kde4_add_plugin(kdevsqllanguagesupport ${kdevsqllanguagesupport_PART_SRCS})

target_link_libraries(kdevsqllanguagesupport
    ${QT_QTSQL_LIBRARY}
    ${KDE4_KDEUI_LIBS}
    ${KDEVPLATFORM_INTERFACES_LIBRARIES}
    ${KDEVPLATFORM_LANGUAGE_LIBRARIES}
)

install(TARGETS kdevsqllanguagesupport DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES kdevsqlsupport.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES kdevsqlui.rc DESTINATION ${DATA_INSTALL_DIR}/kdevsqlsupport )
