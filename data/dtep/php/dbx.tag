<!DOCTYPE tags>
<tags>	<tag name="dbx_close" type="function" returnType="bool">
		<attr name="link_identifier" type="object" status="optional"></attr>
	</tag>
	<tag name="dbx_compare" type="function" returnType="int">
		<attr name="row_a" type="array" status="optional"></attr>
		<attr name="row_b" type="array" status="optional"></attr>
		<attr name="column_key" type="string" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="dbx_connect" type="function" returnType="object">
		<attr name="module" type="mixed" status="optional"></attr>
		<attr name="host" type="string" status="optional"></attr>
		<attr name="database" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="persistent" type="int" status="optional"></attr>
	</tag>
	<tag name="dbx_error" type="function" returnType="string">
		<attr name="link_identifier" type="object" status="optional"></attr>
	</tag>
	<tag name="dbx_query" type="function" returnType="object">
		<attr name="link_identifier" type="object" status="optional"></attr>
		<attr name="sql_statement" type="string" status="optional"></attr>
		<attr name="flags" type="long" status="optional"></attr>
	</tag>
	<tag name="dbx_sort" type="function" returnType="bool">
		<attr name="result" type="object" status="optional"></attr>
		<attr name="user_compare_function" type="string" status="optional"></attr>
	</tag>
</tags>
