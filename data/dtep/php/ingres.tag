<!DOCTYPE tags>
<tags>	<tag name="ingres_autocommit" type="function" returnType="bool">
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_close" type="function" returnType="bool">
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_commit" type="function" returnType="bool">
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_connect" type="function" returnType="resource">
		<attr name="database" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="ingres_fetch_array" type="function" returnType="array">
		<attr name="result_type" type="int" status="optional"></attr>
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_fetch_object" type="function" returnType="object">
		<attr name="result_type" type="int" status="optional"></attr>
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_fetch_row" type="function" returnType="array">
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_field_length" type="function" returnType="int">
		<attr name="index" type="int" status="optional"></attr>
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_field_name" type="function" returnType="string">
		<attr name="index" type="int" status="optional"></attr>
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_field_nullable" type="function" returnType="bool">
		<attr name="index" type="int" status="optional"></attr>
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_field_precision" type="function" returnType="int">
		<attr name="index" type="int" status="optional"></attr>
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_field_scale" type="function" returnType="int">
		<attr name="index" type="int" status="optional"></attr>
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_field_type" type="function" returnType="string">
		<attr name="index" type="int" status="optional"></attr>
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_num_fields" type="function" returnType="int">
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_num_rows" type="function" returnType="int">
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_pconnect" type="function" returnType="resource">
		<attr name="database" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="ingres_query" type="function" returnType="bool">
		<attr name="query" type="string" status="optional"></attr>
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_rollback" type="function" returnType="bool">
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
</tags>
