<!DOCTYPE tags>
<tags>	<tag name="OCIBindByName" type="function" returnType="int">
		<attr name="stmt" type="int" status="optional"></attr>
		<attr name="ph_name" type="string" status="optional"></attr>
		<attr name="& variable" type="mixed" status="optional"></attr>
		<attr name="length" type="int" status="optional"></attr>
		<attr name="type" type="int" status="optional"></attr>
	</tag>
	<tag name="OCICancel" type="function" returnType="int">
		<attr name="stmt" type="int" status="optional"></attr>
	</tag>
	<tag name="OCICollAppend" type="function" returnType="string">
		<attr name="collection" type="object" status="optional"></attr>
		<attr name="object" type="object" status="optional"></attr>
	</tag>
	<tag name="OCICollAssign" type="function" returnType="string">
		<attr name="collection" type="object" status="optional"></attr>
		<attr name="object" type="object" status="optional"></attr>
	</tag>
	<tag name="OCICollAssignElem" type="function" returnType="string">
		<attr name="collection" type="object" status="optional"></attr>
		<attr name="ndx" type="string" status="optional"></attr>
		<attr name="val" type="string" status="optional"></attr>
	</tag>
	<tag name="OCICollGetElem" type="function" returnType="string">
		<attr name="collection" type="object" status="optional"></attr>
		<attr name="ndx" type="string" status="optional"></attr>
	</tag>
	<tag name="OCICollMax" type="function" returnType="string">
		<attr name="collection" type="object" status="optional"></attr>
	</tag>
	<tag name="OCICollSize" type="function" returnType="string">
		<attr name="collection" type="object" status="optional"></attr>
	</tag>
	<tag name="OCICollTrim" type="function" returnType="string">
		<attr name="collection" type="object" status="optional"></attr>
		<attr name="num" type="int" status="optional"></attr>
	</tag>
	<tag name="OCIColumnIsNULL" type="function" returnType="int">
		<attr name="stmt" type="int" status="optional"></attr>
		<attr name="column" type="mixed" status="optional"></attr>
	</tag>
	<tag name="OCIColumnName" type="function" returnType="string">
		<attr name="stmt" type="int" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
	</tag>
	<tag name="OCIColumnPrecision" type="function" returnType="int">
		<attr name="stmt" type="int" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
	</tag>
	<tag name="OCIColumnScale" type="function" returnType="int">
		<attr name="stmt" type="int" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
	</tag>
	<tag name="OCIColumnSize" type="function" returnType="int">
		<attr name="stmt" type="int" status="optional"></attr>
		<attr name="column" type="mixed" status="optional"></attr>
	</tag>
	<tag name="OCIColumnType" type="function" returnType="mixed">
		<attr name="stmt" type="int" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
	</tag>
	<tag name="OCIColumnTypeRaw" type="function" returnType="mixed">
		<attr name="stmt" type="int" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
	</tag>
	<tag name="OCICommit" type="function" returnType="int">
		<attr name="connection" type="int" status="optional"></attr>
	</tag>
	<tag name="OCIDefineByName" type="function" returnType="int">
		<attr name="stmt" type="int" status="optional"></attr>
		<attr name="Column-Name" type="string" status="optional"></attr>
		<attr name="variable" type="mixed" status="optional"></attr>
		<attr name="type" type="int" status="optional"></attr>
	</tag>
	<tag name="OCIError" type="function" returnType="array">
		<attr name="stmt|conn|global" type="int" status="optional"></attr>
	</tag>
	<tag name="OCIExecute" type="function" returnType="int">
		<attr name="statement" type="int" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="OCIFetch" type="function" returnType="int">
		<attr name="statement" type="int" status="optional"></attr>
	</tag>
	<tag name="OCIFetchInto" type="function" returnType="int">
		<attr name="stmt" type="int" status="optional"></attr>
		<attr name="& result" type="array" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="OCIFetchStatement" type="function" returnType="int">
		<attr name="stmt" type="int" status="optional"></attr>
		<attr name="& variable" type="array" status="optional"></attr>
	</tag>
	<tag name="OCIFreeCollection" type="function" returnType="string">
		<attr name="lob" type="object" status="optional"></attr>
	</tag>
	<tag name="OCIFreeCursor" type="function" returnType="int">
		<attr name="stmt" type="int" status="optional"></attr>
	</tag>
	<tag name="OCIFreeDesc" type="function" returnType="int">
		<attr name="lob" type="object" status="optional"></attr>
	</tag>
	<tag name="OCIFreeStatement" type="function" returnType="int">
		<attr name="stmt" type="int" status="optional"></attr>
	</tag>
	<tag name="OCIInternalDebug" type="function" returnType="void">
		<attr name="onoff" type="int" status="optional"></attr>
	</tag>
	<tag name="OCILoadLob" type="function" returnType="string">
		<attr name="lob" type="object" status="optional"></attr>
	</tag>
	<tag name="OCILogOff" type="function" returnType="int">
		<attr name="connection" type="int" status="optional"></attr>
	</tag>
	<tag name="OCILogon" type="function" returnType="int">
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="db" type="string" status="optional"></attr>
	</tag>
	<tag name="OCINewCollection" type="function" returnType="string">
		<attr name="conn" type="int" status="optional"></attr>
		<attr name="tdo" type="string" status="optional"></attr>
		<attr name="shema" type="string" status="optional"></attr>
	</tag>
	<tag name="OCINewCursor" type="function" returnType="int">
		<attr name="conn" type="int" status="optional"></attr>
	</tag>
	<tag name="OCINewDescriptor" type="function" returnType="string">
		<attr name="connection" type="int" status="optional"></attr>
		<attr name="type" type="int" status="optional"></attr>
	</tag>
	<tag name="OCINLogon" type="function" returnType="int">
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="db" type="string" status="optional"></attr>
	</tag>
	<tag name="OCINumCols" type="function" returnType="int">
		<attr name="stmt" type="int" status="optional"></attr>
	</tag>
	<tag name="OCIParse" type="function" returnType="int">
		<attr name="conn" type="int" status="optional"></attr>
		<attr name="query" type="string" status="optional"></attr>
	</tag>
	<tag name="OCIPLogon" type="function" returnType="int">
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="db" type="string" status="optional"></attr>
	</tag>
	<tag name="OCIResult" type="function" returnType="mixed">
		<attr name="statement" type="int" status="optional"></attr>
		<attr name="column" type="mixed" status="optional"></attr>
	</tag>
	<tag name="OCIRollback" type="function" returnType="int">
		<attr name="connection" type="int" status="optional"></attr>
	</tag>
	<tag name="OCIRowCount" type="function" returnType="int">
		<attr name="statement" type="int" status="optional"></attr>
	</tag>
	<tag name="OCISaveLob" type="function" returnType="string">
		<attr name="lob" type="object" status="optional"></attr>
	</tag>
	<tag name="OCISaveLobFile" type="function" returnType="string">
		<attr name="lob" type="object" status="optional"></attr>
	</tag>
	<tag name="OCIServerVersion" type="function" returnType="string">
		<attr name="conn" type="int" status="optional"></attr>
	</tag>
	<tag name="OCISetPrefetch" type="function" returnType="int">
		<attr name="stmt" type="int" status="optional"></attr>
		<attr name="rows" type="int" status="optional"></attr>
	</tag>
	<tag name="OCIStatementType" type="function" returnType="string">
		<attr name="stmt" type="int" status="optional"></attr>
	</tag>
	<tag name="OCIWriteLobToFile" type="function" returnType="void">
		<attr name="lob" type="object" status="optional"></attr>
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="start" type="int" status="optional"></attr>
		<attr name="lenght" type="int" status="optional"></attr>
	</tag>
</tags>
