<!DOCTYPE tags>
<tags>
	<tag name="dbx_close" type="function" returnType="bool">
		<attr name="link_identifier" type="object"></attr>
	</tag>
	<tag name="dbx_compare" type="function" returnType="int">
		<attr name="row_a" type="array"></attr>
		<attr name="row_b" type="array"></attr>
		<attr name="column_key" type="string"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="dbx_connect" type="function" returnType="object">
		<attr name="module" type="mixed"></attr>
		<attr name="host" type="string"></attr>
		<attr name="database" type="string"></attr>
		<attr name="username" type="string"></attr>
		<attr name="password" type="string"></attr>
		<attr name="persistent" type="int" status="optional"></attr>
	</tag>
	<tag name="dbx_error" type="function" returnType="string">
		<attr name="link_identifier" type="object"></attr>
	</tag>
	<tag name="dbx_escape_string" type="function" returnType="string">
		<attr name="link_identifier" type="object"></attr>
		<attr name="text" type="string"></attr>
	</tag>
	<tag name="dbx_fetch_row" type="function" returnType="object">
		<attr name="result_identifier" type="object"></attr>
	</tag>
	<tag name="dbx_query" type="function" returnType="object">
		<attr name="link_identifier" type="object"></attr>
		<attr name="sql_statement" type="string"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="dbx_sort" type="function" returnType="bool">
		<attr name="result" type="object"></attr>
		<attr name="user_compare_function" type="string"></attr>
	</tag>
</tags>