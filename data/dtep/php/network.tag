<!DOCTYPE tags>
<tags>
	<tag name="checkdnsrr" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="host" type="string" status="required"></attr>
		<attr name="type" type="string" status="optional"></attr>
	</tag>
	<tag name="closelog" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
	</tag>
	<tag name="debugger_off" type="function" returnType="int" version="PHP 3">
	</tag>
	<tag name="debugger_on" type="function" returnType="int" version="PHP 3">
		<attr name="address" type="string" status="required"></attr>
	</tag>
	<tag name="define_syslog_variables" type="function" returnType="void" version="PHP 3, PHP 4 , PHP 5">
	</tag>
	<tag name="dns_check_record" type="function" returnType="int" version="PHP 5">
		<attr name="host" type="string" status="required"></attr>
		<attr name="type" type="string" status="optional"></attr>
	</tag>
	<tag name="dns_get_mx" type="function" returnType="int" version="PHP 5">
		<attr name="hostname" type="string" status="required"></attr>
		<attr name="&amp;mxhosts" type="array" status="required"></attr>
		<attr name="&amp;weight" type="array" status="optional"></attr>
	</tag>
	<tag name="dns_get_record" type="function" returnType="array" version="PHP 5">
		<attr name="hostname" type="string" status="required"></attr>
		<attr name="type" type="int" status="optional"></attr>
		<attr name="&amp;authns" type="array" status="optional"></attr>
		<attr name="&amp;addtl" type="array" status="required"></attr>
	</tag>
	<tag name="fsockopen" type="function" returnType="resource" version="PHP 3, PHP 4 , PHP 5">
		<attr name="target" type="string" status="required"></attr>
		<attr name="port" type="int" status="required"></attr>
		<attr name="&amp;errno" type="int" status="optional"></attr>
		<attr name="&amp;errstr" type="string" status="optional"></attr>
		<attr name="timeout" type="float" status="optional"></attr>
	</tag>
	<tag name="gethostbyaddr" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="ip_address" type="string" status="required"></attr>
	</tag>
	<tag name="gethostbyname" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="hostname" type="string" status="required"></attr>
	</tag>
	<tag name="gethostbynamel" type="function" returnType="array" version="PHP 3, PHP 4 , PHP 5">
		<attr name="hostname" type="string" status="required"></attr>
	</tag>
	<tag name="getmxrr" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="hostname" type="string" status="required"></attr>
		<attr name="&amp;mxhosts" type="array" status="required"></attr>
		<attr name="&amp;weight" type="array" status="optional"></attr>
	</tag>
	<tag name="getprotobyname" type="function" returnType="int" version="PHP 4 , PHP 5">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="getprotobynumber" type="function" returnType="string" version="PHP 4 , PHP 5">
		<attr name="number" type="int" status="required"></attr>
	</tag>
	<tag name="getservbyname" type="function" returnType="int" version="PHP 4 , PHP 5">
		<attr name="service" type="string" status="required"></attr>
		<attr name="protocol" type="string" status="required"></attr>
	</tag>
	<tag name="getservbyport" type="function" returnType="string" version="PHP 4 , PHP 5">
		<attr name="port" type="int" status="required"></attr>
		<attr name="protocol" type="string" status="required"></attr>
	</tag>
	<tag name="inet_ntop" type="function" returnType="string" version="">
		<attr name="in_addr" type="string" status="required"></attr>
	</tag>
	<tag name="inet_pton" type="function" returnType="string" version="">
		<attr name="address" type="string" status="required"></attr>
	</tag>
	<tag name="ip2long" type="function" returnType="int" version="PHP 4 , PHP 5">
		<attr name="ip_address" type="string" status="required"></attr>
	</tag>
	<tag name="long2ip" type="function" returnType="string" version="PHP 4 , PHP 5">
		<attr name="proper_address" type="int" status="required"></attr>
	</tag>
	<tag name="openlog" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="ident" type="string" status="required"></attr>
		<attr name="option" type="int" status="required"></attr>
		<attr name="facility" type="int" status="required"></attr>
	</tag>
	<tag name="pfsockopen" type="function" returnType="resource" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="hostname" type="string" status="required"></attr>
		<attr name="port" type="int" status="required"></attr>
		<attr name="&amp;errno" type="int" status="optional"></attr>
		<attr name="&amp;errstr" type="string" status="optional"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
	</tag>
	<tag name="syslog" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="priority" type="int" status="required"></attr>
		<attr name="message" type="string" status="required"></attr>
	</tag>
</tags>