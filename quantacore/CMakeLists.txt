find_package(LibXml2 REQUIRED)

add_subdirectory( dialogs )
add_subdirectory( parsers )
add_subdirectory( app_templates )
add_subdirectory( autocomplete )

include_directories( ${LIBXML2_INCLUDE_DIR} )

include_directories( 
   ${CMAKE_SOURCE_DIR}/quanta/lib
   ${CMAKE_SOURCE_DIR}/quanta/quantacore/parsers
   ${CMAKE_SOURCE_DIR}/quanta/quantacore/parsers/dtd
   ${CMAKE_SOURCE_DIR}/quanta/quantacore/dialogs
   ${CMAKE_SOURCE_DIR}/quanta/quantacore/autocomplete
   ${CMAKE_BINARY_DIR}/quanta/lib
   ${KDE4_INCLUDE_DIR}/kdevelop
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
   parsers/parsercommon.cpp
   parsers/parser.cpp
   parsers/parsermanager.cpp
   parsers/sagroupparser.cpp
   parsers/saparser.cpp

   parsers/quantaxmlinputsource.cpp
   parsers/statemachine.cpp
   parsers/parserstatus.cpp
   parsers/stateactions.cpp
   parsers/dombuilder.cpp
   
   parsers/dtd/dtdparser.cpp

   autocomplete/completionbase.cpp
   autocomplete/dtdcompletion.cpp
   autocomplete/specialareaattributecompletion.cpp
   autocomplete/specialareagroupcompletion.cpp
   autocomplete/specialareatagcompletion.cpp
   autocomplete/xmlattributecompletion.cpp
   autocomplete/xmlattributevaluecompletion.cpp
   autocomplete/xmlentitycompletion.cpp
   autocomplete/xmltagcompletion.cpp
)

kde4_add_ui_files(kdevquantacore_PART_SRCS
dialogs/autocompletionconfigbase.ui
dialogs/dtepeditdlgs.ui
dialogs/dtepstructureeditdlgs.ui
dialogs/dtdselectdialog.ui
dialogs/donationdialog.ui
dialogs/environmentconfigbase.ui

parsers/dtd/dtepcreationdlg.ui
)   

kde4_automoc(${kdevquantacore_PART_SRCS})

kde4_add_plugin(kdevquantacore WITH_PREFIX ${kdevquantacore_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kdevquantacore )

target_link_libraries(kdevquantacore kdevquanta
kdevplatform kdevutil ktexteditor
${LIBXML2_LIBRARIES} ${KDE4_KDECORE_LIBS})

install(TARGETS kdevquantacore  DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install( FILES  kdevquantacore.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES  kdevquantacore.rc  DESTINATION ${DATA_INSTALL_DIR}/kdevquantacore)


