<!DOCTYPE tags>
<tags>
	<tag name="zip_close" type="function" returnType="void">
		<attr name="zip" type="resource"></attr>
	</tag>
	<tag name="zip_entry_close" type="function" returnType="void">
		<attr name="zip_entry" type="resource"></attr>
	</tag>
	<tag name="zip_entry_compressedsize" type="function" returnType="int">
		<attr name="zip_entry" type="resource"></attr>
	</tag>
	<tag name="zip_entry_compressionmethod" type="function" returnType="string">
		<attr name="zip_entry" type="resource"></attr>
	</tag>
	<tag name="zip_entry_filesize" type="function" returnType="int">
		<attr name="zip_entry" type="resource"></attr>
	</tag>
	<tag name="zip_entry_name" type="function" returnType="string">
		<attr name="zip_entry" type="resource"></attr>
	</tag>
	<tag name="zip_entry_open" type="function" returnType="bool">
		<attr name="zip" type="resource"></attr>
		<attr name="zip_entry" type="resource"></attr>
		<attr name="mode" type="string" status="optional"></attr>
	</tag>
	<tag name="zip_entry_read" type="function" returnType="string">
		<attr name="zip_entry" type="resource"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="zip_open" type="function" returnType="resource">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="zip_read" type="function" returnType="resource">
		<attr name="zip" type="resource"></attr>
	</tag>
</tags>