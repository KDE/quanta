<!DOCTYPE tags>
<tags>
	<tag name="xslt_backend_info" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0">
	</tag>
	<tag name="xslt_backend_name" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0">
	</tag>
	<tag name="xslt_backend_version" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0">
	</tag>
	<tag name="xslt_create" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.3">
	</tag>
	<tag name="xslt_errno" type="function" returnType="int" version="PHP 4 &gt;= 4.0.3">
		<attr name="xh" type="resource" status="required"></attr>
	</tag>
	<tag name="xslt_error" type="function" returnType="mixed" version="PHP 4 &gt;= 4.0.3">
		<attr name="xh" type="resource" status="required"></attr>
	</tag>
	<tag name="xslt_free" type="function" returnType="void" version="PHP 4 &gt;= 4.0.3">
		<attr name="xh" type="resource" status="required"></attr>
	</tag>
	<tag name="xslt_getopt" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0">
		<attr name="processor" type="resource" status="required"></attr>
	</tag>
	<tag name="xslt_process" type="function" returnType="mixed" version="PHP 4 &gt;= 4.0.3">
		<attr name="xh" type="resource" status="required"></attr>
		<attr name="xmlcontainer" type="string" status="required"></attr>
		<attr name="xslcontainer" type="string" status="required"></attr>
		<attr name="resultcontainer" type="string" status="optional"></attr>
		<attr name="arguments" type="array" status="optional"></attr>
		<attr name="parameters" type="array" status="optional"></attr>
	</tag>
	<tag name="xslt_set_base" type="function" returnType="void" version="PHP 4 &gt;= 4.0.5">
		<attr name="xh" type="resource" status="required"></attr>
		<attr name="uri" type="string" status="required"></attr>
	</tag>
	<tag name="xslt_set_encoding" type="function" returnType="void" version="PHP 4 &gt;= 4.0.5">
		<attr name="xh" type="resource" status="required"></attr>
		<attr name="encoding" type="string" status="required"></attr>
	</tag>
	<tag name="xslt_set_error_handler" type="function" returnType="void" version="PHP 4 &gt;= 4.0.4">
		<attr name="xh" type="resource" status="required"></attr>
		<attr name="handler" type="mixed" status="required"></attr>
	</tag>
	<tag name="xslt_set_log" type="function" returnType="void" version="PHP 4 &gt;= 4.0.6">
		<attr name="xh" type="resource" status="required"></attr>
		<attr name="log" type="mixed" status="optional"></attr>
	</tag>
	<tag name="xslt_set_object" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0">
		<attr name="processor" type="resource" status="required"></attr>
		<attr name="&amp;obj" type="object" status="required"></attr>
	</tag>
	<tag name="xslt_set_sax_handler" type="function" returnType="void" version="4.0.3 - 4.0.6 only">
		<attr name="xh" type="resource" status="required"></attr>
		<attr name="handlers" type="array" status="required"></attr>
	</tag>
	<tag name="xslt_set_sax_handlers" type="function" returnType="void" version="PHP 4 &gt;= 4.0.6">
		<attr name="processor" type="resource" status="required"></attr>
		<attr name="handlers" type="array" status="required"></attr>
	</tag>
	<tag name="xslt_set_scheme_handler" type="function" returnType="void" version="4.0.5 - 4.0.6 only">
		<attr name="xh" type="resource" status="required"></attr>
		<attr name="handlers" type="array" status="required"></attr>
	</tag>
	<tag name="xslt_set_scheme_handlers" type="function" returnType="void" version="PHP 4 &gt;= 4.0.6">
		<attr name="processor" type="resource" status="required"></attr>
		<attr name="handlers" type="array" status="required"></attr>
	</tag>
	<tag name="xslt_setopt" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0">
		<attr name="processor" type="resource" status="required"></attr>
		<attr name="newmask" type="int" status="required"></attr>
	</tag>
</tags>