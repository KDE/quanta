<!DOCTYPE tags>
<tags>
	<tag name="mysql_affected_rows" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_change_user" type="function" returnType="int" version="PHP 3&gt;= 3.0.13">
		<attr name="user" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="database" type="string" status="optional"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_client_encoding" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_close" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_connect" type="function" returnType="resource" version="PHP 3, PHP 4 , PHP 5">
		<attr name="server" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="new_link" type="bool" status="optional"></attr>
		<attr name="client_flags" type="int" status="optional"></attr>
	</tag>
	<tag name="mysql_create_db" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="database_name" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_data_seek" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result_identifier" type="resource" status="required"></attr>
		<attr name="row_number" type="int" status="required"></attr>
	</tag>
	<tag name="mysql_db_name" type="function" returnType="string" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="row" type="int" status="required"></attr>
		<attr name="field" type="mixed" status="optional"></attr>
	</tag>
	<tag name="mysql_db_query" type="function" returnType="resource" version="PHP 3, PHP 4 , PHP 5">
		<attr name="database" type="string" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_drop_db" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="database_name" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_errno" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_error" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_escape_string" type="function" returnType="string" version="PHP 4 &gt;= 4.0.3, PHP 5">
		<attr name="unescaped_string" type="string" status="required"></attr>
	</tag>
	<tag name="mysql_fetch_array" type="function" returnType="array" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="result_type" type="int" status="optional"></attr>
	</tag>
	<tag name="mysql_fetch_assoc" type="function" returnType="array" version="PHP 4 &gt;= 4.0.3, PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="mysql_fetch_field" type="function" returnType="object" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="mysql_fetch_lengths" type="function" returnType="array" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="mysql_fetch_object" type="function" returnType="object" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="mysql_fetch_row" type="function" returnType="array" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="mysql_field_flags" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="field_offset" type="int" status="required"></attr>
	</tag>
	<tag name="mysql_field_len" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="field_offset" type="int" status="required"></attr>
	</tag>
	<tag name="mysql_field_name" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="field_index" type="int" status="required"></attr>
	</tag>
	<tag name="mysql_field_seek" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="field_offset" type="int" status="required"></attr>
	</tag>
	<tag name="mysql_field_table" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="field_offset" type="int" status="required"></attr>
	</tag>
	<tag name="mysql_field_type" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="field_offset" type="int" status="required"></attr>
	</tag>
	<tag name="mysql_free_result" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="mysql_get_client_info" type="function" returnType="string" version="PHP 4 &gt;= 4.0.5, PHP 5">
	</tag>
	<tag name="mysql_get_host_info" type="function" returnType="string" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_get_proto_info" type="function" returnType="int" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_get_server_info" type="function" returnType="string" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_info" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_insert_id" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_list_dbs" type="function" returnType="resource" version="PHP 3, PHP 4 , PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_list_fields" type="function" returnType="resource" version="PHP 3, PHP 4 , PHP 5">
		<attr name="database_name" type="string" status="required"></attr>
		<attr name="table_name" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_list_processes" type="function" returnType="resource" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_list_tables" type="function" returnType="resource" version="PHP 3, PHP 4 , PHP 5">
		<attr name="database" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_num_fields" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="mysql_num_rows" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="mysql_pconnect" type="function" returnType="resource" version="PHP 3, PHP 4 , PHP 5">
		<attr name="server" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="client_flags" type="int" status="optional"></attr>
	</tag>
	<tag name="mysql_ping" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_query" type="function" returnType="resource" version="PHP 3, PHP 4 , PHP 5">
		<attr name="query" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_real_escape_string" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="unescaped_string" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_result" type="function" returnType="mixed" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="row" type="int" status="required"></attr>
		<attr name="field" type="mixed" status="optional"></attr>
	</tag>
	<tag name="mysql_select_db" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="database_name" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_stat" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_tablename" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="i" type="int" status="required"></attr>
	</tag>
	<tag name="mysql_thread_id" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mysql_unbuffered_query" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="query" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
</tags>