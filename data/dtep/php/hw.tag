<!DOCTYPE tags>
<tags>
	<tag name="hw_array2objrec" type="function" returnType="string">
		<attr name="object_array" type="array"></attr>
	</tag>
	<tag name="hw_changeobject" type="function" returnType="void">
		<attr name="link" type="int"></attr>
		<attr name="objid" type="int"></attr>
		<attr name="attributes" type="array"></attr>
	</tag>
	<tag name="hw_children" type="function" returnType="array">
		<attr name="connection" type="int"></attr>
		<attr name="objectID" type="int"></attr>
	</tag>
	<tag name="hw_childrenobj" type="function" returnType="array">
		<attr name="connection" type="int"></attr>
		<attr name="objectID" type="int"></attr>
	</tag>
	<tag name="hw_close" type="function" returnType="int">
		<attr name="connection" type="int"></attr>
	</tag>
	<tag name="hw_connect" type="function" returnType="int">
		<attr name="host" type="string"></attr>
		<attr name="port" type="int"></attr>
		<attr name="username" type="string"></attr>
		<attr name="password" type="string"></attr>
	</tag>
	<tag name="hw_connection_info" type="function" returnType="void">
		<attr name="link" type="int"></attr>
	</tag>
	<tag name="hw_cp" type="function" returnType="int">
		<attr name="connection" type="int"></attr>
		<attr name="object_id_array" type="array"></attr>
		<attr name="destination_id" type="int"></attr>
	</tag>
	<tag name="hw_deleteobject" type="function" returnType="int">
		<attr name="connection" type="int"></attr>
		<attr name="object_to_delete" type="int"></attr>
	</tag>
	<tag name="hw_docbyanchor" type="function" returnType="int">
		<attr name="connection" type="int"></attr>
		<attr name="anchorID" type="int"></attr>
	</tag>
	<tag name="hw_docbyanchorobj" type="function" returnType="string">
		<attr name="connection" type="int"></attr>
		<attr name="anchorID" type="int"></attr>
	</tag>
	<tag name="hw_document_attributes" type="function" returnType="string">
		<attr name="hw_document" type="int"></attr>
	</tag>
	<tag name="hw_document_bodytag" type="function" returnType="string">
		<attr name="hw_document" type="int"></attr>
		<attr name="prefix" type="string" status="optional"></attr>
	</tag>
	<tag name="hw_document_content" type="function" returnType="string">
		<attr name="hw_document" type="int"></attr>
	</tag>
	<tag name="hw_document_setcontent" type="function" returnType="string">
		<attr name="hw_document" type="int"></attr>
		<attr name="content" type="string"></attr>
	</tag>
	<tag name="hw_document_size" type="function" returnType="int">
		<attr name="hw_document" type="int"></attr>
	</tag>
	<tag name="hw_dummy" type="function" returnType="string">
		<attr name="link" type="int"></attr>
		<attr name="id" type="int"></attr>
		<attr name="msgid" type="int"></attr>
	</tag>
	<tag name="hw_edittext" type="function" returnType="int">
		<attr name="connection" type="int"></attr>
		<attr name="hw_document" type="int"></attr>
	</tag>
	<tag name="hw_error" type="function" returnType="int">
		<attr name="connection" type="int"></attr>
	</tag>
	<tag name="hw_errormsg" type="function" returnType="string">
		<attr name="connection" type="int"></attr>
	</tag>
	<tag name="hw_free_document" type="function" returnType="int">
		<attr name="hw_document" type="int"></attr>
	</tag>
	<tag name="hw_getanchors" type="function" returnType="array">
		<attr name="connection" type="int"></attr>
		<attr name="objectID" type="int"></attr>
	</tag>
	<tag name="hw_getanchorsobj" type="function" returnType="array">
		<attr name="connection" type="int"></attr>
		<attr name="objectID" type="int"></attr>
	</tag>
	<tag name="hw_getandlock" type="function" returnType="string">
		<attr name="connection" type="int"></attr>
		<attr name="objectID" type="int"></attr>
	</tag>
	<tag name="hw_getchildcoll" type="function" returnType="array">
		<attr name="connection" type="int"></attr>
		<attr name="objectID" type="int"></attr>
	</tag>
	<tag name="hw_getchildcollobj" type="function" returnType="array">
		<attr name="connection" type="int"></attr>
		<attr name="objectID" type="int"></attr>
	</tag>
	<tag name="hw_getchilddoccoll" type="function" returnType="array">
		<attr name="connection" type="int"></attr>
		<attr name="objectID" type="int"></attr>
	</tag>
	<tag name="hw_getchilddoccollobj" type="function" returnType="array">
		<attr name="connection" type="int"></attr>
		<attr name="objectID" type="int"></attr>
	</tag>
	<tag name="hw_getobject" type="function" returnType="array">
		<attr name="connection" type="int"></attr>
		<attr name="objectID" type="mixed"></attr>
		<attr name="query" type="string" status="optional"></attr>
	</tag>
	<tag name="hw_getobjectbyquery" type="function" returnType="array">
		<attr name="connection" type="int"></attr>
		<attr name="query" type="string"></attr>
		<attr name="max_hits" type="int"></attr>
	</tag>
	<tag name="hw_getobjectbyquerycoll" type="function" returnType="array">
		<attr name="connection" type="int"></attr>
		<attr name="objectID" type="int"></attr>
		<attr name="query" type="string"></attr>
		<attr name="max_hits" type="int"></attr>
	</tag>
	<tag name="hw_getobjectbyquerycollobj" type="function" returnType="array">
		<attr name="connection" type="int"></attr>
		<attr name="objectID" type="int"></attr>
		<attr name="query" type="string"></attr>
		<attr name="max_hits" type="int"></attr>
	</tag>
	<tag name="hw_getobjectbyqueryobj" type="function" returnType="array">
		<attr name="connection" type="int"></attr>
		<attr name="query" type="string"></attr>
		<attr name="max_hits" type="int"></attr>
	</tag>
	<tag name="hw_getparents" type="function" returnType="array">
		<attr name="connection" type="int"></attr>
		<attr name="objectID" type="int"></attr>
	</tag>
	<tag name="hw_getparentsobj" type="function" returnType="array">
		<attr name="connection" type="int"></attr>
		<attr name="objectID" type="int"></attr>
	</tag>
	<tag name="hw_getrellink" type="function" returnType="string">
		<attr name="link" type="int"></attr>
		<attr name="rootid" type="int"></attr>
		<attr name="sourceid" type="int"></attr>
		<attr name="destid" type="int"></attr>
	</tag>
	<tag name="hw_getremote" type="function" returnType="int">
		<attr name="connection" type="int"></attr>
		<attr name="objectID" type="int"></attr>
	</tag>
	<tag name="hw_getremotechildren" type="function" returnType="int">
		<attr name="connection" type="int"></attr>
		<attr name="object_record" type="string"></attr>
	</tag>
	<tag name="hw_getsrcbydestobj" type="function" returnType="array">
		<attr name="connection" type="int"></attr>
		<attr name="objectID" type="int"></attr>
	</tag>
	<tag name="hw_gettext" type="function" returnType="int">
		<attr name="connection" type="int"></attr>
		<attr name="objectID" type="int"></attr>
		<attr name="rootID/prefix" type="mixed" status="optional"></attr>
	</tag>
	<tag name="hw_getusername" type="function" returnType="string">
		<attr name="connection" type="int"></attr>
	</tag>
	<tag name="hw_identify" type="function" returnType="int">
		<attr name="link" type="int"></attr>
		<attr name="username" type="string"></attr>
		<attr name="password" type="string"></attr>
	</tag>
	<tag name="hw_incollections" type="function" returnType="array">
		<attr name="connection" type="int"></attr>
		<attr name="object_id_array" type="array"></attr>
		<attr name="collection_id_array" type="array"></attr>
		<attr name="return_collections" type="int"></attr>
	</tag>
	<tag name="hw_info" type="function" returnType="string">
		<attr name="connection" type="int"></attr>
	</tag>
	<tag name="hw_inscoll" type="function" returnType="int">
		<attr name="connection" type="int"></attr>
		<attr name="objectID" type="int"></attr>
		<attr name="object_array" type="array"></attr>
	</tag>
	<tag name="hw_insdoc" type="function" returnType="int">
		<attr name="connection" type="resource"></attr>
		<attr name="parentID" type="int"></attr>
		<attr name="object_record" type="string"></attr>
		<attr name="text" type="string" status="optional"></attr>
	</tag>
	<tag name="hw_insertanchors" type="function" returnType="string">
		<attr name="hwdoc" type="int"></attr>
		<attr name="anchorecs" type="array"></attr>
		<attr name="dest" type="array"></attr>
		<attr name="urlprefixes" type="array" status="optional"></attr>
	</tag>
	<tag name="hw_insertdocument" type="function" returnType="int">
		<attr name="connection" type="int"></attr>
		<attr name="parent_id" type="int"></attr>
		<attr name="hw_document" type="int"></attr>
	</tag>
	<tag name="hw_insertobject" type="function" returnType="int">
		<attr name="connection" type="int"></attr>
		<attr name="object_rec" type="string"></attr>
		<attr name="parameter" type="string"></attr>
	</tag>
	<tag name="hw_mapid" type="function" returnType="int">
		<attr name="connection" type="int"></attr>
		<attr name="server_id" type="int"></attr>
		<attr name="object_id" type="int"></attr>
	</tag>
	<tag name="hw_modifyobject" type="function" returnType="int">
		<attr name="connection" type="int"></attr>
		<attr name="object_to_change" type="int"></attr>
		<attr name="remove" type="array"></attr>
		<attr name="add" type="array"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="hw_mv" type="function" returnType="int">
		<attr name="connection" type="int"></attr>
		<attr name="object_id_array" type="array"></attr>
		<attr name="source_id" type="int"></attr>
		<attr name="destination_id" type="int"></attr>
	</tag>
	<tag name="hw_new_document" type="function" returnType="int">
		<attr name="object_record" type="string"></attr>
		<attr name="document_data" type="string"></attr>
		<attr name="document_size" type="int"></attr>
	</tag>
	<tag name="hw_objrec2array" type="function" returnType="array">
		<attr name="object_record" type="string"></attr>
		<attr name="format" type="array" status="optional"></attr>
	</tag>
	<tag name="hw_output_document" type="function" returnType="int">
		<attr name="hw_document" type="int"></attr>
	</tag>
	<tag name="hw_pconnect" type="function" returnType="int">
		<attr name="host" type="string"></attr>
		<attr name="port" type="int"></attr>
		<attr name="username" type="string"></attr>
		<attr name="password" type="string"></attr>
	</tag>
	<tag name="hw_pipedocument" type="function" returnType="int">
		<attr name="connection" type="int"></attr>
		<attr name="objectID" type="int"></attr>
		<attr name="url_prefixes" type="array" status="optional"></attr>
	</tag>
	<tag name="hw_root" type="function" returnType="int">
		<attr name="" type=""></attr>
	</tag>
	<tag name="hw_setlinkroot" type="function" returnType="void">
		<attr name="link" type="int"></attr>
		<attr name="rootid" type="int"></attr>
	</tag>
	<tag name="hw_stat" type="function" returnType="string">
		<attr name="link" type="int"></attr>
	</tag>
	<tag name="hw_unlock" type="function" returnType="int">
		<attr name="connection" type="int"></attr>
		<attr name="objectID" type="int"></attr>
	</tag>
	<tag name="hw_who" type="function" returnType="int">
		<attr name="connection" type="int"></attr>
	</tag>
</tags>