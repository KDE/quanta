kde4_header()


########### install files ###############

install_files(${QUANTA_DATA_INSTALL_DIR}/toolbar  FILES  delete.png empty1x16.png java.png mini-book1.png mini-book2.png mini-modules.png tag_br_small.png tag_font_small.png tag_hr_small.png empty16x16.png debug_breakpoint.png debug_connect.png debug_disconnect.png debug_kill.png debug_pause.png debug_currentline.png )

kde4_footer()



#original Makefile.am contents follow:

#quantadir = ${quanta_datadir}/toolbar
#
#quanta_DATA = delete.png empty1x16.png java.png mini-book1.png mini-book2.png mini-modules.png tag_br_small.png tag_font_small.png tag_hr_small.png empty16x16.png debug_breakpoint.png debug_connect.png debug_disconnect.png debug_kill.png debug_pause.png debug_currentline.png
#
