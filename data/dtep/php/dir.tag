<!DOCTYPE tags>
<tags>
	<tag name="chdir" type="function" returnType="bool" langVersion="PHP 3, PHP 4 , PHP 5">
		<attr name="directory" type="string" status="required"></attr>
	</tag>
	<tag name="chroot" type="function" returnType="bool" langVersion="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="directory" type="string" status="required"></attr>
	</tag>
	<tag name="read" type="function" returnType="string" langVersion="4.0.2 - 4.0.6 only">
	</tag>
	<tag name="closedir" type="function" returnType="void" langVersion="PHP 3, PHP 4 , PHP 5">
		<attr name="dir_handle" type="resource" status="required"></attr>
	</tag>
	<tag name="getcwd" type="function" returnType="string" langVersion="PHP 4 , PHP 5">
	</tag>
	<tag name="opendir" type="function" returnType="resource" langVersion="PHP 3, PHP 4 , PHP 5">
		<attr name="path" type="string" status="required"></attr>
	</tag>
	<tag name="readdir" type="function" returnType="string" langVersion="PHP 3, PHP 4 , PHP 5">
		<attr name="dir_handle" type="resource" status="required"></attr>
	</tag>
	<tag name="rewinddir" type="function" returnType="void" langVersion="PHP 3, PHP 4 , PHP 5">
		<attr name="dir_handle" type="resource" status="required"></attr>
	</tag>
	<tag name="scandir" type="function" returnType="array" langVersion="PHP 5">
		<attr name="directory" type="string" status="required"></attr>
		<attr name="sorting_order" type="int" status="optional"></attr>
		<attr name="context" type="resource" status="optional"></attr>
	</tag>
</tags>