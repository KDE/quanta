<!DOCTYPE tags>
<tags>
	<tag name="mysqli_affected_rows" type="function" returnType="mixed" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_autocommit" type="function" returnType="bool" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
		<attr name="mode" type="bool" status="required"></attr>
	</tag>
	<tag name="mysqli_change_user" type="function" returnType="bool" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
		<attr name="user" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="database" type="string" status="required"></attr>
	</tag>
	<tag name="mysqli_character_set_name" type="function" returnType="string" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_close" type="function" returnType="bool" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_commit" type="function" returnType="bool" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_connect_errno" type="function" returnType="int" version="PHP 5">
	</tag>
	<tag name="mysqli_connect_error" type="function" returnType="string" version="PHP 5">
	</tag>
	<tag name="mysqli_connect" type="function" returnType="object" version="PHP 5">
		<attr name="host" type="string"></attr>
		<attr name="username" type="string"></attr>
		<attr name="passwd" type="string"></attr>
		<attr name="dbname" type="string"></attr>
		<attr name="port" type="int"></attr>
		<attr name="socket" type="string"></attr>
	</tag>
	<tag name="mysqli_data_seek" type="function" returnType="bool" version="PHP 5">
		<attr name="result" type="object" status="required"></attr>
		<attr name="offset" type="int" status="required"></attr>
	</tag>
	<tag name="mysqli_debug" type="function" returnType="void" version="PHP 5">
		<attr name="debug" type="string" status="required"></attr>
	</tag>
	<tag name="mysqli_disable_reads_from_master" type="function" returnType="void" version="PHP 5">
		<attr name="link" type="resource" status="required"></attr>
	</tag>
	<tag name="mysqli_disable_rpl_parse" type="function" returnType="void" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_dump_debug_info" type="function" returnType="bool" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_embedded_connect" type="function" returnType="object" version="PHP 5">
		<attr name="dbname" type="string" status="optional"></attr>
	</tag>
	<tag name="mysqli_enable_reads_from_master" type="function" returnType="void" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_enable_rpl_parse" type="function" returnType="void" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_errno" type="function" returnType="int" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_error" type="function" returnType="string" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_fetch_array" type="function" returnType="mixed" version="PHP 5">
		<attr name="result" type="object" status="required"></attr>
		<attr name="resulttype" type="int" status="required"></attr>
	</tag>
	<tag name="mysqli_fetch_assoc" type="function" returnType="array" version="PHP 5">
		<attr name="result" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_fetch_field_direct" type="function" returnType="mixed" version="PHP 5">
		<attr name="result" type="object" status="required"></attr>
		<attr name="fieldnr" type="int" status="required"></attr>
	</tag>
	<tag name="mysqli_fetch_field" type="function" returnType="mixed" version="PHP 5">
		<attr name="result" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_fetch_fields" type="function" returnType="mixed" version="PHP 5">
		<attr name="result" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_fetch_lengths" type="function" returnType="mixed" version="PHP 5">
		<attr name="result" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_fetch_object" type="function" returnType="mixed" version="PHP 5">
		<attr name="result" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_fetch_row" type="function" returnType="mixed" version="PHP 5">
		<attr name="result" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_field_count" type="function" returnType="int" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_field_seek" type="function" returnType="int" version="PHP 5">
		<attr name="result" type="object" status="required"></attr>
		<attr name="fieldnr" type="int" status="required"></attr>
	</tag>
	<tag name="mysqli_field_tell" type="function" returnType="int" version="PHP 5">
		<attr name="result" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_free_result" type="function" returnType="void" version="PHP 5">
		<attr name="result" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_get_client_info" type="function" returnType="string" version="PHP 5">
	</tag>
	<tag name="mysqli_get_client_version" type="function" returnType="int" version="PHP 5">
	</tag>
	<tag name="mysqli_get_host_info" type="function" returnType="string" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_get_proto_info" type="function" returnType="int" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_get_server_info" type="function" returnType="string" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_get_server_version" type="function" returnType="int" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_info" type="function" returnType="string" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_init" type="function" returnType="object" version="PHP 5">
	</tag>
	<tag name="mysqli_insert_id" type="function" returnType="mixed" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_kill" type="function" returnType="bool" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
		<attr name="processid" type="int" status="required"></attr>
	</tag>
	<tag name="mysqli_master_query" type="function" returnType="bool" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
	</tag>
	<tag name="mysqli_more_results" type="function" returnType="bool" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_multi_query" type="function" returnType="bool" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
	</tag>
	<tag name="mysqli_next_result" type="function" returnType="bool" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_num_fields" type="function" returnType="int" version="PHP 5">
		<attr name="result" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_num_rows" type="function" returnType="mixed" version="PHP 5">
		<attr name="result" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_options" type="function" returnType="bool" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
		<attr name="option" type="int" status="required"></attr>
		<attr name="value" type="mixed" status="required"></attr>
	</tag>
	<tag name="mysqli_ping" type="function" returnType="bool" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_prepare" type="function" returnType="mixed" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
	</tag>
	<tag name="mysqli_query" type="function" returnType="mixed" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
		<attr name="resultmode" type="int" status="required"></attr>
	</tag>
	<tag name="mysqli_real_connect" type="function" returnType="bool" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
		<attr name="hostname" type="string" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="passwd" type="string" status="required"></attr>
		<attr name="dbname" type="string" status="required"></attr>
		<attr name="port" type="int" status="required"></attr>
		<attr name="socket" type="string" status="required"></attr>
		<attr name="flags" type="int" status="required"></attr>
	</tag>
	<tag name="mysqli_real_escape_string" type="function" returnType="string" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
		<attr name="escapestr" type="string" status="required"></attr>
	</tag>
	<tag name="mysqli_real_query" type="function" returnType="bool" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
	</tag>
	<tag name="mysqli_report" type="function" returnType="bool" version="PHP 5">
		<attr name="flags" type="int" status="required"></attr>
	</tag>
	<tag name="mysqli_rollback" type="function" returnType="bool" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_rpl_parse_enabled" type="function" returnType="int" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_rpl_probe" type="function" returnType="bool" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_rpl_query_type" type="function" returnType="int" version="PHP 5">
		<attr name="query" type="string" status="required"></attr>
	</tag>
	<tag name="mysqli_select_db" type="function" returnType="bool" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
		<attr name="dbname" type="string" status="required"></attr>
	</tag>
	<tag name="mysqli_send_query" type="function" returnType="bool" version="PHP 5">
		<attr name="link" type="resource" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
	</tag>
	<tag name="mysqli_server_end" type="function" returnType="void" version="PHP 5">
	</tag>
	<tag name="mysqli_server_init" type="function" returnType="bool" version="PHP 5">
		<attr name="server" type="array" status="optional"></attr>
		<attr name="groups" type="array" status="optional"></attr>
	</tag>
	<tag name="mysqli_sqlstate" type="function" returnType="string" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_ssl_set" type="function" returnType="bool" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
		<attr name="cert" type="string" status="required"></attr>
		<attr name="ca" type="string" status="required"></attr>
		<attr name="capath" type="string" status="required"></attr>
		<attr name="cipher" type="string" status="required"></attr>
	</tag>
	<tag name="mysqli_stat" type="function" returnType="mixed" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_stmt_affected_rows" type="function" returnType="mixed" version="PHP 5">
		<attr name="stmt" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_stmt_bind_param" type="function" returnType="bool" version="PHP 5">
		<attr name="stmt" type="object" status="required"></attr>
		<attr name="types" type="string" status="required"></attr>
		<attr name="&amp;var1" type="mixed" status="required"></attr>
		<attr name="&amp;..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="mysqli_stmt_bind_result" type="function" returnType="bool" version="PHP 5">
		<attr name="stmt" type="object" status="required"></attr>
		<attr name="&amp;var1" type="mixed" status="required"></attr>
		<attr name="&amp;..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="mysqli_stmt_close" type="function" returnType="bool" version="PHP 5">
		<attr name="stmt" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_stmt_data_seek" type="function" returnType="bool" version="PHP 5">
		<attr name="statement" type="object" status="required"></attr>
		<attr name="offset" type="int" status="required"></attr>
	</tag>
	<tag name="mysqli_stmt_errno" type="function" returnType="int" version="PHP 5">
		<attr name="stmt" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_stmt_error" type="function" returnType="string" version="PHP 5">
		<attr name="stmt" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_stmt_execute" type="function" returnType="bool" version="PHP 5">
		<attr name="stmt" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_stmt_fetch" type="function" returnType="mixed" version="PHP 5">
		<attr name="stmt" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_stmt_free_result" type="function" returnType="void" version="PHP 5">
		<attr name="stmt" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_stmt_init" type="function" returnType="object" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_stmt_num_rows" type="function" returnType="mixed" version="PHP 5">
		<attr name="stmt" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_stmt_param_count" type="function" returnType="int" version="PHP 5">
		<attr name="stmt" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_stmt_prepare" type="function" returnType="bool" version="PHP 5">
		<attr name="stmt" type="object" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
	</tag>
	<tag name="mysqli_stmt_reset" type="function" returnType="bool" version="PHP 5">
		<attr name="stmt" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_stmt_result_metadata" type="function" returnType="mixed" version="PHP 5">
		<attr name="stmt" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_stmt_send_long_data" type="function" returnType="bool" version="PHP 5">
		<attr name="stmt" type="object" status="required"></attr>
		<attr name="param_nr" type="int" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
	</tag>
	<tag name="mysqli_stmt_sqlstate" type="function" returnType="string" version="PHP 5">
		<attr name="stmt" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_stmt_store_result" type="function" returnType="bool" version="PHP 5">
		<attr name="stmt" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_store_result" type="function" returnType="object" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_thread_id" type="function" returnType="int" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_thread_safe" type="function" returnType="bool" version="PHP 5">
	</tag>
	<tag name="mysqli_use_result" type="function" returnType="mixed" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
	<tag name="mysqli_warning_count" type="function" returnType="int" version="PHP 5">
		<attr name="link" type="object" status="required"></attr>
	</tag>
</tags>