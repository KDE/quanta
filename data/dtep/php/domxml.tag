<!DOCTYPE tags>
<tags>
	<tag name="DomAttribute->name" type="function" returnType="string">
	</tag>
	<tag name="DomAttribute->specified" type="function" returnType="bool">
	</tag>
	<tag name="DomAttribute->value" type="function" returnType="mixed">
	</tag>
	<tag name="DomDocument->add_root" type="function" returnType="object">
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="DomDocument->create_attribute" type="function" returnType="object">
		<attr name="name" type="string"></attr>
		<attr name="value" type="string"></attr>
	</tag>
	<tag name="DomDocument->create_cdata_section" type="function" returnType="object">
		<attr name="content" type="string"></attr>
	</tag>
	<tag name="DomDocument->create_comment" type="function" returnType="object">
		<attr name="content" type="string"></attr>
	</tag>
	<tag name="DomDocument->create_element_ns" type="function" returnType="object">
		<attr name="uri" type="string"></attr>
		<attr name="name" type="string"></attr>
		<attr name="prefix" type="string" status="optional"></attr>
	</tag>
	<tag name="DomDocument->create_element" type="function" returnType="object">
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="DomDocument->create_entity_reference" type="function" returnType="object">
		<attr name="content" type="string"></attr>
	</tag>
	<tag name="DomDocument->create_processing_instruction" type="function" returnType="object">
		<attr name="content" type="string"></attr>
	</tag>
	<tag name="DomDocument->create_text_node" type="function" returnType="object">
		<attr name="content" type="string"></attr>
	</tag>
	<tag name="DomDocument->doctype" type="function" returnType="object">
	</tag>
	<tag name="DomDocument->document_element" type="function" returnType="object">
	</tag>
	<tag name="DomDocument->dump_file" type="function" returnType="string">
		<attr name="filename" type="string"></attr>
		<attr name="compressionmode" type="bool" status="optional"></attr>
		<attr name="format" type="bool" status="optional"></attr>
	</tag>
	<tag name="DomDocument->dump_mem" type="function" returnType="string">
		<attr name="format" type="bool" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="DomDocument->get_element_by_id" type="function" returnType="object">
		<attr name="id" type="string"></attr>
	</tag>
	<tag name="DomDocument->get_elements_by_tagname" type="function" returnType="array">
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="DomDocument->html_dump_mem" type="function" returnType="string">
	</tag>
	<tag name="DomDocument->xinclude" type="function" returnType="int">
	</tag>
	<tag name="DomDocumentType->entities" type="function" returnType="array">
	</tag>
	<tag name="DomDocumentType->internal_subset" type="function" returnType="bool">
	</tag>
	<tag name="DomDocumentType->name" type="function" returnType="string">
	</tag>
	<tag name="DomDocumentType->notations" type="function" returnType="array">
	</tag>
	<tag name="DomDocumentType->public_id" type="function" returnType="string">
	</tag>
	<tag name="DomDocumentType->system_id" type="function" returnType="string">
	</tag>
	<tag name="DomElement->get_attribute_node" type="function" returnType="object">
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="DomElement->get_attribute" type="function" returnType="string">
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="DomElement->get_elements_by_tagname" type="function" returnType="array">
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="DomElement->has_attribute" type="function" returnType="bool">
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="DomElement->remove_attribute" type="function" returnType="bool">
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="DomElement->set_attribute_node" type="function" returnType="bool">
		<attr name="attr" type="object"></attr>
	</tag>
	<tag name="DomElement->set_attribute" type="function" returnType="object">
		<attr name="name" type="string"></attr>
		<attr name="value" type="string"></attr>
	</tag>
	<tag name="DomElement->tagname" type="function" returnType="string">
	</tag>
	<tag name="DomNode->add_namespace" type="function" returnType="bool">
		<attr name="uri" type="string"></attr>
		<attr name="prefix" type="string"></attr>
	</tag>
	<tag name="DomNode->append_child" type="function" returnType="object">
		<attr name="newnode" type="object"></attr>
	</tag>
	<tag name="DomNode->append_sibling" type="function" returnType="object">
		<attr name="newnode" type="object"></attr>
	</tag>
	<tag name="DomNode->attributes" type="function" returnType="array">
	</tag>
	<tag name="DomNode->child_nodes" type="function" returnType="array">
	</tag>
	<tag name="DomNode->clone_node" type="function" returnType="object">
	</tag>
	<tag name="DomNode->dump_node" type="function" returnType="string">
	</tag>
	<tag name="DomNode->first_child" type="function" returnType="object">
	</tag>
	<tag name="DomNode->get_content" type="function" returnType="string">
	</tag>
	<tag name="DomNode->has_attributes" type="function" returnType="bool">
	</tag>
	<tag name="DomNode->has_child_nodes" type="function" returnType="bool">
	</tag>
	<tag name="DomNode->insert_before" type="function" returnType="object">
		<attr name="newnode" type="object"></attr>
		<attr name="refnode" type="object"></attr>
	</tag>
	<tag name="DomNode->is_blank_node" type="function" returnType="bool">
	</tag>
	<tag name="DomNode->last_child" type="function" returnType="object">
	</tag>
	<tag name="DomNode->next_sibling" type="function" returnType="object">
	</tag>
	<tag name="DomNode->node_name" type="function" returnType="string">
	</tag>
	<tag name="DomNode->node_type" type="function" returnType="int">
	</tag>
	<tag name="DomNode->node_value" type="function" returnType="string">
	</tag>
	<tag name="DomNode->owner_document" type="function" returnType="object">
	</tag>
	<tag name="DomNode->parent_node" type="function" returnType="object">
	</tag>
	<tag name="DomNode->prefix" type="function" returnType="string">
	</tag>
	<tag name="DomNode->previous_sibling" type="function" returnType="object">
	</tag>
	<tag name="DomNode->remove_child" type="function" returnType="object">
		<attr name="oldchild" type="object"></attr>
	</tag>
	<tag name="DomNode->replace_child" type="function" returnType="object">
		<attr name="oldnode" type="object"></attr>
		<attr name="newnode" type="object"></attr>
	</tag>
	<tag name="DomNode->replace_node" type="function" returnType="object">
		<attr name="newnode" type="object"></attr>
	</tag>
	<tag name="DomNode->set_content" type="function" returnType="bool">
	</tag>
	<tag name="DomNode->set_name" type="function" returnType="bool">
	</tag>
	<tag name="DomNode->set_namespace" type="function" returnType="void">
		<attr name="uri" type="string"></attr>
		<attr name="prefix" type="string" status="optional"></attr>
	</tag>
	<tag name="DomNode->unlink_node" type="function" returnType="object">
	</tag>
	<tag name="DomProcessingInstruction->data" type="function" returnType="string">
	</tag>
	<tag name="DomProcessingInstruction->target" type="function" returnType="string">
	</tag>
	<tag name="DomXsltStylesheet->process" type="function" returnType="object">
		<attr name="DomDocument" type="object"></attr>
		<attr name="xslt_parameters" type="array" status="optional"></attr>
		<attr name="param_is_xpath" type="bool" status="optional"></attr>
	</tag>
	<tag name="DomXsltStylesheet->result_dump_file" type="function" returnType="string">
		<attr name="DomDocument" type="object"></attr>
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="DomXsltStylesheet->result_dump_mem" type="function" returnType="string">
		<attr name="DomDocument" type="object"></attr>
	</tag>
	<tag name="domxml_new_doc" type="function" returnType="object">
		<attr name="version" type="string"></attr>
	</tag>
	<tag name="domxml_open_file" type="function" returnType="object">
		<attr name="filename" type="string"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="&amp;error" type="array" status="optional"></attr>
	</tag>
	<tag name="domxml_open_mem" type="function" returnType="object">
		<attr name="str" type="string"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="&amp;error" type="array" status="optional"></attr>
	</tag>
	<tag name="domxml_version" type="function" returnType="string">
	</tag>
	<tag name="domxml_xmltree" type="function" returnType="object">
		<attr name="str" type="string"></attr>
	</tag>
	<tag name="domxml_xslt_stylesheet_doc" type="function" returnType="object">
		<attr name="DocDocument Object" type="object"></attr>
	</tag>
	<tag name="domxml_xslt_stylesheet_file" type="function" returnType="object">
		<attr name="xsl file" type="string"></attr>
	</tag>
	<tag name="domxml_xslt_stylesheet" type="function" returnType="object">
		<attr name="xsl document" type="string"></attr>
	</tag>
	<tag name="xpath_eval_expression" type="function" returnType="object">
		<attr name="xpath_context" type="object"></attr>
		<attr name="expression" type="string"></attr>
	</tag>
	<tag name="xpath_eval" type="function" returnType="array">
		<attr name="xpath context" type="object"></attr>
		<attr name="xpath expression" type="string"></attr>
		<attr name="contextnode" type="object" status="optional"></attr>
	</tag>
	<tag name="xpath_new_context" type="function" returnType="object">
		<attr name="dom document" type="object"></attr>
	</tag>
	<tag name="xptr_eval" type="function" returnType="int">
		<attr name="xpath_context" type="object" status="optional"></attr>
		<attr name="eval_str" type="string" status="optional"></attr>
	</tag>
	<tag name="xptr_new_context" type="function" returnType="string">
		<attr name="doc_handle" type="object" status="optional"></attr>
	</tag>
</tags>