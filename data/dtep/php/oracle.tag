<!DOCTYPE tags>
<tags>
	<tag name="ora_bind" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="cursor" type="resource" status="required"></attr>
		<attr name="PHP_variable_name" type="string" status="required"></attr>
		<attr name="SQL_parameter_name" type="string" status="required"></attr>
		<attr name="length" type="int" status="required"></attr>
		<attr name="type" type="int" status="optional"></attr>
	</tag>
	<tag name="ora_close" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="cursor" type="resource" status="required"></attr>
	</tag>
	<tag name="ora_columnname" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="cursor" type="resource" status="required"></attr>
		<attr name="column" type="int" status="required"></attr>
	</tag>
	<tag name="ora_columnsize" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="cursor" type="resource" status="required"></attr>
		<attr name="column" type="int" status="required"></attr>
	</tag>
	<tag name="ora_columntype" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="cursor" type="resource" status="required"></attr>
		<attr name="column" type="int" status="required"></attr>
	</tag>
	<tag name="ora_commit" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
	</tag>
	<tag name="ora_commitoff" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
	</tag>
	<tag name="ora_commiton" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
	</tag>
	<tag name="ora_do" type="function" returnType="resource" version="PHP 3, PHP 4 , PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
	</tag>
	<tag name="ora_error" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="cursor_or_connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="ora_errorcode" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="cursor_or_connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="ora_exec" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="cursor" type="resource" status="required"></attr>
	</tag>
	<tag name="ora_fetch_into" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="cursor" type="resource" status="required"></attr>
		<attr name="&amp;result" type="array" status="required"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="ora_fetch" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="cursor" type="resource" status="required"></attr>
	</tag>
	<tag name="ora_getcolumn" type="function" returnType="mixed" version="PHP 3, PHP 4 , PHP 5">
		<attr name="cursor" type="resource" status="required"></attr>
		<attr name="column" type="int" status="required"></attr>
	</tag>
	<tag name="ora_logoff" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
	</tag>
	<tag name="ora_logon" type="function" returnType="resource" version="PHP 3, PHP 4 , PHP 5">
		<attr name="user" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
	</tag>
	<tag name="ora_numcols" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="cursor" type="resource" status="required"></attr>
	</tag>
	<tag name="ora_numrows" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="cursor" type="resource" status="required"></attr>
	</tag>
	<tag name="ora_open" type="function" returnType="resource" version="PHP 3, PHP 4 , PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
	</tag>
	<tag name="ora_parse" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="cursor" type="resource" status="required"></attr>
		<attr name="sql_statement" type="string" status="required"></attr>
		<attr name="defer" type="int" status="optional"></attr>
	</tag>
	<tag name="ora_plogon" type="function" returnType="resource" version="PHP 3, PHP 4 , PHP 5">
		<attr name="user" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
	</tag>
	<tag name="ora_rollback" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
	</tag>
</tags>