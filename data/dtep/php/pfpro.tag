<!DOCTYPE tags>
<tags>
	<tag name="pfpro_cleanup" type="function" returnType="void">
	</tag>
	<tag name="pfpro_init" type="function" returnType="void">
	</tag>
	<tag name="pfpro_process_raw" type="function" returnType="string">
		<attr name="parameters" type="string"></attr>
		<attr name="address" type="string" status="optional"></attr>
		<attr name="port" type="int" status="optional"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
		<attr name="proxy_address" type="string" status="optional"></attr>
		<attr name="proxy_port" type="int" status="optional"></attr>
		<attr name="proxy_logon" type="string" status="optional"></attr>
		<attr name="proxy_password" type="string" status="optional"></attr>
	</tag>
	<tag name="pfpro_process" type="function" returnType="array">
		<attr name="parameters" type="array"></attr>
		<attr name="address" type="string" status="optional"></attr>
		<attr name="port" type="int" status="optional"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
		<attr name="proxy_address" type="string" status="optional"></attr>
		<attr name="proxy_port" type="int" status="optional"></attr>
		<attr name="proxy_logon" type="string" status="optional"></attr>
		<attr name="proxy_password" type="string" status="optional"></attr>
	</tag>
	<tag name="pfpro_version" type="function" returnType="string">
	</tag>
</tags>