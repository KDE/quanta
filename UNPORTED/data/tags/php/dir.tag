<!DOCTYPE tags>
<tags>	<tag name="chdir" type="function" returnType="bool">
		<attr name="directory" type="string" status="optional"></attr>
	</tag>
	<tag name="chroot" type="function" returnType="bool">
		<attr name="directory" type="string" status="optional"></attr>
	</tag>
	<tag name="closedir" type="function" returnType="void">
		<attr name="dir_handle" type="resource" status="optional"></attr>
	</tag>
	<tag name="getcwd" type="function" returnType="string">
	</tag>
	<tag name="opendir" type="function" returnType="resource">
		<attr name="path" type="string" status="optional"></attr>
	</tag>
	<tag name="readdir" type="function" returnType="string">
		<attr name="dir_handle" type="resource" status="optional"></attr>
	</tag>
	<tag name="rewinddir" type="function" returnType="void">
		<attr name="dir_handle" type="resource" status="optional"></attr>
	</tag>
</tags>
