<!DOCTYPE tags>
<tags>
	<tag name="socket_accept" type="function" returnType="resource" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="socket" type="resource" status="required"></attr>
	</tag>
	<tag name="socket_bind" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="socket" type="resource" status="required"></attr>
		<attr name="address" type="string" status="required"></attr>
		<attr name="port" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_clear_error" type="function" returnType="void" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="socket" type="resource" status="optional"></attr>
	</tag>
	<tag name="socket_close" type="function" returnType="void" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="socket" type="resource" status="required"></attr>
	</tag>
	<tag name="socket_connect" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="socket" type="resource" status="required"></attr>
		<attr name="address" type="string" status="required"></attr>
		<attr name="port" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_create_listen" type="function" returnType="resource" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="port" type="int" status="required"></attr>
		<attr name="backlog" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_create_pair" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="domain" type="int" status="required"></attr>
		<attr name="type" type="int" status="required"></attr>
		<attr name="protocol" type="int" status="required"></attr>
		<attr name="&amp;fd" type="array" status="required"></attr>
	</tag>
	<tag name="socket_create" type="function" returnType="resource" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="domain" type="int" status="required"></attr>
		<attr name="type" type="int" status="required"></attr>
		<attr name="protocol" type="int" status="required"></attr>
	</tag>
	<tag name="socket_get_option" type="function" returnType="mixed" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="socket" type="resource" status="required"></attr>
		<attr name="level" type="int" status="required"></attr>
		<attr name="optname" type="int" status="required"></attr>
	</tag>
	<tag name="socket_getpeername" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="socket" type="resource" status="required"></attr>
		<attr name="&amp;addr" type="string" status="required"></attr>
		<attr name="&amp;port" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_getsockname" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="socket" type="resource" status="required"></attr>
		<attr name="&amp;addr" type="string" status="required"></attr>
		<attr name="&amp;port" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_last_error" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="socket" type="resource" status="optional"></attr>
	</tag>
	<tag name="socket_listen" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="socket" type="resource" status="required"></attr>
		<attr name="backlog" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_read" type="function" returnType="string" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="socket" type="resource" status="required"></attr>
		<attr name="length" type="int" status="required"></attr>
		<attr name="type" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_recv" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="socket" type="resource" status="required"></attr>
		<attr name="&amp;buf" type="string" status="required"></attr>
		<attr name="len" type="int" status="required"></attr>
		<attr name="flags" type="int" status="required"></attr>
	</tag>
	<tag name="socket_recvfrom" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="socket" type="resource" status="required"></attr>
		<attr name="&amp;buf" type="string" status="required"></attr>
		<attr name="len" type="int" status="required"></attr>
		<attr name="flags" type="int" status="required"></attr>
		<attr name="&amp;name" type="string" status="required"></attr>
		<attr name="&amp;port" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_select" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="&amp;read" type="array" status="required"></attr>
		<attr name="&amp;write" type="array" status="required"></attr>
		<attr name="&amp;except" type="array" status="required"></attr>
		<attr name="tv_sec" type="int" status="required"></attr>
		<attr name="tv_usec" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_send" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="socket" type="resource" status="required"></attr>
		<attr name="buf" type="string" status="required"></attr>
		<attr name="len" type="int" status="required"></attr>
		<attr name="flags" type="int" status="required"></attr>
	</tag>
	<tag name="socket_sendto" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="socket" type="resource" status="required"></attr>
		<attr name="buf" type="string" status="required"></attr>
		<attr name="len" type="int" status="required"></attr>
		<attr name="flags" type="int" status="required"></attr>
		<attr name="addr" type="string" status="required"></attr>
		<attr name="port" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_set_block" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="socket" type="resource" status="required"></attr>
	</tag>
	<tag name="socket_set_nonblock" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="socket" type="resource" status="required"></attr>
	</tag>
	<tag name="socket_set_option" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="socket" type="resource" status="required"></attr>
		<attr name="level" type="int" status="required"></attr>
		<attr name="optname" type="int" status="required"></attr>
		<attr name="optval" type="mixed" status="required"></attr>
	</tag>
	<tag name="socket_shutdown" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="socket" type="resource" status="required"></attr>
		<attr name="how" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_strerror" type="function" returnType="string" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="errno" type="int" status="required"></attr>
	</tag>
	<tag name="socket_write" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="socket" type="resource" status="required"></attr>
		<attr name="buffer" type="string" status="required"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
</tags>