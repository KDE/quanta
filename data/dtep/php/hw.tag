<!DOCTYPE tags>
<tags>
	<tag name="hw_array2objrec" type="function" returnType="string" version="PHP 3&gt;= 3.0.4, PHP 4 ">
		<attr name="object_array" type="array" status="required"></attr>
	</tag>
	<tag name="hw_changeobject" type="function" returnType="void" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="link" type="int" status="required"></attr>
		<attr name="objid" type="int" status="required"></attr>
		<attr name="attributes" type="array" status="required"></attr>
	</tag>
	<tag name="hw_children" type="function" returnType="array" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="objectID" type="int" status="required"></attr>
	</tag>
	<tag name="hw_childrenobj" type="function" returnType="array" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="objectID" type="int" status="required"></attr>
	</tag>
	<tag name="hw_close" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
	</tag>
	<tag name="hw_connect" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="host" type="string" status="required"></attr>
		<attr name="port" type="int" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
	</tag>
	<tag name="hw_connection_info" type="function" returnType="void" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="link" type="int" status="required"></attr>
	</tag>
	<tag name="hw_cp" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="object_id_array" type="array" status="required"></attr>
		<attr name="destination_id" type="int" status="required"></attr>
	</tag>
	<tag name="hw_deleteobject" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="object_to_delete" type="int" status="required"></attr>
	</tag>
	<tag name="hw_docbyanchor" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="anchorID" type="int" status="required"></attr>
	</tag>
	<tag name="hw_docbyanchorobj" type="function" returnType="string" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="anchorID" type="int" status="required"></attr>
	</tag>
	<tag name="hw_document_attributes" type="function" returnType="string" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="hw_document" type="int" status="required"></attr>
	</tag>
	<tag name="hw_document_bodytag" type="function" returnType="string" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="hw_document" type="int" status="required"></attr>
		<attr name="prefix" type="string" status="optional"></attr>
	</tag>
	<tag name="hw_document_content" type="function" returnType="string" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="hw_document" type="int" status="required"></attr>
	</tag>
	<tag name="hw_document_setcontent" type="function" returnType="string" version="PHP 4 ">
		<attr name="hw_document" type="int" status="required"></attr>
		<attr name="content" type="string" status="required"></attr>
	</tag>
	<tag name="hw_document_size" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="hw_document" type="int" status="required"></attr>
	</tag>
	<tag name="hw_dummy" type="function" returnType="string" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="link" type="int" status="required"></attr>
		<attr name="id" type="int" status="required"></attr>
		<attr name="msgid" type="int" status="required"></attr>
	</tag>
	<tag name="hw_edittext" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="hw_document" type="int" status="required"></attr>
	</tag>
	<tag name="hw_error" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
	</tag>
	<tag name="hw_errormsg" type="function" returnType="string" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
	</tag>
	<tag name="hw_free_document" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="hw_document" type="int" status="required"></attr>
	</tag>
	<tag name="hw_getanchors" type="function" returnType="array" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="objectID" type="int" status="required"></attr>
	</tag>
	<tag name="hw_getanchorsobj" type="function" returnType="array" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="objectID" type="int" status="required"></attr>
	</tag>
	<tag name="hw_getandlock" type="function" returnType="string" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="objectID" type="int" status="required"></attr>
	</tag>
	<tag name="hw_getchildcoll" type="function" returnType="array" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="objectID" type="int" status="required"></attr>
	</tag>
	<tag name="hw_getchildcollobj" type="function" returnType="array" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="objectID" type="int" status="required"></attr>
	</tag>
	<tag name="hw_getchilddoccoll" type="function" returnType="array" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="objectID" type="int" status="required"></attr>
	</tag>
	<tag name="hw_getchilddoccollobj" type="function" returnType="array" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="objectID" type="int" status="required"></attr>
	</tag>
	<tag name="hw_getobject" type="function" returnType="array" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="objectID" type="mixed" status="required"></attr>
		<attr name="query" type="string" status="optional"></attr>
	</tag>
	<tag name="hw_getobjectbyquery" type="function" returnType="array" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
		<attr name="max_hits" type="int" status="required"></attr>
	</tag>
	<tag name="hw_getobjectbyquerycoll" type="function" returnType="array" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="objectID" type="int" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
		<attr name="max_hits" type="int" status="required"></attr>
	</tag>
	<tag name="hw_getobjectbyquerycollobj" type="function" returnType="array" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="objectID" type="int" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
		<attr name="max_hits" type="int" status="required"></attr>
	</tag>
	<tag name="hw_getobjectbyqueryobj" type="function" returnType="array" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
		<attr name="max_hits" type="int" status="required"></attr>
	</tag>
	<tag name="hw_getparents" type="function" returnType="array" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="objectID" type="int" status="required"></attr>
	</tag>
	<tag name="hw_getparentsobj" type="function" returnType="array" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="objectID" type="int" status="required"></attr>
	</tag>
	<tag name="hw_getrellink" type="function" returnType="string" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="link" type="int" status="required"></attr>
		<attr name="rootid" type="int" status="required"></attr>
		<attr name="sourceid" type="int" status="required"></attr>
		<attr name="destid" type="int" status="required"></attr>
	</tag>
	<tag name="hw_getremote" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="objectID" type="int" status="required"></attr>
	</tag>
	<tag name="hw_getremotechildren" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="object_record" type="string" status="required"></attr>
	</tag>
	<tag name="hw_getsrcbydestobj" type="function" returnType="array" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="objectID" type="int" status="required"></attr>
	</tag>
	<tag name="hw_gettext" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="objectID" type="int" status="required"></attr>
		<attr name="rootID/prefix" type="mixed" status="optional"></attr>
	</tag>
	<tag name="hw_getusername" type="function" returnType="string" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
	</tag>
	<tag name="hw_identify" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="link" type="int" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
	</tag>
	<tag name="hw_incollections" type="function" returnType="array" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="object_id_array" type="array" status="required"></attr>
		<attr name="collection_id_array" type="array" status="required"></attr>
		<attr name="return_collections" type="int" status="required"></attr>
	</tag>
	<tag name="hw_info" type="function" returnType="string" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
	</tag>
	<tag name="hw_inscoll" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="objectID" type="int" status="required"></attr>
		<attr name="object_array" type="array" status="required"></attr>
	</tag>
	<tag name="hw_insdoc" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="parentID" type="int" status="required"></attr>
		<attr name="object_record" type="string" status="required"></attr>
		<attr name="text" type="string" status="optional"></attr>
	</tag>
	<tag name="hw_insertanchors" type="function" returnType="string" version="PHP 4 &gt;= 4.0.4">
		<attr name="hwdoc" type="int" status="required"></attr>
		<attr name="anchorecs" type="array" status="required"></attr>
		<attr name="dest" type="array" status="required"></attr>
		<attr name="urlprefixes" type="array" status="optional"></attr>
	</tag>
	<tag name="hw_insertdocument" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="parent_id" type="int" status="required"></attr>
		<attr name="hw_document" type="int" status="required"></attr>
	</tag>
	<tag name="hw_insertobject" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="object_rec" type="string" status="required"></attr>
		<attr name="parameter" type="string" status="required"></attr>
	</tag>
	<tag name="hw_mapid" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="server_id" type="int" status="required"></attr>
		<attr name="object_id" type="int" status="required"></attr>
	</tag>
	<tag name="hw_modifyobject" type="function" returnType="int" version="PHP 3&gt;= 3.0.7, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="object_to_change" type="int" status="required"></attr>
		<attr name="remove" type="array" status="required"></attr>
		<attr name="add" type="array" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="hw_mv" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="object_id_array" type="array" status="required"></attr>
		<attr name="source_id" type="int" status="required"></attr>
		<attr name="destination_id" type="int" status="required"></attr>
	</tag>
	<tag name="hw_new_document" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="object_record" type="string" status="required"></attr>
		<attr name="document_data" type="string" status="required"></attr>
		<attr name="document_size" type="int" status="required"></attr>
	</tag>
	<tag name="hw_objrec2array" type="function" returnType="array" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="object_record" type="string" status="required"></attr>
		<attr name="format" type="array" status="optional"></attr>
	</tag>
	<tag name="hw_output_document" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="hw_document" type="int" status="required"></attr>
	</tag>
	<tag name="hw_pconnect" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="host" type="string" status="required"></attr>
		<attr name="port" type="int" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
	</tag>
	<tag name="hw_pipedocument" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="objectID" type="int" status="required"></attr>
		<attr name="url_prefixes" type="array" status="optional"></attr>
	</tag>
	<tag name="hw_root" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="&lt;/methodparam&gt;" type="" status="required"></attr>
	</tag>
	<tag name="hw_setlinkroot" type="function" returnType="void" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="link" type="int" status="required"></attr>
		<attr name="rootid" type="int" status="required"></attr>
	</tag>
	<tag name="hw_stat" type="function" returnType="string" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="link" type="int" status="required"></attr>
	</tag>
	<tag name="hw_unlock" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="objectID" type="int" status="required"></attr>
	</tag>
	<tag name="hw_who" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 ">
		<attr name="connection" type="int" status="required"></attr>
	</tag>
</tags>