<!DOCTYPE tags>
<tags>
	<tag name="dba_close" type="function" returnType="void" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="dba_delete" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="key" type="string" status="required"></attr>
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="dba_exists" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="key" type="string" status="required"></attr>
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="dba_fetch" type="function" returnType="string" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="key" type="string" status="required"></attr>
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="dba_firstkey" type="function" returnType="string" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="dba_handlers" type="function" returnType="array" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="full_info" type="bool" status="optional"></attr>
	</tag>
	<tag name="dba_insert" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="key" type="string" status="required"></attr>
		<attr name="value" type="string" status="required"></attr>
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="dba_key_split" type="function" returnType="mixed" version="PHP 5">
		<attr name="key" type="mixed" status="required"></attr>
	</tag>
	<tag name="dba_list" type="function" returnType="array" version="PHP 4 &gt;= 4.3.0, PHP 5">
	</tag>
	<tag name="dba_nextkey" type="function" returnType="string" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="dba_open" type="function" returnType="resource" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="path" type="string" status="required"></attr>
		<attr name="mode" type="string" status="required"></attr>
		<attr name="handler" type="string" status="required"></attr>
		<attr name="..." type="" status="optional"></attr>
	</tag>
	<tag name="dba_optimize" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="dba_popen" type="function" returnType="resource" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="path" type="string" status="required"></attr>
		<attr name="mode" type="string" status="required"></attr>
		<attr name="handler" type="string" status="required"></attr>
		<attr name="..." type="" status="optional"></attr>
	</tag>
	<tag name="dba_replace" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="key" type="string" status="required"></attr>
		<attr name="value" type="string" status="required"></attr>
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="dba_sync" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
</tags>