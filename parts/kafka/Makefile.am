  SUBDIRS = . pics

  INCLUDES = $(all_includes)
  KDE_CXXFLAGS = $(USE_EXCEPTIONS)
  lib_LTLIBRARIES = libkafkalibrary.la

  libkafkalibrary_la_SOURCES = htmltranslator.cpp domtreeview.cpp kafkacommon.cpp kafkahtmlpart.cpp kafkahtmlview.cpp nodeproperties.cpp wkafkapart.cpp undoredo.cpp
  libkafkalibrary_la_METASOURCES = AUTO
  libkafkalibrary_la_LDFLAGS = $(all_libraries)
  libkafkalibrary_la_LIBADD = $(LIB_KHTML)
