
include_directories(
  ${CMAKE_SOURCE_DIR}/quanta/lib
  ${CMAKE_BINARY_DIR}
  ${KDE4_INCLUDE_DIR}/kdevelop
  ${KDE4_INCLUDE_DIR}/kdevelop/interfaces
  ${KDE4_INCLUDE_DIR}/kdevelop/shell
  ${KDE4_INCLUDE_DIR}/kdevelop/shell/profileengine
  
)


########### next target ###############

set(quanta_SRCS main.cpp kdevquantaextension.cpp )

kde4_automoc(${quanta_SRCS})

kde4_add_executable(quanta ${quanta_SRCS})

target_link_libraries(quanta  ${KDE4_KDECORE_LIBS} kdevquanta kdevshell )

install(TARGETS quanta  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES  quanta.desktop  DESTINATION ${XDG_APPS_DIR})
install( FILES  profile.config  DESTINATION ${DATA_INSTALL_DIR}/kdevelop/profiles/IDE/quanta)
install( FILES  x-quanta.desktop  DESTINATION ${MIME_INSTALL_DIR}/application)
install( FILES  quantarc quantaui.rc  DESTINATION ${DATA_INSTALL_DIR}/quanta)
install( FILES  kdevelopquanta.desktop  DESTINATION ${SERVICETYPES_INSTALL_DIR})




#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/lib -I$(kde_includes)/kinterfacedesigner \
#	-I$(kde_includes)/kdevelop/shell/profileengine -I$(kde_includes)/kdevelop/interfaces -I$(kde_includes)/kdevelop/shell \
#	$(all_includes)
#METASOURCES = AUTO
#bin_PROGRAMS = quanta
#quanta_LDFLAGS = -avoid-version -L$(top_builddir)/lib $(all_libraries)
#quanta_SOURCES = main.cpp kdevquantaextension.cpp
#noinst_HEADERS = kdevquantaextension.h
#quanta_LDADD = -lkdevquanta -lkdevshell
#
#rcdir = $(kde_datadir)/quanta
#
#rc_DATA = quantarc quantaui.rc
#
#xdg_apps_DATA = quanta.desktop
#
#servicedir = $(kde_servicesdir)
#
#servicetypedir = $(kde_servicetypesdir)
#servicetype_DATA = kdevelopquanta.desktop
#
#profiledir = $(kde_datadir)/kdevelop/profiles/quanta
#profile_DATA = profile.config
#
#kdemimedir = $(kde_mimedir)/application
#kdemime_DATA = x-quanta.desktop
