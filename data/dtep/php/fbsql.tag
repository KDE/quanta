<!DOCTYPE tags>
<tags>
	<tag name="fbsql_affected_rows" type="function" returnType="int">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_autocommit" type="function" returnType="bool">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="OnOff" type="bool" status="optional"></attr>
	</tag>
	<tag name="fbsql_blob_size" type="function" returnType="int">
		<attr name="blob_handle" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_change_user" type="function" returnType="resource">
		<attr name="user" type="string"></attr>
		<attr name="password" type="string"></attr>
		<attr name="database" type="string" status="optional"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_clob_size" type="function" returnType="int">
		<attr name="clob_handle" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_close" type="function" returnType="bool">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_commit" type="function" returnType="bool">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_connect" type="function" returnType="resource">
		<attr name="hostname" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="fbsql_create_blob" type="function" returnType="string">
		<attr name="blob_data" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_create_clob" type="function" returnType="string">
		<attr name="clob_data" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_create_db" type="function" returnType="bool">
		<attr name="database_name" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_data_seek" type="function" returnType="bool">
		<attr name="result_identifier" type="resource"></attr>
		<attr name="row_number" type="int"></attr>
	</tag>
	<tag name="fbsql_database_password" type="function" returnType="string">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="database_password" type="string" status="optional"></attr>
	</tag>
	<tag name="fbsql_database" type="function" returnType="string">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="database" type="string" status="optional"></attr>
	</tag>
	<tag name="fbsql_db_query" type="function" returnType="resource">
		<attr name="database" type="string"></attr>
		<attr name="query" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_db_status" type="function" returnType="int">
		<attr name="database_name" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_drop_db" type="function" returnType="bool">
		<attr name="database_name" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_errno" type="function" returnType="int">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_error" type="function" returnType="string">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_fetch_array" type="function" returnType="array">
		<attr name="result" type="resource"></attr>
		<attr name="result_type" type="int" status="optional"></attr>
	</tag>
	<tag name="fbsql_fetch_assoc" type="function" returnType="array">
		<attr name="result" type="resource"></attr>
	</tag>
	<tag name="fbsql_fetch_field" type="function" returnType="object">
		<attr name="result" type="resource"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="fbsql_fetch_lengths" type="function" returnType="array">
		<attr name="result" type="resource"></attr>
	</tag>
	<tag name="fbsql_fetch_object" type="function" returnType="object">
		<attr name="result" type="resource"></attr>
		<attr name="result_type" type="int" status="optional"></attr>
	</tag>
	<tag name="fbsql_fetch_row" type="function" returnType="array">
		<attr name="result" type="resource"></attr>
	</tag>
	<tag name="fbsql_field_flags" type="function" returnType="string">
		<attr name="result" type="resource"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="fbsql_field_len" type="function" returnType="int">
		<attr name="result" type="resource"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="fbsql_field_name" type="function" returnType="string">
		<attr name="result" type="resource"></attr>
		<attr name="field_index" type="int" status="optional"></attr>
	</tag>
	<tag name="fbsql_field_seek" type="function" returnType="bool">
		<attr name="result" type="resource"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="fbsql_field_table" type="function" returnType="string">
		<attr name="result" type="resource"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="fbsql_field_type" type="function" returnType="string">
		<attr name="result" type="resource"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="fbsql_free_result" type="function" returnType="bool">
		<attr name="result" type="resource"></attr>
	</tag>
	<tag name="fbsql_get_autostart_info" type="function" returnType="array">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_hostname" type="function" returnType="string">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="host_name" type="string" status="optional"></attr>
	</tag>
	<tag name="fbsql_insert_id" type="function" returnType="int">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_list_dbs" type="function" returnType="resource">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_list_fields" type="function" returnType="resource">
		<attr name="database_name" type="string"></attr>
		<attr name="table_name" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_list_tables" type="function" returnType="resource">
		<attr name="database" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_next_result" type="function" returnType="bool">
		<attr name="result_id" type="resource"></attr>
	</tag>
	<tag name="fbsql_num_fields" type="function" returnType="int">
		<attr name="result" type="resource"></attr>
	</tag>
	<tag name="fbsql_num_rows" type="function" returnType="int">
		<attr name="result" type="resource"></attr>
	</tag>
	<tag name="fbsql_password" type="function" returnType="string">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="fbsql_pconnect" type="function" returnType="resource">
		<attr name="hostname" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="fbsql_query" type="function" returnType="resource">
		<attr name="query" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_read_blob" type="function" returnType="string">
		<attr name="blob_handle" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_read_clob" type="function" returnType="string">
		<attr name="clob_handle" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_result" type="function" returnType="mixed">
		<attr name="result" type="resource"></attr>
		<attr name="row" type="int" status="optional"></attr>
		<attr name="field" type="mixed" status="optional"></attr>
	</tag>
	<tag name="fbsql_rollback" type="function" returnType="bool">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_select_db" type="function" returnType="bool">
		<attr name="database_name" type="string" status="optional"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_set_lob_mode" type="function" returnType="bool">
		<attr name="result" type="resource"></attr>
		<attr name="database_name" type="string"></attr>
	</tag>
	<tag name="fbsql_set_password" type="function" returnType="bool">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="user" type="string"></attr>
		<attr name="password" type="string"></attr>
		<attr name="old_password" type="string"></attr>
	</tag>
	<tag name="fbsql_set_transaction" type="function" returnType="void">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="Locking" type="int"></attr>
		<attr name="Isolation" type="int"></attr>
	</tag>
	<tag name="fbsql_start_db" type="function" returnType="bool">
		<attr name="database_name" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_stop_db" type="function" returnType="bool">
		<attr name="database_name" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="fbsql_tablename" type="function" returnType="string">
		<attr name="result" type="resource"></attr>
		<attr name="i" type="int"></attr>
	</tag>
	<tag name="fbsql_username" type="function" returnType="string">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="username" type="string" status="optional"></attr>
	</tag>
	<tag name="fbsql_warnings" type="function" returnType="bool">
		<attr name="OnOff" type="bool" status="optional"></attr>
	</tag>
</tags>