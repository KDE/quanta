<!DOCTYPE tags>
<tags>	<tag name="curl_close" type="function" returnType="void">
		<attr name="ch" type="resource" status="optional"></attr>
	</tag>
	<tag name="curl_errno" type="function" returnType="int">
		<attr name="ch" type="resource" status="optional"></attr>
	</tag>
	<tag name="curl_error" type="function" returnType="string">
		<attr name="ch" type="resource" status="optional"></attr>
	</tag>
	<tag name="curl_exec" type="function" returnType="bool">
		<attr name="ch" type="resource" status="optional"></attr>
	</tag>
	<tag name="curl_getinfo" type="function" returnType="string">
		<attr name="ch" type="resource" status="optional"></attr>
		<attr name="opt" type="int" status="optional"></attr>
	</tag>
	<tag name="curl_init" type="function" returnType="resource">
		<attr name="url" type="string" status="optional"></attr>
	</tag>
	<tag name="curl_setopt" type="function" returnType="bool">
		<attr name="ch" type="resource" status="optional"></attr>
		<attr name="option" type="string" status="optional"></attr>
		<attr name="value" type="mixed" status="optional"></attr>
	</tag>
	<tag name="curl_version" type="function" returnType="string">
	</tag>
</tags>
