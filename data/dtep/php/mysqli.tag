<!DOCTYPE tags>
<tags>
	<tag name="mysqli_affected_rows" type="function" returnType="mixed">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_autocommit" type="function" returnType="bool">
		<attr name="link" type="object"></attr>
		<attr name="mode" type="bool"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="mysqli_change_user" type="function" returnType="bool">
		<attr name="link" type="object"></attr>
		<attr name="user" type="string"></attr>
		<attr name="password" type="string"></attr>
		<attr name="database" type="string"></attr>
	</tag>
	<tag name="mysqli_character_set_name" type="function" returnType="string">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="mysqli_close" type="function" returnType="bool">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_commit" type="function" returnType="bool">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_connect_errno" type="function" returnType="int">
	</tag>
	<tag name="mysqli_connect_error" type="function" returnType="string">
	</tag>
	<tag name="mysqli_connect" type="function" returnType="object">
		<attr name="host" type="string"></attr>
		<attr name="username" type="string"></attr>
		<attr name="passwd" type="string"></attr>
		<attr name="dbname" type="string"></attr>
		<attr name="port" type="int"></attr>
		<attr name="socket" type="string"></attr>
	</tag>
	<tag name="mysqli_data_seek" type="function" returnType="bool">
		<attr name="result" type="object"></attr>
		<attr name="offset" type="int"></attr>
	</tag>
	<tag name="mysqli_debug" type="function" returnType="void">
		<attr name="debug" type="string"></attr>
	</tag>
	<tag name="mysqli_disable_reads_from_master" type="function" returnType="void">
		<attr name="link" type="resource"></attr>
	</tag>
	<tag name="mysqli_disable_rpl_parse" type="function" returnType="void">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_dump_debug_info" type="function" returnType="bool">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_embedded_connect" type="function" returnType="object">
		<attr name="dbname" type="string" status="optional"></attr>
	</tag>
	<tag name="mysqli_enable_reads_from_master" type="function" returnType="void">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_enable_rpl_parse" type="function" returnType="void">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_errno" type="function" returnType="int">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_error" type="function" returnType="string">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="mysqli_fetch_array" type="function" returnType="mixed">
		<attr name="result" type="object"></attr>
		<attr name="resulttype" type="int"></attr>
	</tag>
	<tag name="mysqli_fetch_assoc" type="function" returnType="array">
		<attr name="result" type="object"></attr>
	</tag>
	<tag name="mysqli_fetch_field_direct" type="function" returnType="mixed">
		<attr name="result" type="object"></attr>
		<attr name="fieldnr" type="int"></attr>
	</tag>
	<tag name="mysqli_fetch_field" type="function" returnType="mixed">
		<attr name="result" type="object"></attr>
	</tag>
	<tag name="mysqli_fetch_fields" type="function" returnType="mixed">
		<attr name="result" type="object"></attr>
	</tag>
	<tag name="mysqli_fetch_lengths" type="function" returnType="mixed">
		<attr name="result" type="object"></attr>
	</tag>
	<tag name="mysqli_fetch_object" type="function" returnType="mixed">
		<attr name="result" type="object"></attr>
	</tag>
	<tag name="mysqli_fetch_row" type="function" returnType="mixed">
		<attr name="result" type="object"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="mysqli_field_count" type="function" returnType="int">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_field_seek" type="function" returnType="int">
		<attr name="result" type="object"></attr>
		<attr name="fieldnr" type="int"></attr>
	</tag>
	<tag name="mysqli_field_tell" type="function" returnType="int">
		<attr name="result" type="object"></attr>
	</tag>
	<tag name="mysqli_free_result" type="function" returnType="void">
		<attr name="result" type="object"></attr>
	</tag>
	<tag name="mysqli_get_client_info" type="function" returnType="string">
	</tag>
	<tag name="mysqli_get_client_version" type="function" returnType="int">
	</tag>
	<tag name="mysqli_get_host_info" type="function" returnType="string">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="mysqli_get_proto_info" type="function" returnType="int">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_get_server_info" type="function" returnType="string">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_get_server_version" type="function" returnType="int">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_info" type="function" returnType="string">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_init" type="function" returnType="object">
	</tag>
	<tag name="mysqli_insert_id" type="function" returnType="mixed">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_kill" type="function" returnType="bool">
		<attr name="link" type="object"></attr>
		<attr name="processid" type="int"></attr>
	</tag>
	<tag name="mysqli_master_query" type="function" returnType="bool">
		<attr name="link" type="object"></attr>
		<attr name="query" type="string"></attr>
	</tag>
	<tag name="mysqli_more_results" type="function" returnType="bool">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_multi_query" type="function" returnType="bool">
		<attr name="link" type="object"></attr>
		<attr name="query" type="string"></attr>
	</tag>
	<tag name="mysqli_next_result" type="function" returnType="bool">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_num_fields" type="function" returnType="int">
		<attr name="result" type="object"></attr>
	</tag>
	<tag name="mysqli_num_rows" type="function" returnType="mixed">
		<attr name="result" type="object"></attr>
	</tag>
	<tag name="mysqli_options" type="function" returnType="bool">
		<attr name="link" type="object"></attr>
		<attr name="option" type="int"></attr>
		<attr name="value" type="mixed"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="mysqli_ping" type="function" returnType="bool">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_prepare" type="function" returnType="mixed">
		<attr name="link" type="object"></attr>
		<attr name="query" type="string"></attr>
	</tag>
	<tag name="mysqli_query" type="function" returnType="mixed">
		<attr name="link" type="object"></attr>
		<attr name="query" type="string"></attr>
		<attr name="resultmode" type="int"></attr>
	</tag>
	<tag name="mysqli_real_connect" type="function" returnType="bool">
		<attr name="link" type="object"></attr>
		<attr name="hostname" type="string"></attr>
		<attr name="username" type="string"></attr>
		<attr name="passwd" type="string"></attr>
		<attr name="dbname" type="string"></attr>
		<attr name="port" type="int"></attr>
		<attr name="socket" type="string"></attr>
		<attr name="flags" type="int"></attr>
	</tag>
	<tag name="mysqli_real_escape_string" type="function" returnType="string">
		<attr name="link" type="object"></attr>
		<attr name="escapestr" type="string"></attr>
	</tag>
	<tag name="mysqli_real_query" type="function" returnType="bool">
		<attr name="link" type="object"></attr>
		<attr name="query" type="string"></attr>
	</tag>
	<tag name="mysqli_report" type="function" returnType="bool">
		<attr name="flags" type="int"></attr>
	</tag>
	<tag name="mysqli_rollback" type="function" returnType="bool">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_rpl_parse_enabled" type="function" returnType="int">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_rpl_probe" type="function" returnType="bool">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_rpl_query_type" type="function" returnType="int">
		<attr name="query" type="string"></attr>
	</tag>
	<tag name="mysqli_select_db" type="function" returnType="bool">
		<attr name="link" type="object"></attr>
		<attr name="dbname" type="string"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="mysqli_send_query" type="function" returnType="bool">
		<attr name="link" type="resource"></attr>
		<attr name="query" type="string"></attr>
	</tag>
	<tag name="mysqli_server_end" type="function" returnType="void">
	</tag>
	<tag name="mysqli_server_init" type="function" returnType="bool">
		<attr name="server" type="array" status="optional"></attr>
		<attr name="groups" type="array" status="optional"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="mysqli_sqlstate" type="function" returnType="string">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_ssl_set" type="function" returnType="bool">
		<attr name="link" type="object"></attr>
		<attr name="key" type="string"></attr>
		<attr name="cert" type="string"></attr>
		<attr name="ca" type="string"></attr>
		<attr name="capath" type="string"></attr>
		<attr name="cipher" type="string"></attr>
	</tag>
	<tag name="mysqli_stat" type="function" returnType="mixed">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_stmt_affected_rows" type="function" returnType="mixed">
		<attr name="stmt" type="object"></attr>
	</tag>
	<tag name="mysqli_stmt_bind_param" type="function" returnType="bool">
		<attr name="stmt" type="object"></attr>
		<attr name="types" type="string"></attr>
		<attr name="&amp;var1" type="mixed"></attr>
		<attr name="&amp;..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="mysqli_stmt_bind_result" type="function" returnType="bool">
		<attr name="stmt" type="object"></attr>
		<attr name="&amp;var1" type="mixed"></attr>
		<attr name="&amp;..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="mysqli_stmt_close" type="function" returnType="bool">
		<attr name="stmt" type="object"></attr>
	</tag>
	<tag name="mysqli_stmt_data_seek" type="function" returnType="bool">
		<attr name="statement" type="object"></attr>
		<attr name="offset" type="int"></attr>
	</tag>
	<tag name="mysqli_stmt_errno" type="function" returnType="int">
		<attr name="stmt" type="object"></attr>
	</tag>
	<tag name="mysqli_stmt_error" type="function" returnType="string">
		<attr name="stmt" type="object"></attr>
	</tag>
	<tag name="mysqli_stmt_execute" type="function" returnType="bool">
		<attr name="stmt" type="object"></attr>
	</tag>
	<tag name="mysqli_stmt_fetch" type="function" returnType="mixed">
		<attr name="stmt" type="object"></attr>
	</tag>
	<tag name="mysqli_stmt_free_result" type="function" returnType="void">
		<attr name="stmt" type="object"></attr>
	</tag>
	<tag name="mysqli_stmt_init" type="function" returnType="object">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_stmt_num_rows" type="function" returnType="mixed">
		<attr name="stmt" type="object"></attr>
	</tag>
	<tag name="mysqli_stmt_param_count" type="function" returnType="int">
		<attr name="stmt" type="object"></attr>
	</tag>
	<tag name="mysqli_stmt_prepare" type="function" returnType="bool">
		<attr name="stmt" type="object"></attr>
		<attr name="query" type="string"></attr>
	</tag>
	<tag name="mysqli_stmt_reset" type="function" returnType="bool">
		<attr name="stmt" type="object"></attr>
	</tag>
	<tag name="mysqli_stmt_result_metadata" type="function" returnType="mixed">
		<attr name="stmt" type="object"></attr>
	</tag>
	<tag name="mysqli_stmt_send_long_data" type="function" returnType="bool">
		<attr name="stmt" type="object"></attr>
		<attr name="param_nr" type="int"></attr>
		<attr name="data" type="string"></attr>
	</tag>
	<tag name="mysqli_stmt_sqlstate" type="function" returnType="string">
		<attr name="stmt" type="object"></attr>
	</tag>
	<tag name="mysqli_stmt_store_result" type="function" returnType="bool">
		<attr name="stmt" type="object"></attr>
	</tag>
	<tag name="mysqli_store_result" type="function" returnType="object">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_thread_id" type="function" returnType="int">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_thread_safe" type="function" returnType="bool">
	</tag>
	<tag name="mysqli_use_result" type="function" returnType="mixed">
		<attr name="link" type="object"></attr>
	</tag>
	<tag name="mysqli_warning_count" type="function" returnType="int">
		<attr name="link" type="object"></attr>
	</tag>
</tags>