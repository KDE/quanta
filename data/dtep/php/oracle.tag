<!DOCTYPE tags>
<tags>
	<tag name="ora_bind" type="function" returnType="bool">
		<attr name="cursor" type="resource"></attr>
		<attr name="PHP_variable_name" type="string"></attr>
		<attr name="SQL_parameter_name" type="string"></attr>
		<attr name="length" type="int"></attr>
		<attr name="type" type="int" status="optional"></attr>
	</tag>
	<tag name="ora_close" type="function" returnType="bool">
		<attr name="cursor" type="resource"></attr>
	</tag>
	<tag name="ora_columnname" type="function" returnType="string">
		<attr name="cursor" type="resource"></attr>
		<attr name="column" type="int"></attr>
	</tag>
	<tag name="ora_columnsize" type="function" returnType="int">
		<attr name="cursor" type="resource"></attr>
		<attr name="column" type="int"></attr>
	</tag>
	<tag name="ora_columntype" type="function" returnType="string">
		<attr name="cursor" type="resource"></attr>
		<attr name="column" type="int"></attr>
	</tag>
	<tag name="ora_commit" type="function" returnType="bool">
		<attr name="conn" type="resource"></attr>
	</tag>
	<tag name="ora_commitoff" type="function" returnType="bool">
		<attr name="conn" type="resource"></attr>
	</tag>
	<tag name="ora_commiton" type="function" returnType="bool">
		<attr name="conn" type="resource"></attr>
	</tag>
	<tag name="ora_do" type="function" returnType="resource">
		<attr name="conn" type="resource"></attr>
		<attr name="query" type="string"></attr>
	</tag>
	<tag name="ora_error" type="function" returnType="string">
		<attr name="cursor_or_connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="ora_errorcode" type="function" returnType="int">
		<attr name="cursor_or_connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="ora_exec" type="function" returnType="bool">
		<attr name="cursor" type="resource"></attr>
	</tag>
	<tag name="ora_fetch_into" type="function" returnType="int">
		<attr name="cursor" type="resource"></attr>
		<attr name="&amp;result" type="array"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="ora_fetch" type="function" returnType="bool">
		<attr name="cursor" type="resource"></attr>
	</tag>
	<tag name="ora_getcolumn" type="function" returnType="mixed">
		<attr name="cursor" type="resource"></attr>
		<attr name="column" type="int"></attr>
	</tag>
	<tag name="ora_logoff" type="function" returnType="bool">
		<attr name="connection" type="resource"></attr>
	</tag>
	<tag name="ora_logon" type="function" returnType="resource">
		<attr name="user" type="string"></attr>
		<attr name="password" type="string"></attr>
	</tag>
	<tag name="ora_numcols" type="function" returnType="int">
		<attr name="cursor" type="resource"></attr>
	</tag>
	<tag name="ora_numrows" type="function" returnType="int">
		<attr name="cursor" type="resource"></attr>
	</tag>
	<tag name="ora_open" type="function" returnType="resource">
		<attr name="connection" type="resource"></attr>
	</tag>
	<tag name="ora_parse" type="function" returnType="bool">
		<attr name="cursor" type="resource"></attr>
		<attr name="sql_statement" type="string"></attr>
		<attr name="defer" type="int" status="optional"></attr>
	</tag>
	<tag name="ora_plogon" type="function" returnType="resource">
		<attr name="user" type="string"></attr>
		<attr name="password" type="string"></attr>
	</tag>
	<tag name="ora_rollback" type="function" returnType="bool">
		<attr name="connection" type="resource"></attr>
	</tag>
</tags>