add_subdirectory(tests)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(xmlcompletion_SRCS
    sgmlcodecompletionmodel.cpp
)


kde4_add_library( kdevsgmlcompletion SHARED ${xmlcompletion_SRCS} )
target_link_libraries(kdevsgmlcompletion
    ${KDE4_KDECORE_LIBS}
    ${KDEVPLATFORM_LANGUAGE_LIBRARIES}
    ${KDE4_KTEXTEDITOR_LIBS}
    ${KDEVPLATFORM_INTERFACES_LIBRARIES}
    ${KDEVPLATFORM_PROJECT_LIBRARIES}
    ${KDE4_KIO_LIBS}
    kdevsgmlparser
    kdevsgmlduchain
 )

install(TARGETS kdevsgmlcompletion DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})
