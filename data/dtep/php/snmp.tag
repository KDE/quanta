<!DOCTYPE tags>
<tags>
	<tag name="snmp_get_quick_print" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
	</tag>
	<tag name="snmp_get_valueretrieval" type="function" returnType="int" version="PHP 4 &gt;= 4.3.3, PHP 5">
	</tag>
	<tag name="snmp_read_mib" type="function" returnType="int" version="PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="snmp_set_enum_print" type="function" returnType="void" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="enum_print" type="int" status="required"></attr>
	</tag>
	<tag name="snmp_set_oid_numeric_print" type="function" returnType="void" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="oid_numeric_print" type="int" status="required"></attr>
	</tag>
	<tag name="snmp_set_quick_print" type="function" returnType="void" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="quick_print" type="bool" status="required"></attr>
	</tag>
	<tag name="snmp_set_valueretrieval" type="function" returnType="int" version="PHP 4 &gt;= 4.3.3, PHP 5">
		<attr name="method" type="int" status="required"></attr>
	</tag>
	<tag name="snmpget" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="hostname" type="string" status="required"></attr>
		<attr name="community" type="string" status="required"></attr>
		<attr name="object_id" type="string" status="required"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
		<attr name="retries" type="int" status="optional"></attr>
	</tag>
	<tag name="snmpgetnext" type="function" returnType="string" version="PHP 5">
		<attr name="host" type="string" status="required"></attr>
		<attr name="community" type="string" status="required"></attr>
		<attr name="object_id" type="string" status="required"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
		<attr name="retries" type="int" status="optional"></attr>
	</tag>
	<tag name="snmprealwalk" type="function" returnType="array" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="host" type="string" status="required"></attr>
		<attr name="community" type="string" status="required"></attr>
		<attr name="object_id" type="string" status="required"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
		<attr name="retries" type="int" status="optional"></attr>
	</tag>
	<tag name="snmpset" type="function" returnType="bool" version="PHP 3&gt;= 3.0.12, PHP 4 , PHP 5">
		<attr name="hostname" type="string" status="required"></attr>
		<attr name="community" type="string" status="required"></attr>
		<attr name="object_id" type="string" status="required"></attr>
		<attr name="type" type="string" status="required"></attr>
		<attr name="value" type="mixed" status="required"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
		<attr name="retries" type="int" status="optional"></attr>
	</tag>
	<tag name="snmpwalk" type="function" returnType="array" version="PHP 3, PHP 4 , PHP 5">
		<attr name="hostname" type="string" status="required"></attr>
		<attr name="community" type="string" status="required"></attr>
		<attr name="object_id" type="string" status="required"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
		<attr name="retries" type="int" status="optional"></attr>
	</tag>
	<tag name="snmpwalkoid" type="function" returnType="array" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="hostname" type="string" status="required"></attr>
		<attr name="community" type="string" status="required"></attr>
		<attr name="object_id" type="string" status="required"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
		<attr name="retries" type="int" status="optional"></attr>
	</tag>
</tags>