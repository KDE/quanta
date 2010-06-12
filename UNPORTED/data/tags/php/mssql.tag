<!DOCTYPE tags>
<tags>	<tag name="mssql_bind" type="function" returnType="int">
		<attr name="stmt" type="int" status="optional"></attr>
		<attr name="param_name" type="string" status="optional"></attr>
		<attr name="var" type="mixed" status="optional"></attr>
		<attr name="type" type="int" status="optional"></attr>
		<attr name="is_output" type="int" status="optional"></attr>
		<attr name="is_null" type="int" status="optional"></attr>
		<attr name="maxlen" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_close" type="function" returnType="int">
		<attr name="link_identifier" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_connect" type="function" returnType="int">
		<attr name="servername" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="mssql_data_seek" type="function" returnType="int">
		<attr name="result_identifier" type="int" status="optional"></attr>
		<attr name="row_number" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_execute" type="function" returnType="int">
		<attr name="stmt" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_fetch_array" type="function" returnType="int">
		<attr name="result" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_fetch_assoc" type="function" returnType="array">
		<attr name="result_id" type="int" status="optional"></attr>
		<attr name="result_type" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_fetch_batch" type="function" returnType="int">
		<attr name="result_index" type="string" status="optional"></attr>
	</tag>
	<tag name="mssql_fetch_field" type="function" returnType="object">
		<attr name="result" type="int" status="optional"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_fetch_object" type="function" returnType="int">
		<attr name="result" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_fetch_row" type="function" returnType="array">
		<attr name="result" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_field_length" type="function" returnType="int">
		<attr name="result" type="int" status="optional"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_field_name" type="function" returnType="int">
		<attr name="result" type="int" status="optional"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_field_seek" type="function" returnType="int">
		<attr name="result" type="int" status="optional"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_field_type" type="function" returnType="string">
		<attr name="result" type="int" status="optional"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_free_result" type="function" returnType="int">
		<attr name="result" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_get_last_message" type="function" returnType="string">
	</tag>
	<tag name="mssql_guid_string" type="function" returnType="string">
		<attr name="binary" type="string" status="optional"></attr>
		<attr name="short_format" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_init" type="function" returnType="int">
		<attr name="sp_name" type="string" status="optional"></attr>
		<attr name="conn_id" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_min_error_severity" type="function" returnType="void">
		<attr name="severity" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_min_message_severity" type="function" returnType="void">
		<attr name="severity" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_next_result" type="function" returnType="bool">
		<attr name="result_id" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_num_fields" type="function" returnType="int">
		<attr name="result" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_num_rows" type="function" returnType="int">
		<attr name="result" type="string" status="optional"></attr>
	</tag>
	<tag name="mssql_pconnect" type="function" returnType="int">
		<attr name="servername" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="mssql_query" type="function" returnType="int">
		<attr name="query" type="string" status="optional"></attr>
		<attr name="link_identifier" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_result" type="function" returnType="int">
		<attr name="result" type="int" status="optional"></attr>
		<attr name="i" type="int" status="optional"></attr>
		<attr name="field" type="mixed" status="optional"></attr>
	</tag>
	<tag name="mssql_rows_affected" type="function" returnType="int">
		<attr name="conn_id" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_select_db" type="function" returnType="int">
		<attr name="database_name" type="string" status="optional"></attr>
		<attr name="link_identifier" type="int" status="optional"></attr>
	</tag>
</tags>
