<!DOCTYPE tags>
<tags>
	<tag name="ob_tidyhandler" type="function" returnType="string">
		<attr name="input" type="string"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="tidy_access_count" type="function" returnType="int">
		<attr name="tidy" type="resource"></attr>
	</tag>
	<tag name="tidy_clean_repair" type="function" returnType="bool">
		<attr name="tidy" type="resource"></attr>
	</tag>
	<tag name="tidy_config_count" type="function" returnType="int">
		<attr name="tidy" type="resource"></attr>
	</tag>
	<tag name="tidy::__construct" type="function" returnType="object">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="config" type="mixed" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
		<attr name="use_include_path" type="bool" status="optional"></attr>
	</tag>
	<tag name="tidy_diagnose" type="function" returnType="bool">
		<attr name="tidy" type="resource"></attr>
	</tag>
	<tag name="tidy_error_count" type="function" returnType="int">
		<attr name="tidy" type="resource"></attr>
	</tag>
	<tag name="tidy_get_body" type="function" returnType="object">
		<attr name="tidy" type="resource"></attr>
	</tag>
	<tag name="tidy_get_config" type="function" returnType="array">
		<attr name="tidy" type="resource"></attr>
	</tag>
	<tag name="tidy_get_error_buffer" type="function" returnType="string">
		<attr name="tidy" type="resource"></attr>
	</tag>
	<tag name="tidy_get_head" type="function" returnType="object">
		<attr name="tidy" type="resource"></attr>
	</tag>
	<tag name="tidy_get_html_ver" type="function" returnType="int">
		<attr name="tidy" type="resource"></attr>
	</tag>
	<tag name="tidy_get_html" type="function" returnType="object">
		<attr name="tidy" type="resource"></attr>
	</tag>
	<tag name="tidy_get_output" type="function" returnType="string">
		<attr name="tidy" type="resource"></attr>
	</tag>
	<tag name="tidy_get_release" type="function" returnType="string">
	</tag>
	<tag name="tidy_get_root" type="function" returnType="object">
		<attr name="tidy" type="resource"></attr>
	</tag>
	<tag name="tidy_get_status" type="function" returnType="int">
		<attr name="tidy" type="resource"></attr>
	</tag>
	<tag name="tidy_getopt" type="function" returnType="mixed">
		<attr name="tidy" type="resource"></attr>
		<attr name="option" type="string"></attr>
	</tag>
	<tag name="tidy_is_xhtml" type="function" returnType="bool">
		<attr name="tidy" type="resource"></attr>
	</tag>
	<tag name="tidy_is_xml" type="function" returnType="bool">
		<attr name="tidy" type="resource"></attr>
	</tag>
	<tag name="tidy_load_config" type="function" returnType="void">
		<attr name="filename" type="string"></attr>
		<attr name="encoding" type="string"></attr>
	</tag>
	<tag name="tidy_node->attributes" type="function" returnType="array">
	</tag>
	<tag name="tidy_node->children" type="function" returnType="array">
	</tag>
	<tag name="tidy_node->get_attr" type="function" returnType="tidy_attr">
		<attr name="attrib_id" type="int"></attr>
	</tag>
	<tag name="tidy_node->get_nodes" type="function" returnType="array">
		<attr name="node_id" type="int"></attr>
	</tag>
	<tag name="tidy_node->hasChildren" type="function" returnType="bool">
	</tag>
	<tag name="tidy_node->hasSiblings" type="function" returnType="bool">
	</tag>
	<tag name="tidy_node->isAsp" type="function" returnType="bool">
	</tag>
	<tag name="tidy_node->isComment" type="function" returnType="bool">
	</tag>
	<tag name="tidy_node->isHtml" type="function" returnType="bool">
	</tag>
	<tag name="tidy_node->isJste" type="function" returnType="bool">
	</tag>
	<tag name="tidy_node->isPhp" type="function" returnType="bool">
	</tag>
	<tag name="tidy_node->isText" type="function" returnType="bool">
	</tag>
	<tag name="tidy_node->isXhtml" type="function" returnType="bool">
	</tag>
	<tag name="tidy_node->isXml" type="function" returnType="bool">
	</tag>
	<tag name="tidy_node->next" type="function" returnType="tidy_node">
	</tag>
	<tag name="tidy_node->prev" type="function" returnType="tidy_node">
	</tag>
	<tag name="tidy_node->tidy_node" type="function" returnType="void">
	</tag>
	<tag name="tidy_parse_file" type="function" returnType="resource">
		<attr name="filename" type="string"></attr>
		<attr name="config" type="mixed" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
		<attr name="use_include_path" type="bool" status="optional"></attr>
	</tag>
	<tag name="tidy_parse_string" type="function" returnType="resource">
		<attr name="input" type="string"></attr>
		<attr name="config" type="mixed" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="tidy_repair_file" type="function" returnType="string">
		<attr name="filename" type="string"></attr>
		<attr name="config" type="mixed" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
		<attr name="use_include_path" type="bool" status="optional"></attr>
	</tag>
	<tag name="tidy_repair_string" type="function" returnType="string">
		<attr name="data" type="string"></attr>
		<attr name="config" type="mixed" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="tidy_reset_config" type="function" returnType="bool">
	</tag>
	<tag name="tidy_save_config" type="function" returnType="bool">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="tidy_set_encoding" type="function" returnType="bool">
		<attr name="encoding" type="string"></attr>
	</tag>
	<tag name="tidy_setopt" type="function" returnType="bool">
		<attr name="option" type="string"></attr>
		<attr name="value" type="mixed"></attr>
	</tag>
	<tag name="tidy_warning_count" type="function" returnType="int">
		<attr name="tidy" type="resource"></attr>
	</tag>
</tags>