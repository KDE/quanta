<!DOCTYPE tags>
<tags>
	<tag name="dbx_close" type="function" returnType="bool" langVersion="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="link_identifier" type="object" status="required"></attr>
	</tag>
	<tag name="dbx_compare" type="function" returnType="int" langVersion="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="row_a" type="array" status="required"></attr>
		<attr name="row_b" type="array" status="required"></attr>
		<attr name="column_key" type="string" status="required"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="dbx_connect" type="function" returnType="object" langVersion="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="module" type="mixed" status="required"></attr>
		<attr name="host" type="string" status="required"></attr>
		<attr name="database" type="string" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="persistent" type="int" status="optional"></attr>
	</tag>
	<tag name="dbx_error" type="function" returnType="string" langVersion="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="link_identifier" type="object" status="required"></attr>
	</tag>
	<tag name="dbx_escape_string" type="function" returnType="string" langVersion="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="link_identifier" type="object" status="required"></attr>
		<attr name="text" type="string" status="required"></attr>
	</tag>
	<tag name="dbx_fetch_row" type="function" returnType="object" langVersion="PHP 5">
		<attr name="result_identifier" type="object" status="required"></attr>
	</tag>
	<tag name="dbx_query" type="function" returnType="object" langVersion="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="link_identifier" type="object" status="required"></attr>
		<attr name="sql_statement" type="string" status="required"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="dbx_sort" type="function" returnType="bool" langVersion="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="result" type="object" status="required"></attr>
		<attr name="user_compare_function" type="string" status="required"></attr>
	</tag>
</tags>