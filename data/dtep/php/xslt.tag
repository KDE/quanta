<!DOCTYPE tags>
<tags>
	<tag name="xslt_backend_info" type="function" returnType="string">
	</tag>
	<tag name="xslt_backend_name" type="function" returnType="string">
	</tag>
	<tag name="xslt_backend_version" type="function" returnType="string">
	</tag>
	<tag name="xslt_create" type="function" returnType="resource">
	</tag>
	<tag name="xslt_errno" type="function" returnType="int">
		<attr name="xh" type="resource"></attr>
	</tag>
	<tag name="xslt_error" type="function" returnType="mixed">
		<attr name="xh" type="resource"></attr>
	</tag>
	<tag name="xslt_free" type="function" returnType="void">
		<attr name="xh" type="resource"></attr>
	</tag>
	<tag name="xslt_getopt" type="function" returnType="int">
		<attr name="processor" type="resource"></attr>
	</tag>
	<tag name="xslt_process" type="function" returnType="mixed">
		<attr name="xh" type="resource"></attr>
		<attr name="xmlcontainer" type="string"></attr>
		<attr name="xslcontainer" type="string"></attr>
		<attr name="resultcontainer" type="string" status="optional"></attr>
		<attr name="arguments" type="array" status="optional"></attr>
		<attr name="parameters" type="array" status="optional"></attr>
	</tag>
	<tag name="xslt_set_base" type="function" returnType="void">
		<attr name="xh" type="resource"></attr>
		<attr name="uri" type="string"></attr>
	</tag>
	<tag name="xslt_set_encoding" type="function" returnType="void">
		<attr name="xh" type="resource"></attr>
		<attr name="encoding" type="string"></attr>
	</tag>
	<tag name="xslt_set_error_handler" type="function" returnType="void">
		<attr name="xh" type="resource"></attr>
		<attr name="handler" type="mixed"></attr>
	</tag>
	<tag name="xslt_set_log" type="function" returnType="void">
		<attr name="xh" type="resource"></attr>
		<attr name="log" type="mixed" status="optional"></attr>
	</tag>
	<tag name="xslt_set_object" type="function" returnType="int">
		<attr name="processor" type="resource"></attr>
		<attr name="&amp;obj" type="object"></attr>
	</tag>
	<tag name="xslt_set_sax_handler" type="function" returnType="void">
		<attr name="xh" type="resource"></attr>
		<attr name="handlers" type="array"></attr>
	</tag>
	<tag name="xslt_set_sax_handlers" type="function" returnType="void">
		<attr name="processor" type="resource"></attr>
		<attr name="handlers" type="array"></attr>
	</tag>
	<tag name="xslt_set_scheme_handler" type="function" returnType="void">
		<attr name="xh" type="resource"></attr>
		<attr name="handlers" type="array"></attr>
	</tag>
	<tag name="xslt_set_scheme_handlers" type="function" returnType="void">
		<attr name="processor" type="resource"></attr>
		<attr name="handlers" type="array"></attr>
	</tag>
	<tag name="xslt_setopt" type="function" returnType="int">
		<attr name="processor" type="resource"></attr>
		<attr name="newmask" type="int"></attr>
	</tag>
</tags>