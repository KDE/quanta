<!DOCTYPE tags>
<tags>
	<tag name="dblist" type="function" returnType="string" version="PHP 3, PHP 4 ">
	</tag>
	<tag name="dbmclose" type="function" returnType="bool" version="PHP 3, PHP 4 ">
		<attr name="dbm_identifier" type="resource" status="required"></attr>
	</tag>
	<tag name="dbmdelete" type="function" returnType="bool" version="PHP 3, PHP 4 ">
		<attr name="dbm_identifier" type="resource" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
	</tag>
	<tag name="dbmexists" type="function" returnType="bool" version="PHP 3, PHP 4 ">
		<attr name="dbm_identifier" type="resource" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
	</tag>
	<tag name="dbmfetch" type="function" returnType="string" version="PHP 3, PHP 4 ">
		<attr name="dbm_identifier" type="resource" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
	</tag>
	<tag name="dbmfirstkey" type="function" returnType="string" version="PHP 3, PHP 4 ">
		<attr name="dbm_identifier" type="resource" status="required"></attr>
	</tag>
	<tag name="dbminsert" type="function" returnType="int" version="PHP 3, PHP 4 ">
		<attr name="dbm_identifier" type="resource" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
		<attr name="value" type="string" status="required"></attr>
	</tag>
	<tag name="dbmnextkey" type="function" returnType="string" version="PHP 3, PHP 4 ">
		<attr name="dbm_identifier" type="resource" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
	</tag>
	<tag name="dbmopen" type="function" returnType="resource" version="PHP 3, PHP 4 ">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="flags" type="string" status="required"></attr>
	</tag>
	<tag name="dbmreplace" type="function" returnType="int" version="PHP 3, PHP 4 ">
		<attr name="dbm_identifier" type="resource" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
		<attr name="value" type="string" status="required"></attr>
	</tag>
</tags>