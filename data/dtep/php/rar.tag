<!DOCTYPE tags>
<tags>
	<tag name="rar_close" type="function" returnType="bool">
		<attr name="rar_file" type="resource"></attr>
	</tag>
	<tag name="rar_entry_get" type="function" returnType="object">
		<attr name="rar_file" type="resource"></attr>
		<attr name="entry_name" type="string"></attr>
	</tag>
	<tag name="Rar::extract" type="function" returnType="bool">
		<attr name="dir" type="string"></attr>
		<attr name="filepath" type="string" status="optional"></attr>
	</tag>
	<tag name="Rar::getAttr" type="function" returnType="int">
	</tag>
	<tag name="Rar::getCrc" type="function" returnType="int">
	</tag>
	<tag name="Rar::getFileTime" type="function" returnType="string">
	</tag>
	<tag name="Rar::getHostOs" type="function" returnType="int">
	</tag>
	<tag name="Rar::getMethod" type="function" returnType="int">
	</tag>
	<tag name="Rar::getName" type="function" returnType="string">
	</tag>
	<tag name="Rar::getPackedSize" type="function" returnType="int">
	</tag>
	<tag name="Rar::getUnpackedSize" type="function" returnType="int">
	</tag>
	<tag name="Rar::getVersion" type="function" returnType="int">
	</tag>
	<tag name="rar_list" type="function" returnType="array">
		<attr name="rar_file" type="resource"></attr>
	</tag>
	<tag name="rar_open" type="function" returnType="resource">
		<attr name="filename" type="string"></attr>
	</tag>
</tags>