<!DOCTYPE tags>
<tags>
	<tag name="fbsql_affected_rows" type="function" returnType="int" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_autocommit" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="link_identifier" type="resource" status="required"></attr>
		<attr name="OnOff" type="bool" status="optional"></attr>
	</tag>
	<tag name="fbsql_blob_size" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="blob_handle" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_change_user" type="function" returnType="resource" version="">
		<attr name="user" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="database" type="string" status="optional"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_clob_size" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="clob_handle" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_close" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_commit" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_connect" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="hostname" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="fbsql_create_blob" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="blob_data" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_create_clob" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="clob_data" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_create_db" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="database_name" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_data_seek" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="result_identifier" type="resource" status="required"></attr>
		<attr name="row_number" type="int" status="required"></attr>
	</tag>
	<tag name="fbsql_database_password" type="function" returnType="string" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="link_identifier" type="resource" status="required"></attr>
		<attr name="database_password" type="string" status="optional"></attr>
	</tag>
	<tag name="fbsql_database" type="function" returnType="string" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="link_identifier" type="resource" status="required"></attr>
		<attr name="database" type="string" status="optional"></attr>
	</tag>
	<tag name="fbsql_db_query" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="database" type="string" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_db_status" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="database_name" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_drop_db" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="database_name" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_errno" type="function" returnType="int" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_error" type="function" returnType="string" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_fetch_array" type="function" returnType="array" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="result_type" type="int" status="optional"></attr>
	</tag>
	<tag name="fbsql_fetch_assoc" type="function" returnType="array" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="fbsql_fetch_field" type="function" returnType="object" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="fbsql_fetch_lengths" type="function" returnType="array" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="fbsql_fetch_object" type="function" returnType="object" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="result_type" type="int" status="optional"></attr>
	</tag>
	<tag name="fbsql_fetch_row" type="function" returnType="array" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="fbsql_field_flags" type="function" returnType="string" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="fbsql_field_len" type="function" returnType="int" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="fbsql_field_name" type="function" returnType="string" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="field_index" type="int" status="optional"></attr>
	</tag>
	<tag name="fbsql_field_seek" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="fbsql_field_table" type="function" returnType="string" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="fbsql_field_type" type="function" returnType="string" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="fbsql_free_result" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="fbsql_get_autostart_info" type="function" returnType="array" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_hostname" type="function" returnType="string" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="link_identifier" type="resource" status="required"></attr>
		<attr name="host_name" type="string" status="optional"></attr>
	</tag>
	<tag name="fbsql_insert_id" type="function" returnType="int" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_list_dbs" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_list_fields" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="database_name" type="string" status="required"></attr>
		<attr name="table_name" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_list_tables" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="database" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_next_result" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="result_id" type="resource" status="required"></attr>
	</tag>
	<tag name="fbsql_num_fields" type="function" returnType="int" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="fbsql_num_rows" type="function" returnType="int" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="fbsql_password" type="function" returnType="string" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="link_identifier" type="resource" status="required"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="fbsql_pconnect" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="hostname" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="fbsql_query" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="query" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_read_blob" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="blob_handle" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_read_clob" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="clob_handle" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_result" type="function" returnType="mixed" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="row" type="int" status="optional"></attr>
		<attr name="field" type="mixed" status="optional"></attr>
	</tag>
	<tag name="fbsql_rollback" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_select_db" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="database_name" type="string" status="optional"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_set_lob_mode" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="database_name" type="string" status="required"></attr>
	</tag>
	<tag name="fbsql_set_password" type="function" returnType="bool" version="PHP 5">
		<attr name="link_identifier" type="resource" status="required"></attr>
		<attr name="user" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="old_password" type="string" status="required"></attr>
	</tag>
	<tag name="fbsql_set_transaction" type="function" returnType="void" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="link_identifier" type="resource" status="required"></attr>
		<attr name="Locking" type="int" status="required"></attr>
		<attr name="Isolation" type="int" status="required"></attr>
	</tag>
	<tag name="fbsql_start_db" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="database_name" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_stop_db" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="database_name" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_tablename" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="i" type="int" status="required"></attr>
	</tag>
	<tag name="fbsql_username" type="function" returnType="string" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="link_identifier" type="resource" status="required"></attr>
		<attr name="username" type="string" status="optional"></attr>
	</tag>
	<tag name="fbsql_warnings" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="OnOff" type="bool" status="optional"></attr>
	</tag>
</tags>