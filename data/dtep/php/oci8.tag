<!DOCTYPE tags>
<tags>
	<tag name="oci_bind_by_name" type="function" returnType="bool" version="PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
		<attr name="ph_name" type="string" status="required"></attr>
		<attr name="&amp;variable" type="mixed" status="required"></attr>
		<attr name="maxlength" type="int" status="optional"></attr>
		<attr name="type" type="int" status="optional"></attr>
	</tag>
	<tag name="oci_cancel" type="function" returnType="bool" version="PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
	</tag>
	<tag name="oci_close" type="function" returnType="bool" version="PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
	</tag>
	<tag name="collection->append" type="function" returnType="bool" version="">
		<attr name="value" type="mixed" status="required"></attr>
	</tag>
	<tag name="collection->assign" type="function" returnType="bool" version="">
		<attr name="from" type="object" status="required"></attr>
	</tag>
	<tag name="collection->assignElem" type="function" returnType="bool" version="">
		<attr name="index" type="int" status="required"></attr>
		<attr name="value" type="mixed" status="required"></attr>
	</tag>
	<tag name="collection->getElem" type="function" returnType="mixed" version="">
		<attr name="index" type="int" status="required"></attr>
	</tag>
	<tag name="collection->max" type="function" returnType="int" version="">
	</tag>
	<tag name="collection->size" type="function" returnType="int" version="">
	</tag>
	<tag name="collection->trim" type="function" returnType="bool" version="">
		<attr name="num" type="int" status="required"></attr>
	</tag>
	<tag name="oci_commit" type="function" returnType="bool" version="PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
	</tag>
	<tag name="oci_connect" type="function" returnType="resource" version="PHP 5">
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="db" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="oci_define_by_name" type="function" returnType="bool" version="PHP 5">
		<attr name="statement" type="resource" status="required"></attr>
		<attr name="column_name" type="string" status="required"></attr>
		<attr name="&amp;variable" type="mixed" status="required"></attr>
		<attr name="type" type="int" status="optional"></attr>
	</tag>
	<tag name="oci_error" type="function" returnType="array" version="PHP 5">
		<attr name="source" type="resource" status="optional"></attr>
	</tag>
	<tag name="oci_execute" type="function" returnType="bool" version="PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="oci_fetch_all" type="function" returnType="int" version="PHP 5">
		<attr name="statement" type="resource" status="required"></attr>
		<attr name="&amp;output" type="array" status="required"></attr>
		<attr name="skip" type="int" status="optional"></attr>
		<attr name="maxrows" type="int" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="oci_fetch_array" type="function" returnType="array" version="PHP 5">
		<attr name="statement" type="resource" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="oci_fetch_assoc" type="function" returnType="array" version="PHP 5">
		<attr name="statement" type="resource" status="required"></attr>
	</tag>
	<tag name="oci_fetch_object" type="function" returnType="object" version="PHP 5">
		<attr name="statement" type="resource" status="required"></attr>
	</tag>
	<tag name="oci_fetch_row" type="function" returnType="array" version="PHP 5">
		<attr name="statement" type="resource" status="required"></attr>
	</tag>
	<tag name="oci_fetch" type="function" returnType="bool" version="PHP 5">
		<attr name="statement" type="resource" status="required"></attr>
	</tag>
	<tag name="oci_field_is_null" type="function" returnType="bool" version="PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
		<attr name="field" type="mixed" status="required"></attr>
	</tag>
	<tag name="oci_field_name" type="function" returnType="string" version="PHP 5">
		<attr name="statement" type="resource" status="required"></attr>
		<attr name="field" type="int" status="required"></attr>
	</tag>
	<tag name="oci_field_precision" type="function" returnType="int" version="PHP 5">
		<attr name="statement" type="resource" status="required"></attr>
		<attr name="field" type="int" status="required"></attr>
	</tag>
	<tag name="oci_field_scale" type="function" returnType="int" version="PHP 5">
		<attr name="statement" type="resource" status="required"></attr>
		<attr name="field" type="int" status="required"></attr>
	</tag>
	<tag name="oci_field_size" type="function" returnType="int" version="PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
		<attr name="field" type="mixed" status="required"></attr>
	</tag>
	<tag name="oci_field_type_raw" type="function" returnType="int" version="PHP 5">
		<attr name="statement" type="resource" status="required"></attr>
		<attr name="field" type="int" status="required"></attr>
	</tag>
	<tag name="oci_field_type" type="function" returnType="mixed" version="PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
		<attr name="field" type="int" status="required"></attr>
	</tag>
	<tag name="collection->free" type="function" returnType="bool" version="">
	</tag>
	<tag name="descriptor->free" type="function" returnType="bool" version="">
	</tag>
	<tag name="oci_free_statement" type="function" returnType="bool" version="PHP 5">
		<attr name="statement" type="resource" status="required"></attr>
	</tag>
	<tag name="oci_internal_debug" type="function" returnType="void" version="PHP 5">
		<attr name="onoff" type="int" status="required"></attr>
	</tag>
	<tag name="lob->append" type="function" returnType="bool" version="">
		<attr name="lob_from" type="object" status="required"></attr>
	</tag>
	<tag name="lob->close" type="function" returnType="bool" version="">
	</tag>
	<tag name="oci_lob_copy" type="function" returnType="bool" version="PHP 5">
		<attr name="lob_to" type="object" status="required"></attr>
		<attr name="lob_from" type="object" status="required"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="lob->eof" type="function" returnType="bool" version="">
	</tag>
	<tag name="lob->erase" type="function" returnType="int" version="">
		<attr name="offset" type="int" status="optional"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="lob->export" type="function" returnType="bool" version="">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="start" type="int" status="optional"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="lob->flush" type="function" returnType="bool" version="">
		<attr name="flag" type="int" status="optional"></attr>
	</tag>
	<tag name="lob->import" type="function" returnType="bool" version="">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="oci_lob_is_equal" type="function" returnType="bool" version="PHP 5">
		<attr name="lob1" type="object" status="required"></attr>
		<attr name="lob2" type="object" status="required"></attr>
	</tag>
	<tag name="lob->load" type="function" returnType="string" version="">
	</tag>
	<tag name="lob->read" type="function" returnType="string" version="">
		<attr name="length" type="int" status="required"></attr>
	</tag>
	<tag name="lob->rewind" type="function" returnType="bool" version="">
	</tag>
	<tag name="lob->save" type="function" returnType="bool" version="">
		<attr name="data" type="string" status="required"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="lob->seek" type="function" returnType="bool" version="">
		<attr name="offset" type="int" status="required"></attr>
		<attr name="whence" type="int" status="optional"></attr>
	</tag>
	<tag name="lob->size" type="function" returnType="int" version="">
	</tag>
	<tag name="lob->tell" type="function" returnType="int" version="">
	</tag>
	<tag name="lob->truncate" type="function" returnType="bool" version="">
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="lob->writeTemporary" type="function" returnType="bool" version="">
		<attr name="data" type="string" status="required"></attr>
		<attr name="lob_type" type="int" status="optional"></attr>
	</tag>
	<tag name="lob->write" type="function" returnType="int" version="">
		<attr name="data" type="string" status="required"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="oci_new_collection" type="function" returnType="object" version="PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="tdo" type="string" status="required"></attr>
		<attr name="schema" type="string" status="optional"></attr>
	</tag>
	<tag name="oci_new_connect" type="function" returnType="resource" version="PHP 5">
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="db" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="oci_new_cursor" type="function" returnType="resource" version="PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
	</tag>
	<tag name="oci_new_descriptor" type="function" returnType="object" version="PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="type" type="int" status="optional"></attr>
	</tag>
	<tag name="oci_num_fields" type="function" returnType="int" version="PHP 5">
		<attr name="statement" type="resource" status="required"></attr>
	</tag>
	<tag name="oci_num_rows" type="function" returnType="int" version="PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
	</tag>
	<tag name="oci_parse" type="function" returnType="resource" version="PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
	</tag>
	<tag name="oci_password_change" type="function" returnType="bool" version="PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="old_password" type="string" status="required"></attr>
		<attr name="new_password" type="string" status="required"></attr>
	</tag>
	<tag name="oci_pconnect" type="function" returnType="resource" version="PHP 5">
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="db" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="oci_result" type="function" returnType="mixed" version="PHP 5">
		<attr name="statement" type="resource" status="required"></attr>
		<attr name="field" type="mixed" status="required"></attr>
	</tag>
	<tag name="oci_rollback" type="function" returnType="bool" version="PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
	</tag>
	<tag name="oci_server_version" type="function" returnType="string" version="PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
	</tag>
	<tag name="oci_set_prefetch" type="function" returnType="bool" version="PHP 5">
		<attr name="statement" type="resource" status="required"></attr>
		<attr name="rows" type="int" status="optional"></attr>
	</tag>
	<tag name="oci_statement_type" type="function" returnType="string" version="PHP 5">
		<attr name="statement" type="resource" status="required"></attr>
	</tag>
	<tag name="ocibindbyname" type="function" returnType="bool" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
		<attr name="ph_name" type="string" status="required"></attr>
		<attr name="&amp;variable" type="mixed" status="required"></attr>
		<attr name="maxlength" type="int" status="optional"></attr>
		<attr name="type" type="int" status="optional"></attr>
	</tag>
	<tag name="ocicancel" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
	</tag>
	<tag name="ocicloselob" type="function" returnType="bool" version="">
	</tag>
	<tag name="ocicollappend" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="value" type="string" status="required"></attr>
	</tag>
	<tag name="ocicollassign" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6">
		<attr name="from" type="object" status="required"></attr>
	</tag>
	<tag name="ocicollassignelem" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="ndx" type="int" status="required"></attr>
		<attr name="val" type="string" status="required"></attr>
	</tag>
	<tag name="ocicollgetelem" type="function" returnType="string" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="ndx" type="int" status="required"></attr>
	</tag>
	<tag name="ocicollmax" type="function" returnType="int" version="PHP 4 &gt;= 4.0.6, PHP 5">
	</tag>
	<tag name="ocicollsize" type="function" returnType="int" version="PHP 4 &gt;= 4.0.6, PHP 5">
	</tag>
	<tag name="ocicolltrim" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="num" type="int" status="required"></attr>
	</tag>
	<tag name="ocicolumnisnull" type="function" returnType="bool" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
		<attr name="col" type="mixed" status="required"></attr>
	</tag>
	<tag name="ocicolumnname" type="function" returnType="string" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
		<attr name="col" type="int" status="required"></attr>
	</tag>
	<tag name="ocicolumnprecision" type="function" returnType="int" version="PHP 4 , PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
		<attr name="col" type="int" status="required"></attr>
	</tag>
	<tag name="ocicolumnscale" type="function" returnType="int" version="PHP 4 , PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
		<attr name="col" type="int" status="required"></attr>
	</tag>
	<tag name="ocicolumnsize" type="function" returnType="int" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
		<attr name="column" type="mixed" status="required"></attr>
	</tag>
	<tag name="ocicolumntype" type="function" returnType="mixed" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
		<attr name="col" type="int" status="required"></attr>
	</tag>
	<tag name="ocicolumntyperaw" type="function" returnType="int" version="PHP 4 , PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
		<attr name="col" type="int" status="required"></attr>
	</tag>
	<tag name="ocicommit" type="function" returnType="bool" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
	</tag>
	<tag name="ocidefinebyname" type="function" returnType="bool" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
		<attr name="column_name" type="string" status="required"></attr>
		<attr name="&amp;variable" type="mixed" status="required"></attr>
		<attr name="type" type="int" status="optional"></attr>
	</tag>
	<tag name="ocierror" type="function" returnType="array" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="stmt|conn|global" type="resource" status="optional"></attr>
	</tag>
	<tag name="ociexecute" type="function" returnType="bool" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="ocifetch" type="function" returnType="bool" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
	</tag>
	<tag name="ocifetchinto" type="function" returnType="int" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="statement" type="resource" status="required"></attr>
		<attr name="&amp;result" type="array" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="ocifetchstatement" type="function" returnType="int" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
		<attr name="&amp;output" type="array" status="required"></attr>
		<attr name="skip" type="int" status="optional"></attr>
		<attr name="maxrows" type="int" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="ocifreecollection" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ocifreecursor" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
	</tag>
	<tag name="ocifreedesc" type="function" returnType="bool" version="PHP 4 , PHP 5">
	</tag>
	<tag name="ocifreestatement" type="function" returnType="bool" version="PHP 3&gt;= 3.0.5, PHP 4 , PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
	</tag>
	<tag name="lob->getBuffering" type="function" returnType="bool" version="">
	</tag>
	<tag name="ociinternaldebug" type="function" returnType="void" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="onoff" type="int" status="required"></attr>
	</tag>
	<tag name="ociloadlob" type="function" returnType="string" version="PHP 4 , PHP 5">
	</tag>
	<tag name="ocilogoff" type="function" returnType="bool" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
	</tag>
	<tag name="ocilogon" type="function" returnType="resource" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="db" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="ocinewcollection" type="function" returnType="object" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="tdo" type="string" status="required"></attr>
		<attr name="schema" type="string" status="optional"></attr>
	</tag>
	<tag name="ocinewcursor" type="function" returnType="resource" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
	</tag>
	<tag name="ocinewdescriptor" type="function" returnType="object" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="type" type="int" status="optional"></attr>
	</tag>
	<tag name="ocinlogon" type="function" returnType="resource" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="db" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="ocinumcols" type="function" returnType="int" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
	</tag>
	<tag name="ociparse" type="function" returnType="resource" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
	</tag>
	<tag name="ociplogon" type="function" returnType="resource" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="db" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="ociresult" type="function" returnType="mixed" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="statement" type="resource" status="required"></attr>
		<attr name="col" type="mixed" status="required"></attr>
	</tag>
	<tag name="ocirollback" type="function" returnType="bool" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
	</tag>
	<tag name="ocirowcount" type="function" returnType="int" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
	</tag>
	<tag name="ocisavelob" type="function" returnType="bool" version="PHP 4 , PHP 5">
	</tag>
	<tag name="ocisavelobfile" type="function" returnType="bool" version="PHP 4 , PHP 5">
	</tag>
	<tag name="ociserverversion" type="function" returnType="string" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
	</tag>
	<tag name="lob->setBuffering" type="function" returnType="bool" version="">
		<attr name="on_off" type="bool" status="required"></attr>
	</tag>
	<tag name="ocisetprefetch" type="function" returnType="bool" version="PHP 3&gt;= 3.0.12, PHP 4 , PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
		<attr name="rows" type="int" status="required"></attr>
	</tag>
	<tag name="ocistatementtype" type="function" returnType="string" version="PHP 3&gt;= 3.0.5, PHP 4 , PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
	</tag>
	<tag name="ociwritelobtofile" type="function" returnType="bool" version="PHP 4 , PHP 5">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="start" type="int" status="optional"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="ociwritetemporarylob" type="function" returnType="bool" version="">
		<attr name="var" type="string" status="required"></attr>
		<attr name="lob_type" type="int" status="optional"></attr>
	</tag>
</tags>