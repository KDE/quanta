<!DOCTYPE tags>
<tags>
	<tag name="curl_close" type="function" returnType="void" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="ch" type="resource" status="required"></attr>
	</tag>
	<tag name="curl_copy_handle" type="function" returnType="resource" version="PHP 5">
		<attr name="ch" type="resource" status="required"></attr>
	</tag>
	<tag name="curl_errno" type="function" returnType="int" version="PHP 4 &gt;= 4.0.3, PHP 5">
		<attr name="ch" type="resource" status="required"></attr>
	</tag>
	<tag name="curl_error" type="function" returnType="string" version="PHP 4 &gt;= 4.0.3, PHP 5">
		<attr name="ch" type="resource" status="required"></attr>
	</tag>
	<tag name="curl_exec" type="function" returnType="mixed" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="ch" type="resource" status="required"></attr>
	</tag>
	<tag name="curl_getinfo" type="function" returnType="string" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="ch" type="resource" status="required"></attr>
		<attr name="opt" type="int" status="optional"></attr>
	</tag>
	<tag name="curl_init" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="url" type="string" status="optional"></attr>
	</tag>
	<tag name="curl_multi_add_handle" type="function" returnType="int" version="PHP 5">
		<attr name="mh" type="resource" status="required"></attr>
		<attr name="ch" type="resource" status="required"></attr>
	</tag>
	<tag name="curl_multi_close" type="function" returnType="void" version="PHP 5">
		<attr name="mh" type="resource" status="required"></attr>
	</tag>
	<tag name="curl_multi_exec" type="function" returnType="int" version="PHP 5">
		<attr name="mh" type="resource" status="required"></attr>
		<attr name="&amp;still_running" type="int" status="required"></attr>
	</tag>
	<tag name="curl_multi_getcontent" type="function" returnType="string" version="PHP 5">
		<attr name="ch" type="resource" status="required"></attr>
	</tag>
	<tag name="curl_multi_info_read" type="function" returnType="array" version="PHP 5">
		<attr name="mh" type="resource" status="required"></attr>
	</tag>
	<tag name="curl_multi_init" type="function" returnType="resource" version="PHP 5">
	</tag>
	<tag name="curl_multi_remove_handle" type="function" returnType="int" version="PHP 5">
		<attr name="mh" type="resource" status="required"></attr>
		<attr name="ch" type="resource" status="required"></attr>
	</tag>
	<tag name="curl_multi_select" type="function" returnType="int" version="PHP 5">
		<attr name="mh" type="resource" status="required"></attr>
		<attr name="timeout" type="float" status="optional"></attr>
	</tag>
	<tag name="curl_setopt" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="ch" type="resource" status="required"></attr>
		<attr name="option" type="integer" status="required"></attr>
		<attr name="value" type="mixed" status="required"></attr>
	</tag>
	<tag name="curl_version" type="function" returnType="string" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="version" type="int" status="optional"></attr>
	</tag>
</tags>