kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES}  )


########### install files ###############

install_files(${QUANTA_DATA_INSTALL_DIR}/dtep/xhtml-strict  FILES  a.tag area.tag base.tag blockquote.tag body.tag br.tag col.tag colgroup.tag div.tag empty.tag form.tag head.tag headings.tag hr.tag html.tag input.tag li.tag link.tag meta.tag ol.tag option.tag p.tag script.tag select.tag style.tag table.tag tbody.tag td.tag textarea.tag tfoot.tag th.tag thead.tag tr.tag ul.tag description.rc abbr.tag acronym.tag address.tag b.tag bdo.tag big.tag button.tag caption.tag cite.tag code.tag dd.tag del.tag dfn.tag dl.tag dt.tag em.tag fieldset.tag i.tag img.tag ins.tag kbd.tag label.tag legend.tag map.tag noscript.tag object.tag optgroup.tag param.tag pre.tag q.tag samp.tag small.tag span.tag strong.tag sub.tag sup.tag title.tag tt.tag var.tag common.tag )

kde4_footer()



#original Makefile.am contents follow:

#quanta_DATA = a.tag area.tag base.tag blockquote.tag body.tag br.tag col.tag colgroup.tag div.tag empty.tag form.tag head.tag headings.tag hr.tag html.tag input.tag li.tag link.tag meta.tag ol.tag option.tag p.tag script.tag select.tag style.tag table.tag tbody.tag td.tag textarea.tag tfoot.tag th.tag thead.tag tr.tag ul.tag description.rc abbr.tag acronym.tag address.tag b.tag bdo.tag big.tag button.tag caption.tag cite.tag code.tag dd.tag del.tag dfn.tag dl.tag dt.tag em.tag fieldset.tag i.tag img.tag ins.tag kbd.tag label.tag legend.tag map.tag noscript.tag object.tag optgroup.tag param.tag pre.tag q.tag samp.tag small.tag span.tag strong.tag sub.tag sup.tag title.tag tt.tag var.tag common.tag
#quantadir = ${quanta_datadir}/dtep/xhtml-strict
#
