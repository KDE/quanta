<!DOCTYPE tags>
<tags>
	<tag name="snmp_get_quick_print" type="function" returnType="bool">
	</tag>
	<tag name="snmp_get_valueretrieval" type="function" returnType="int">
	</tag>
	<tag name="snmp_read_mib" type="function" returnType="int">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="snmp_set_enum_print" type="function" returnType="void">
		<attr name="enum_print" type="int"></attr>
	</tag>
	<tag name="snmp_set_oid_numeric_print" type="function" returnType="void">
		<attr name="oid_numeric_print" type="int"></attr>
	</tag>
	<tag name="snmp_set_quick_print" type="function" returnType="void">
		<attr name="quick_print" type="bool"></attr>
	</tag>
	<tag name="snmp_set_valueretrieval" type="function" returnType="int">
		<attr name="method" type="int"></attr>
	</tag>
	<tag name="snmpget" type="function" returnType="string">
		<attr name="hostname" type="string"></attr>
		<attr name="community" type="string"></attr>
		<attr name="object_id" type="string"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
		<attr name="retries" type="int" status="optional"></attr>
	</tag>
	<tag name="snmpgetnext" type="function" returnType="string">
		<attr name="host" type="string"></attr>
		<attr name="community" type="string"></attr>
		<attr name="object_id" type="string"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
		<attr name="retries" type="int" status="optional"></attr>
	</tag>
	<tag name="snmprealwalk" type="function" returnType="array">
		<attr name="host" type="string"></attr>
		<attr name="community" type="string"></attr>
		<attr name="object_id" type="string"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
		<attr name="retries" type="int" status="optional"></attr>
	</tag>
	<tag name="snmpset" type="function" returnType="bool">
		<attr name="hostname" type="string"></attr>
		<attr name="community" type="string"></attr>
		<attr name="object_id" type="string"></attr>
		<attr name="type" type="string"></attr>
		<attr name="value" type="mixed"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
		<attr name="retries" type="int" status="optional"></attr>
	</tag>
	<tag name="snmpwalk" type="function" returnType="array">
		<attr name="hostname" type="string"></attr>
		<attr name="community" type="string"></attr>
		<attr name="object_id" type="string"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
		<attr name="retries" type="int" status="optional"></attr>
	</tag>
	<tag name="snmpwalkoid" type="function" returnType="array">
		<attr name="hostname" type="string"></attr>
		<attr name="community" type="string"></attr>
		<attr name="object_id" type="string"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
		<attr name="retries" type="int" status="optional"></attr>
	</tag>
</tags>