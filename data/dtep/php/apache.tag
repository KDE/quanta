<!DOCTYPE tags>
<tags>
	<tag name="apache_child_terminate" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
	</tag>
	<tag name="apache_get_modules" type="function" returnType="array" version="PHP 4 &gt;= 4.3.2, PHP 5">
	</tag>
	<tag name="apache_get_version" type="function" returnType="string" version="PHP 4 &gt;= 4.3.2, PHP 5">
	</tag>
	<tag name="apache_getenv" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="variable" type="string" status="required"></attr>
		<attr name="walk_to_top" type="bool" status="optional"></attr>
	</tag>
	<tag name="apache_lookup_uri" type="function" returnType="object" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="apache_note" type="function" returnType="string" version="PHP 3&gt;= 3.0.2, PHP 4 , PHP 5">
		<attr name="note_name" type="string" status="required"></attr>
		<attr name="note_value" type="string" status="optional"></attr>
	</tag>
	<tag name="apache_request_headers" type="function" returnType="array" version="PHP 4 &gt;= 4.3.0, PHP 5">
	</tag>
	<tag name="apache_response_headers" type="function" returnType="array" version="PHP 4 &gt;= 4.3.0, PHP 5">
	</tag>
	<tag name="apache_setenv" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="variable" type="string" status="required"></attr>
		<attr name="value" type="string" status="required"></attr>
		<attr name="walk_to_top" type="bool" status="optional"></attr>
	</tag>
	<tag name="ascii2ebcdic" type="function" returnType="int" version="PHP 3&gt;= 3.0.17">
		<attr name="ascii_str" type="string" status="required"></attr>
	</tag>
	<tag name="ebcdic2ascii" type="function" returnType="int" version="PHP 3&gt;= 3.0.17">
		<attr name="ebcdic_str" type="string" status="required"></attr>
	</tag>
	<tag name="getallheaders" type="function" returnType="array" version="PHP 3, PHP 4 , PHP 5">
	</tag>
	<tag name="virtual" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
</tags>