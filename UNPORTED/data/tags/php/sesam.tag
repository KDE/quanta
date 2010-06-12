<!DOCTYPE tags>
<tags>	<tag name="sesam_affected_rows" type="function" returnType="int">
		<attr name="result_id" type="string" status="optional"></attr>
	</tag>
	<tag name="sesam_commit" type="function" returnType="bool">
	</tag>
	<tag name="sesam_connect" type="function" returnType="bool">
		<attr name="catalog" type="string" status="optional"></attr>
		<attr name="schema" type="string" status="optional"></attr>
		<attr name="user" type="string" status="optional"></attr>
	</tag>
	<tag name="sesam_diagnostic" type="function" returnType="array">
	</tag>
	<tag name="sesam_disconnect" type="function" returnType="bool">
	</tag>
	<tag name="sesam_errormsg" type="function" returnType="string">
	</tag>
	<tag name="sesam_execimm" type="function" returnType="string">
		<attr name="query" type="string" status="optional"></attr>
	</tag>
	<tag name="sesam_fetch_array" type="function" returnType="array">
		<attr name="result_id" type="string" status="optional"></attr>
		<attr name="whence" type="int" status="optional"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="sesam_fetch_result" type="function" returnType="mixed">
		<attr name="result_id" type="string" status="optional"></attr>
		<attr name="max_rows" type="int" status="optional"></attr>
	</tag>
	<tag name="sesam_fetch_row" type="function" returnType="array">
		<attr name="result_id" type="string" status="optional"></attr>
		<attr name="whence" type="int" status="optional"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="sesam_field_array" type="function" returnType="array">
		<attr name="result_id" type="string" status="optional"></attr>
	</tag>
	<tag name="sesam_field_name" type="function" returnType="int">
		<attr name="result_id" type="string" status="optional"></attr>
		<attr name="index" type="int" status="optional"></attr>
	</tag>
	<tag name="sesam_free_result" type="function" returnType="int">
		<attr name="result_id" type="string" status="optional"></attr>
	</tag>
	<tag name="sesam_num_fields" type="function" returnType="int">
		<attr name="result_id" type="string" status="optional"></attr>
	</tag>
	<tag name="sesam_query" type="function" returnType="string">
		<attr name="query" type="string" status="optional"></attr>
		<attr name="scrollable" type="bool" status="optional"></attr>
	</tag>
	<tag name="sesam_rollback" type="function" returnType="bool">
	</tag>
	<tag name="sesam_seek_row" type="function" returnType="bool">
		<attr name="result_id" type="string" status="optional"></attr>
		<attr name="whence" type="int" status="optional"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="sesam_settransaction" type="function" returnType="bool">
		<attr name="isolation_level" type="int" status="optional"></attr>
		<attr name="read_only" type="int" status="optional"></attr>
	</tag>
</tags>
