<!DOCTYPE tags>
<tags>
	<tag name="fdf_add_doc_javascript" type="function" returnType="bool">
		<attr name="fdfdoc" type="resource"></attr>
		<attr name="script_name" type="string"></attr>
		<attr name="script_code" type="string"></attr>
	</tag>
	<tag name="fdf_add_template" type="function" returnType="bool">
		<attr name="fdfdoc" type="resource"></attr>
		<attr name="newpage" type="int"></attr>
		<attr name="filename" type="string"></attr>
		<attr name="template" type="string"></attr>
		<attr name="rename" type="int"></attr>
	</tag>
	<tag name="fdf_close" type="function" returnType="bool">
		<attr name="fdf_document" type="resource"></attr>
	</tag>
	<tag name="fdf_create" type="function" returnType="resource">
	</tag>
	<tag name="fdf_enum_values" type="function" returnType="bool">
		<attr name="fdfdoc" type="resource"></attr>
		<attr name="function" type="callback"></attr>
		<attr name="userdata" type="mixed"></attr>
	</tag>
	<tag name="fdf_errno" type="function" returnType="int">
	</tag>
	<tag name="fdf_error" type="function" returnType="string">
		<attr name="error_code" type="int"></attr>
	</tag>
	<tag name="fdf_get_ap" type="function" returnType="bool">
		<attr name="fdf_document" type="resource"></attr>
		<attr name="field" type="string"></attr>
		<attr name="face" type="int"></attr>
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="fdf_get_attachment" type="function" returnType="array">
		<attr name="fdf_document" type="resource"></attr>
		<attr name="fieldname" type="string"></attr>
		<attr name="savepath" type="string"></attr>
	</tag>
	<tag name="fdf_get_encoding" type="function" returnType="string">
		<attr name="fdf_document" type="resource"></attr>
	</tag>
	<tag name="fdf_get_file" type="function" returnType="string">
		<attr name="fdf_document" type="resource"></attr>
	</tag>
	<tag name="fdf_get_flags" type="function" returnType="int">
		<attr name="fdfdoc" type="resource"></attr>
		<attr name="fieldname" type="string"></attr>
		<attr name="whichflags" type="int"></attr>
	</tag>
	<tag name="fdf_get_opt" type="function" returnType="mixed">
		<attr name="fdfdof" type="resource"></attr>
		<attr name="fieldname" type="string"></attr>
		<attr name="element" type="int"></attr>
	</tag>
	<tag name="fdf_get_status" type="function" returnType="string">
		<attr name="fdf_document" type="resource"></attr>
	</tag>
	<tag name="fdf_get_value" type="function" returnType="string">
		<attr name="fdf_document" type="resource"></attr>
		<attr name="fieldname" type="string"></attr>
		<attr name="which" type="int"></attr>
	</tag>
	<tag name="fdf_get_version" type="function" returnType="string">
		<attr name="fdf_document" type="resource"></attr>
	</tag>
	<tag name="fdf_header" type="function" returnType="bool">
	</tag>
	<tag name="fdf_next_field_name" type="function" returnType="string">
		<attr name="fdf_document" type="resource"></attr>
		<attr name="fieldname" type="string" status="optional"></attr>
	</tag>
	<tag name="fdf_open_string" type="function" returnType="resource">
		<attr name="fdf_data" type="string"></attr>
	</tag>
	<tag name="fdf_open" type="function" returnType="resource">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="fdf_remove_item" type="function" returnType="bool">
		<attr name="fdfdoc" type="resource"></attr>
		<attr name="fieldname" type="string"></attr>
		<attr name="item" type="int"></attr>
	</tag>
	<tag name="fdf_save_string" type="function" returnType="string">
		<attr name="fdf_document" type="resource"></attr>
	</tag>
	<tag name="fdf_save" type="function" returnType="bool">
		<attr name="fdf_document" type="resource"></attr>
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="fdf_set_ap" type="function" returnType="bool">
		<attr name="fdf_document" type="resource"></attr>
		<attr name="field_name" type="string"></attr>
		<attr name="face" type="int"></attr>
		<attr name="filename" type="string"></attr>
		<attr name="page_number" type="int"></attr>
	</tag>
	<tag name="fdf_set_encoding" type="function" returnType="bool">
		<attr name="fdf_document" type="resource"></attr>
		<attr name="encoding" type="string"></attr>
	</tag>
	<tag name="fdf_set_file" type="function" returnType="bool">
		<attr name="fdf_document" type="resource"></attr>
		<attr name="url" type="string"></attr>
		<attr name="target_frame" type="string"></attr>
	</tag>
	<tag name="fdf_set_flags" type="function" returnType="bool">
		<attr name="fdf_document" type="resource"></attr>
		<attr name="fieldname" type="string"></attr>
		<attr name="whichFlags" type="int"></attr>
		<attr name="newFlags" type="int"></attr>
	</tag>
	<tag name="fdf_set_javascript_action" type="function" returnType="bool">
		<attr name="fdf_document" type="resource"></attr>
		<attr name="fieldname" type="string"></attr>
		<attr name="trigger" type="int"></attr>
		<attr name="script" type="string"></attr>
	</tag>
	<tag name="fdf_set_on_import_javascript" type="function" returnType="bool">
		<attr name="fdfdoc" type="resource"></attr>
		<attr name="script" type="string"></attr>
		<attr name="before_data_import" type="bool"></attr>
	</tag>
	<tag name="fdf_set_opt" type="function" returnType="bool">
		<attr name="fdf_document" type="resource"></attr>
		<attr name="fieldname" type="string"></attr>
		<attr name="element" type="int"></attr>
		<attr name="str1" type="string"></attr>
		<attr name="str2" type="string"></attr>
	</tag>
	<tag name="fdf_set_status" type="function" returnType="bool">
		<attr name="fdf_document" type="resource"></attr>
		<attr name="status" type="string"></attr>
	</tag>
	<tag name="fdf_set_submit_form_action" type="function" returnType="bool">
		<attr name="fdf_document" type="resource"></attr>
		<attr name="fieldname" type="string"></attr>
		<attr name="trigger" type="int"></attr>
		<attr name="script" type="string"></attr>
		<attr name="flags" type="int"></attr>
	</tag>
	<tag name="fdf_set_target_frame" type="function" returnType="bool">
		<attr name="fdf_document" type="resource"></attr>
		<attr name="frame_name" type="string"></attr>
	</tag>
	<tag name="fdf_set_value" type="function" returnType="bool">
		<attr name="fdf_document" type="resource"></attr>
		<attr name="fieldname" type="string"></attr>
		<attr name="value" type="mixed"></attr>
		<attr name="isName" type="int"></attr>
	</tag>
	<tag name="fdf_set_version" type="function" returnType="string">
		<attr name="fdf_document" type="resource"></attr>
		<attr name="version" type="string"></attr>
	</tag>
</tags>