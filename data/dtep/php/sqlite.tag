<!DOCTYPE tags>
<tags>
	<tag name="sqlite_array_query" type="function" returnType="array">
		<attr name="dbhandle" type="resource"></attr>
		<attr name="query" type="string"></attr>
		<attr name="result_type" type="int" status="optional"></attr>
		<attr name="decode_binary" type="bool" status="optional"></attr>
	</tag>
	<tag name="sqlite_busy_timeout" type="function" returnType="void">
		<attr name="dbhandle" type="resource"></attr>
		<attr name="milliseconds" type="int"></attr>
	</tag>
	<tag name="sqlite_changes" type="function" returnType="int">
		<attr name="dbhandle" type="resource"></attr>
	</tag>
	<tag name="sqlite_close" type="function" returnType="void">
		<attr name="dbhandle" type="resource"></attr>
	</tag>
	<tag name="sqlite_column" type="function" returnType="mixed">
		<attr name="result" type="resource"></attr>
		<attr name="index_or_name" type="mixed"></attr>
		<attr name="decode_binary" type="bool" status="optional"></attr>
	</tag>
	<tag name="sqlite_create_aggregate" type="function" returnType="bool">
		<attr name="dbhandle" type="resource"></attr>
		<attr name="function_name" type="string"></attr>
		<attr name="step_func" type="callback"></attr>
		<attr name="finalize_func" type="callback"></attr>
		<attr name="num_args" type="int" status="optional"></attr>
	</tag>
	<tag name="sqlite_create_function" type="function" returnType="bool">
		<attr name="dbhandle" type="resource"></attr>
		<attr name="function_name" type="string"></attr>
		<attr name="callback" type="callback"></attr>
		<attr name="num_args" type="int" status="optional"></attr>
	</tag>
	<tag name="sqlite_current" type="function" returnType="array">
		<attr name="result" type="resource"></attr>
		<attr name="result_type" type="int" status="optional"></attr>
		<attr name="decode_binary" type="bool" status="optional"></attr>
	</tag>
	<tag name="sqlite_error_string" type="function" returnType="string">
		<attr name="error_code" type="int"></attr>
	</tag>
	<tag name="sqlite_escape_string" type="function" returnType="string">
		<attr name="item" type="string"></attr>
	</tag>
	<tag name="sqlite_exec" type="function" returnType="bool">
		<attr name="dbhandle" type="resource"></attr>
		<attr name="query" type="string"></attr>
	</tag>
	<tag name="sqlite_factory" type="function" returnType="object">
		<attr name="filename" type="string"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="&amp;error_message" type="string" status="optional"></attr>
	</tag>
	<tag name="sqlite_fetch_all" type="function" returnType="array">
		<attr name="result" type="resource"></attr>
		<attr name="result_type" type="int" status="optional"></attr>
		<attr name="decode_binary" type="bool" status="optional"></attr>
	</tag>
	<tag name="sqlite_fetch_array" type="function" returnType="array">
		<attr name="result" type="resource"></attr>
		<attr name="result_type" type="int" status="optional"></attr>
		<attr name="decode_binary" type="bool" status="optional"></attr>
	</tag>
	<tag name="sqlite_fetch_column_types" type="function" returnType="resource">
		<attr name="table_name" type="string"></attr>
		<attr name="db" type="resource"></attr>
	</tag>
	<tag name="sqlite_fetch_object" type="function" returnType="object">
		<attr name="result" type="resource"></attr>
		<attr name="class_name" type="string" status="optional"></attr>
		<attr name="ctor_params" type="array" status="optional"></attr>
		<attr name="decode_binary" type="bool" status="optional"></attr>
	</tag>
	<tag name="sqlite_fetch_single" type="function" returnType="string">
		<attr name="result" type="resource"></attr>
		<attr name="decode_binary" type="bool" status="optional"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="sqlite_field_name" type="function" returnType="string">
		<attr name="result" type="resource"></attr>
		<attr name="field_index" type="int"></attr>
	</tag>
	<tag name="sqlite_has_more" type="function" returnType="bool">
		<attr name="result" type="resource"></attr>
	</tag>
	<tag name="sqlite_has_prev" type="function" returnType="bool">
		<attr name="result" type="resource"></attr>
	</tag>
	<tag name="sqlite_last_error" type="function" returnType="int">
		<attr name="dbhandle" type="resource"></attr>
	</tag>
	<tag name="sqlite_last_insert_rowid" type="function" returnType="int">
		<attr name="dbhandle" type="resource"></attr>
	</tag>
	<tag name="sqlite_libencoding" type="function" returnType="string">
	</tag>
	<tag name="sqlite_libversion" type="function" returnType="string">
	</tag>
	<tag name="sqlite_next" type="function" returnType="bool">
		<attr name="result" type="resource"></attr>
	</tag>
	<tag name="sqlite_num_fields" type="function" returnType="int">
		<attr name="result" type="resource"></attr>
	</tag>
	<tag name="sqlite_num_rows" type="function" returnType="int">
		<attr name="result" type="resource"></attr>
	</tag>
	<tag name="sqlite_open" type="function" returnType="resource">
		<attr name="filename" type="string"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="&amp;error_message" type="string" status="optional"></attr>
	</tag>
	<tag name="sqlite_popen" type="function" returnType="resource">
		<attr name="filename" type="string"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="&amp;error_message" type="string" status="optional"></attr>
	</tag>
	<tag name="sqlite_prev" type="function" returnType="bool">
		<attr name="result" type="resource"></attr>
	</tag>
	<tag name="sqlite_query" type="function" returnType="resource">
		<attr name="dbhandle" type="resource"></attr>
		<attr name="query" type="string"></attr>
	</tag>
	<tag name="sqlite_rewind" type="function" returnType="bool">
		<attr name="result" type="resource"></attr>
	</tag>
	<tag name="sqlite_seek" type="function" returnType="bool">
		<attr name="result" type="resource"></attr>
		<attr name="rownum" type="int"></attr>
	</tag>
	<tag name="sqlite_single_query" type="function" returnType="mixed">
		<attr name="db" type="resource"></attr>
		<attr name="query" type="string"></attr>
		<attr name="first_row_only" type="bool" status="optional"></attr>
		<attr name="decode_binary" type="bool" status="optional"></attr>
	</tag>
	<tag name="sqlite_udf_decode_binary" type="function" returnType="string">
		<attr name="data" type="string"></attr>
	</tag>
	<tag name="sqlite_udf_encode_binary" type="function" returnType="string">
		<attr name="data" type="string"></attr>
	</tag>
	<tag name="sqlite_unbuffered_query" type="function" returnType="resource">
		<attr name="dbhandle" type="resource"></attr>
		<attr name="query" type="string"></attr>
	</tag>
</tags>