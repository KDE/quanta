<!DOCTYPE tags>
<tags>	<tag name="snmp_get_quick_print" type="function" returnType="bool">
	</tag>
	<tag name="snmp_set_quick_print" type="function" returnType="void">
		<attr name="quick_print" type="bool" status="optional"></attr>
	</tag>
	<tag name="snmpget" type="function" returnType="string">
		<attr name="hostname" type="string" status="optional"></attr>
		<attr name="community" type="string" status="optional"></attr>
		<attr name="object_id" type="string" status="optional"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
		<attr name="retries" type="int" status="optional"></attr>
	</tag>
	<tag name="snmprealwalk" type="function" returnType="array">
		<attr name="host" type="string" status="optional"></attr>
		<attr name="community" type="string" status="optional"></attr>
		<attr name="object_id" type="string" status="optional"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
		<attr name="retries" type="int" status="optional"></attr>
	</tag>
	<tag name="snmpset" type="function" returnType="bool">
		<attr name="hostname" type="string" status="optional"></attr>
		<attr name="community" type="string" status="optional"></attr>
		<attr name="object_id" type="string" status="optional"></attr>
		<attr name="type" type="string" status="optional"></attr>
		<attr name="value" type="mixed" status="optional"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
		<attr name="retries" type="int" status="optional"></attr>
	</tag>
	<tag name="snmpwalk" type="function" returnType="array">
		<attr name="hostname" type="string" status="optional"></attr>
		<attr name="community" type="string" status="optional"></attr>
		<attr name="object_id" type="string" status="optional"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
		<attr name="retries" type="int" status="optional"></attr>
	</tag>
	<tag name="snmpwalkoid" type="function" returnType="array">
		<attr name="hostname" type="string" status="optional"></attr>
		<attr name="community" type="string" status="optional"></attr>
		<attr name="object_id" type="string" status="optional"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
		<attr name="retries" type="int" status="optional"></attr>
	</tag>
</tags>
