<!DOCTYPE tags>
<tags>
	<tag name="ob_tidyhandler" type="function" returnType="string" version="PHP 5">
		<attr name="input" type="string" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="tidy_access_count" type="function" returnType="int" version="PHP 5">
		<attr name="tidy" type="resource" status="required"></attr>
	</tag>
	<tag name="tidy_clean_repair" type="function" returnType="bool" version="PHP 5">
		<attr name="tidy" type="resource" status="required"></attr>
	</tag>
	<tag name="tidy_config_count" type="function" returnType="int" version="PHP 5">
		<attr name="tidy" type="resource" status="required"></attr>
	</tag>
	<tag name="tidy::__construct" type="function" returnType="object" version="">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="config" type="mixed" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
		<attr name="use_include_path" type="bool" status="optional"></attr>
	</tag>
	<tag name="tidy_diagnose" type="function" returnType="bool" version="PHP 5">
		<attr name="tidy" type="resource" status="required"></attr>
	</tag>
	<tag name="tidy_error_count" type="function" returnType="int" version="PHP 5">
		<attr name="tidy" type="resource" status="required"></attr>
	</tag>
	<tag name="tidy_get_body" type="function" returnType="object" version="PHP 5">
		<attr name="tidy" type="resource" status="required"></attr>
	</tag>
	<tag name="tidy_get_config" type="function" returnType="array" version="PHP 5">
		<attr name="tidy" type="resource" status="required"></attr>
	</tag>
	<tag name="tidy_get_error_buffer" type="function" returnType="string" version="PHP 5">
		<attr name="tidy" type="resource" status="required"></attr>
	</tag>
	<tag name="tidy_get_head" type="function" returnType="object" version="PHP 5">
		<attr name="tidy" type="resource" status="required"></attr>
	</tag>
	<tag name="tidy_get_html_ver" type="function" returnType="int" version="PHP 5">
		<attr name="tidy" type="resource" status="required"></attr>
	</tag>
	<tag name="tidy_get_html" type="function" returnType="object" version="PHP 5">
		<attr name="tidy" type="resource" status="required"></attr>
	</tag>
	<tag name="tidy_get_output" type="function" returnType="string" version="PHP 5">
		<attr name="tidy" type="resource" status="required"></attr>
	</tag>
	<tag name="tidy_get_release" type="function" returnType="string" version="PHP 5">
	</tag>
	<tag name="tidy_get_root" type="function" returnType="object" version="PHP 5">
		<attr name="tidy" type="resource" status="required"></attr>
	</tag>
	<tag name="tidy_get_status" type="function" returnType="int" version="PHP 5">
		<attr name="tidy" type="resource" status="required"></attr>
	</tag>
	<tag name="tidy_getopt" type="function" returnType="mixed" version="PHP 5">
		<attr name="tidy" type="resource" status="required"></attr>
		<attr name="option" type="string" status="required"></attr>
	</tag>
	<tag name="tidy_is_xhtml" type="function" returnType="bool" version="PHP 5">
		<attr name="tidy" type="resource" status="required"></attr>
	</tag>
	<tag name="tidy_is_xml" type="function" returnType="bool" version="PHP 5">
		<attr name="tidy" type="resource" status="required"></attr>
	</tag>
	<tag name="tidy_load_config" type="function" returnType="void" version="">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="encoding" type="string" status="required"></attr>
	</tag>
	<tag name="tidy_node->attributes" type="function" returnType="array" version="">
	</tag>
	<tag name="tidy_node->children" type="function" returnType="array" version="">
	</tag>
	<tag name="tidy_node->get_attr" type="function" returnType="tidy_attr" version="">
		<attr name="attrib_id" type="int" status="required"></attr>
	</tag>
	<tag name="tidy_node->get_nodes" type="function" returnType="array" version="">
		<attr name="node_id" type="int" status="required"></attr>
	</tag>
	<tag name="tidy_node->hasChildren" type="function" returnType="bool" version="">
	</tag>
	<tag name="tidy_node->hasSiblings" type="function" returnType="bool" version="">
	</tag>
	<tag name="tidy_node->isAsp" type="function" returnType="bool" version="">
	</tag>
	<tag name="tidy_node->isComment" type="function" returnType="bool" version="">
	</tag>
	<tag name="tidy_node->isHtml" type="function" returnType="bool" version="">
	</tag>
	<tag name="tidy_node->isJste" type="function" returnType="bool" version="">
	</tag>
	<tag name="tidy_node->isPhp" type="function" returnType="bool" version="">
	</tag>
	<tag name="tidy_node->isText" type="function" returnType="bool" version="">
	</tag>
	<tag name="tidy_node->isXhtml" type="function" returnType="bool" version="">
	</tag>
	<tag name="tidy_node->isXml" type="function" returnType="bool" version="">
	</tag>
	<tag name="tidy_node->next" type="function" returnType="tidy_node" version="">
	</tag>
	<tag name="tidy_node->prev" type="function" returnType="tidy_node" version="">
	</tag>
	<tag name="tidy_node->tidy_node" type="function" returnType="void" version="">
	</tag>
	<tag name="tidy_parse_file" type="function" returnType="resource" version="PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="config" type="mixed" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
		<attr name="use_include_path" type="bool" status="optional"></attr>
	</tag>
	<tag name="tidy_parse_string" type="function" returnType="resource" version="PHP 5">
		<attr name="input" type="string" status="required"></attr>
		<attr name="config" type="mixed" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="tidy_repair_file" type="function" returnType="string" version="PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="config" type="mixed" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
		<attr name="use_include_path" type="bool" status="optional"></attr>
	</tag>
	<tag name="tidy_repair_string" type="function" returnType="string" version="PHP 5">
		<attr name="data" type="string" status="required"></attr>
		<attr name="config" type="mixed" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="tidy_reset_config" type="function" returnType="bool" version="">
	</tag>
	<tag name="tidy_save_config" type="function" returnType="bool" version="">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="tidy_set_encoding" type="function" returnType="bool" version="">
		<attr name="encoding" type="string" status="required"></attr>
	</tag>
	<tag name="tidy_setopt" type="function" returnType="bool" version="">
		<attr name="option" type="string" status="required"></attr>
		<attr name="value" type="mixed" status="required"></attr>
	</tag>
	<tag name="tidy_warning_count" type="function" returnType="int" version="PHP 5">
		<attr name="tidy" type="resource" status="required"></attr>
	</tag>
</tags>