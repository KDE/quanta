<!DOCTYPE tags>
<tags>
	<tag name="fdf_add_doc_javascript" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="fdfdoc" type="resource" status="required"></attr>
		<attr name="script_name" type="string" status="required"></attr>
		<attr name="script_code" type="string" status="required"></attr>
	</tag>
	<tag name="fdf_add_template" type="function" returnType="bool" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="fdfdoc" type="resource" status="required"></attr>
		<attr name="newpage" type="int" status="required"></attr>
		<attr name="filename" type="string" status="required"></attr>
		<attr name="template" type="string" status="required"></attr>
		<attr name="rename" type="int" status="required"></attr>
	</tag>
	<tag name="fdf_close" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="fdf_document" type="resource" status="required"></attr>
	</tag>
	<tag name="fdf_create" type="function" returnType="resource" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
	</tag>
	<tag name="fdf_enum_values" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="fdfdoc" type="resource" status="required"></attr>
		<attr name="function" type="callback" status="required"></attr>
		<attr name="userdata" type="mixed" status="required"></attr>
	</tag>
	<tag name="fdf_errno" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
	</tag>
	<tag name="fdf_error" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="error_code" type="int"></attr>
	</tag>
	<tag name="fdf_get_ap" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="fdf_document" type="resource" status="required"></attr>
		<attr name="field" type="string" status="required"></attr>
		<attr name="face" type="int" status="required"></attr>
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="fdf_get_attachment" type="function" returnType="array" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="fdf_document" type="resource" status="required"></attr>
		<attr name="fieldname" type="string" status="required"></attr>
		<attr name="savepath" type="string" status="required"></attr>
	</tag>
	<tag name="fdf_get_encoding" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="fdf_document" type="resource" status="required"></attr>
	</tag>
	<tag name="fdf_get_file" type="function" returnType="string" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="fdf_document" type="resource" status="required"></attr>
	</tag>
	<tag name="fdf_get_flags" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="fdfdoc" type="resource" status="required"></attr>
		<attr name="fieldname" type="string" status="required"></attr>
		<attr name="whichflags" type="int" status="required"></attr>
	</tag>
	<tag name="fdf_get_opt" type="function" returnType="mixed" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="fdfdof" type="resource" status="required"></attr>
		<attr name="fieldname" type="string" status="required"></attr>
		<attr name="element" type="int" status="required"></attr>
	</tag>
	<tag name="fdf_get_status" type="function" returnType="string" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="fdf_document" type="resource" status="required"></attr>
	</tag>
	<tag name="fdf_get_value" type="function" returnType="string" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="fdf_document" type="resource" status="required"></attr>
		<attr name="fieldname" type="string" status="required"></attr>
		<attr name="which" type="int" status="required"></attr>
	</tag>
	<tag name="fdf_get_version" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="fdf_document" type="resource"></attr>
	</tag>
	<tag name="fdf_header" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
	</tag>
	<tag name="fdf_next_field_name" type="function" returnType="string" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="fdf_document" type="resource" status="required"></attr>
		<attr name="fieldname" type="string" status="optional"></attr>
	</tag>
	<tag name="fdf_open_string" type="function" returnType="resource" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="fdf_data" type="string" status="required"></attr>
	</tag>
	<tag name="fdf_open" type="function" returnType="resource" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="fdf_remove_item" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="fdfdoc" type="resource" status="required"></attr>
		<attr name="fieldname" type="string" status="required"></attr>
		<attr name="item" type="int" status="required"></attr>
	</tag>
	<tag name="fdf_save_string" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="fdf_document" type="resource" status="required"></attr>
	</tag>
	<tag name="fdf_save" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="fdf_document" type="resource" status="required"></attr>
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="fdf_set_ap" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="fdf_document" type="resource" status="required"></attr>
		<attr name="field_name" type="string" status="required"></attr>
		<attr name="face" type="int" status="required"></attr>
		<attr name="filename" type="string" status="required"></attr>
		<attr name="page_number" type="int" status="required"></attr>
	</tag>
	<tag name="fdf_set_encoding" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="fdf_document" type="resource" status="required"></attr>
		<attr name="encoding" type="string" status="required"></attr>
	</tag>
	<tag name="fdf_set_file" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="fdf_document" type="resource" status="required"></attr>
		<attr name="url" type="string" status="required"></attr>
		<attr name="target_frame" type="string" status="required"></attr>
	</tag>
	<tag name="fdf_set_flags" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="fdf_document" type="resource" status="required"></attr>
		<attr name="fieldname" type="string" status="required"></attr>
		<attr name="whichFlags" type="int" status="required"></attr>
		<attr name="newFlags" type="int" status="required"></attr>
	</tag>
	<tag name="fdf_set_javascript_action" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="fdf_document" type="resource" status="required"></attr>
		<attr name="fieldname" type="string" status="required"></attr>
		<attr name="trigger" type="int" status="required"></attr>
		<attr name="script" type="string" status="required"></attr>
	</tag>
	<tag name="fdf_set_on_import_javascript" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="fdfdoc" type="resource" status="required"></attr>
		<attr name="script" type="string" status="required"></attr>
		<attr name="before_data_import" type="bool" status="required"></attr>
	</tag>
	<tag name="fdf_set_opt" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="fdf_document" type="resource" status="required"></attr>
		<attr name="fieldname" type="string" status="required"></attr>
		<attr name="element" type="int" status="required"></attr>
		<attr name="str1" type="string" status="required"></attr>
		<attr name="str2" type="string" status="required"></attr>
	</tag>
	<tag name="fdf_set_status" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="fdf_document" type="resource" status="required"></attr>
		<attr name="status" type="string" status="required"></attr>
	</tag>
	<tag name="fdf_set_submit_form_action" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="fdf_document" type="resource" status="required"></attr>
		<attr name="fieldname" type="string" status="required"></attr>
		<attr name="trigger" type="int" status="required"></attr>
		<attr name="script" type="string" status="required"></attr>
		<attr name="flags" type="int" status="required"></attr>
	</tag>
	<tag name="fdf_set_target_frame" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="fdf_document" type="resource" status="required"></attr>
		<attr name="frame_name" type="string" status="required"></attr>
	</tag>
	<tag name="fdf_set_value" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="fdf_document" type="resource" status="required"></attr>
		<attr name="fieldname" type="string" status="required"></attr>
		<attr name="value" type="mixed" status="required"></attr>
		<attr name="isName" type="int" status="required"></attr>
	</tag>
	<tag name="fdf_set_version" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="fdf_document" type="resource" status="required"></attr>
		<attr name="version" type="string" status="required"></attr>
	</tag>
</tags>