<!DOCTYPE tags>
<tags>	<tag name="dbase_add_record" type="function" returnType="bool">
		<attr name="dbase_identifier" type="int" status="optional"></attr>
		<attr name="record" type="array" status="optional"></attr>
	</tag>
	<tag name="dbase_close" type="function" returnType="bool">
		<attr name="dbase_identifier" type="int" status="optional"></attr>
	</tag>
	<tag name="dbase_create" type="function" returnType="int">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="fields" type="array" status="optional"></attr>
	</tag>
	<tag name="dbase_delete_record" type="function" returnType="bool">
		<attr name="dbase_identifier" type="int" status="optional"></attr>
		<attr name="record" type="int" status="optional"></attr>
	</tag>
	<tag name="dbase_get_record_with_names" type="function" returnType="array">
		<attr name="dbase_identifier" type="int" status="optional"></attr>
		<attr name="record" type="int" status="optional"></attr>
	</tag>
	<tag name="dbase_get_record" type="function" returnType="array">
		<attr name="dbase_identifier" type="int" status="optional"></attr>
		<attr name="record" type="int" status="optional"></attr>
	</tag>
	<tag name="dbase_numfields" type="function" returnType="int">
		<attr name="dbase_identifier" type="int" status="optional"></attr>
	</tag>
	<tag name="dbase_numrecords" type="function" returnType="int">
		<attr name="dbase_identifier" type="int" status="optional"></attr>
	</tag>
	<tag name="dbase_open" type="function" returnType="int">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="dbase_pack" type="function" returnType="bool">
		<attr name="dbase_identifier" type="int" status="optional"></attr>
	</tag>
	<tag name="dbase_replace_record" type="function" returnType="bool">
		<attr name="dbase_identifier" type="int" status="optional"></attr>
		<attr name="record" type="array" status="optional"></attr>
		<attr name="dbase_record_number" type="int" status="optional"></attr>
	</tag>
</tags>
