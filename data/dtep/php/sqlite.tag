<!DOCTYPE tags>
<tags>
	<tag name="sqlite_array_query" type="function" returnType="array" version="PHP 5">
		<attr name="dbhandle" type="resource" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
		<attr name="result_type" type="int" status="optional"></attr>
		<attr name="decode_binary" type="bool" status="optional"></attr>
	</tag>
	<tag name="sqlite_busy_timeout" type="function" returnType="void" version="PHP 5">
		<attr name="dbhandle" type="resource" status="required"></attr>
		<attr name="milliseconds" type="int" status="required"></attr>
	</tag>
	<tag name="sqlite_changes" type="function" returnType="int" version="PHP 5">
		<attr name="dbhandle" type="resource" status="required"></attr>
	</tag>
	<tag name="sqlite_close" type="function" returnType="void" version="PHP 5">
		<attr name="dbhandle" type="resource" status="required"></attr>
	</tag>
	<tag name="sqlite_column" type="function" returnType="mixed" version="PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="index_or_name" type="mixed" status="required"></attr>
		<attr name="decode_binary" type="bool" status="optional"></attr>
	</tag>
	<tag name="sqlite_create_aggregate" type="function" returnType="bool" version="PHP 5">
		<attr name="dbhandle" type="resource" status="required"></attr>
		<attr name="function_name" type="string" status="required"></attr>
		<attr name="step_func" type="callback" status="required"></attr>
		<attr name="finalize_func" type="callback" status="required"></attr>
		<attr name="num_args" type="int" status="optional"></attr>
	</tag>
	<tag name="sqlite_create_function" type="function" returnType="bool" version="PHP 5">
		<attr name="dbhandle" type="resource" status="required"></attr>
		<attr name="function_name" type="string" status="required"></attr>
		<attr name="callback" type="callback" status="required"></attr>
		<attr name="num_args" type="int" status="optional"></attr>
	</tag>
	<tag name="sqlite_current" type="function" returnType="array" version="PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="result_type" type="int" status="optional"></attr>
		<attr name="decode_binary" type="bool" status="optional"></attr>
	</tag>
	<tag name="sqlite_error_string" type="function" returnType="string" version="PHP 5">
		<attr name="error_code" type="int" status="required"></attr>
	</tag>
	<tag name="sqlite_escape_string" type="function" returnType="string" version="PHP 5">
		<attr name="item" type="string" status="required"></attr>
	</tag>
	<tag name="sqlite_exec" type="function" returnType="bool" version="">
		<attr name="dbhandle" type="resource" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
	</tag>
	<tag name="sqlite_factory" type="function" returnType="object" version="PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="&amp;error_message" type="string" status="optional"></attr>
	</tag>
	<tag name="sqlite_fetch_all" type="function" returnType="array" version="PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="result_type" type="int" status="optional"></attr>
		<attr name="decode_binary" type="bool" status="optional"></attr>
	</tag>
	<tag name="sqlite_fetch_array" type="function" returnType="array" version="PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="result_type" type="int" status="optional"></attr>
		<attr name="decode_binary" type="bool" status="optional"></attr>
	</tag>
	<tag name="sqlite_fetch_column_types" type="function" returnType="resource" version="PHP 5">
		<attr name="table_name" type="string" status="required"></attr>
		<attr name="db" type="resource" status="required"></attr>
	</tag>
	<tag name="sqlite_fetch_object" type="function" returnType="object" version="PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="class_name" type="string" status="optional"></attr>
		<attr name="ctor_params" type="array" status="optional"></attr>
		<attr name="decode_binary" type="bool" status="optional"></attr>
	</tag>
	<tag name="sqlite_fetch_single" type="function" returnType="string" version="PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="decode_binary" type="bool" status="optional"></attr>
	</tag>
	<tag name="sqlite_field_name" type="function" returnType="string" version="PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="field_index" type="int" status="required"></attr>
	</tag>
	<tag name="sqlite_has_more" type="function" returnType="bool" version="PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="sqlite_has_prev" type="function" returnType="bool" version="PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="sqlite_last_error" type="function" returnType="int" version="PHP 5">
		<attr name="dbhandle" type="resource" status="required"></attr>
	</tag>
	<tag name="sqlite_last_insert_rowid" type="function" returnType="int" version="PHP 5">
		<attr name="dbhandle" type="resource" status="required"></attr>
	</tag>
	<tag name="sqlite_libencoding" type="function" returnType="string" version="PHP 5">
	</tag>
	<tag name="sqlite_libversion" type="function" returnType="string" version="PHP 5">
	</tag>
	<tag name="sqlite_next" type="function" returnType="bool" version="PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="sqlite_num_fields" type="function" returnType="int" version="PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="sqlite_num_rows" type="function" returnType="int" version="PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="sqlite_open" type="function" returnType="resource" version="PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="&amp;error_message" type="string" status="optional"></attr>
	</tag>
	<tag name="sqlite_popen" type="function" returnType="resource" version="PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="&amp;error_message" type="string" status="optional"></attr>
	</tag>
	<tag name="sqlite_prev" type="function" returnType="bool" version="PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="sqlite_query" type="function" returnType="resource" version="PHP 5">
		<attr name="dbhandle" type="resource" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
	</tag>
	<tag name="sqlite_rewind" type="function" returnType="bool" version="PHP 5">
		<attr name="result" type="resource" status="required"></attr>
	</tag>
	<tag name="sqlite_seek" type="function" returnType="bool" version="PHP 5">
		<attr name="result" type="resource" status="required"></attr>
		<attr name="rownum" type="int" status="required"></attr>
	</tag>
	<tag name="sqlite_single_query" type="function" returnType="mixed" version="PHP 5">
		<attr name="db" type="resource" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
		<attr name="first_row_only" type="bool" status="optional"></attr>
		<attr name="decode_binary" type="bool" status="optional"></attr>
	</tag>
	<tag name="sqlite_udf_decode_binary" type="function" returnType="string" version="PHP 5">
		<attr name="data" type="string" status="required"></attr>
	</tag>
	<tag name="sqlite_udf_encode_binary" type="function" returnType="string" version="PHP 5">
		<attr name="data" type="string" status="required"></attr>
	</tag>
	<tag name="sqlite_unbuffered_query" type="function" returnType="resource" version="PHP 5">
		<attr name="dbhandle" type="resource" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
	</tag>
</tags>