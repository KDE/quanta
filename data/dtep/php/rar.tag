<!DOCTYPE tags>
<tags>
	<tag name="rar_close" type="function" returnType="bool" comment="">
		<attr name="rar_file" type="resource" status="required"></attr>
	</tag>
	<tag name="rar_entry_get" type="function" returnType="object" comment="">
		<attr name="rar_file" type="resource" status="required"></attr>
		<attr name="entry_name" type="string" status="required"></attr>
	</tag>
	<tag name="Rar::extract" type="function" returnType="bool" comment="">
		<attr name="dir" type="string" status="required"></attr>
		<attr name="filepath" type="string" status="optional"></attr>
	</tag>
	<tag name="Rar::getAttr" type="function" returnType="int" comment="">
	</tag>
	<tag name="Rar::getCrc" type="function" returnType="int" comment="">
	</tag>
	<tag name="Rar::getFileTime" type="function" returnType="string" comment="">
	</tag>
	<tag name="Rar::getHostOs" type="function" returnType="int" comment="">
	</tag>
	<tag name="Rar::getMethod" type="function" returnType="int" comment="">
	</tag>
	<tag name="Rar::getName" type="function" returnType="string" comment="">
	</tag>
	<tag name="Rar::getPackedSize" type="function" returnType="int" comment="">
	</tag>
	<tag name="Rar::getUnpackedSize" type="function" returnType="int" comment="">
	</tag>
	<tag name="Rar::getVersion" type="function" returnType="int" comment="">
	</tag>
	<tag name="rar_list" type="function" returnType="array" comment="">
		<attr name="rar_file" type="resource" status="required"></attr>
	</tag>
	<tag name="rar_open" type="function" returnType="resource" comment="">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
</tags>