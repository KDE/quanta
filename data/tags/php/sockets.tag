<!DOCTYPE tags>
<tags>	<tag name="socket_accept" type="function" returnType="resource">
		<attr name="socket" type="resource" status="optional"></attr>
	</tag>
	<tag name="socket_bind" type="function" returnType="bool">
		<attr name="socket" type="resource" status="optional"></attr>
		<attr name="address" type="string" status="optional"></attr>
		<attr name="port" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_clear_error" type="function" returnType="void">
		<attr name="socket" type="resource" status="optional"></attr>
	</tag>
	<tag name="socket_close" type="function" returnType="void">
		<attr name="socket" type="resource" status="optional"></attr>
	</tag>
	<tag name="socket_connect" type="function" returnType="bool">
		<attr name="socket" type="resource" status="optional"></attr>
		<attr name="address" type="string" status="optional"></attr>
		<attr name="port" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_create_listen" type="function" returnType="resource">
		<attr name="port" type="int" status="optional"></attr>
		<attr name="backlog" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_create_pair" type="function" returnType="bool">
		<attr name="domain" type="int" status="optional"></attr>
		<attr name="type" type="int" status="optional"></attr>
		<attr name="protocol" type="int" status="optional"></attr>
		<attr name="&amp;fd" type="array" status="optional"></attr>
	</tag>
	<tag name="socket_create" type="function" returnType="resource">
		<attr name="domain" type="int" status="optional"></attr>
		<attr name="type" type="int" status="optional"></attr>
		<attr name="protocol" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_get_option" type="function" returnType="mixed">
		<attr name="socket" type="resource" status="optional"></attr>
		<attr name="level" type="int" status="optional"></attr>
		<attr name="optname" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_getpeername" type="function" returnType="bool">
		<attr name="socket" type="resource" status="optional"></attr>
		<attr name="&amp;addr" type="string" status="optional"></attr>
		<attr name="&amp;port" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_getsockname" type="function" returnType="bool">
		<attr name="socket" type="resource" status="optional"></attr>
		<attr name="&amp;addr" type="string" status="optional"></attr>
		<attr name="&amp;port" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_iovec_add" type="function" returnType="bool">
		<attr name="iovec" type="resource" status="optional"></attr>
		<attr name="iov_len" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_iovec_alloc" type="function" returnType="resource">
		<attr name="num_vectors" type="int" status="optional"></attr>
		<attr name="intN" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_iovec_delete" type="function" returnType="bool">
		<attr name="iovec" type="resource" status="optional"></attr>
		<attr name="iov_pos" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_iovec_fetch" type="function" returnType="string">
		<attr name="iovec" type="resource" status="optional"></attr>
		<attr name="iovec_position" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_iovec_free" type="function" returnType="bool">
		<attr name="iovec" type="resource" status="optional"></attr>
	</tag>
	<tag name="socket_iovec_set" type="function" returnType="bool">
		<attr name="iovec" type="resource" status="optional"></attr>
		<attr name="iovec_position" type="int" status="optional"></attr>
		<attr name="new_val" type="string" status="optional"></attr>
	</tag>
	<tag name="socket_last_error" type="function" returnType="int">
		<attr name="socket" type="resource" status="optional"></attr>
	</tag>
	<tag name="socket_listen" type="function" returnType="bool">
		<attr name="socket" type="resource" status="optional"></attr>
		<attr name="backlog" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_read" type="function" returnType="string">
		<attr name="socket" type="resource" status="optional"></attr>
		<attr name="length" type="int" status="optional"></attr>
		<attr name="type" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_readv" type="function" returnType="bool">
		<attr name="socket" type="resource" status="optional"></attr>
		<attr name="iovec_id" type="resource" status="optional"></attr>
	</tag>
	<tag name="socket_recv" type="function" returnType="string">
		<attr name="socket" type="resource" status="optional"></attr>
		<attr name="len" type="int" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_recvfrom" type="function" returnType="int">
		<attr name="socket" type="resource" status="optional"></attr>
		<attr name="&amp;buf" type="string" status="optional"></attr>
		<attr name="len" type="int" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="&amp;name" type="string" status="optional"></attr>
		<attr name="&amp;port" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_recvmsg" type="function" returnType="bool">
		<attr name="socket" type="resource" status="optional"></attr>
		<attr name="iovec" type="resource" status="optional"></attr>
		<attr name="&amp;control" type="array" status="optional"></attr>
		<attr name="&amp;controllen" type="int" status="optional"></attr>
		<attr name="&amp;flags" type="int" status="optional"></attr>
		<attr name="&amp;addr" type="string" status="optional"></attr>
		<attr name="&amp;port" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_select" type="function" returnType="int">
		<attr name="&amp;read" type="resource" status="optional"></attr>
		<attr name="&amp;write" type="resource" status="optional"></attr>
		<attr name="&amp;except" type="resource" status="optional"></attr>
		<attr name="tv_sec" type="int" status="optional"></attr>
		<attr name="tv_usec" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_send" type="function" returnType="int">
		<attr name="socket" type="resource" status="optional"></attr>
		<attr name="buf" type="string" status="optional"></attr>
		<attr name="len" type="int" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_sendmsg" type="function" returnType="bool">
		<attr name="socket" type="resource" status="optional"></attr>
		<attr name="iovec" type="resource" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="addr" type="string" status="optional"></attr>
		<attr name="port" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_sendto" type="function" returnType="int">
		<attr name="socket" type="resource" status="optional"></attr>
		<attr name="buf" type="string" status="optional"></attr>
		<attr name="len" type="int" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="addr" type="string" status="optional"></attr>
		<attr name="port" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_set_nonblock" type="function" returnType="bool">
		<attr name="socket" type="resource" status="optional"></attr>
	</tag>
	<tag name="socket_set_option" type="function" returnType="bool">
		<attr name="socket" type="resource" status="optional"></attr>
		<attr name="level" type="int" status="optional"></attr>
		<attr name="optname" type="int" status="optional"></attr>
		<attr name="intN" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_shutdown" type="function" returnType="bool">
		<attr name="socket" type="resource" status="optional"></attr>
		<attr name="how" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_strerror" type="function" returnType="string">
		<attr name="errno" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_write" type="function" returnType="int">
		<attr name="socket" type="resource" status="optional"></attr>
		<attr name="buffer" type="string" status="optional"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="socket_writev" type="function" returnType="bool">
		<attr name="socket" type="resource" status="optional"></attr>
		<attr name="iovec_id" type="resource" status="optional"></attr>
	</tag>
</tags>
