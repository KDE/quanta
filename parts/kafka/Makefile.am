  SUBDIRS = . pics

  INCLUDES = -I$(top_srcdir) -I$(top_srcdir)/quanta/parts/kafka  $(all_includes)
  KDE_CXXFLAGS = $(USE_EXCEPTIONS)
  lib_LTLIBRARIES = libkafkalibrary.la

  libkafkalibrary_la_SOURCES = htmlenhancer.cpp domtreeview.cpp kafkacommon.cpp kafkahtmlpart.cpp kafkahtmlview.cpp nodeproperties.cpp wkafkapart.cpp undoredo.cpp kafkasyncoptionsui.ui kafkasyncoptions.cpp
  libkafkalibrary_la_METASOURCES = AUTO
  libkafkalibrary_la_LDFLAGS = $(all_libraries)
  libkafkalibrary_la_LIBADD = $(LIB_KHTML)
