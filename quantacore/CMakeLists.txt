find_package(LibXml2 REQUIRED)

add_subdirectory( app_templates )
# TODO PORT
#add_subdirectory( autocomplete )

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  ${KDE4_ENABLE_EXCEPTIONS}")

include_directories( ${LIBXML2_INCLUDE_DIR} )

include_directories( 
   ${quanta_SOURCE_DIR}/lib
   ${quanta_SOURCE_DIR}/quantacore/parsers
   ${quanta_SOURCE_DIR}/quantacore/parsers/dtd
   ${quanta_SOURCE_DIR}/quantacore/dialogs
   ${quanta_SOURCE_DIR}/quantacore/autocomplete
   ${quanta_BINARY_DIR}/lib
   ${KDEVPLATFORM_INCLUDE_DIR}
   ${KDEVPLATFORM_INCLUDE_DIR}/shell
   ${KDEVPLATFORM_INCLUDE_DIR}/interfaces
   )


########### next target ###############

set(kdevquantacore_PART_SRCS 
   quantacorepart.cpp 
   dtds.cpp 
   quantacommon.cpp 
   quantadoc.cpp
   dialogs/autocompletionconfig.cpp
   dialogs/dtepeditdlg.cpp
   dialogs/environmentconfig.cpp

   parsers/comparator.cpp
#   parsers/parsercommon.cpp
#   parsers/parser.cpp
   parsers/parsermanager.cpp
#   parsers/sagroupparser.cpp
#   parsers/saparser.cpp

   parsers/quantaxmlinputsource.cpp
   parsers/statemachine.cpp
   parsers/parserstatus.cpp
   parsers/stateactions.cpp
   parsers/dombuilder.cpp
#   parsers/dommodel.cpp
   
   parsers/dtd/dtdparser.cpp
)

# TODO PORT
#  autocomplete/completionbase.cpp
#  autocomplete/dtdcompletion.cpp
#  autocomplete/specialareaattributecompletion.cpp
#  autocomplete/specialareagroupcompletion.cpp
#  autocomplete/specialareatagcompletion.cpp
#  autocomplete/xmlattributecompletion.cpp
#  autocomplete/xmlattributevaluecompletion.cpp
#  autocomplete/xmlentitycompletion.cpp
#  autocomplete/xmltagcompletion.cpp


kde4_add_ui_files(kdevquantacore_PART_SRCS
dialogs/autocompletionconfigbase.ui
dialogs/dtepeditdlgs.ui
dialogs/dtepstructureeditdlgs.ui
dialogs/dtdselectdialog.ui
dialogs/donationdialog.ui
dialogs/environmentconfigbase.ui

parsers/dtd/dtepcreationdlg.ui
)   


kde4_add_plugin(kdevquantacore WITH_PREFIX ${kdevquantacore_PART_SRCS})



target_link_libraries(kdevquantacore
   kdevquanta
   ${KDEVPLATFORM_INTERFACES_LIBRARIES}
   ${KDEVPLATFORM_SHELL_LIBRARIES}
   ktexteditor
   ${KDE4_KHTML_LIBS}
   ${LIBXML2_LIBRARIES}
   ${KDE4_KDECORE_LIBS}
   ${QT_QT3SUPPORT_LIBRARY}
)

install(TARGETS kdevquantacore  DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install( FILES  kdevquantacore.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES  kdevquantacore.rc  DESTINATION ${DATA_INSTALL_DIR}/kdevquantacore)


