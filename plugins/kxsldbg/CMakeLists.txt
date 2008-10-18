include_directories(
   ${KDevPlatform_INCLUDE_DIR}
   ${KDevPlatform_INCLUDE_DIR}/interfaces
   ${KDevPlatform_INCLUDE_DIR}/shell
   ${KDevPlatform_INCLUDE_DIR}/project
   ${KDevPlatform_INCLUDE_DIR}/outputview
   ${CMAKE_SOURCE_DIR}/kxsldbg/
   ${CMAKE_BINARY_DIR}/kxsldbg/
   ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/
   ${LIBXSLT_INCLUDE_DIR}
   ${LIBXML2_INCLUDE_DIR}
)

########### next target ###############

#setings
# set(kcm_kdev_kxsldbg_PART_SRCS
#       ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/xsldbgsettingsmodel.cpp
#       ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/options_common.cpp
#       ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/utils.cpp
#       ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbgconfigimpl.cpp
#       ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/configdialogprivate.cpp
#       ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbgdialogbase.cpp
#       kcm_kdevkxsldbg.cpp
# )
# 
# set(kcm_kdev_kxsldbg_PART_UI
#       ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbgconfig.ui
# )
# 
# kde4_add_ui_files(kcm_kdev_kxsldbg_PART_SRCS ${kcm_kdev_kxsldbg_PART_UI})
# #kde4_add_kcfg_files( kcm_kdev_kxsldbg_PART_SRCS kxsldbg.kcfgc )
# kde4_add_plugin(kcm_kdev_kxsldbg ${kcm_kdev_kxsldbg_PART_SRCS})
# 
# target_link_libraries(kcm_kdev_kxsldbg
#    kdevquanta
#    kdevplatforminterfaces
#    kdevplatformproject
#    ${KDE4_KUTILS_LIBS}
#    ${LIBXML2_LIBRARIES}
# )
# 
# install(TARGETS kcm_kdev_kxsldbg DESTINATION ${PLUGIN_INSTALL_DIR})
# install(FILES kcm_kdev_kxsldbg.desktop DESTINATION ${SERVICES_INSTALL_DIR})
# 

########### next target ###############


set(libqtnotfier_SRCS
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libqtnotfier/xsldbgthread.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libqtnotfier/xsldbgnotifier.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libqtnotfier/xsldbgevent.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libqtnotfier/xsldbgdebuggerbase.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libqtnotfier/qtnotifier2.cpp
    )

set(libxsldbg_SRCS
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/allmoc.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/xsldbgthread.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/xsldbg.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/xsldbgmsg.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/variable_cmds.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/utils.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/trace_cmds.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/template_cmds.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/search_cmds.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/search.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/param_cmds.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/os_cmds.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/options_unix.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/options.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/options_common.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/option_cmds.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/nodeview_cmds.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/help_unix.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/files_unix.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/files.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/file_cmds.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/debugXSL.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/debug.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/callstack.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/breakpoint_cmds.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/breakpoint.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/arraylist.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/libxsldbg/xsldbgsettingsmodel.cpp
)

set(libkxsldbgcommon_SRCS
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbgwalkspeedimpl.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbgtemplatesimpl.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbgsourcesimpl.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbgvariablesimpl.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbginspector.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbgentitiesimpl.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbgdoc.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbgdialogbase.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbgdebugger.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbgconfigimpl.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbgcallstackimpl.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbgbreakpointsimpl.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/configdialogprivate.cpp
    ${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/qxsldbgdoc.cpp
)

set(kdevkxsldbg_PART_SRCS
   kdevkxsldbgplugin.cpp
   kdevkxsldbgview.cpp
   ${libqtnotfier_SRCS}
   ${libxsldbg_SRCS}
   ${libkxsldbgcommon_SRCS}
)

kde4_add_ui_files(kdevkxsldbg_PART_SRCS
	${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbgbreakpoints.ui
	${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbgcallstack.ui 
	${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbgconfig.ui
	${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbgentities.ui 
	${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbgmsgdialog.ui 
	${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbgvariables.ui
	${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbgsources.ui
	${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbgtemplates.ui
	${CMAKE_SOURCE_DIR}/kxsldbg/kxsldbgpart/xsldbgwalkspeed.ui
)


kde4_add_plugin(kdevkxsldbg WITH_PREFIX ${kdevkxsldbg_PART_SRCS})

target_link_libraries(kdevkxsldbg
   kdevquanta
   kdevplatforminterfaces
   kdevplatformproject
   ${KDE4_KTEXTEDITOR_LIBS}
   exslt
   ${LIBXML2_LIBRARIES}
   ${LIBXSLT_LIBRARIES}
   ${KDE4_KUTILS_LIBS}
   ${KDE4_KDEUI_LIBS}
   ${KDE4_KPARTS_LIBS}
)

########### install files ###############
install(TARGETS kdevkxsldbg  DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES  kdevkxsldbg.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES  kdevkxsldbg.rc DESTINATION ${DATA_INSTALL_DIR}/kdevkxsldbg)

kde4_install_icons( ${ICON_INSTALL_DIR}   )

