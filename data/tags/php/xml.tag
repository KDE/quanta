<!DOCTYPE tags>
<tags>	<tag name="utf8_decode" type="function" returnType="string">
		<attr name="data" type="string" status="optional"></attr>
	</tag>
	<tag name="utf8_encode" type="function" returnType="string">
		<attr name="data" type="string" status="optional"></attr>
	</tag>
	<tag name="xml_error_string" type="function" returnType="string">
		<attr name="code" type="int" status="optional"></attr>
	</tag>
	<tag name="xml_get_current_byte_index" type="function" returnType="int">
		<attr name="parser" type="resource" status="optional"></attr>
	</tag>
	<tag name="xml_get_current_column_number" type="function" returnType="int">
		<attr name="parser" type="resource" status="optional"></attr>
	</tag>
	<tag name="xml_get_current_line_number" type="function" returnType="int">
		<attr name="parser" type="resource" status="optional"></attr>
	</tag>
	<tag name="xml_get_error_code" type="function" returnType="int">
		<attr name="parser" type="resource" status="optional"></attr>
	</tag>
	<tag name="xml_parse_into_struct" type="function" returnType="int">
		<attr name="parser" type="resource" status="optional"></attr>
		<attr name="data" type="string" status="optional"></attr>
		<attr name="&amp;values" type="array" status="optional"></attr>
		<attr name="&amp;index" type="array" status="optional"></attr>
	</tag>
	<tag name="xml_parse" type="function" returnType="bool">
		<attr name="parser" type="resource" status="optional"></attr>
		<attr name="data" type="string" status="optional"></attr>
		<attr name="is_final" type="bool" status="optional"></attr>
	</tag>
	<tag name="xml_parser_create_ns" type="function" returnType="resource">
		<attr name="encoding" type="string" status="optional"></attr>
		<attr name="sep" type="string" status="optional"></attr>
	</tag>
	<tag name="xml_parser_create" type="function" returnType="resource">
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="xml_parser_free" type="function" returnType="bool">
		<attr name="parser" type="resource" status="optional"></attr>
	</tag>
	<tag name="xml_parser_get_option" type="function" returnType="mixed">
		<attr name="parser" type="resource" status="optional"></attr>
		<attr name="option" type="int" status="optional"></attr>
	</tag>
	<tag name="xml_parser_set_option" type="function" returnType="bool">
		<attr name="parser" type="resource" status="optional"></attr>
		<attr name="option" type="int" status="optional"></attr>
		<attr name="value" type="mixed" status="optional"></attr>
	</tag>
	<tag name="xml_set_character_data_handler" type="function" returnType="bool">
		<attr name="parser" type="resource" status="optional"></attr>
		<attr name="handler" type="string" status="optional"></attr>
	</tag>
	<tag name="xml_set_default_handler" type="function" returnType="bool">
		<attr name="parser" type="resource" status="optional"></attr>
		<attr name="handler" type="string" status="optional"></attr>
	</tag>
	<tag name="xml_set_element_handler" type="function" returnType="bool">
		<attr name="parser" type="resource" status="optional"></attr>
		<attr name="start_element_handler" type="string" status="optional"></attr>
		<attr name="end_element_handler" type="string" status="optional"></attr>
	</tag>
	<tag name="xml_set_end_namespace_decl_handler" type="function" returnType="bool">
		<attr name="pind" type="resource" status="optional"></attr>
		<attr name="hdl" type="string" status="optional"></attr>
	</tag>
	<tag name="xml_set_external_entity_ref_handler" type="function" returnType="bool">
		<attr name="parser" type="resource" status="optional"></attr>
		<attr name="handler" type="string" status="optional"></attr>
	</tag>
	<tag name="xml_set_notation_decl_handler" type="function" returnType="bool">
		<attr name="parser" type="resource" status="optional"></attr>
		<attr name="handler" type="string" status="optional"></attr>
	</tag>
	<tag name="xml_set_object" type="function" returnType="void">
		<attr name="parser" type="resource" status="optional"></attr>
		<attr name="object" type="object" status="optional"></attr>
	</tag>
	<tag name="xml_set_processing_instruction_handler" type="function" returnType="bool">
		<attr name="parser" type="resource" status="optional"></attr>
		<attr name="handler" type="string" status="optional"></attr>
	</tag>
	<tag name="xml_set_start_namespace_decl_handler" type="function" returnType="bool">
		<attr name="pind" type="resource" status="optional"></attr>
		<attr name="hdl" type="string" status="optional"></attr>
	</tag>
	<tag name="xml_set_unparsed_entity_decl_handler" type="function" returnType="bool">
		<attr name="parser" type="resource" status="optional"></attr>
		<attr name="handler" type="string" status="optional"></attr>
	</tag>
</tags>
