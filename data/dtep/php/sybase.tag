<!DOCTYPE tags>
<tags>
	<tag name="sybase_affected_rows" type="function" returnType="int" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="sybase_close" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="sybase_connect" type="function" returnType="resource" version="PHP 3, PHP 4 , PHP 5">
		<attr name="servername" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
		<attr name="appname" type="string" status="optional"></attr>
	</tag>
	<tag name="sybase_data_seek" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result_identifier" type="resource" status="required"></attr>
		<attr name="row_number" type="int" status="required"></attr>
	</tag>
	<tag name="sybase_deadlock_retry_count" type="function" returnType="void" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="retry_count" type="int" status="required"></attr>
	</tag>
	<tag name="sybase_fetch_array" type="function" returnType="array" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="sybase_fetch_assoc" type="function" returnType="array" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="sybase_fetch_field" type="function" returnType="object" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="sybase_fetch_object" type="function" returnType="object" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="object" type="mixed" status="optional"></attr>
	</tag>
	<tag name="sybase_fetch_row" type="function" returnType="array" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="sybase_field_seek" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="field_offset" type="int" status="required"></attr>
	</tag>
	<tag name="sybase_free_result" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="sybase_get_last_message" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
	</tag>
	<tag name="sybase_min_client_severity" type="function" returnType="void" version="PHP 3, PHP 4 , PHP 5">
		<attr name="severity" type="int" status="required"></attr>
	</tag>
	<tag name="sybase_min_error_severity" type="function" returnType="void" version="PHP 3, PHP 4 , PHP 5">
		<attr name="severity" type="int" status="required"></attr>
	</tag>
	<tag name="sybase_min_message_severity" type="function" returnType="void" version="PHP 3, PHP 4 , PHP 5">
		<attr name="severity" type="int" status="required"></attr>
	</tag>
	<tag name="sybase_min_server_severity" type="function" returnType="void" version="PHP 3, PHP 4 , PHP 5">
		<attr name="severity" type="int" status="required"></attr>
	</tag>
	<tag name="sybase_num_fields" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="sybase_num_rows" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="sybase_pconnect" type="function" returnType="resource" version="PHP 3, PHP 4 , PHP 5">
		<attr name="servername" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
		<attr name="appname" type="string" status="optional"></attr>
	</tag>
	<tag name="sybase_query" type="function" returnType="resource" version="PHP 3, PHP 4 , PHP 5">
		<attr name="query" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="sybase_result" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="row" type="int" status="required"></attr>
		<attr name="field" type="mixed" status="required"></attr>
	</tag>
	<tag name="sybase_select_db" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="database_name" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="sybase_set_message_handler" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="handler" type="callback" status="required"></attr>
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="sybase_unbuffered_query" type="function" returnType="resource" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="query" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="required"></attr>
		<attr name="store_result" type="bool" status="optional"></attr>
	</tag>
</tags>