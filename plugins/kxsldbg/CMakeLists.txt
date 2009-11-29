find_package(Threads REQUIRED)

include_directories(
   ${KDEVPLATFORM_INCLUDE_DIR}
   ${KDEVPLATFORM_INCLUDE_DIR}/interfaces
   ${KDEVPLATFORM_INCLUDE_DIR}/shell
   ${KDEVPLATFORM_INCLUDE_DIR}/project
   ${KDEVPLATFORM_INCLUDE_DIR}/outputview
   ${quanta_SOURCE_DIR}/..//kxsldbg/
   ${quanta_BINARY_DIR}/..//kxsldbg/
   ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/
   ${LIBXSLT_INCLUDE_DIR}
   ${LIBXML2_INCLUDE_DIR}
)

########### next target ###############

#setings
# set(kcm_kdev_kxsldbg_PART_SRCS
#       ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/xsldbgsettingsmodel.cpp
#       ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/options_common.cpp
#       ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/utils.cpp
#       ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbgconfigimpl.cpp
#       ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/configdialogprivate.cpp
#       ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbgdialogbase.cpp
#       kcm_kdevkxsldbg.cpp
# )
# 
# set(kcm_kdev_kxsldbg_PART_UI
#       ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbgconfig.ui
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
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libqtnotfier/xsldbgthread.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libqtnotfier/xsldbgnotifier.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libqtnotfier/xsldbgevent.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libqtnotfier/xsldbgdebuggerbase.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libqtnotfier/qtnotifier2.cpp
    )

set(libxsldbg_SRCS
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/allmoc.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/xsldbgthread.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/xsldbg.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/xsldbgmsg.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/variable_cmds.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/utils.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/trace_cmds.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/template_cmds.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/search_cmds.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/search.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/param_cmds.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/os_cmds.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/options_unix.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/options.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/options_common.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/option_cmds.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/nodeview_cmds.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/help_unix.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/files_unix.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/files.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/file_cmds.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/debugXSL.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/debug.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/callstack.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/breakpoint_cmds.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/breakpoint.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/arraylist.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/libxsldbg/xsldbgsettingsmodel.cpp
)

set(libkxsldbgcommon_SRCS
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbgwalkspeedimpl.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbgtemplatesimpl.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbgsourcesimpl.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbgvariablesimpl.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbginspector.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbgentitiesimpl.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbgdoc.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbgdialogbase.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbgdebugger.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbgconfigimpl.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbgcallstackimpl.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbgbreakpointsimpl.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/configdialogprivate.cpp
    ${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/qxsldbgdoc.cpp
)

set(kdevkxsldbg_PART_SRCS
   kdevkxsldbgplugin.cpp
   kdevkxsldbgview.cpp
   ${libqtnotfier_SRCS}
   ${libxsldbg_SRCS}
   ${libkxsldbgcommon_SRCS}
)

kde4_add_ui_files(kdevkxsldbg_PART_SRCS
	${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbgbreakpoints.ui
	${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbgcallstack.ui 
	${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbgconfig.ui
	${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbgentities.ui 
	${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbgmsgdialog.ui 
	${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbgvariables.ui
	${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbgsources.ui
	${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbgtemplates.ui
	${quanta_SOURCE_DIR}/..//kxsldbg/kxsldbgpart/xsldbgwalkspeed.ui
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
   ${CMAKE_THREAD_LIBS_INIT}
)

########### install files ###############
install(TARGETS kdevkxsldbg  DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES  kdevkxsldbg.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES  kdevkxsldbg.rc DESTINATION ${DATA_INSTALL_DIR}/kdevkxsldbg)

kde4_install_icons( ${ICON_INSTALL_DIR}   )

