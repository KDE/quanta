<!DOCTYPE tags>
<tags>
	<tag name="xsl_xsltprocessor_get_parameter" type="function" returnType="string" version="">
		<attr name="namespace" type="string" status="required"></attr>
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="xsl_xsltprocessor_has_exslt_support" type="function" returnType="bool" version="">
	</tag>
	<tag name="xsl_xsltprocessor_import_stylesheet" type="function" returnType="bool" version="">
		<attr name="index" type="object" status="required"></attr>
	</tag>
	<tag name="xsl_xsltprocessor_register_php_functions" type="function" returnType="void" version="">
	</tag>
	<tag name="xsl_xsltprocessor_remove_parameter" type="function" returnType="bool" version="">
		<attr name="namespace" type="string" status="required"></attr>
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="xsl_xsltprocessor_set_parameter" type="function" returnType="bool" version="">
		<attr name="namespace" type="string" status="required"></attr>
		<attr name="name" type="string" status="required"></attr>
		<attr name="value" type="string" status="required"></attr>
	</tag>
	<tag name="xsl_xsltprocessor_transform_to_doc" type="function" returnType="bool" version="">
		<attr name="doc" type="object" status="required"></attr>
	</tag>
	<tag name="xsl_xsltprocessor_transform_to_uri" type="function" returnType="bool" version="">
		<attr name="doc" type="object" status="required"></attr>
		<attr name="uri" type="string" status="required"></attr>
	</tag>
	<tag name="xsl_xsltprocessor_transform_to_xml" type="function" returnType="bool" version="">
		<attr name="doc" type="object" status="required"></attr>
	</tag>
</tags>