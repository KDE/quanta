<!DOCTYPE tags>
<tags>
	<tag name="ibase_add_user" type="function" returnType="bool">
		<attr name="server" type="string"></attr>
		<attr name="dba_user_name" type="string"></attr>
		<attr name="dba_user_password" type="string"></attr>
		<attr name="user_name" type="string"></attr>
		<attr name="password" type="string"></attr>
		<attr name="first_name" type="string" status="optional"></attr>
		<attr name="middle_name" type="string" status="optional"></attr>
		<attr name="last_name" type="string" status="optional"></attr>
	</tag>
	<tag name="ibase_affected_rows" type="function" returnType="int">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ibase_backup" type="function" returnType="mixed">
		<attr name="service_handle" type="resource"></attr>
		<attr name="source_db" type="string"></attr>
		<attr name="dest_file" type="string"></attr>
		<attr name="options" type="int" status="optional"></attr>
		<attr name="verbose" type="bool" status="optional"></attr>
	</tag>
	<tag name="ibase_blob_add" type="function" returnType="bool">
		<attr name="blob_handle" type="resource"></attr>
		<attr name="data" type="string"></attr>
	</tag>
	<tag name="ibase_blob_cancel" type="function" returnType="bool">
		<attr name="blob_handle" type="resource"></attr>
	</tag>
	<tag name="ibase_blob_close" type="function" returnType="mixed">
		<attr name="blob_handle" type="resource"></attr>
	</tag>
	<tag name="ibase_blob_create" type="function" returnType="resource">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ibase_blob_echo" type="function" returnType="bool">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="blob_id" type="string"></attr>
	</tag>
	<tag name="ibase_blob_get" type="function" returnType="string">
		<attr name="blob_handle" type="resource"></attr>
		<attr name="len" type="int"></attr>
	</tag>
	<tag name="ibase_blob_import" type="function" returnType="string">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="file_handle" type="resource"></attr>
	</tag>
	<tag name="ibase_blob_info" type="function" returnType="array">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="blob_id" type="string"></attr>
	</tag>
	<tag name="ibase_blob_open" type="function" returnType="resource">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="blob_id" type="string"></attr>
	</tag>
	<tag name="ibase_close" type="function" returnType="bool">
		<attr name="connection_id" type="resource" status="optional"></attr>
	</tag>
	<tag name="ibase_commit_ret" type="function" returnType="bool">
		<attr name="link_or_trans_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ibase_commit" type="function" returnType="bool">
		<attr name="link_or_trans_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ibase_connect" type="function" returnType="resource">
		<attr name="database" type="string"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
		<attr name="buffers" type="int" status="optional"></attr>
		<attr name="dialect" type="int" status="optional"></attr>
		<attr name="role" type="string" status="optional"></attr>
	</tag>
	<tag name="ibase_db_info" type="function" returnType="string">
		<attr name="service_handle" type="resource"></attr>
		<attr name="db" type="string"></attr>
		<attr name="action" type="int"></attr>
		<attr name="argument" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_delete_user" type="function" returnType="bool">
		<attr name="server" type="string"></attr>
		<attr name="dba_user_name" type="string"></attr>
		<attr name="dba_user_password" type="string"></attr>
		<attr name="user_name" type="string"></attr>
	</tag>
	<tag name="ibase_drop_db" type="function" returnType="bool">
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="ibase_errcode" type="function" returnType="int">
	</tag>
	<tag name="ibase_errmsg" type="function" returnType="string">
	</tag>
	<tag name="ibase_execute" type="function" returnType="resource">
		<attr name="query" type="resource"></attr>
		<attr name="bind_arg" type="mixed" status="optional"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="ibase_fetch_assoc" type="function" returnType="array">
		<attr name="result" type="resource"></attr>
		<attr name="fetch_flag" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_fetch_object" type="function" returnType="object">
		<attr name="result_id" type="resource"></attr>
		<attr name="fetch_flag" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_fetch_row" type="function" returnType="array">
		<attr name="result_identifier" type="resource"></attr>
		<attr name="fetch_flag" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_field_info" type="function" returnType="array">
		<attr name="result" type="resource"></attr>
		<attr name="field_number" type="int"></attr>
	</tag>
	<tag name="ibase_free_event_handler" type="function" returnType="bool">
		<attr name="event" type="resource"></attr>
	</tag>
	<tag name="ibase_free_query" type="function" returnType="bool">
		<attr name="query" type="resource"></attr>
	</tag>
	<tag name="ibase_free_result" type="function" returnType="bool">
		<attr name="result_identifier" type="resource"></attr>
	</tag>
	<tag name="ibase_gen_id" type="function" returnType="int">
		<attr name="generator" type="string"></attr>
		<attr name="increment" type="int" status="optional"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ibase_maintain_db" type="function" returnType="bool">
		<attr name="service_handle" type="resource"></attr>
		<attr name="db" type="string"></attr>
		<attr name="action" type="int"></attr>
		<attr name="argument" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_modify_user" type="function" returnType="bool">
		<attr name="server" type="string"></attr>
		<attr name="dba_user_name" type="string"></attr>
		<attr name="dba_user_password" type="string"></attr>
		<attr name="user_name" type="string"></attr>
		<attr name="password" type="string"></attr>
		<attr name="first_name" type="string" status="optional"></attr>
		<attr name="middle_name" type="string" status="optional"></attr>
		<attr name="last_name" type="string" status="optional"></attr>
	</tag>
	<tag name="ibase_name_result" type="function" returnType="bool">
		<attr name="result" type="resource"></attr>
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="ibase_num_fields" type="function" returnType="int">
		<attr name="result_id" type="resource"></attr>
	</tag>
	<tag name="ibase_num_params" type="function" returnType="int">
		<attr name="query" type="resource"></attr>
	</tag>
	<tag name="ibase_param_info" type="function" returnType="array">
		<attr name="query" type="resource"></attr>
		<attr name="param_number" type="int"></attr>
	</tag>
	<tag name="ibase_pconnect" type="function" returnType="resource">
		<attr name="database" type="string"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
		<attr name="buffers" type="int" status="optional"></attr>
		<attr name="dialect" type="int" status="optional"></attr>
		<attr name="role" type="string" status="optional"></attr>
	</tag>
	<tag name="ibase_prepare" type="function" returnType="resource">
		<attr name="query" type="string"></attr>
	</tag>
	<tag name="ibase_query" type="function" returnType="resource">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="query" type="string" status="optional"></attr>
		<attr name="bind_args" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_restore" type="function" returnType="mixed">
		<attr name="service_handle" type="resource"></attr>
		<attr name="source_file" type="string"></attr>
		<attr name="dest_db" type="string"></attr>
		<attr name="options" type="int" status="optional"></attr>
		<attr name="verbose" type="bool" status="optional"></attr>
	</tag>
	<tag name="ibase_rollback_ret" type="function" returnType="bool">
		<attr name="link_or_trans_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ibase_rollback" type="function" returnType="bool">
		<attr name="link_or_trans_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ibase_server_info" type="function" returnType="string">
		<attr name="service_handle" type="resource"></attr>
		<attr name="action" type="int"></attr>
	</tag>
	<tag name="ibase_service_attach" type="function" returnType="resource">
		<attr name="host" type="string"></attr>
		<attr name="dba_username" type="string"></attr>
		<attr name="dba_password" type="string"></attr>
	</tag>
	<tag name="ibase_service_detach" type="function" returnType="bool">
		<attr name="service_handle" type="resource"></attr>
	</tag>
	<tag name="ibase_set_event_handler" type="function" returnType="resource">
		<attr name="event_handler" type="callback"></attr>
		<attr name="event_name1" type="string"></attr>
		<attr name="event_name2" type="string" status="optional"></attr>
		<attr name="..." type="string" status="optional"></attr>
	</tag>
	<tag name="ibase_timefmt" type="function" returnType="int">
		<attr name="format" type="string"></attr>
		<attr name="columntype" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_trans" type="function" returnType="resource">
		<attr name="trans_args" type="int" status="optional"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ibase_wait_event" type="function" returnType="string">
		<attr name="event_name1" type="string"></attr>
		<attr name="event_name2" type="string" status="optional"></attr>
		<attr name="..." type="string" status="optional"></attr>
	</tag>
</tags>