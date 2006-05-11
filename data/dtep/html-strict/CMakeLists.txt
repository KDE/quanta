kde4_header()




########### install files ###############

install_files(${QUANTA_DATA_INSTALL_DIR}/dtep/html-strict  FILES  a.tag b.tag c.tag d.tag e.tag f.tag h.tag i.tag k.tag l.tag m.tag n.tag o.tag p.tag  	    q.tag s.tag t.tag u.tag v.tag common.tag description.rc  )

kde4_footer()



#original Makefile.am contents follow:

#quanta_DATA = a.tag b.tag c.tag d.tag e.tag f.tag h.tag i.tag k.tag l.tag m.tag n.tag o.tag p.tag \
#	    q.tag s.tag t.tag u.tag v.tag common.tag description.rc 
#
#quantadir = ${quanta_datadir}/dtep/html-strict
#
