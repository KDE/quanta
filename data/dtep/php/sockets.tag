<!DOCTYPE tags>
<tags>
	<tag name="socket_accept" type="function" returnType="resource">
		<attr name="socket" type="resource"></attr>
	</tag>
	<tag name="socket_bind" type="function" returnType="bool">
		<attr name="socket" type="resource"></attr>
		<attr name="address" type="string"></attr>
		<attr name="port" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_clear_error" type="function" returnType="void">
		<attr name="socket" type="resource" status="optional"></attr>
	</tag>
	<tag name="socket_close" type="function" returnType="void">
		<attr name="socket" type="resource"></attr>
	</tag>
	<tag name="socket_connect" type="function" returnType="bool">
		<attr name="socket" type="resource"></attr>
		<attr name="address" type="string"></attr>
		<attr name="port" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_create_listen" type="function" returnType="resource">
		<attr name="port" type="int"></attr>
		<attr name="backlog" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_create_pair" type="function" returnType="bool">
		<attr name="domain" type="int"></attr>
		<attr name="type" type="int"></attr>
		<attr name="protocol" type="int"></attr>
		<attr name="&amp;fd" type="array"></attr>
	</tag>
	<tag name="socket_create" type="function" returnType="resource">
		<attr name="domain" type="int"></attr>
		<attr name="type" type="int"></attr>
		<attr name="protocol" type="int"></attr>
	</tag>
	<tag name="socket_get_option" type="function" returnType="mixed">
		<attr name="socket" type="resource"></attr>
		<attr name="level" type="int"></attr>
		<attr name="optname" type="int"></attr>
	</tag>
	<tag name="socket_getpeername" type="function" returnType="bool">
		<attr name="socket" type="resource"></attr>
		<attr name="&amp;addr" type="string"></attr>
		<attr name="&amp;port" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_getsockname" type="function" returnType="bool">
		<attr name="socket" type="resource"></attr>
		<attr name="&amp;addr" type="string"></attr>
		<attr name="&amp;port" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_last_error" type="function" returnType="int">
		<attr name="socket" type="resource" status="optional"></attr>
	</tag>
	<tag name="socket_listen" type="function" returnType="bool">
		<attr name="socket" type="resource"></attr>
		<attr name="backlog" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_read" type="function" returnType="string">
		<attr name="socket" type="resource"></attr>
		<attr name="length" type="int"></attr>
		<attr name="type" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_recv" type="function" returnType="int">
		<attr name="socket" type="resource"></attr>
		<attr name="&amp;buf" type="string"></attr>
		<attr name="len" type="int"></attr>
		<attr name="flags" type="int"></attr>
	</tag>
	<tag name="socket_recvfrom" type="function" returnType="int">
		<attr name="socket" type="resource"></attr>
		<attr name="&amp;buf" type="string"></attr>
		<attr name="len" type="int"></attr>
		<attr name="flags" type="int"></attr>
		<attr name="&amp;name" type="string"></attr>
		<attr name="&amp;port" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_select" type="function" returnType="int">
		<attr name="&amp;read" type="array"></attr>
		<attr name="&amp;write" type="array"></attr>
		<attr name="&amp;except" type="array"></attr>
		<attr name="tv_sec" type="int"></attr>
		<attr name="tv_usec" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_send" type="function" returnType="int">
		<attr name="socket" type="resource"></attr>
		<attr name="buf" type="string"></attr>
		<attr name="len" type="int"></attr>
		<attr name="flags" type="int"></attr>
	</tag>
	<tag name="socket_sendto" type="function" returnType="int">
		<attr name="socket" type="resource"></attr>
		<attr name="buf" type="string"></attr>
		<attr name="len" type="int"></attr>
		<attr name="flags" type="int"></attr>
		<attr name="addr" type="string"></attr>
		<attr name="port" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_set_block" type="function" returnType="bool">
		<attr name="socket" type="resource"></attr>
	</tag>
	<tag name="socket_set_nonblock" type="function" returnType="bool">
		<attr name="socket" type="resource"></attr>
	</tag>
	<tag name="socket_set_option" type="function" returnType="bool">
		<attr name="socket" type="resource"></attr>
		<attr name="level" type="int"></attr>
		<attr name="optname" type="int"></attr>
		<attr name="optval" type="mixed"></attr>
	</tag>
	<tag name="socket_shutdown" type="function" returnType="bool">
		<attr name="socket" type="resource"></attr>
		<attr name="how" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_strerror" type="function" returnType="string">
		<attr name="errno" type="int"></attr>
	</tag>
	<tag name="socket_write" type="function" returnType="int">
		<attr name="socket" type="resource"></attr>
		<attr name="buffer" type="string"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
</tags>