<!DOCTYPE tags>
<tags>
	<tag name="ccvs_add" type="function" returnType="string" version="4.0.2 - 4.2.3 only">
		<attr name="session" type="string" status="required"></attr>
		<attr name="invoice" type="string" status="required"></attr>
		<attr name="argtype" type="string" status="required"></attr>
		<attr name="argval" type="string" status="required"></attr>
	</tag>
	<tag name="ccvs_auth" type="function" returnType="string" version="4.0.2 - 4.2.3 only">
		<attr name="session" type="string" status="required"></attr>
		<attr name="invoice" type="string" status="required"></attr>
	</tag>
	<tag name="ccvs_command" type="function" returnType="string" version="4.0.2 - 4.2.3 only">
		<attr name="session" type="string" status="required"></attr>
		<attr name="type" type="string" status="required"></attr>
		<attr name="argval" type="string" status="required"></attr>
	</tag>
	<tag name="ccvs_count" type="function" returnType="int" version="4.0.2 - 4.2.3 only">
		<attr name="session" type="string" status="required"></attr>
		<attr name="type" type="string" status="required"></attr>
	</tag>
	<tag name="ccvs_delete" type="function" returnType="string" version="4.0.2 - 4.2.3 only">
		<attr name="session" type="string" status="required"></attr>
		<attr name="invoice" type="string" status="required"></attr>
	</tag>
	<tag name="ccvs_done" type="function" returnType="string" version="4.0.2 - 4.2.3 only">
		<attr name="sess" type="string" status="required"></attr>
	</tag>
	<tag name="ccvs_init" type="function" returnType="string" version="4.0.2 - 4.2.3 only">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="ccvs_lookup" type="function" returnType="string" version="4.0.2 - 4.2.3 only">
		<attr name="session" type="string" status="required"></attr>
		<attr name="invoice" type="string" status="required"></attr>
		<attr name="inum" type="int" status="required"></attr>
	</tag>
	<tag name="ccvs_new" type="function" returnType="string" version="4.0.2 - 4.2.3 only">
		<attr name="session" type="string" status="required"></attr>
		<attr name="invoice" type="string" status="required"></attr>
	</tag>
	<tag name="ccvs_report" type="function" returnType="string" version="4.0.2 - 4.2.3 only">
		<attr name="session" type="string" status="required"></attr>
		<attr name="type" type="string" status="required"></attr>
	</tag>
	<tag name="ccvs_return" type="function" returnType="string" version="4.0.2 - 4.2.3 only">
		<attr name="session" type="string" status="required"></attr>
		<attr name="invoice" type="string" status="required"></attr>
	</tag>
	<tag name="ccvs_reverse" type="function" returnType="string" version="4.0.2 - 4.2.3 only">
		<attr name="session" type="string" status="required"></attr>
		<attr name="invoice" type="string" status="required"></attr>
	</tag>
	<tag name="ccvs_sale" type="function" returnType="string" version="4.0.2 - 4.2.3 only">
		<attr name="session" type="string" status="required"></attr>
		<attr name="invoice" type="string" status="required"></attr>
	</tag>
	<tag name="ccvs_status" type="function" returnType="string" version="4.0.2 - 4.2.3 only">
		<attr name="session" type="string" status="required"></attr>
		<attr name="invoice" type="string" status="required"></attr>
	</tag>
	<tag name="ccvs_textvalue" type="function" returnType="string" version="4.0.2 - 4.2.3 only">
		<attr name="session" type="string" status="required"></attr>
	</tag>
	<tag name="ccvs_void" type="function" returnType="string" version="4.0.2 - 4.2.3 only">
		<attr name="session" type="string" status="required"></attr>
		<attr name="invoice" type="string" status="required"></attr>
	</tag>
</tags>