<!DOCTYPE tags>
<tags>	<tag name="fdf_add_template" type="function" returnType="bool">
		<attr name="fdfdoc" type="int" status="optional"></attr>
		<attr name="newpage" type="int" status="optional"></attr>
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="template" type="string" status="optional"></attr>
		<attr name="rename" type="int" status="optional"></attr>
	</tag>
	<tag name="fdf_close" type="function" returnType="bool">
		<attr name="fdf_document" type="int" status="optional"></attr>
	</tag>
	<tag name="fdf_create" type="function" returnType="int">
	</tag>
	<tag name="fdf_get_file" type="function" returnType="string">
		<attr name="fdf_document" type="int" status="optional"></attr>
	</tag>
	<tag name="fdf_get_status" type="function" returnType="string">
		<attr name="fdf_document" type="int" status="optional"></attr>
	</tag>
	<tag name="fdf_get_value" type="function" returnType="string">
		<attr name="fdf_document" type="int" status="optional"></attr>
		<attr name="fieldname" type="string" status="optional"></attr>
	</tag>
	<tag name="fdf_next_field_name" type="function" returnType="string">
		<attr name="fdf_document" type="int" status="optional"></attr>
		<attr name="fieldname" type="string" status="optional"></attr>
	</tag>
	<tag name="fdf_open" type="function" returnType="int">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="fdf_save" type="function" returnType="int">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="fdf_set_ap" type="function" returnType="bool">
		<attr name="fdf_document" type="int" status="optional"></attr>
		<attr name="field_name" type="string" status="optional"></attr>
		<attr name="face" type="int" status="optional"></attr>
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="page_number" type="int" status="optional"></attr>
	</tag>
	<tag name="fdf_set_encoding" type="function" returnType="bool">
		<attr name="fdf_document" type="int" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="fdf_set_file" type="function" returnType="bool">
		<attr name="fdf_document" type="int" status="optional"></attr>
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="fdf_set_flags" type="function" returnType="bool">
		<attr name="fdf_document" type="int" status="optional"></attr>
		<attr name="fieldname" type="string" status="optional"></attr>
		<attr name="whichFlags" type="int" status="optional"></attr>
		<attr name="newFlags" type="int" status="optional"></attr>
	</tag>
	<tag name="fdf_set_javascript_action" type="function" returnType="bool">
		<attr name="fdf_document" type="int" status="optional"></attr>
		<attr name="fieldname" type="string" status="optional"></attr>
		<attr name="trigger" type="int" status="optional"></attr>
		<attr name="script" type="string" status="optional"></attr>
	</tag>
	<tag name="fdf_set_opt" type="function" returnType="bool">
		<attr name="fdf_document" type="int" status="optional"></attr>
		<attr name="fieldname" type="string" status="optional"></attr>
		<attr name="element" type="int" status="optional"></attr>
		<attr name="str1" type="string" status="optional"></attr>
		<attr name="str2" type="string" status="optional"></attr>
	</tag>
	<tag name="fdf_set_status" type="function" returnType="bool">
		<attr name="fdf_document" type="int" status="optional"></attr>
		<attr name="status" type="string" status="optional"></attr>
	</tag>
	<tag name="fdf_set_submit_form_action" type="function" returnType="bool">
		<attr name="fdf_document" type="int" status="optional"></attr>
		<attr name="fieldname" type="string" status="optional"></attr>
		<attr name="trigger" type="int" status="optional"></attr>
		<attr name="script" type="string" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="fdf_set_value" type="function" returnType="bool">
		<attr name="fdf_document" type="int" status="optional"></attr>
		<attr name="fieldname" type="string" status="optional"></attr>
		<attr name="value" type="string" status="optional"></attr>
		<attr name="isName" type="int" status="optional"></attr>
	</tag>
</tags>
