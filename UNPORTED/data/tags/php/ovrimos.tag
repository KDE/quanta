<!DOCTYPE tags>
<tags>	<tag name="ovrimos_close" type="function" returnType="void">
		<attr name="connection" type="int" status="optional"></attr>
	</tag>
	<tag name="ovrimos_commit" type="function" returnType="int">
		<attr name="connection_id" type="int" status="optional"></attr>
	</tag>
	<tag name="ovrimos_connect" type="function" returnType="int">
		<attr name="host" type="string" status="optional"></attr>
		<attr name="db" type="string" status="optional"></attr>
		<attr name="user" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="ovrimos_cursor" type="function" returnType="int">
		<attr name="result_id" type="int" status="optional"></attr>
	</tag>
	<tag name="ovrimos_exec" type="function" returnType="int">
		<attr name="connection_id" type="int" status="optional"></attr>
		<attr name="query" type="string" status="optional"></attr>
	</tag>
	<tag name="ovrimos_execute" type="function" returnType="bool">
		<attr name="result_id" type="int" status="optional"></attr>
		<attr name="parameters_array" type="array" status="optional"></attr>
	</tag>
	<tag name="ovrimos_fetch_into" type="function" returnType="bool">
		<attr name="result_id" type="int" status="optional"></attr>
		<attr name="result_array" type="array" status="optional"></attr>
		<attr name="how" type="string" status="optional"></attr>
		<attr name="rownumber" type="int" status="optional"></attr>
	</tag>
	<tag name="ovrimos_fetch_row" type="function" returnType="bool">
		<attr name="result_id" type="int" status="optional"></attr>
		<attr name="how" type="int" status="optional"></attr>
		<attr name="row_number" type="int" status="optional"></attr>
	</tag>
	<tag name="ovrimos_field_len" type="function" returnType="int">
		<attr name="result_id" type="int" status="optional"></attr>
		<attr name="field_number" type="int" status="optional"></attr>
	</tag>
	<tag name="ovrimos_field_name" type="function" returnType="int">
		<attr name="result_id" type="int" status="optional"></attr>
		<attr name="field_number" type="int" status="optional"></attr>
	</tag>
	<tag name="ovrimos_field_num" type="function" returnType="int">
		<attr name="result_id" type="int" status="optional"></attr>
		<attr name="field_name" type="string" status="optional"></attr>
	</tag>
	<tag name="ovrimos_field_type" type="function" returnType="int">
		<attr name="result_id" type="int" status="optional"></attr>
		<attr name="field_number" type="int" status="optional"></attr>
	</tag>
	<tag name="ovrimos_free_result" type="function" returnType="bool">
		<attr name="result_id" type="int" status="optional"></attr>
	</tag>
	<tag name="ovrimos_longreadlen" type="function" returnType="int">
		<attr name="result_id" type="int" status="optional"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="ovrimos_num_fields" type="function" returnType="int">
		<attr name="result_id" type="int" status="optional"></attr>
	</tag>
	<tag name="ovrimos_num_rows" type="function" returnType="int">
		<attr name="result_id" type="int" status="optional"></attr>
	</tag>
	<tag name="ovrimos_prepare" type="function" returnType="int">
		<attr name="connection_id" type="int" status="optional"></attr>
		<attr name="query" type="string" status="optional"></attr>
	</tag>
	<tag name="ovrimos_result_all" type="function" returnType="bool">
		<attr name="result_id" type="int" status="optional"></attr>
		<attr name="format" type="string" status="optional"></attr>
	</tag>
	<tag name="ovrimos_result" type="function" returnType="int">
		<attr name="result_id" type="int" status="optional"></attr>
		<attr name="field" type="mixed" status="optional"></attr>
	</tag>
	<tag name="ovrimos_rollback" type="function" returnType="int">
		<attr name="connection_id" type="int" status="optional"></attr>
	</tag>
</tags>
