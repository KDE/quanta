<!DOCTYPE tags>
<tags>
	<tag name="curl_close" type="function" returnType="void">
		<attr name="ch" type="resource"></attr>
	</tag>
	<tag name="curl_copy_handle" type="function" returnType="resource">
		<attr name="ch" type="resource"></attr>
	</tag>
	<tag name="curl_errno" type="function" returnType="int">
		<attr name="ch" type="resource"></attr>
	</tag>
	<tag name="curl_error" type="function" returnType="string">
		<attr name="ch" type="resource"></attr>
	</tag>
	<tag name="curl_exec" type="function" returnType="mixed">
		<attr name="ch" type="resource"></attr>
	</tag>
	<tag name="curl_getinfo" type="function" returnType="string">
		<attr name="ch" type="resource"></attr>
		<attr name="opt" type="int" status="optional"></attr>
	</tag>
	<tag name="curl_init" type="function" returnType="resource">
		<attr name="url" type="string" status="optional"></attr>
	</tag>
	<tag name="curl_multi_add_handle" type="function" returnType="int">
		<attr name="mh" type="resource"></attr>
		<attr name="ch" type="resource"></attr>
	</tag>
	<tag name="curl_multi_close" type="function" returnType="void">
		<attr name="mh" type="resource"></attr>
	</tag>
	<tag name="curl_multi_exec" type="function" returnType="int">
		<attr name="mh" type="resource"></attr>
		<attr name="&amp;still_running" type="int"></attr>
	</tag>
	<tag name="curl_multi_getcontent" type="function" returnType="string">
		<attr name="ch" type="resource"></attr>
	</tag>
	<tag name="curl_multi_info_read" type="function" returnType="array">
		<attr name="mh" type="resource"></attr>
	</tag>
	<tag name="curl_multi_init" type="function" returnType="resource">
	</tag>
	<tag name="curl_multi_remove_handle" type="function" returnType="int">
		<attr name="mh" type="resource"></attr>
		<attr name="ch" type="resource"></attr>
	</tag>
	<tag name="curl_multi_select" type="function" returnType="int">
		<attr name="mh" type="resource"></attr>
		<attr name="timeout" type="float" status="optional"></attr>
	</tag>
	<tag name="curl_setopt" type="function" returnType="bool">
		<attr name="ch" type="resource"></attr>
		<attr name="option" type="integer"></attr>
		<attr name="value" type="mixed"></attr>
	</tag>
	<tag name="curl_version" type="function" returnType="string">
		<attr name="version" type="int" status="optional"></attr>
	</tag>
</tags>