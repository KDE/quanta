<!DOCTYPE tags>
<tags>
	<tag name="chdir" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="directory" type="string" status="required"></attr>
	</tag>
	<tag name="chroot" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="directory" type="string" status="required"></attr>
	</tag>
	<tag name="read" type="function" returnType="string" version="4.0.2 - 4.0.6 only">
	</tag>
	<tag name="closedir" type="function" returnType="void" version="PHP 3, PHP 4 , PHP 5">
		<attr name="dir_handle" type="resource" status="required"></attr>
	</tag>
	<tag name="getcwd" type="function" returnType="string" version="PHP 4 , PHP 5">
	</tag>
	<tag name="opendir" type="function" returnType="resource" version="PHP 3, PHP 4 , PHP 5">
		<attr name="path" type="string" status="required"></attr>
	</tag>
	<tag name="readdir" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="dir_handle" type="resource" status="required"></attr>
	</tag>
	<tag name="rewinddir" type="function" returnType="void" version="PHP 3, PHP 4 , PHP 5">
		<attr name="dir_handle" type="resource" status="required"></attr>
	</tag>
	<tag name="scandir" type="function" returnType="array" version="PHP 5">
		<attr name="directory" type="string" status="required"></attr>
		<attr name="sorting_order" type="int" status="optional"></attr>
		<attr name="context" type="resource" status="optional"></attr>
	</tag>
</tags>