<!DOCTYPE tags>
<tags>	<tag name="dbplus_add" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
		<attr name="tuple" type="array" status="optional"></attr>
	</tag>
	<tag name="dbplus_aql" type="function" returnType="resource">
		<attr name="query" type="string" status="optional"></attr>
		<attr name="server" type="string" status="optional"></attr>
		<attr name="dbpath" type="string" status="optional"></attr>
	</tag>
	<tag name="dbplus_chdir" type="function" returnType="string">
		<attr name="newdir" type="string" status="optional"></attr>
	</tag>
	<tag name="dbplus_close" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
	</tag>
	<tag name="dbplus_curr" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
		<attr name="tuple" type="array" status="optional"></attr>
	</tag>
	<tag name="dbplus_errcode" type="function" returnType="string">
		<attr name="errno" type="int" status="optional"></attr>
	</tag>
	<tag name="dbplus_errno" type="function" returnType="int">
	</tag>
	<tag name="dbplus_find" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
		<attr name="constraints" type="array" status="optional"></attr>
		<attr name="tuple" type="mixed" status="optional"></attr>
	</tag>
	<tag name="dbplus_first" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
		<attr name="tuple" type="array" status="optional"></attr>
	</tag>
	<tag name="dbplus_flush" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
	</tag>
	<tag name="dbplus_freealllocks" type="function" returnType="int">
	</tag>
	<tag name="dbplus_freelock" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
		<attr name="tname" type="string" status="optional"></attr>
	</tag>
	<tag name="dbplus_freerlocks" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
	</tag>
	<tag name="dbplus_getlock" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
		<attr name="tname" type="string" status="optional"></attr>
	</tag>
	<tag name="dbplus_getunique" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
		<attr name="uniqueid" type="int" status="optional"></attr>
	</tag>
	<tag name="dbplus_info" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
		<attr name="key" type="string" status="optional"></attr>
		<attr name="arrayN" type="array" status="optional"></attr>
	</tag>
	<tag name="dbplus_last" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
		<attr name="tuple" type="array" status="optional"></attr>
	</tag>
	<tag name="dbplus_lockrel" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
	</tag>
	<tag name="dbplus_next" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
		<attr name="arrayN" type="array" status="optional"></attr>
	</tag>
	<tag name="dbplus_open" type="function" returnType="resource">
		<attr name="name" type="string" status="optional"></attr>
	</tag>
	<tag name="dbplus_prev" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
		<attr name="tuple" type="array" status="optional"></attr>
	</tag>
	<tag name="dbplus_rchperm" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
		<attr name="mask" type="int" status="optional"></attr>
		<attr name="user" type="string" status="optional"></attr>
		<attr name="group" type="string" status="optional"></attr>
	</tag>
	<tag name="dbplus_rcreate" type="function" returnType="resource">
		<attr name="name" type="string" status="optional"></attr>
		<attr name="domlist" type="mixed" status="optional"></attr>
		<attr name="overwrite" type="boolean" status="optional"></attr>
	</tag>
	<tag name="dbplus_rcrtexact" type="function" returnType="resource">
		<attr name="name" type="string" status="optional"></attr>
		<attr name="relation" type="resource" status="optional"></attr>
		<attr name="overwrite" type="boolean" status="optional"></attr>
	</tag>
	<tag name="dbplus_rcrtlike" type="function" returnType="resource">
		<attr name="name" type="string" status="optional"></attr>
		<attr name="relation" type="resource" status="optional"></attr>
		<attr name="flag" type="int" status="optional"></attr>
	</tag>
	<tag name="dbplus_resolve" type="function" returnType="int">
		<attr name="relation_name" type="string" status="optional"></attr>
	</tag>
	<tag name="dbplus_restorepos" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
		<attr name="tuple" type="array" status="optional"></attr>
	</tag>
	<tag name="dbplus_rkeys" type="function" returnType="resource">
		<attr name="relation" type="resource" status="optional"></attr>
		<attr name="domlist" type="mixed" status="optional"></attr>
	</tag>
	<tag name="dbplus_ropen" type="function" returnType="resource">
		<attr name="name" type="string" status="optional"></attr>
	</tag>
	<tag name="dbplus_rquery" type="function" returnType="int">
		<attr name="query" type="string" status="optional"></attr>
		<attr name="dbpath" type="string" status="optional"></attr>
	</tag>
	<tag name="dbplus_rrename" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
		<attr name="name" type="string" status="optional"></attr>
	</tag>
	<tag name="dbplus_rsecindex" type="function" returnType="resource">
		<attr name="relation" type="resource" status="optional"></attr>
		<attr name="domlist" type="mixed" status="optional"></attr>
		<attr name="type" type="int" status="optional"></attr>
	</tag>
	<tag name="dbplus_runlink" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
	</tag>
	<tag name="dbplus_rzap" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
	</tag>
	<tag name="dbplus_savepos" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
	</tag>
	<tag name="dbplus_setindex" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
		<attr name="idx_name" type="string" status="optional"></attr>
	</tag>
	<tag name="dbplus_setindexbynumber" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
		<attr name="idx_number" type="int" status="optional"></attr>
	</tag>
	<tag name="dbplus_sql" type="function" returnType="resource">
		<attr name="query" type="string" status="optional"></attr>
		<attr name="server" type="string" status="optional"></attr>
		<attr name="dbpath" type="string" status="optional"></attr>
	</tag>
	<tag name="dbplus_tcl" type="function" returnType="int">
		<attr name="sid" type="int" status="optional"></attr>
		<attr name="script" type="string" status="optional"></attr>
	</tag>
	<tag name="dbplus_tremove" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
		<attr name="tuple" type="array" status="optional"></attr>
		<attr name="current" type="array" status="optional"></attr>
	</tag>
	<tag name="dbplus_undo" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
	</tag>
	<tag name="dbplus_undoprepare" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
	</tag>
	<tag name="dbplus_unlockrel" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
	</tag>
	<tag name="dbplus_unselect" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
	</tag>
	<tag name="dbplus_update" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
		<attr name="old" type="array" status="optional"></attr>
		<attr name="new" type="array" status="optional"></attr>
	</tag>
	<tag name="dbplus_xlockrel" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
	</tag>
	<tag name="dbplus_xunlockrel" type="function" returnType="int">
		<attr name="relation" type="resource" status="optional"></attr>
	</tag>
</tags>
