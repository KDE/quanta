<!DOCTYPE tags>
<tags>
	<tag name="sesam_affected_rows" type="function" returnType="int" version="PHP 3 CVS only">
		<attr name="result_id" type="string" status="required"></attr>
	</tag>
	<tag name="sesam_commit" type="function" returnType="bool" version="PHP 3 CVS only">
	</tag>
	<tag name="sesam_connect" type="function" returnType="bool" version="PHP 3 CVS only">
		<attr name="catalog" type="string" status="required"></attr>
		<attr name="schema" type="string" status="required"></attr>
		<attr name="user" type="string" status="required"></attr>
	</tag>
	<tag name="sesam_diagnostic" type="function" returnType="array" version="PHP 3 CVS only">
	</tag>
	<tag name="sesam_disconnect" type="function" returnType="bool" version="PHP 3 CVS only">
	</tag>
	<tag name="sesam_errormsg" type="function" returnType="string" version="PHP 3 CVS only">
	</tag>
	<tag name="sesam_execimm" type="function" returnType="string" version="PHP 3 CVS only">
		<attr name="query" type="string" status="required"></attr>
	</tag>
	<tag name="sesam_fetch_array" type="function" returnType="array" version="PHP 3 CVS only">
		<attr name="result_id" type="string" status="required"></attr>
		<attr name="whence" type="int" status="optional"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="sesam_fetch_result" type="function" returnType="mixed" version="PHP 3 CVS only">
		<attr name="result_id" type="string" status="required"></attr>
		<attr name="max_rows" type="int" status="optional"></attr>
	</tag>
	<tag name="sesam_fetch_row" type="function" returnType="array" version="PHP 3 CVS only">
		<attr name="result_id" type="string" status="required"></attr>
		<attr name="whence" type="int" status="optional"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="sesam_field_array" type="function" returnType="array" version="PHP 3 CVS only">
		<attr name="result_id" type="string" status="required"></attr>
	</tag>
	<tag name="sesam_field_name" type="function" returnType="int" version="PHP 3 CVS only">
		<attr name="result_id" type="string" status="required"></attr>
		<attr name="index" type="int" status="required"></attr>
	</tag>
	<tag name="sesam_free_result" type="function" returnType="int" version="PHP 3 CVS only">
		<attr name="result_id" type="string" status="required"></attr>
	</tag>
	<tag name="sesam_num_fields" type="function" returnType="int" version="PHP 3 CVS only">
		<attr name="result_id" type="string" status="required"></attr>
	</tag>
	<tag name="sesam_query" type="function" returnType="string" version="PHP 3 CVS only">
		<attr name="query" type="string" status="required"></attr>
		<attr name="scrollable" type="bool" status="optional"></attr>
	</tag>
	<tag name="sesam_rollback" type="function" returnType="bool" version="PHP 3 CVS only">
	</tag>
	<tag name="sesam_seek_row" type="function" returnType="bool" version="PHP 3 CVS only">
		<attr name="result_id" type="string" status="required"></attr>
		<attr name="whence" type="int" status="required"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="sesam_settransaction" type="function" returnType="bool" version="PHP 3 CVS only">
		<attr name="isolation_level" type="int" status="required"></attr>
		<attr name="read_only" type="int" status="required"></attr>
	</tag>
</tags>