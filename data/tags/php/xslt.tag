<!DOCTYPE tags>
<tags>	<tag name="xslt_create" type="function" returnType="resource">
	</tag>
	<tag name="xslt_errno" type="function" returnType="int">
		<attr name="xh" type="resource" status="optional"></attr>
	</tag>
	<tag name="xslt_error" type="function" returnType="mixed">
		<attr name="xh" type="resource" status="optional"></attr>
	</tag>
	<tag name="xslt_free" type="function" returnType="void">
		<attr name="xh" type="resource" status="optional"></attr>
	</tag>
	<tag name="xslt_process" type="function" returnType="mixed">
		<attr name="xh" type="resource" status="optional"></attr>
		<attr name="xml" type="string" status="optional"></attr>
		<attr name="xsl" type="string" status="optional"></attr>
		<attr name="result" type="string" status="optional"></attr>
		<attr name="arguments" type="array" status="optional"></attr>
		<attr name="parameters" type="array" status="optional"></attr>
	</tag>
	<tag name="xslt_set_base" type="function" returnType="void">
		<attr name="xh" type="resource" status="optional"></attr>
		<attr name="uri" type="string" status="optional"></attr>
	</tag>
	<tag name="xslt_set_encoding" type="function" returnType="void">
		<attr name="xh" type="resource" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="xslt_set_error_handler" type="function" returnType="void">
		<attr name="xh" type="resource" status="optional"></attr>
		<attr name="handler" type="mixed" status="optional"></attr>
	</tag>
	<tag name="xslt_set_log" type="function" returnType="void">
		<attr name="xh" type="resource" status="optional"></attr>
		<attr name="log" type="mixed" status="optional"></attr>
	</tag>
	<tag name="xslt_set_sax_handler" type="function" returnType="void">
		<attr name="xh" type="resource" status="optional"></attr>
		<attr name="handlers" type="array" status="optional"></attr>
	</tag>
	<tag name="xslt_set_sax_handlers" type="function" returnType="void">
		<attr name="processor" type="resource" status="optional"></attr>
		<attr name="handlers" type="array" status="optional"></attr>
	</tag>
	<tag name="xslt_set_scheme_handler" type="function" returnType="void">
		<attr name="xh" type="resource" status="optional"></attr>
		<attr name="handlers" type="array" status="optional"></attr>
	</tag>
	<tag name="xslt_set_scheme_handlers" type="function" returnType="void">
		<attr name="processor" type="resource" status="optional"></attr>
		<attr name="handlers" type="array" status="optional"></attr>
	</tag>
</tags>
