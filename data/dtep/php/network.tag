<!DOCTYPE tags>
<tags>
	<tag name="checkdnsrr" type="function" returnType="int">
		<attr name="host" type="string"></attr>
		<attr name="type" type="string" status="optional"></attr>
	</tag>
	<tag name="closelog" type="function" returnType="int">
	</tag>
	<tag name="debugger_off" type="function" returnType="int">
	</tag>
	<tag name="debugger_on" type="function" returnType="int">
		<attr name="address" type="string"></attr>
	</tag>
	<tag name="define_syslog_variables" type="function" returnType="void">
	</tag>
	<tag name="dns_check_record" type="function" returnType="int">
		<attr name="host" type="string"></attr>
		<attr name="type" type="string" status="optional"></attr>
	</tag>
	<tag name="dns_get_mx" type="function" returnType="int">
		<attr name="hostname" type="string"></attr>
		<attr name="&amp;mxhosts" type="array"></attr>
		<attr name="&amp;weight" type="array" status="optional"></attr>
	</tag>
	<tag name="dns_get_record" type="function" returnType="array">
		<attr name="hostname" type="string"></attr>
		<attr name="type" type="int" status="optional"></attr>
		<attr name="&amp;authns" type="array" status="optional"></attr>
		<attr name="&amp;addtl" type="array" status="optional"></attr>
	</tag>
	<tag name="fsockopen" type="function" returnType="resource">
		<attr name="target" type="string"></attr>
		<attr name="port" type="int"></attr>
		<attr name="&amp;errno" type="int" status="optional"></attr>
		<attr name="&amp;errstr" type="string" status="optional"></attr>
		<attr name="timeout" type="float" status="optional"></attr>
	</tag>
	<tag name="gethostbyaddr" type="function" returnType="string">
		<attr name="ip_address" type="string"></attr>
	</tag>
	<tag name="gethostbyname" type="function" returnType="string">
		<attr name="hostname" type="string"></attr>
	</tag>
	<tag name="gethostbynamel" type="function" returnType="array">
		<attr name="hostname" type="string"></attr>
	</tag>
	<tag name="getmxrr" type="function" returnType="bool">
		<attr name="hostname" type="string"></attr>
		<attr name="&amp;mxhosts" type="array"></attr>
		<attr name="&amp;weight" type="array" status="optional"></attr>
	</tag>
	<tag name="getprotobyname" type="function" returnType="int">
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="getprotobynumber" type="function" returnType="string">
		<attr name="number" type="int"></attr>
	</tag>
	<tag name="getservbyname" type="function" returnType="int">
		<attr name="service" type="string"></attr>
		<attr name="protocol" type="string"></attr>
	</tag>
	<tag name="getservbyport" type="function" returnType="string">
		<attr name="port" type="int"></attr>
		<attr name="protocol" type="string"></attr>
	</tag>
	<tag name="inet_ntop" type="function" returnType="string">
		<attr name="in_addr" type="string"></attr>
	</tag>
	<tag name="inet_pton" type="function" returnType="string">
		<attr name="address" type="string"></attr>
	</tag>
	<tag name="ip2long" type="function" returnType="int">
		<attr name="ip_address" type="string"></attr>
	</tag>
	<tag name="long2ip" type="function" returnType="string">
		<attr name="proper_address" type="int"></attr>
	</tag>
	<tag name="openlog" type="function" returnType="int">
		<attr name="ident" type="string"></attr>
		<attr name="option" type="int"></attr>
		<attr name="facility" type="int"></attr>
	</tag>
	<tag name="pfsockopen" type="function" returnType="resource">
		<attr name="hostname" type="string"></attr>
		<attr name="port" type="int"></attr>
		<attr name="&amp;errno" type="int" status="optional"></attr>
		<attr name="&amp;errstr" type="string" status="optional"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="syslog" type="function" returnType="int">
		<attr name="priority" type="int"></attr>
		<attr name="message" type="string"></attr>
	</tag>
</tags>