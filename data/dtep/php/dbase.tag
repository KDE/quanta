<!DOCTYPE tags>
<tags>
	<tag name="dbase_add_record" type="function" returnType="bool">
		<attr name="dbase_identifier" type="int"></attr>
		<attr name="record" type="array"></attr>
	</tag>
	<tag name="dbase_close" type="function" returnType="bool">
		<attr name="dbase_identifier" type="int"></attr>
	</tag>
	<tag name="dbase_create" type="function" returnType="int">
		<attr name="filename" type="string"></attr>
		<attr name="fields" type="array"></attr>
	</tag>
	<tag name="dbase_delete_record" type="function" returnType="bool">
		<attr name="dbase_identifier" type="int"></attr>
		<attr name="record" type="int"></attr>
	</tag>
	<tag name="dbase_get_header_info" type="function" returnType="array">
		<attr name="dbase_identifier" type="int"></attr>
	</tag>
	<tag name="dbase_get_record_with_names" type="function" returnType="array">
		<attr name="dbase_identifier" type="int"></attr>
		<attr name="record" type="int"></attr>
	</tag>
	<tag name="dbase_get_record" type="function" returnType="array">
		<attr name="dbase_identifier" type="int"></attr>
		<attr name="record" type="int"></attr>
	</tag>
	<tag name="dbase_numfields" type="function" returnType="int">
		<attr name="dbase_identifier" type="int"></attr>
	</tag>
	<tag name="dbase_numrecords" type="function" returnType="int">
		<attr name="dbase_identifier" type="int"></attr>
	</tag>
	<tag name="dbase_open" type="function" returnType="int">
		<attr name="filename" type="string"></attr>
		<attr name="flags" type="int"></attr>
	</tag>
	<tag name="dbase_pack" type="function" returnType="bool">
		<attr name="dbase_identifier" type="int"></attr>
	</tag>
	<tag name="dbase_replace_record" type="function" returnType="bool">
		<attr name="dbase_identifier" type="int"></attr>
		<attr name="record" type="array"></attr>
		<attr name="dbase_record_number" type="int"></attr>
	</tag>
</tags>