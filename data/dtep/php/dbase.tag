<!DOCTYPE tags>
<tags>
	<tag name="dbase_add_record" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="dbase_identifier" type="int" status="required"></attr>
		<attr name="record" type="array" status="required"></attr>
	</tag>
	<tag name="dbase_close" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="dbase_identifier" type="int" status="required"></attr>
	</tag>
	<tag name="dbase_create" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="fields" type="array" status="required"></attr>
	</tag>
	<tag name="dbase_delete_record" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="dbase_identifier" type="int" status="required"></attr>
		<attr name="record" type="int" status="required"></attr>
	</tag>
	<tag name="dbase_get_header_info" type="function" returnType="array" version="PHP 5">
		<attr name="dbase_identifier" type="int" status="required"></attr>
	</tag>
	<tag name="dbase_get_record_with_names" type="function" returnType="array" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="dbase_identifier" type="int" status="required"></attr>
		<attr name="record" type="int" status="required"></attr>
	</tag>
	<tag name="dbase_get_record" type="function" returnType="array" version="PHP 3, PHP 4 , PHP 5">
		<attr name="dbase_identifier" type="int" status="required"></attr>
		<attr name="record" type="int" status="required"></attr>
	</tag>
	<tag name="dbase_numfields" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="dbase_identifier" type="int" status="required"></attr>
	</tag>
	<tag name="dbase_numrecords" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="dbase_identifier" type="int" status="required"></attr>
	</tag>
	<tag name="dbase_open" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="flags" type="int" status="required"></attr>
	</tag>
	<tag name="dbase_pack" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="dbase_identifier" type="int" status="required"></attr>
	</tag>
	<tag name="dbase_replace_record" type="function" returnType="bool" version="PHP 3&gt;= 3.0.11, PHP 4 , PHP 5">
		<attr name="dbase_identifier" type="int" status="required"></attr>
		<attr name="record" type="array" status="required"></attr>
		<attr name="dbase_record_number" type="int" status="required"></attr>
	</tag>
</tags>