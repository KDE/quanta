<!DOCTYPE tags>
<tags>
	<tag name="utf8_decode" type="function" returnType="string" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="data" type="string" status="required"></attr>
	</tag>
	<tag name="utf8_encode" type="function" returnType="string" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="data" type="string" status="required"></attr>
	</tag>
	<tag name="xml_error_string" type="function" returnType="string" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="code" type="int" status="required"></attr>
	</tag>
	<tag name="xml_get_current_byte_index" type="function" returnType="int" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="parser" type="resource" status="required"></attr>
	</tag>
	<tag name="xml_get_current_column_number" type="function" returnType="int" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="parser" type="resource" status="required"></attr>
	</tag>
	<tag name="xml_get_current_line_number" type="function" returnType="int" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="parser" type="resource" status="required"></attr>
	</tag>
	<tag name="xml_get_error_code" type="function" returnType="int" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="parser" type="resource" status="required"></attr>
	</tag>
	<tag name="xml_parse_into_struct" type="function" returnType="int" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="parser" type="resource" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
		<attr name="&amp;values" type="array" status="required"></attr>
		<attr name="&amp;index" type="array" status="optional"></attr>
	</tag>
	<tag name="xml_parse" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="parser" type="resource" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
		<attr name="is_final" type="bool" status="optional"></attr>
	</tag>
	<tag name="xml_parser_create_ns" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="encoding" type="string" status="optional"></attr>
		<attr name="separator" type="string" status="optional"></attr>
	</tag>
	<tag name="xml_parser_create" type="function" returnType="resource" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="xml_parser_free" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="parser" type="resource" status="required"></attr>
	</tag>
	<tag name="xml_parser_get_option" type="function" returnType="mixed" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="parser" type="resource" status="required"></attr>
		<attr name="option" type="int" status="required"></attr>
	</tag>
	<tag name="xml_parser_set_option" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="parser" type="resource" status="required"></attr>
		<attr name="option" type="int" status="required"></attr>
		<attr name="value" type="mixed" status="required"></attr>
	</tag>
	<tag name="xml_set_character_data_handler" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="parser" type="resource" status="required"></attr>
		<attr name="handler" type="callback" status="required"></attr>
	</tag>
	<tag name="xml_set_default_handler" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="parser" type="resource" status="required"></attr>
		<attr name="handler" type="callback" status="required"></attr>
	</tag>
	<tag name="xml_set_element_handler" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="parser" type="resource" status="required"></attr>
		<attr name="start_element_handler" type="callback" status="required"></attr>
		<attr name="end_element_handler" type="callback" status="required"></attr>
	</tag>
	<tag name="xml_set_end_namespace_decl_handler" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="parser" type="resource" status="required"></attr>
		<attr name="handler" type="callback" status="required"></attr>
	</tag>
	<tag name="xml_set_external_entity_ref_handler" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="parser" type="resource" status="required"></attr>
		<attr name="handler" type="callback" status="required"></attr>
	</tag>
	<tag name="xml_set_notation_decl_handler" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="parser" type="resource" status="required"></attr>
		<attr name="handler" type="callback" status="required"></attr>
	</tag>
	<tag name="xml_set_object" type="function" returnType="void" version="PHP 4 , PHP 5">
		<attr name="parser" type="resource" status="required"></attr>
		<attr name="&amp;object" type="object" status="required"></attr>
	</tag>
	<tag name="xml_set_processing_instruction_handler" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="parser" type="resource" status="required"></attr>
		<attr name="handler" type="callback" status="required"></attr>
	</tag>
	<tag name="xml_set_start_namespace_decl_handler" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="parser" type="resource" status="required"></attr>
		<attr name="handler" type="callback" status="required"></attr>
	</tag>
	<tag name="xml_set_unparsed_entity_decl_handler" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="parser" type="resource" status="required"></attr>
		<attr name="handler" type="callback" status="required"></attr>
	</tag>
</tags>