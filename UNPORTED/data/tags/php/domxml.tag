<!DOCTYPE tags>
<tags>	<tag name="domxml_new_doc" type="function" returnType="object">
		<attr name="version" type="string" status="optional"></attr>
	</tag>
	<tag name="domxml_open_file" type="function" returnType="object">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="domxml_open_mem" type="function" returnType="object">
		<attr name="str" type="string" status="optional"></attr>
	</tag>
	<tag name="domxml_version" type="function" returnType="string">
	</tag>
	<tag name="domxml_xmltree" type="function" returnType="object">
		<attr name="str" type="string" status="optional"></attr>
	</tag>
	<tag name="domxml_xslt_stylesheet_doc" type="function" returnType="object">
		<attr name="DocDocument Object" type="object" status="optional"></attr>
	</tag>
	<tag name="domxml_xslt_stylesheet_file" type="function" returnType="object">
		<attr name="xsl file" type="string" status="optional"></attr>
	</tag>
	<tag name="domxml_xslt_stylesheet" type="function" returnType="object">
		<attr name="xsl document" type="string" status="optional"></attr>
	</tag>
	<tag name="xpath_eval_expression" type="function" returnType="array">
		<attr name="xpath_context" type="object" status="optional"></attr>
	</tag>
	<tag name="xpath_eval" type="function" returnType="array">
		<attr name="xpath context" type="object" status="optional"></attr>
	</tag>
	<tag name="xpath_new_context" type="function" returnType="object">
		<attr name="dom document" type="object" status="optional"></attr>
	</tag>
	<tag name="xptr_eval" type="function" returnType="int">
		<attr name="xpath_context" type="object" status="optional"></attr>
		<attr name="eval_str" type="string" status="optional"></attr>
	</tag>
	<tag name="xptr_new_context" type="function" returnType="string">
		<attr name="doc_handle" type="object" status="optional"></attr>
	</tag>
</tags>
