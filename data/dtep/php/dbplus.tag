<!DOCTYPE tags>
<tags>
	<tag name="dbplus_add" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
		<attr name="tuple" type="array"></attr>
	</tag>
	<tag name="dbplus_aql" type="function" returnType="resource">
		<attr name="query" type="string"></attr>
		<attr name="server" type="string" status="optional"></attr>
		<attr name="dbpath" type="string" status="optional"></attr>
	</tag>
	<tag name="dbplus_chdir" type="function" returnType="string">
		<attr name="newdir" type="string" status="optional"></attr>
	</tag>
	<tag name="dbplus_close" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
	</tag>
	<tag name="dbplus_curr" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
		<attr name="&amp;tuple" type="array"></attr>
	</tag>
	<tag name="dbplus_errcode" type="function" returnType="string">
		<attr name="errno" type="int" status="optional"></attr>
	</tag>
	<tag name="dbplus_errno" type="function" returnType="int">
	</tag>
	<tag name="dbplus_find" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
		<attr name="constraints" type="array"></attr>
		<attr name="tuple" type="mixed"></attr>
	</tag>
	<tag name="dbplus_first" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
		<attr name="&amp;tuple" type="array"></attr>
	</tag>
	<tag name="dbplus_flush" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
	</tag>
	<tag name="dbplus_freealllocks" type="function" returnType="int">
	</tag>
	<tag name="dbplus_freelock" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
		<attr name="tname" type="string"></attr>
	</tag>
	<tag name="dbplus_freerlocks" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
	</tag>
	<tag name="dbplus_getlock" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
		<attr name="tname" type="string"></attr>
	</tag>
	<tag name="dbplus_getunique" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
		<attr name="uniqueid" type="int"></attr>
	</tag>
	<tag name="dbplus_info" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
		<attr name="key" type="string"></attr>
		<attr name="&amp;result" type="array"></attr>
	</tag>
	<tag name="dbplus_last" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
		<attr name="&amp;tuple" type="array"></attr>
	</tag>
	<tag name="dbplus_lockrel" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
	</tag>
	<tag name="dbplus_next" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
		<attr name="&amp;tuple" type="array"></attr>
	</tag>
	<tag name="dbplus_open" type="function" returnType="resource">
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="dbplus_prev" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
		<attr name="&amp;tuple" type="array"></attr>
	</tag>
	<tag name="dbplus_rchperm" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
		<attr name="mask" type="int"></attr>
		<attr name="user" type="string"></attr>
		<attr name="group" type="string"></attr>
	</tag>
	<tag name="dbplus_rcreate" type="function" returnType="resource">
		<attr name="name" type="string"></attr>
		<attr name="domlist" type="mixed"></attr>
		<attr name="overwrite" type="bool" status="optional"></attr>
	</tag>
	<tag name="dbplus_rcrtexact" type="function" returnType="resource">
		<attr name="name" type="string"></attr>
		<attr name="relation" type="resource"></attr>
		<attr name="overwrite" type="bool" status="optional"></attr>
	</tag>
	<tag name="dbplus_rcrtlike" type="function" returnType="resource">
		<attr name="name" type="string"></attr>
		<attr name="relation" type="resource"></attr>
		<attr name="overwrite" type="int" status="optional"></attr>
	</tag>
	<tag name="dbplus_resolve" type="function" returnType="int">
		<attr name="relation_name" type="string"></attr>
	</tag>
	<tag name="dbplus_restorepos" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
		<attr name="tuple" type="array"></attr>
	</tag>
	<tag name="dbplus_rkeys" type="function" returnType="resource">
		<attr name="relation" type="resource"></attr>
		<attr name="domlist" type="mixed"></attr>
	</tag>
	<tag name="dbplus_ropen" type="function" returnType="resource">
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="dbplus_rquery" type="function" returnType="int">
		<attr name="query" type="string"></attr>
		<attr name="dbpath" type="string" status="optional"></attr>
	</tag>
	<tag name="dbplus_rrename" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="dbplus_rsecindex" type="function" returnType="resource">
		<attr name="relation" type="resource"></attr>
		<attr name="domlist" type="mixed"></attr>
		<attr name="type" type="int"></attr>
	</tag>
	<tag name="dbplus_runlink" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
	</tag>
	<tag name="dbplus_rzap" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
	</tag>
	<tag name="dbplus_savepos" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
	</tag>
	<tag name="dbplus_setindex" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
		<attr name="idx_name" type="string"></attr>
	</tag>
	<tag name="dbplus_setindexbynumber" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
		<attr name="idx_number" type="int"></attr>
	</tag>
	<tag name="dbplus_sql" type="function" returnType="resource">
		<attr name="query" type="string"></attr>
		<attr name="server" type="string" status="optional"></attr>
		<attr name="dbpath" type="string" status="optional"></attr>
	</tag>
	<tag name="dbplus_tcl" type="function" returnType="int">
		<attr name="sid" type="int"></attr>
		<attr name="script" type="string"></attr>
	</tag>
	<tag name="dbplus_tremove" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
		<attr name="tuple" type="array"></attr>
		<attr name="&amp;current" type="array" status="optional"></attr>
	</tag>
	<tag name="dbplus_undo" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
	</tag>
	<tag name="dbplus_undoprepare" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
	</tag>
	<tag name="dbplus_unlockrel" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
	</tag>
	<tag name="dbplus_unselect" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
	</tag>
	<tag name="dbplus_update" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
		<attr name="old" type="array"></attr>
		<attr name="new" type="array"></attr>
	</tag>
	<tag name="dbplus_xlockrel" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
	</tag>
	<tag name="dbplus_xunlockrel" type="function" returnType="int">
		<attr name="relation" type="resource"></attr>
	</tag>
</tags>