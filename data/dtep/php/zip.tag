<!DOCTYPE tags>
<tags>
	<tag name="zip_close" type="function" returnType="void" version="PHP 4 &gt;= 4.1.0">
		<attr name="zip" type="resource" status="required"></attr>
	</tag>
	<tag name="zip_entry_close" type="function" returnType="void" version="PHP 4 &gt;= 4.1.0">
		<attr name="zip_entry" type="resource" status="required"></attr>
	</tag>
	<tag name="zip_entry_compressedsize" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0">
		<attr name="zip_entry" type="resource" status="required"></attr>
	</tag>
	<tag name="zip_entry_compressionmethod" type="function" returnType="string" version="PHP 4 &gt;= 4.1.0">
		<attr name="zip_entry" type="resource" status="required"></attr>
	</tag>
	<tag name="zip_entry_filesize" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0">
		<attr name="zip_entry" type="resource" status="required"></attr>
	</tag>
	<tag name="zip_entry_name" type="function" returnType="string" version="PHP 4 &gt;= 4.1.0">
		<attr name="zip_entry" type="resource" status="required"></attr>
	</tag>
	<tag name="zip_entry_open" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0">
		<attr name="zip" type="resource" status="required"></attr>
		<attr name="zip_entry" type="resource" status="required"></attr>
		<attr name="mode" type="string" status="optional"></attr>
	</tag>
	<tag name="zip_entry_read" type="function" returnType="string" version="PHP 4 &gt;= 4.1.0">
		<attr name="zip_entry" type="resource" status="required"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="zip_open" type="function" returnType="resource" version="PHP 4 &gt;= 4.1.0">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="zip_read" type="function" returnType="resource" version="PHP 4 &gt;= 4.1.0">
		<attr name="zip" type="resource" status="required"></attr>
	</tag>
</tags>