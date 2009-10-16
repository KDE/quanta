find_package(KDE4 REQUIRED)
find_package(KDevPlatform REQUIRED)

add_definitions( -DKDE_DEFAULT_DEBUG_AREA=9515 )

include_directories(
    ${KDEVPLATFORM_INCLUDE_DIR}
    ${KDE4_INCLUDES}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

########### next target ###############

set(kdevexecutebrowser_PART_UIS
    browserappconfig.ui
)

set(kdevexecutebrowser_PART_SRCS
    executebrowserplugin.cpp
    browserappconfig.cpp
    browserappjob.cpp
)

kde4_add_ui_files( kdevexecutebrowser_PART_SRCS ${kdevexecutebrowser_PART_UIS} )
kde4_add_plugin(kdevexecutebrowser ${kdevexecutebrowser_PART_SRCS}  )
target_link_libraries(kdevexecutebrowser 
    ${KDE4_KDEUI_LIBS} 
    kdevplatforminterfaces 
    kdevplatformutil 
    kdevplatformproject 
    kdevplatformoutputview)

install(TARGETS kdevexecutebrowser DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############
install(FILES iexecutebrowserplugin.h DESTINATION ${INCLUDE_INSTALL_DIR}/kdevexecutebrowser COMPONENT Devel)
install(FILES kdevexecutebrowser.desktop DESTINATION ${SERVICES_INSTALL_DIR})
