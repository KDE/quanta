<!DOCTYPE tags>
<tags>
	<tag name="ibase_add_user" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="server" type="string" status="required"></attr>
		<attr name="dba_user_name" type="string" status="required"></attr>
		<attr name="dba_user_password" type="string" status="required"></attr>
		<attr name="user_name" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="first_name" type="string" status="optional"></attr>
		<attr name="middle_name" type="string" status="optional"></attr>
		<attr name="last_name" type="string" status="optional"></attr>
	</tag>
	<tag name="ibase_affected_rows" type="function" returnType="int" version="PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ibase_backup" type="function" returnType="mixed" version="PHP 5">
		<attr name="service_handle" type="resource" status="required"></attr>
		<attr name="source_db" type="string" status="required"></attr>
		<attr name="dest_file" type="string" status="required"></attr>
		<attr name="options" type="int" status="optional"></attr>
		<attr name="verbose" type="bool" status="optional"></attr>
	</tag>
	<tag name="ibase_blob_add" type="function" returnType="bool" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="blob_handle" type="resource" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
	</tag>
	<tag name="ibase_blob_cancel" type="function" returnType="bool" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="blob_handle" type="resource" status="required"></attr>
	</tag>
	<tag name="ibase_blob_close" type="function" returnType="mixed" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="blob_handle" type="resource" status="required"></attr>
	</tag>
	<tag name="ibase_blob_create" type="function" returnType="resource" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ibase_blob_echo" type="function" returnType="bool" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="link_identifier" type="resource" status="required"></attr>
		<attr name="blob_id" type="string" status="required"></attr>
	</tag>
	<tag name="ibase_blob_get" type="function" returnType="string" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="blob_handle" type="resource" status="required"></attr>
		<attr name="len" type="int" status="required"></attr>
	</tag>
	<tag name="ibase_blob_import" type="function" returnType="string" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="link_identifier" type="resource" status="required"></attr>
		<attr name="file_handle" type="resource" status="required"></attr>
	</tag>
	<tag name="ibase_blob_info" type="function" returnType="array" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="link_identifier" type="resource" status="required"></attr>
		<attr name="blob_id" type="string" status="required"></attr>
	</tag>
	<tag name="ibase_blob_open" type="function" returnType="resource" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="link_identifier" type="resource" status="required"></attr>
		<attr name="blob_id" type="string" status="required"></attr>
	</tag>
	<tag name="ibase_close" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="connection_id" type="resource" status="optional"></attr>
	</tag>
	<tag name="ibase_commit_ret" type="function" returnType="bool" version="PHP 5">
		<attr name="link_or_trans_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ibase_commit" type="function" returnType="bool" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="link_or_trans_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ibase_connect" type="function" returnType="resource" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="database" type="string" status="required"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
		<attr name="buffers" type="int" status="optional"></attr>
		<attr name="dialect" type="int" status="optional"></attr>
		<attr name="role" type="string" status="optional"></attr>
	</tag>
	<tag name="ibase_db_info" type="function" returnType="string" version="PHP 5">
		<attr name="service_handle" type="resource" status="required"></attr>
		<attr name="db" type="string" status="required"></attr>
		<attr name="action" type="int" status="required"></attr>
		<attr name="argument" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_delete_user" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="server" type="string" status="required"></attr>
		<attr name="dba_user_name" type="string" status="required"></attr>
		<attr name="dba_user_password" type="string" status="required"></attr>
		<attr name="user_name" type="string" status="required"></attr>
	</tag>
	<tag name="ibase_drop_db" type="function" returnType="bool" version="PHP 5">
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="ibase_errcode" type="function" returnType="int" version="PHP 5">
	</tag>
	<tag name="ibase_errmsg" type="function" returnType="string" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
	</tag>
	<tag name="ibase_execute" type="function" returnType="resource" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="query" type="resource" status="required"></attr>
		<attr name="bind_arg" type="mixed" status="optional"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="ibase_fetch_assoc" type="function" returnType="array" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="fetch_flag" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_fetch_object" type="function" returnType="object" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="result_id" type="resource" status="required"></attr>
		<attr name="fetch_flag" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_fetch_row" type="function" returnType="array" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="result_identifier" type="resource" status="required"></attr>
		<attr name="fetch_flag" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_field_info" type="function" returnType="array" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="field_number" type="int" status="required"></attr>
	</tag>
	<tag name="ibase_free_event_handler" type="function" returnType="bool" version="PHP 5">
		<attr name="event" type="resource" status="required"></attr>
	</tag>
	<tag name="ibase_free_query" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="query" type="resource" status="required"></attr>
	</tag>
	<tag name="ibase_free_result" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="result_identifier" type="resource" status="required"></attr>
	</tag>
	<tag name="ibase_gen_id" type="function" returnType="int" version="PHP 5">
		<attr name="generator" type="string" status="required"></attr>
		<attr name="increment" type="int" status="optional"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ibase_maintain_db" type="function" returnType="bool" version="PHP 5">
		<attr name="service_handle" type="resource" status="required"></attr>
		<attr name="db" type="string" status="required"></attr>
		<attr name="action" type="int" status="required"></attr>
		<attr name="argument" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_modify_user" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="server" type="string" status="required"></attr>
		<attr name="dba_user_name" type="string" status="required"></attr>
		<attr name="dba_user_password" type="string" status="required"></attr>
		<attr name="user_name" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="first_name" type="string" status="optional"></attr>
		<attr name="middle_name" type="string" status="optional"></attr>
		<attr name="last_name" type="string" status="optional"></attr>
	</tag>
	<tag name="ibase_name_result" type="function" returnType="bool" version="PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="ibase_num_fields" type="function" returnType="int" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="result_id" type="resource" status="required"></attr>
	</tag>
	<tag name="ibase_num_params" type="function" returnType="int" version="PHP 5">
		<attr name="query" type="resource" status="required"></attr>
	</tag>
	<tag name="ibase_param_info" type="function" returnType="array" version="PHP 5">
		<attr name="query" type="resource" status="required"></attr>
		<attr name="param_number" type="int" status="required"></attr>
	</tag>
	<tag name="ibase_pconnect" type="function" returnType="resource" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="database" type="string" status="required"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
		<attr name="buffers" type="int" status="optional"></attr>
		<attr name="dialect" type="int" status="optional"></attr>
		<attr name="role" type="string" status="optional"></attr>
	</tag>
	<tag name="ibase_prepare" type="function" returnType="resource" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="query" type="string" status="required"></attr>
	</tag>
	<tag name="ibase_query" type="function" returnType="resource" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="query" type="string" status="required"></attr>
		<attr name="bind_args" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_restore" type="function" returnType="mixed" version="PHP 5">
		<attr name="service_handle" type="resource" status="required"></attr>
		<attr name="source_file" type="string" status="required"></attr>
		<attr name="dest_db" type="string" status="required"></attr>
		<attr name="options" type="int" status="optional"></attr>
		<attr name="verbose" type="bool" status="optional"></attr>
	</tag>
	<tag name="ibase_rollback_ret" type="function" returnType="bool" version="PHP 5">
		<attr name="link_or_trans_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ibase_rollback" type="function" returnType="bool" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="link_or_trans_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ibase_server_info" type="function" returnType="string" version="PHP 5">
		<attr name="service_handle" type="resource" status="required"></attr>
		<attr name="action" type="int" status="required"></attr>
	</tag>
	<tag name="ibase_service_attach" type="function" returnType="resource" version="PHP 5">
		<attr name="host" type="string" status="required"></attr>
		<attr name="dba_username" type="string" status="required"></attr>
		<attr name="dba_password" type="string" status="required"></attr>
	</tag>
	<tag name="ibase_service_detach" type="function" returnType="bool" version="PHP 5">
		<attr name="service_handle" type="resource" status="required"></attr>
	</tag>
	<tag name="ibase_set_event_handler" type="function" returnType="resource" version="PHP 5">
		<attr name="event_handler" type="callback" status="required"></attr>
		<attr name="event_name1" type="string" status="required"></attr>
		<attr name="event_name2" type="string" status="optional"></attr>
		<attr name="..." type="string" status="optional"></attr>
	</tag>
	<tag name="ibase_timefmt" type="function" returnType="int" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="format" type="string" status="required"></attr>
		<attr name="columntype" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_trans" type="function" returnType="resource" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="trans_args" type="int" status="optional"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ibase_wait_event" type="function" returnType="string" version="PHP 5">
		<attr name="event_name1" type="string" status="required"></attr>
		<attr name="event_name2" type="string" status="optional"></attr>
		<attr name="..." type="string" status="optional"></attr>
	</tag>
</tags>