<!DOCTYPE tags>
<tags>	<tag name="pg_affected_rows" type="function" returnType="int">
		<attr name="result" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_cancel_query" type="function" returnType="bool">
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_client_encoding" type="function" returnType="string">
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_close" type="function" returnType="bool">
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_connect" type="function" returnType="resource">
		<attr name="connection_string" type="string" status="optional"></attr>
	</tag>
	<tag name="pg_connection_busy" type="function" returnType="bool">
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_connection_reset" type="function" returnType="bool">
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_connection_status" type="function" returnType="int">
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_convert" type="function" returnType="array">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="table_name" type="string" status="optional"></attr>
		<attr name="assoc_array" type="array" status="optional"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="pg_copy_from" type="function" returnType="int">
		<attr name="connection" type="int" status="optional"></attr>
		<attr name="table_name" type="string" status="optional"></attr>
		<attr name="rows" type="array" status="optional"></attr>
		<attr name="delimiter" type="string" status="optional"></attr>
		<attr name="null_as" type="string" status="optional"></attr>
	</tag>
	<tag name="pg_copy_to" type="function" returnType="int">
		<attr name="connection" type="int" status="optional"></attr>
		<attr name="table_name" type="string" status="optional"></attr>
		<attr name="delimiter" type="string" status="optional"></attr>
		<attr name="null_as" type="string" status="optional"></attr>
	</tag>
	<tag name="pg_dbname" type="function" returnType="string">
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_delete" type="function" returnType="long">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="table_name" type="string" status="optional"></attr>
		<attr name="assoc_array" type="array" status="optional"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="pg_end_copy" type="function" returnType="bool">
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_escape_bytea" type="function" returnType="string">
		<attr name="data" type="string" status="optional"></attr>
	</tag>
	<tag name="pg_escape_string" type="function" returnType="string">
		<attr name="data" type="string" status="optional"></attr>
	</tag>
	<tag name="pg_fetch_array" type="function" returnType="array">
		<attr name="result" type="resource" status="optional"></attr>
		<attr name="row" type="int" status="optional"></attr>
		<attr name="result_type" type="int" status="optional"></attr>
	</tag>
	<tag name="pg_fetch_object" type="function" returnType="object">
		<attr name="result" type="resource" status="optional"></attr>
		<attr name="row" type="int" status="optional"></attr>
		<attr name="result_type" type="int" status="optional"></attr>
	</tag>
	<tag name="pg_fetch_result" type="function" returnType="mixed">
		<attr name="result" type="resource" status="optional"></attr>
		<attr name="row" type="int" status="optional"></attr>
		<attr name="field" type="mixed" status="optional"></attr>
	</tag>
	<tag name="pg_fetch_row" type="function" returnType="array">
		<attr name="result" type="resource" status="optional"></attr>
		<attr name="row" type="int" status="optional"></attr>
	</tag>
	<tag name="pg_field_is_null" type="function" returnType="int">
		<attr name="result" type="resource" status="optional"></attr>
		<attr name="row" type="int" status="optional"></attr>
		<attr name="field" type="mixed" status="optional"></attr>
	</tag>
	<tag name="pg_field_name" type="function" returnType="string">
		<attr name="result" type="resource" status="optional"></attr>
		<attr name="field_number" type="int" status="optional"></attr>
	</tag>
	<tag name="pg_field_num" type="function" returnType="int">
		<attr name="result" type="resource" status="optional"></attr>
		<attr name="field_name" type="string" status="optional"></attr>
	</tag>
	<tag name="pg_field_prtlen" type="function" returnType="int">
		<attr name="result" type="resource" status="optional"></attr>
		<attr name="row_number" type="int" status="optional"></attr>
		<attr name="field_name" type="string" status="optional"></attr>
	</tag>
	<tag name="pg_field_size" type="function" returnType="int">
		<attr name="result" type="resource" status="optional"></attr>
		<attr name="field_number" type="int" status="optional"></attr>
	</tag>
	<tag name="pg_field_type" type="function" returnType="string">
		<attr name="result" type="resource" status="optional"></attr>
		<attr name="field_number" type="int" status="optional"></attr>
	</tag>
	<tag name="pg_free_result" type="function" returnType="bool">
		<attr name="result" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_get_result" type="function" returnType="resource">
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_host" type="function" returnType="string">
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_insert" type="function" returnType="bool">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="table_name" type="string" status="optional"></attr>
		<attr name="assoc_array" type="array" status="optional"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="pg_last_error" type="function" returnType="string">
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_last_notice" type="function" returnType="string">
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_last_oid" type="function" returnType="int">
		<attr name="result" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_lo_close" type="function" returnType="bool">
		<attr name="large_object" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_lo_create" type="function" returnType="int">
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_lo_export" type="function" returnType="bool">
		<attr name="oid" type="int" status="optional"></attr>
		<attr name="pathname" type="string" status="optional"></attr>
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_lo_import" type="function" returnType="int">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="pathname" type="string" status="optional"></attr>
	</tag>
	<tag name="pg_lo_open" type="function" returnType="resource">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="oid" type="int" status="optional"></attr>
		<attr name="mode" type="string" status="optional"></attr>
	</tag>
	<tag name="pg_lo_read_all" type="function" returnType="int">
		<attr name="large_object" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_lo_read" type="function" returnType="string">
		<attr name="large_object" type="resource" status="optional"></attr>
		<attr name="len" type="int" status="optional"></attr>
	</tag>
	<tag name="pg_lo_seek" type="function" returnType="bool">
		<attr name="large_object" type="resource" status="optional"></attr>
		<attr name="offset" type="int" status="optional"></attr>
		<attr name="whence" type="int" status="optional"></attr>
	</tag>
	<tag name="pg_lo_tell" type="function" returnType="int">
		<attr name="large_object" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_lo_unlink" type="function" returnType="bool">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="oid" type="int" status="optional"></attr>
	</tag>
	<tag name="pg_lo_write" type="function" returnType="int">
		<attr name="large_object" type="resource" status="optional"></attr>
		<attr name="data" type="string" status="optional"></attr>
	</tag>
	<tag name="pg_metadata" type="function" returnType="array">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="table_name" type="string" status="optional"></attr>
	</tag>
	<tag name="pg_num_fields" type="function" returnType="int">
		<attr name="result" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_num_rows" type="function" returnType="int">
		<attr name="result" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_options" type="function" returnType="string">
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_pconnect" type="function" returnType="int">
		<attr name="connection_string" type="string" status="optional"></attr>
	</tag>
	<tag name="pg_port" type="function" returnType="int">
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_put_line" type="function" returnType="bool">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="data" type="string" status="optional"></attr>
	</tag>
	<tag name="pg_query" type="function" returnType="resource">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="query" type="string" status="optional"></attr>
	</tag>
	<tag name="pg_result_error" type="function" returnType="string">
		<attr name="result" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_result_status" type="function" returnType="int">
		<attr name="result" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_select" type="function" returnType="array">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="table_name" type="string" status="optional"></attr>
		<attr name="assoc_array" type="array" status="optional"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="pg_send_query" type="function" returnType="bool">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="query" type="string" status="optional"></attr>
	</tag>
	<tag name="pg_set_client_encoding" type="function" returnType="int">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="pg_trace" type="function" returnType="bool">
		<attr name="pathname" type="string" status="optional"></attr>
		<attr name="mode" type="string" status="optional"></attr>
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_tty" type="function" returnType="string">
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_untrace" type="function" returnType="bool">
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="pg_update" type="function" returnType="long">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="table_name" type="string" status="optional"></attr>
		<attr name="condition" type="array" status="optional"></attr>
		<attr name="data" type="array" status="optional"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
</tags>
