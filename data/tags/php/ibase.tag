<!DOCTYPE tags>
<tags>	<tag name="ibase_blob_add" type="function" returnType="int">
		<attr name="blob_id" type="int" status="optional"></attr>
		<attr name="data" type="string" status="optional"></attr>
	</tag>
	<tag name="ibase_blob_cancel" type="function" returnType="int">
		<attr name="blob_id" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_blob_close" type="function" returnType="int">
		<attr name="blob_id" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_blob_create" type="function" returnType="int">
		<attr name="link_identifier" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_blob_echo" type="function" returnType="int">
		<attr name="blob_id_str" type="string" status="optional"></attr>
	</tag>
	<tag name="ibase_blob_get" type="function" returnType="string">
		<attr name="blob_id" type="int" status="optional"></attr>
		<attr name="len" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_blob_import" type="function" returnType="string">
		<attr name="link_identifier" type="int" status="optional"></attr>
		<attr name="file_id" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_blob_info" type="function" returnType="object">
		<attr name="blob_id_str" type="string" status="optional"></attr>
	</tag>
	<tag name="ibase_blob_open" type="function" returnType="int">
		<attr name="blob_id" type="string" status="optional"></attr>
	</tag>
	<tag name="ibase_close" type="function" returnType="int">
		<attr name="connection_id" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_commit" type="function" returnType="int">
		<attr name="link_identifier" type="int" status="optional"></attr>
		<attr name="trans_number" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_connect" type="function" returnType="int">
		<attr name="database" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
		<attr name="buffers" type="int" status="optional"></attr>
		<attr name="dialect" type="int" status="optional"></attr>
		<attr name="role" type="string" status="optional"></attr>
	</tag>
	<tag name="ibase_errmsg" type="function" returnType="string">
	</tag>
	<tag name="ibase_execute" type="function" returnType="int">
		<attr name="query" type="int" status="optional"></attr>
		<attr name="bind_args" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_fetch_object" type="function" returnType="object">
		<attr name="result_id" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_fetch_row" type="function" returnType="array">
		<attr name="result_identifier" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_field_info" type="function" returnType="array">
		<attr name="result" type="int" status="optional"></attr>
		<attr name="field number" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_free_query" type="function" returnType="int">
		<attr name="query" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_free_result" type="function" returnType="int">
		<attr name="result_identifier" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_num_fields" type="function" returnType="int">
		<attr name="result_id" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_pconnect" type="function" returnType="int">
		<attr name="database" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
		<attr name="buffers" type="int" status="optional"></attr>
		<attr name="dialect" type="int" status="optional"></attr>
		<attr name="role" type="string" status="optional"></attr>
	</tag>
	<tag name="ibase_prepare" type="function" returnType="int">
		<attr name="link_identifier" type="int" status="optional"></attr>
		<attr name="query" type="string" status="optional"></attr>
	</tag>
	<tag name="ibase_query" type="function" returnType="int">
		<attr name="link_identifier" type="int" status="optional"></attr>
		<attr name="query" type="string" status="optional"></attr>
		<attr name="bind_args" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_rollback" type="function" returnType="int">
		<attr name="link_identifier" type="int" status="optional"></attr>
		<attr name="trans_number" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_timefmt" type="function" returnType="int">
		<attr name="format" type="string" status="optional"></attr>
		<attr name="columntype" type="int" status="optional"></attr>
	</tag>
	<tag name="ibase_trans" type="function" returnType="int">
		<attr name="trans_args" type="int" status="optional"></attr>
		<attr name="link_identifier" type="int" status="optional"></attr>
	</tag>
</tags>
