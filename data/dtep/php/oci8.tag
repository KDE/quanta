<!DOCTYPE tags>
<tags>
	<tag name="oci_bind_by_name" type="function" returnType="bool">
		<attr name="stmt" type="resource"></attr>
		<attr name="ph_name" type="string"></attr>
		<attr name="&amp;variable" type="mixed"></attr>
		<attr name="maxlength" type="int" status="optional"></attr>
		<attr name="type" type="int" status="optional"></attr>
	</tag>
	<tag name="oci_cancel" type="function" returnType="bool">
		<attr name="stmt" type="resource"></attr>
	</tag>
	<tag name="oci_close" type="function" returnType="bool">
		<attr name="connection" type="resource"></attr>
	</tag>
	<tag name="collection->append" type="function" returnType="bool">
		<attr name="value" type="mixed"></attr>
	</tag>
	<tag name="collection->assign" type="function" returnType="bool">
		<attr name="from" type="object"></attr>
	</tag>
	<tag name="collection->assignElem" type="function" returnType="bool">
		<attr name="index" type="int"></attr>
		<attr name="value" type="mixed"></attr>
	</tag>
	<tag name="collection->getElem" type="function" returnType="mixed">
		<attr name="index" type="int"></attr>
	</tag>
	<tag name="collection->max" type="function" returnType="int">
	</tag>
	<tag name="collection->size" type="function" returnType="int">
	</tag>
	<tag name="collection->trim" type="function" returnType="bool">
		<attr name="num" type="int"></attr>
	</tag>
	<tag name="oci_commit" type="function" returnType="bool">
		<attr name="connection" type="resource"></attr>
	</tag>
	<tag name="oci_connect" type="function" returnType="resource">
		<attr name="username" type="string"></attr>
		<attr name="password" type="string"></attr>
		<attr name="db" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="oci_define_by_name" type="function" returnType="bool">
		<attr name="statement" type="resource"></attr>
		<attr name="column_name" type="string"></attr>
		<attr name="&amp;variable" type="mixed"></attr>
		<attr name="type" type="int" status="optional"></attr>
	</tag>
	<tag name="oci_error" type="function" returnType="array">
		<attr name="source" type="resource" status="optional"></attr>
	</tag>
	<tag name="oci_execute" type="function" returnType="bool">
		<attr name="stmt" type="resource"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="oci_fetch_all" type="function" returnType="int">
		<attr name="statement" type="resource"></attr>
		<attr name="&amp;output" type="array"></attr>
		<attr name="skip" type="int" status="optional"></attr>
		<attr name="maxrows" type="int" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="oci_fetch_array" type="function" returnType="array">
		<attr name="statement" type="resource"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="oci_fetch_assoc" type="function" returnType="array">
		<attr name="statement" type="resource"></attr>
	</tag>
	<tag name="oci_fetch_object" type="function" returnType="object">
		<attr name="statement" type="resource"></attr>
	</tag>
	<tag name="oci_fetch_row" type="function" returnType="array">
		<attr name="statement" type="resource"></attr>
	</tag>
	<tag name="oci_fetch" type="function" returnType="bool">
		<attr name="statement" type="resource"></attr>
	</tag>
	<tag name="oci_field_is_null" type="function" returnType="bool">
		<attr name="stmt" type="resource"></attr>
		<attr name="field" type="mixed"></attr>
	</tag>
	<tag name="oci_field_name" type="function" returnType="string">
		<attr name="statement" type="resource"></attr>
		<attr name="field" type="int"></attr>
	</tag>
	<tag name="oci_field_precision" type="function" returnType="int">
		<attr name="statement" type="resource"></attr>
		<attr name="field" type="int"></attr>
	</tag>
	<tag name="oci_field_scale" type="function" returnType="int">
		<attr name="statement" type="resource"></attr>
		<attr name="field" type="int"></attr>
	</tag>
	<tag name="oci_field_size" type="function" returnType="int">
		<attr name="stmt" type="resource"></attr>
		<attr name="field" type="mixed"></attr>
	</tag>
	<tag name="oci_field_type_raw" type="function" returnType="int">
		<attr name="statement" type="resource"></attr>
		<attr name="field" type="int"></attr>
	</tag>
	<tag name="oci_field_type" type="function" returnType="mixed">
		<attr name="stmt" type="resource"></attr>
		<attr name="field" type="int"></attr>
	</tag>
	<tag name="collection->free" type="function" returnType="bool">
	</tag>
	<tag name="descriptor->free" type="function" returnType="bool">
	</tag>
	<tag name="oci_free_statement" type="function" returnType="bool">
		<attr name="statement" type="resource"></attr>
	</tag>
	<tag name="oci_internal_debug" type="function" returnType="void">
		<attr name="onoff" type="int"></attr>
	</tag>
	<tag name="lob->append" type="function" returnType="bool">
		<attr name="lob_from" type="object"></attr>
	</tag>
	<tag name="lob->close" type="function" returnType="bool">
	</tag>
	<tag name="oci_lob_copy" type="function" returnType="bool">
		<attr name="lob_to" type="object"></attr>
		<attr name="lob_from" type="object"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="lob->eof" type="function" returnType="bool">
	</tag>
	<tag name="lob->erase" type="function" returnType="int">
		<attr name="offset" type="int" status="optional"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="lob->export" type="function" returnType="bool">
		<attr name="filename" type="string"></attr>
		<attr name="start" type="int" status="optional"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="lob->flush" type="function" returnType="bool">
		<attr name="flag" type="int" status="optional"></attr>
	</tag>
	<tag name="lob->import" type="function" returnType="bool">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="oci_lob_is_equal" type="function" returnType="bool">
		<attr name="lob1" type="object"></attr>
		<attr name="lob2" type="object"></attr>
	</tag>
	<tag name="lob->load" type="function" returnType="string">
	</tag>
	<tag name="lob->read" type="function" returnType="string">
		<attr name="length" type="int"></attr>
	</tag>
	<tag name="lob->rewind" type="function" returnType="bool">
	</tag>
	<tag name="lob->save" type="function" returnType="bool">
		<attr name="data" type="string"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="lob->seek" type="function" returnType="bool">
		<attr name="offset" type="int"></attr>
		<attr name="whence" type="int" status="optional"></attr>
	</tag>
	<tag name="lob->size" type="function" returnType="int">
	</tag>
	<tag name="lob->tell" type="function" returnType="int">
	</tag>
	<tag name="lob->truncate" type="function" returnType="bool">
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="lob->writeTemporary" type="function" returnType="bool">
		<attr name="data" type="string"></attr>
		<attr name="lob_type" type="int" status="optional"></attr>
	</tag>
	<tag name="lob->write" type="function" returnType="int">
		<attr name="data" type="string"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="oci_new_collection" type="function" returnType="object">
		<attr name="connection" type="resource"></attr>
		<attr name="tdo" type="string"></attr>
		<attr name="schema" type="string" status="optional"></attr>
	</tag>
	<tag name="oci_new_connect" type="function" returnType="resource">
		<attr name="username" type="string"></attr>
		<attr name="password" type="string"></attr>
		<attr name="db" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="oci_new_cursor" type="function" returnType="resource">
		<attr name="connection" type="resource"></attr>
	</tag>
	<tag name="oci_new_descriptor" type="function" returnType="object">
		<attr name="connection" type="resource"></attr>
		<attr name="type" type="int" status="optional"></attr>
	</tag>
	<tag name="oci_num_fields" type="function" returnType="int">
		<attr name="statement" type="resource"></attr>
	</tag>
	<tag name="oci_num_rows" type="function" returnType="int">
		<attr name="stmt" type="resource"></attr>
	</tag>
	<tag name="oci_parse" type="function" returnType="resource">
		<attr name="connection" type="resource"></attr>
		<attr name="query" type="string"></attr>
	</tag>
	<tag name="oci_password_change" type="function" returnType="bool">
		<attr name="connection" type="resource"></attr>
		<attr name="username" type="string"></attr>
		<attr name="old_password" type="string"></attr>
		<attr name="new_password" type="string"></attr>
	</tag>
	<tag name="oci_pconnect" type="function" returnType="resource">
		<attr name="username" type="string"></attr>
		<attr name="password" type="string"></attr>
		<attr name="db" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="oci_result" type="function" returnType="mixed">
		<attr name="statement" type="resource"></attr>
		<attr name="field" type="mixed"></attr>
	</tag>
	<tag name="oci_rollback" type="function" returnType="bool">
		<attr name="connection" type="resource"></attr>
	</tag>
	<tag name="oci_server_version" type="function" returnType="string">
		<attr name="connection" type="resource"></attr>
	</tag>
	<tag name="oci_set_prefetch" type="function" returnType="bool">
		<attr name="statement" type="resource"></attr>
		<attr name="rows" type="int" status="optional"></attr>
	</tag>
	<tag name="oci_statement_type" type="function" returnType="string">
		<attr name="statement" type="resource"></attr>
	</tag>
	<tag name="ocibindbyname" type="function" returnType="bool">
		<attr name="stmt" type="resource"></attr>
		<attr name="ph_name" type="string"></attr>
		<attr name="&amp;variable" type="mixed"></attr>
		<attr name="maxlength" type="int" status="optional"></attr>
		<attr name="type" type="int" status="optional"></attr>
	</tag>
	<tag name="ocicancel" type="function" returnType="bool">
		<attr name="stmt" type="resource"></attr>
	</tag>
	<tag name="ocicloselob" type="function" returnType="bool">
	</tag>
	<tag name="ocicollappend" type="function" returnType="bool">
		<attr name="value" type="string"></attr>
	</tag>
	<tag name="ocicollassign" type="function" returnType="bool">
		<attr name="from" type="object"></attr>
	</tag>
	<tag name="ocicollassignelem" type="function" returnType="bool">
		<attr name="ndx" type="int"></attr>
		<attr name="val" type="string"></attr>
	</tag>
	<tag name="ocicollgetelem" type="function" returnType="string">
		<attr name="ndx" type="int"></attr>
	</tag>
	<tag name="ocicollmax" type="function" returnType="int">
	</tag>
	<tag name="ocicollsize" type="function" returnType="int">
	</tag>
	<tag name="ocicolltrim" type="function" returnType="bool">
		<attr name="num" type="int"></attr>
	</tag>
	<tag name="ocicolumnisnull" type="function" returnType="bool">
		<attr name="stmt" type="resource"></attr>
		<attr name="col" type="mixed"></attr>
	</tag>
	<tag name="ocicolumnname" type="function" returnType="string">
		<attr name="stmt" type="resource"></attr>
		<attr name="col" type="int"></attr>
	</tag>
	<tag name="ocicolumnprecision" type="function" returnType="int">
		<attr name="stmt" type="resource"></attr>
		<attr name="col" type="int"></attr>
	</tag>
	<tag name="ocicolumnscale" type="function" returnType="int">
		<attr name="stmt" type="resource"></attr>
		<attr name="col" type="int"></attr>
	</tag>
	<tag name="ocicolumnsize" type="function" returnType="int">
		<attr name="stmt" type="resource"></attr>
		<attr name="column" type="mixed"></attr>
	</tag>
	<tag name="ocicolumntype" type="function" returnType="mixed">
		<attr name="stmt" type="resource"></attr>
		<attr name="col" type="int"></attr>
	</tag>
	<tag name="ocicolumntyperaw" type="function" returnType="int">
		<attr name="stmt" type="resource"></attr>
		<attr name="col" type="int"></attr>
	</tag>
	<tag name="ocicommit" type="function" returnType="bool">
		<attr name="connection" type="resource"></attr>
	</tag>
	<tag name="ocidefinebyname" type="function" returnType="bool">
		<attr name="stmt" type="resource"></attr>
		<attr name="column_name" type="string"></attr>
		<attr name="&amp;variable" type="mixed"></attr>
		<attr name="type" type="int" status="optional"></attr>
	</tag>
	<tag name="ocierror" type="function" returnType="array">
		<attr name="stmt|conn|global" type="resource" status="optional"></attr>
	</tag>
	<tag name="ociexecute" type="function" returnType="bool">
		<attr name="stmt" type="resource"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="ocifetch" type="function" returnType="bool">
		<attr name="stmt" type="resource"></attr>
	</tag>
	<tag name="ocifetchinto" type="function" returnType="int">
		<attr name="statement" type="resource"></attr>
		<attr name="&amp;result" type="array"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="ocifetchstatement" type="function" returnType="int">
		<attr name="stmt" type="resource"></attr>
		<attr name="&amp;output" type="array"></attr>
		<attr name="skip" type="int" status="optional"></attr>
		<attr name="maxrows" type="int" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="ocifreecollection" type="function" returnType="bool">
	</tag>
	<tag name="ocifreecursor" type="function" returnType="bool">
		<attr name="stmt" type="resource"></attr>
	</tag>
	<tag name="ocifreedesc" type="function" returnType="bool">
	</tag>
	<tag name="ocifreestatement" type="function" returnType="bool">
		<attr name="stmt" type="resource"></attr>
	</tag>
	<tag name="lob->getBuffering" type="function" returnType="bool">
	</tag>
	<tag name="ociinternaldebug" type="function" returnType="void">
		<attr name="onoff" type="int"></attr>
	</tag>
	<tag name="ociloadlob" type="function" returnType="string">
	</tag>
	<tag name="ocilogoff" type="function" returnType="bool">
		<attr name="connection" type="resource"></attr>
	</tag>
	<tag name="ocilogon" type="function" returnType="resource">
		<attr name="username" type="string"></attr>
		<attr name="password" type="string"></attr>
		<attr name="db" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="ocinewcollection" type="function" returnType="object">
		<attr name="connection" type="resource"></attr>
		<attr name="tdo" type="string"></attr>
		<attr name="schema" type="string" status="optional"></attr>
	</tag>
	<tag name="ocinewcursor" type="function" returnType="resource">
		<attr name="conn" type="resource"></attr>
	</tag>
	<tag name="ocinewdescriptor" type="function" returnType="object">
		<attr name="connection" type="resource"></attr>
		<attr name="type" type="int" status="optional"></attr>
	</tag>
	<tag name="ocinlogon" type="function" returnType="resource">
		<attr name="username" type="string"></attr>
		<attr name="password" type="string"></attr>
		<attr name="db" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="ocinumcols" type="function" returnType="int">
		<attr name="stmt" type="resource"></attr>
	</tag>
	<tag name="ociparse" type="function" returnType="resource">
		<attr name="conn" type="resource"></attr>
		<attr name="query" type="string"></attr>
	</tag>
	<tag name="ociplogon" type="function" returnType="resource">
		<attr name="username" type="string"></attr>
		<attr name="password" type="string"></attr>
		<attr name="db" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="ociresult" type="function" returnType="mixed">
		<attr name="statement" type="resource"></attr>
		<attr name="col" type="mixed"></attr>
	</tag>
	<tag name="ocirollback" type="function" returnType="bool">
		<attr name="connection" type="resource"></attr>
	</tag>
	<tag name="ocirowcount" type="function" returnType="int">
		<attr name="stmt" type="resource"></attr>
	</tag>
	<tag name="ocisavelob" type="function" returnType="bool">
	</tag>
	<tag name="ocisavelobfile" type="function" returnType="bool">
	</tag>
	<tag name="ociserverversion" type="function" returnType="string">
		<attr name="conn" type="resource"></attr>
	</tag>
	<tag name="lob->setBuffering" type="function" returnType="bool">
		<attr name="on_off" type="bool"></attr>
	</tag>
	<tag name="ocisetprefetch" type="function" returnType="bool">
		<attr name="stmt" type="resource"></attr>
		<attr name="rows" type="int"></attr>
	</tag>
	<tag name="ocistatementtype" type="function" returnType="string">
		<attr name="stmt" type="resource"></attr>
	</tag>
	<tag name="ociwritelobtofile" type="function" returnType="bool">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="start" type="int" status="optional"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="ociwritetemporarylob" type="function" returnType="bool">
		<attr name="var" type="string"></attr>
		<attr name="lob_type" type="int" status="optional"></attr>
	</tag>
</tags>