kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES}  )


########### install files ###############

install_files(${QUANTA_DATA_INSTALL_DIR}/dtep/xhtml-basic  FILES  base.tag body.tag description.rc form.tag headings.tag input.tag              link.tag ol.tag p.tag table.tag textarea.tag tr.tag a.tag blockquote.tag              br.tag div.tag head.tag html.tag li.tag meta.tag option.tag select.tag              td.tag th.tag ul.tag common.tag )

kde4_footer()



#original Makefile.am contents follow:

#quanta_DATA = base.tag body.tag description.rc form.tag headings.tag input.tag \
#            link.tag ol.tag p.tag table.tag textarea.tag tr.tag a.tag blockquote.tag \
#            br.tag div.tag head.tag html.tag li.tag meta.tag option.tag select.tag \
#            td.tag th.tag ul.tag common.tag
#quantadir = ${quanta_datadir}/dtep/xhtml-basic
#
