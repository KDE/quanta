<!DOCTYPE tags>
<tags>
	<tag name="ingres_autocommit" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_close" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_commit" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_connect" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="database" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="ingres_fetch_array" type="function" returnType="array" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="result_type" type="int" status="optional"></attr>
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_fetch_object" type="function" returnType="object" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="result_type" type="int" status="optional"></attr>
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_fetch_row" type="function" returnType="array" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_field_length" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="index" type="int" status="required"></attr>
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_field_name" type="function" returnType="string" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="index" type="int" status="required"></attr>
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_field_nullable" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="index" type="int" status="required"></attr>
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_field_precision" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="index" type="int" status="required"></attr>
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_field_scale" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="index" type="int" status="required"></attr>
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_field_type" type="function" returnType="string" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="index" type="int" status="required"></attr>
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_num_fields" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_num_rows" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_pconnect" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="database" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="ingres_query" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="query" type="string" status="required"></attr>
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ingres_rollback" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
</tags>