<!DOCTYPE tags>
<tags>
	<tag name="mssql_bind" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
		<attr name="param_name" type="string" status="required"></attr>
		<attr name="&amp;var" type="mixed" status="required"></attr>
		<attr name="type" type="int" status="required"></attr>
		<attr name="is_output" type="int" status="optional"></attr>
		<attr name="is_null" type="int" status="optional"></attr>
		<attr name="maxlen" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_close" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="mssql_connect" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="servername" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="mssql_data_seek" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result_identifier" type="resource" status="required"></attr>
		<attr name="row_number" type="int" status="required"></attr>
	</tag>
	<tag name="mssql_execute" type="function" returnType="mixed" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="stmt" type="resource" status="required"></attr>
		<attr name="skip_results" type="bool" status="optional"></attr>
	</tag>
	<tag name="mssql_fetch_array" type="function" returnType="array" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="result_type" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_fetch_assoc" type="function" returnType="array" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="result_id" type="resource" status="required"></attr>
	</tag>
	<tag name="mssql_fetch_batch" type="function" returnType="int" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="result_index" type="resource" status="required"></attr>
	</tag>
	<tag name="mssql_fetch_field" type="function" returnType="object" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_fetch_object" type="function" returnType="object" version="PHP 3, PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="mssql_fetch_row" type="function" returnType="array" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="mssql_field_length" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_field_name" type="function" returnType="string" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_field_seek" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="field_offset" type="int" status="required"></attr>
	</tag>
	<tag name="mssql_field_type" type="function" returnType="string" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_free_result" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="mssql_free_statement" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.2, PHP 5">
		<attr name="statement" type="resource" status="required"></attr>
	</tag>
	<tag name="mssql_get_last_message" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
	</tag>
	<tag name="mssql_guid_string" type="function" returnType="string" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="binary" type="string" status="required"></attr>
		<attr name="short_format" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_init" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="sp_name" type="string" status="required"></attr>
		<attr name="conn_id" type="resource" status="optional"></attr>
	</tag>
	<tag name="mssql_min_error_severity" type="function" returnType="void" version="PHP 3, PHP 4 , PHP 5">
		<attr name="severity" type="int" status="required"></attr>
	</tag>
	<tag name="mssql_min_message_severity" type="function" returnType="void" version="PHP 3, PHP 4 , PHP 5">
		<attr name="severity" type="int" status="required"></attr>
	</tag>
	<tag name="mssql_next_result" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="result_id" type="resource" status="required"></attr>
	</tag>
	<tag name="mssql_num_fields" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="mssql_num_rows" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="mssql_pconnect" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="servername" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="mssql_query" type="function" returnType="resource" version="PHP 3, PHP 4 , PHP 5">
		<attr name="query" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="batch_size" type="int" status="optional"></attr>
	</tag>
	<tag name="mssql_result" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="row" type="int" status="required"></attr>
		<attr name="field" type="mixed" status="required"></attr>
	</tag>
	<tag name="mssql_rows_affected" type="function" returnType="int" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="conn_id" type="resource" status="required"></attr>
	</tag>
	<tag name="mssql_select_db" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="database_name" type="string" status="required"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
</tags>