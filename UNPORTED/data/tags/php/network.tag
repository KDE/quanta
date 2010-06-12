<!DOCTYPE tags>
<tags>	<tag name="checkdnsrr" type="function" returnType="int">
		<attr name="host" type="string" status="optional"></attr>
		<attr name="type" type="string" status="optional"></attr>
	</tag>
	<tag name="closelog" type="function" returnType="int">
	</tag>
	<tag name="debugger_off" type="function" returnType="int">
	</tag>
	<tag name="debugger_on" type="function" returnType="int">
		<attr name="address" type="string" status="optional"></attr>
	</tag>
	<tag name="define_syslog_variables" type="function" returnType="void">
	</tag>
	<tag name="fsockopen" type="function" returnType="int">
		<attr name="hostname" type="string" status="optional"></attr>
		<attr name="port" type="int" status="optional"></attr>
		<attr name="errno" type="int" status="optional"></attr>
		<attr name="errstr" type="string" status="optional"></attr>
		<attr name="timeout" type="float" status="optional"></attr>
	</tag>
	<tag name="gethostbyaddr" type="function" returnType="string">
		<attr name="ip_address" type="string" status="optional"></attr>
	</tag>
	<tag name="gethostbyname" type="function" returnType="string">
		<attr name="hostname" type="string" status="optional"></attr>
	</tag>
	<tag name="gethostbynamel" type="function" returnType="array">
		<attr name="hostname" type="string" status="optional"></attr>
	</tag>
	<tag name="getmxrr" type="function" returnType="int">
		<attr name="hostname" type="string" status="optional"></attr>
		<attr name="mxhosts" type="array" status="optional"></attr>
		<attr name="weight" type="array" status="optional"></attr>
	</tag>
	<tag name="getprotobyname" type="function" returnType="int">
		<attr name="name" type="string" status="optional"></attr>
	</tag>
	<tag name="getprotobynumber" type="function" returnType="string">
		<attr name="number" type="int" status="optional"></attr>
	</tag>
	<tag name="getservbyname" type="function" returnType="int">
		<attr name="service" type="string" status="optional"></attr>
		<attr name="protocol" type="string" status="optional"></attr>
	</tag>
	<tag name="getservbyport" type="function" returnType="string">
		<attr name="port" type="int" status="optional"></attr>
		<attr name="protocol" type="string" status="optional"></attr>
	</tag>
	<tag name="ip2long" type="function" returnType="int">
		<attr name="ip_address" type="string" status="optional"></attr>
	</tag>
	<tag name="long2ip" type="function" returnType="string">
		<attr name="proper_address" type="int" status="optional"></attr>
	</tag>
	<tag name="openlog" type="function" returnType="int">
		<attr name="ident" type="string" status="optional"></attr>
		<attr name="option" type="int" status="optional"></attr>
		<attr name="facility" type="int" status="optional"></attr>
	</tag>
	<tag name="pfsockopen" type="function" returnType="int">
		<attr name="hostname" type="string" status="optional"></attr>
		<attr name="port" type="int" status="optional"></attr>
		<attr name="errno" type="int" status="optional"></attr>
		<attr name="errstr" type="string" status="optional"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_get_status" type="function" returnType="array">
		<attr name="socketstream" type="resource" status="optional"></attr>
	</tag>
	<tag name="socket_set_blocking" type="function" returnType="int">
		<attr name="socket descriptor" type="int" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_set_timeout" type="function" returnType="bool">
		<attr name="socket descriptor" type="int" status="optional"></attr>
		<attr name="seconds" type="int" status="optional"></attr>
		<attr name="microseconds" type="int" status="optional"></attr>
	</tag>
	<tag name="syslog" type="function" returnType="int">
		<attr name="priority" type="int" status="optional"></attr>
		<attr name="message" type="string" status="optional"></attr>
	</tag>
</tags>
