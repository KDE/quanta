add_subdirectory(tests)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(xmlcompletion_SRCS
    codecompletionmodel.cpp
)


kde4_add_library( kdevxmlcompletion SHARED ${xmlcompletion_SRCS} )
target_link_libraries(kdevxmlcompletion
    ${KDE4_KDECORE_LIBS}
    ${KDEVPLATFORM_LANGUAGE_LIBRARIES}
    ${KDE4_KTEXTEDITOR_LIBS}
    ${KDEVPLATFORM_INTERFACES_LIBRARIES}
    ${KDEVPLATFORM_PROJECT_LIBRARIES}
    ${KDE4_KIO_LIBS}
    kdevxmlparser
 )

install(TARGETS kdevmlcompletion DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})
