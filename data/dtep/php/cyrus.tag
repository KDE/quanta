<!DOCTYPE tags>
<tags>
	<tag name="cyrus_authenticate" type="function" returnType="bool">
		<attr name="connection" type="resource"></attr>
		<attr name="mechlist" type="string" status="optional"></attr>
		<attr name="service" type="string" status="optional"></attr>
		<attr name="user" type="string" status="optional"></attr>
		<attr name="minssf" type="int" status="optional"></attr>
		<attr name="maxssf" type="int" status="optional"></attr>
		<attr name="authname" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="cyrus_bind" type="function" returnType="bool">
		<attr name="connection" type="resource"></attr>
		<attr name="callbacks" type="array"></attr>
	</tag>
	<tag name="cyrus_close" type="function" returnType="bool">
		<attr name="connection" type="resource"></attr>
	</tag>
	<tag name="cyrus_connect" type="function" returnType="resource">
		<attr name="host" type="string" status="optional"></attr>
		<attr name="port" type="string" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="cyrus_query" type="function" returnType="bool">
		<attr name="connection" type="resource"></attr>
		<attr name="query" type="string"></attr>
	</tag>
	<tag name="cyrus_unbind" type="function" returnType="bool">
		<attr name="connection" type="resource"></attr>
		<attr name="trigger_name" type="string"></attr>
	</tag>
</tags>