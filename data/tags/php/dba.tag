<!DOCTYPE tags>
<tags>	<tag name="dba_close" type="function" returnType="void">
		<attr name="handle" type="resource" status="optional"></attr>
	</tag>
	<tag name="dba_delete" type="function" returnType="bool">
		<attr name="key" type="string" status="optional"></attr>
		<attr name="handle" type="resource" status="optional"></attr>
	</tag>
	<tag name="dba_exists" type="function" returnType="bool">
		<attr name="key" type="string" status="optional"></attr>
		<attr name="handle" type="resource" status="optional"></attr>
	</tag>
	<tag name="dba_fetch" type="function" returnType="string">
		<attr name="key" type="string" status="optional"></attr>
		<attr name="handle" type="resource" status="optional"></attr>
	</tag>
	<tag name="dba_firstkey" type="function" returnType="string">
		<attr name="handle" type="resource" status="optional"></attr>
	</tag>
	<tag name="dba_insert" type="function" returnType="bool">
		<attr name="key" type="string" status="optional"></attr>
		<attr name="value" type="string" status="optional"></attr>
		<attr name="handle" type="resource" status="optional"></attr>
	</tag>
	<tag name="dba_nextkey" type="function" returnType="string">
		<attr name="handle" type="resource" status="optional"></attr>
	</tag>
	<tag name="dba_open" type="function" returnType="resource">
		<attr name="path" type="string" status="optional"></attr>
		<attr name="mode" type="string" status="optional"></attr>
		<attr name="handler" type="string" status="optional"></attr>
	</tag>
	<tag name="dba_optimize" type="function" returnType="bool">
		<attr name="handle" type="resource" status="optional"></attr>
	</tag>
	<tag name="dba_popen" type="function" returnType="resource">
		<attr name="path" type="string" status="optional"></attr>
		<attr name="mode" type="string" status="optional"></attr>
		<attr name="handler" type="string" status="optional"></attr>
	</tag>
	<tag name="dba_replace" type="function" returnType="bool">
		<attr name="key" type="string" status="optional"></attr>
		<attr name="value" type="string" status="optional"></attr>
		<attr name="handle" type="resource" status="optional"></attr>
	</tag>
	<tag name="dba_sync" type="function" returnType="bool">
		<attr name="handle" type="resource" status="optional"></attr>
	</tag>
</tags>
