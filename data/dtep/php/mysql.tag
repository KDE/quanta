<!DOCTYPE tags>
<tags>	<tag name="mysql_affected_rows" type="function" returnType="int">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_change_user" type="function" returnType="int">
		<attr name="user" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="database" type="string" status="optional"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_close" type="function" returnType="bool">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_connect" type="function" returnType="resource">
		<attr name="server" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="new_link" type="bool" status="optional"></attr>
		<attr name="client_flags" type="int" status="optional"></attr>
	</tag>
	<tag name="mysql_create_db" type="function" returnType="bool">
		<attr name="database name" type="string" status="optional"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_data_seek" type="function" returnType="bool">
		<attr name="result_identifier" type="resource" status="optional"></attr>
		<attr name="row_number" type="int" status="optional"></attr>
	</tag>
	<tag name="mysql_db_name" type="function" returnType="string">
		<attr name="result" type="resource" status="optional"></attr>
		<attr name="row" type="int" status="optional"></attr>
		<attr name="field" type="mixed" status="optional"></attr>
	</tag>
	<tag name="mysql_db_query" type="function" returnType="resource">
		<attr name="database" type="string" status="optional"></attr>
		<attr name="query" type="string" status="optional"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_drop_db" type="function" returnType="bool">
		<attr name="database_name" type="string" status="optional"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_errno" type="function" returnType="int">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_error" type="function" returnType="string">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_escape_string" type="function" returnType="string">
		<attr name="unescaped_string" type="string" status="optional"></attr>
	</tag>
	<tag name="mysql_fetch_array" type="function" returnType="array">
		<attr name="result" type="resource" status="optional"></attr>
		<attr name="result_type" type="int" status="optional"></attr>
	</tag>
	<tag name="mysql_fetch_assoc" type="function" returnType="array">
		<attr name="result" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_fetch_field" type="function" returnType="object">
		<attr name="result" type="resource" status="optional"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="mysql_fetch_lengths" type="function" returnType="array">
		<attr name="result" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_fetch_object" type="function" returnType="object">
		<attr name="result" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_fetch_row" type="function" returnType="array">
		<attr name="result" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_field_flags" type="function" returnType="string">
		<attr name="result" type="resource" status="optional"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="mysql_field_len" type="function" returnType="int">
		<attr name="result" type="resource" status="optional"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="mysql_field_name" type="function" returnType="string">
		<attr name="result" type="resource" status="optional"></attr>
		<attr name="field_index" type="int" status="optional"></attr>
	</tag>
	<tag name="mysql_field_seek" type="function" returnType="int">
		<attr name="result" type="resource" status="optional"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="mysql_field_table" type="function" returnType="string">
		<attr name="result" type="resource" status="optional"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="mysql_field_type" type="function" returnType="string">
		<attr name="result" type="resource" status="optional"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="mysql_free_result" type="function" returnType="bool">
		<attr name="result" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_get_client_info" type="function" returnType="string">
	</tag>
	<tag name="mysql_get_host_info" type="function" returnType="string">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_get_proto_info" type="function" returnType="int">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_get_server_info" type="function" returnType="string">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_info" type="function" returnType="string">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_insert_id" type="function" returnType="int">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_list_dbs" type="function" returnType="resource">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_list_fields" type="function" returnType="resource">
		<attr name="database_name" type="string" status="optional"></attr>
		<attr name="table_name" type="string" status="optional"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_list_processes" type="function" returnType="resource">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_list_tables" type="function" returnType="resource">
		<attr name="database" type="string" status="optional"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_num_fields" type="function" returnType="int">
		<attr name="result" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_num_rows" type="function" returnType="int">
		<attr name="result" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_pconnect" type="function" returnType="resource">
		<attr name="server" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="client_flags" type="int" status="optional"></attr>
	</tag>
	<tag name="mysql_ping" type="function" returnType="bool">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_query" type="function" returnType="resource">
		<attr name="query" type="string" status="optional"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="result_mode" type="int" status="optional"></attr>
	</tag>
	<tag name="mysql_real_escape_string" type="function" returnType="string">
		<attr name="unescaped_string" type="string" status="optional"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_result" type="function" returnType="mixed">
		<attr name="result" type="resource" status="optional"></attr>
		<attr name="row" type="int" status="optional"></attr>
		<attr name="field" type="mixed" status="optional"></attr>
	</tag>
	<tag name="mysql_select_db" type="function" returnType="bool">
		<attr name="database_name" type="string" status="optional"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_stat" type="function" returnType="string">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_tablename" type="function" returnType="string">
		<attr name="result" type="resource" status="optional"></attr>
		<attr name="i" type="int" status="optional"></attr>
	</tag>
	<tag name="mysql_thread_id" type="function" returnType="int">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_unbuffered_query" type="function" returnType="resource">
		<attr name="query" type="string" status="optional"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="result_mode" type="int" status="optional"></attr>
	</tag>
</tags>
