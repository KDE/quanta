<!DOCTYPE tags>
<tags>	<tag name="ora_bind" type="function" returnType="int">
		<attr name="cursor" type="int" status="optional"></attr>
		<attr name="PHP variable name" type="string" status="optional"></attr>
		<attr name="SQL parameter name" type="string" status="optional"></attr>
		<attr name="length" type="int" status="optional"></attr>
		<attr name="type" type="int" status="optional"></attr>
	</tag>
	<tag name="ora_close" type="function" returnType="int">
		<attr name="cursor" type="int" status="optional"></attr>
	</tag>
	<tag name="Ora_ColumnName" type="function" returnType="string">
		<attr name="cursor" type="int" status="optional"></attr>
		<attr name="column" type="int" status="optional"></attr>
	</tag>
	<tag name="Ora_ColumnSize" type="function" returnType="int">
		<attr name="cursor" type="int" status="optional"></attr>
		<attr name="column" type="int" status="optional"></attr>
	</tag>
	<tag name="Ora_ColumnType" type="function" returnType="string">
		<attr name="cursor" type="int" status="optional"></attr>
		<attr name="column" type="int" status="optional"></attr>
	</tag>
	<tag name="ora_commit" type="function" returnType="int">
		<attr name="conn" type="int" status="optional"></attr>
	</tag>
	<tag name="ora_commitoff" type="function" returnType="int">
		<attr name="conn" type="int" status="optional"></attr>
	</tag>
	<tag name="ora_commiton" type="function" returnType="int">
		<attr name="conn" type="int" status="optional"></attr>
	</tag>
	<tag name="ora_do" type="function" returnType="int">
		<attr name="conn" type="int" status="optional"></attr>
		<attr name="query" type="string" status="optional"></attr>
	</tag>
	<tag name="Ora_Error" type="function" returnType="string">
		<attr name="cursor_or_connection" type="int" status="optional"></attr>
	</tag>
	<tag name="Ora_ErrorCode" type="function" returnType="int">
		<attr name="cursor_or_connection" type="int" status="optional"></attr>
	</tag>
	<tag name="ora_exec" type="function" returnType="int">
		<attr name="cursor" type="int" status="optional"></attr>
	</tag>
	<tag name="ora_fetch_into" type="function" returnType="int">
		<attr name="cursor" type="int" status="optional"></attr>
		<attr name="result" type="array" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="ora_fetch" type="function" returnType="int">
		<attr name="cursor" type="int" status="optional"></attr>
	</tag>
	<tag name="ora_getcolumn" type="function" returnType="mixed">
		<attr name="cursor" type="int" status="optional"></attr>
		<attr name="column" type="mixed" status="optional"></attr>
	</tag>
	<tag name="ora_logoff" type="function" returnType="int">
		<attr name="connection" type="int" status="optional"></attr>
	</tag>
	<tag name="ora_logon" type="function" returnType="int">
		<attr name="user" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="ora_numcols" type="function" returnType="int">
		<attr name="cursor_ind" type="int" status="optional"></attr>
	</tag>
	<tag name="ora_numrows" type="function" returnType="int">
		<attr name="cursor_ind" type="int" status="optional"></attr>
	</tag>
	<tag name="ora_open" type="function" returnType="int">
		<attr name="connection" type="int" status="optional"></attr>
	</tag>
	<tag name="ora_parse" type="function" returnType="int">
		<attr name="cursor_ind" type="int" status="optional"></attr>
		<attr name="sql_statement" type="string" status="optional"></attr>
		<attr name="defer" type="int" status="optional"></attr>
	</tag>
	<tag name="ora_plogon" type="function" returnType="int">
		<attr name="user" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="ora_rollback" type="function" returnType="int">
		<attr name="connection" type="int" status="optional"></attr>
	</tag>
</tags>
