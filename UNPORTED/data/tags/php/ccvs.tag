<!DOCTYPE tags>
<tags>	<tag name="ccvs_add" type="function" returnType="string">
		<attr name="session" type="string" status="optional"></attr>
		<attr name="invoice" type="string" status="optional"></attr>
		<attr name="argtype" type="string" status="optional"></attr>
		<attr name="argval" type="string" status="optional"></attr>
	</tag>
	<tag name="ccvs_auth" type="function" returnType="string">
		<attr name="session" type="string" status="optional"></attr>
		<attr name="invoice" type="string" status="optional"></attr>
	</tag>
	<tag name="ccvs_command" type="function" returnType="string">
		<attr name="session" type="string" status="optional"></attr>
		<attr name="type" type="string" status="optional"></attr>
		<attr name="argval" type="string" status="optional"></attr>
	</tag>
	<tag name="ccvs_count" type="function" returnType="int">
		<attr name="session" type="string" status="optional"></attr>
		<attr name="type" type="string" status="optional"></attr>
	</tag>
	<tag name="ccvs_delete" type="function" returnType="string">
		<attr name="session" type="string" status="optional"></attr>
		<attr name="invoice" type="string" status="optional"></attr>
	</tag>
	<tag name="ccvs_done" type="function" returnType="string">
		<attr name="sess" type="string" status="optional"></attr>
	</tag>
	<tag name="ccvs_init" type="function" returnType="string">
		<attr name="name" type="string" status="optional"></attr>
	</tag>
	<tag name="ccvs_lookup" type="function" returnType="string">
		<attr name="session" type="string" status="optional"></attr>
		<attr name="invoice" type="string" status="optional"></attr>
		<attr name="inum" type="int" status="optional"></attr>
	</tag>
	<tag name="ccvs_new" type="function" returnType="string">
		<attr name="session" type="string" status="optional"></attr>
		<attr name="invoice" type="string" status="optional"></attr>
	</tag>
	<tag name="ccvs_report" type="function" returnType="string">
		<attr name="session" type="string" status="optional"></attr>
		<attr name="type" type="string" status="optional"></attr>
	</tag>
	<tag name="ccvs_return" type="function" returnType="string">
		<attr name="session" type="string" status="optional"></attr>
		<attr name="invoice" type="string" status="optional"></attr>
	</tag>
	<tag name="ccvs_reverse" type="function" returnType="string">
		<attr name="session" type="string" status="optional"></attr>
		<attr name="invoice" type="string" status="optional"></attr>
	</tag>
	<tag name="ccvs_sale" type="function" returnType="string">
		<attr name="session" type="string" status="optional"></attr>
		<attr name="invoice" type="string" status="optional"></attr>
	</tag>
	<tag name="ccvs_status" type="function" returnType="string">
		<attr name="session" type="string" status="optional"></attr>
		<attr name="invoice" type="string" status="optional"></attr>
	</tag>
	<tag name="ccvs_textvalue" type="function" returnType="string">
		<attr name="session" type="string" status="optional"></attr>
	</tag>
	<tag name="ccvs_void" type="function" returnType="string">
		<attr name="session" type="string" status="optional"></attr>
		<attr name="invoice" type="string" status="optional"></attr>
	</tag>
</tags>
