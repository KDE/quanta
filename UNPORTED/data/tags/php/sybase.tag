<!DOCTYPE tags>
<tags>	<tag name="sybase_affected_rows" type="function" returnType="int">
		<attr name="link_identifier" type="int" status="optional"></attr>
	</tag>
	<tag name="sybase_close" type="function" returnType="bool">
		<attr name="link_identifier" type="int" status="optional"></attr>
	</tag>
	<tag name="sybase_connect" type="function" returnType="int">
		<attr name="servername" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="sybase_data_seek" type="function" returnType="bool">
		<attr name="result_identifier" type="int" status="optional"></attr>
		<attr name="row_number" type="int" status="optional"></attr>
	</tag>
	<tag name="sybase_fetch_array" type="function" returnType="array">
		<attr name="result" type="int" status="optional"></attr>
	</tag>
	<tag name="sybase_fetch_field" type="function" returnType="object">
		<attr name="result" type="int" status="optional"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="sybase_fetch_object" type="function" returnType="int">
		<attr name="result" type="int" status="optional"></attr>
	</tag>
	<tag name="sybase_fetch_row" type="function" returnType="array">
		<attr name="result" type="int" status="optional"></attr>
	</tag>
	<tag name="sybase_field_seek" type="function" returnType="int">
		<attr name="result" type="int" status="optional"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="sybase_free_result" type="function" returnType="bool">
		<attr name="result" type="int" status="optional"></attr>
	</tag>
	<tag name="sybase_get_last_message" type="function" returnType="string">
	</tag>
	<tag name="sybase_min_client_severity" type="function" returnType="void">
		<attr name="severity" type="int" status="optional"></attr>
	</tag>
	<tag name="sybase_min_error_severity" type="function" returnType="void">
		<attr name="severity" type="int" status="optional"></attr>
	</tag>
	<tag name="sybase_min_message_severity" type="function" returnType="void">
		<attr name="severity" type="int" status="optional"></attr>
	</tag>
	<tag name="sybase_min_server_severity" type="function" returnType="void">
		<attr name="severity" type="int" status="optional"></attr>
	</tag>
	<tag name="sybase_num_fields" type="function" returnType="int">
		<attr name="result" type="int" status="optional"></attr>
	</tag>
	<tag name="sybase_num_rows" type="function" returnType="int">
		<attr name="result" type="int" status="optional"></attr>
	</tag>
	<tag name="sybase_pconnect" type="function" returnType="int">
		<attr name="servername" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="sybase_query" type="function" returnType="int">
		<attr name="query" type="string" status="optional"></attr>
		<attr name="link_identifier" type="int" status="optional"></attr>
	</tag>
	<tag name="sybase_result" type="function" returnType="string">
		<attr name="result" type="int" status="optional"></attr>
		<attr name="row" type="int" status="optional"></attr>
		<attr name="field" type="mixed" status="optional"></attr>
	</tag>
	<tag name="sybase_select_db" type="function" returnType="bool">
		<attr name="database_name" type="string" status="optional"></attr>
		<attr name="link_identifier" type="int" status="optional"></attr>
	</tag>
</tags>
