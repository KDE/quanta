<!DOCTYPE tags>
<tags>
	<tag name="sybase_affected_rows" type="function" returnType="int">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="sybase_close" type="function" returnType="bool">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="sybase_connect" type="function" returnType="resource">
		<attr name="servername" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
		<attr name="appname" type="string" status="optional"></attr>
	</tag>
	<tag name="sybase_data_seek" type="function" returnType="bool">
		<attr name="result_identifier" type="resource"></attr>
		<attr name="row_number" type="int"></attr>
	</tag>
	<tag name="sybase_deadlock_retry_count" type="function" returnType="void">
		<attr name="retry_count" type="int"></attr>
	</tag>
	<tag name="sybase_fetch_array" type="function" returnType="array">
		<attr name="result" type="resource"></attr>
	</tag>
	<tag name="sybase_fetch_assoc" type="function" returnType="array">
		<attr name="result" type="resource"></attr>
	</tag>
	<tag name="sybase_fetch_field" type="function" returnType="object">
		<attr name="result" type="resource"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="sybase_fetch_object" type="function" returnType="object">
		<attr name="result" type="resource"></attr>
		<attr name="object" type="mixed" status="optional"></attr>
	</tag>
	<tag name="sybase_fetch_row" type="function" returnType="array">
		<attr name="result" type="resource"></attr>
	</tag>
	<tag name="sybase_field_seek" type="function" returnType="bool">
		<attr name="result" type="resource"></attr>
		<attr name="field_offset" type="int"></attr>
	</tag>
	<tag name="sybase_free_result" type="function" returnType="bool">
		<attr name="result" type="resource"></attr>
	</tag>
	<tag name="sybase_get_last_message" type="function" returnType="string">
	</tag>
	<tag name="sybase_min_client_severity" type="function" returnType="void">
		<attr name="severity" type="int"></attr>
	</tag>
	<tag name="sybase_min_error_severity" type="function" returnType="void">
		<attr name="severity" type="int"></attr>
	</tag>
	<tag name="sybase_min_message_severity" type="function" returnType="void">
		<attr name="severity" type="int"></attr>
	</tag>
	<tag name="sybase_min_server_severity" type="function" returnType="void">
		<attr name="severity" type="int"></attr>
	</tag>
	<tag name="sybase_num_fields" type="function" returnType="int">
		<attr name="result" type="resource"></attr>
	</tag>
	<tag name="sybase_num_rows" type="function" returnType="int">
		<attr name="result" type="resource"></attr>
	</tag>
	<tag name="sybase_pconnect" type="function" returnType="resource">
		<attr name="servername" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
		<attr name="appname" type="string" status="optional"></attr>
	</tag>
	<tag name="sybase_query" type="function" returnType="resource">
		<attr name="query" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="sybase_result" type="function" returnType="string">
		<attr name="result" type="resource"></attr>
		<attr name="row" type="int"></attr>
		<attr name="field" type="mixed"></attr>
	</tag>
	<tag name="sybase_select_db" type="function" returnType="bool">
		<attr name="database_name" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="sybase_set_message_handler" type="function" returnType="bool">
		<attr name="handler" type="callback"></attr>
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="sybase_unbuffered_query" type="function" returnType="resource">
		<attr name="query" type="string"></attr>
		<attr name="link_identifier" type="resource"></attr>
		<attr name="store_result" type="bool" status="optional"></attr>
	</tag>
</tags>