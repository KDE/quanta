<!DOCTYPE tags>
<tags>
	<tag name="stream_context_create" type="function" returnType="resource" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="options" type="array" status="optional"></attr>
	</tag>
	<tag name="stream_context_get_default" type="function" returnType="resource" version="">
		<attr name="options" type="array" status="optional"></attr>
	</tag>
	<tag name="stream_context_get_options" type="function" returnType="array" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="stream|context" type="resource" status="required"></attr>
	</tag>
	<tag name="stream_context_set_option" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="context|stream" type="resource" status="required"></attr>
		<attr name="wrapper" type="string" status="required"></attr>
		<attr name="option" type="string" status="required"></attr>
		<attr name="value" type="mixed" status="required"></attr>
	</tag>
	<tag name="stream_context_set_params" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="stream|context" type="resource" status="required"></attr>
		<attr name="params" type="array" status="required"></attr>
	</tag>
	<tag name="stream_copy_to_stream" type="function" returnType="int" version="PHP 5">
		<attr name="source" type="resource" status="required"></attr>
		<attr name="dest" type="resource" status="required"></attr>
		<attr name="maxlength" type="int" status="optional"></attr>
	</tag>
	<tag name="stream_filter_append" type="function" returnType="resource" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="stream" type="resource" status="required"></attr>
		<attr name="filtername" type="string" status="required"></attr>
		<attr name="read_write" type="int" status="optional"></attr>
		<attr name="params" type="mixed" status="optional"></attr>
	</tag>
	<tag name="stream_filter_prepend" type="function" returnType="resource" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="stream" type="resource" status="required"></attr>
		<attr name="filtername" type="string" status="required"></attr>
		<attr name="read_write" type="int" status="optional"></attr>
		<attr name="params" type="mixed" status="optional"></attr>
	</tag>
	<tag name="stream_filter_register" type="function" returnType="bool" version="PHP 5">
		<attr name="filtername" type="string" status="required"></attr>
		<attr name="classname" type="string" status="required"></attr>
	</tag>
	<tag name="stream_filter_append" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="stream_filter" type="resource" status="required"></attr>
	</tag>
	<tag name="stream_get_contents" type="function" returnType="string" version="PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="maxlength" type="int" status="optional"></attr>
	</tag>
	<tag name="stream_get_filters" type="function" returnType="array" version="PHP 5">
	</tag>
	<tag name="stream_get_line" type="function" returnType="string" version="PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="length" type="int" status="required"></attr>
		<attr name="ending" type="string" status="required"></attr>
	</tag>
	<tag name="stream_get_meta_data" type="function" returnType="array" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="stream" type="resource" status="required"></attr>
	</tag>
	<tag name="stream_get_transports" type="function" returnType="array" version="PHP 5">
	</tag>
	<tag name="stream_get_wrappers" type="function" returnType="array" version="PHP 5">
	</tag>
	<tag name="stream_select" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="&amp;read" type="array" status="required"></attr>
		<attr name="&amp;write" type="array" status="required"></attr>
		<attr name="&amp;except" type="array" status="required"></attr>
		<attr name="tv_sec" type="int" status="required"></attr>
		<attr name="tv_usec" type="int" status="optional"></attr>
	</tag>
	<tag name="stream_set_blocking" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="stream" type="resource" status="required"></attr>
		<attr name="mode" type="int" status="required"></attr>
	</tag>
	<tag name="stream_set_timeout" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="stream" type="resource" status="required"></attr>
		<attr name="seconds" type="int" status="required"></attr>
		<attr name="microseconds" type="int" status="optional"></attr>
	</tag>
	<tag name="stream_set_write_buffer" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="stream" type="resource" status="required"></attr>
		<attr name="buffer" type="int" status="required"></attr>
	</tag>
	<tag name="stream_socket_accept" type="function" returnType="resource" version="PHP 5">
		<attr name="server_socket" type="resource" status="required"></attr>
		<attr name="timeout" type="float" status="optional"></attr>
		<attr name="&amp;peername" type="string" status="optional"></attr>
	</tag>
	<tag name="stream_socket_client" type="function" returnType="resource" version="PHP 5">
		<attr name="remote_socket" type="string" status="required"></attr>
		<attr name="&amp;errno" type="int" status="optional"></attr>
		<attr name="&amp;errstr" type="string" status="optional"></attr>
		<attr name="timeout" type="float" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="context" type="resource" status="optional"></attr>
	</tag>
	<tag name="stream_socket_enable_crypto" type="function" returnType="resource" version="">
		<attr name="stream" type="resource" status="required"></attr>
		<attr name="enable" type="bool" status="required"></attr>
		<attr name="crypto_type" type="int" status="optional"></attr>
		<attr name="session_stream" type="resource" status="optional"></attr>
	</tag>
	<tag name="stream_socket_get_name" type="function" returnType="string" version="PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="want_peer" type="bool" status="required"></attr>
	</tag>
	<tag name="stream_socket_recvfrom" type="function" returnType="string" version="PHP 5">
		<attr name="socket" type="resource" status="required"></attr>
		<attr name="length" type="int" status="required"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="&amp;address" type="string" status="optional"></attr>
	</tag>
	<tag name="stream_socket_sendto" type="function" returnType="int" version="PHP 5">
		<attr name="socket" type="resource" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="address" type="string" status="optional"></attr>
	</tag>
	<tag name="stream_socket_server" type="function" returnType="resource" version="PHP 5">
		<attr name="local_socket" type="string" status="required"></attr>
		<attr name="&amp;errno" type="int" status="optional"></attr>
		<attr name="&amp;errstr" type="string" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="context" type="resource" status="optional"></attr>
	</tag>
	<tag name="stream_wrapper_register" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.2, PHP 5">
		<attr name="protocol" type="string" status="required"></attr>
		<attr name="classname" type="string" status="required"></attr>
	</tag>
	<tag name="stream_wrapper_restore" type="function" returnType="bool" version="">
		<attr name="protocol" type="string" status="required"></attr>
	</tag>
	<tag name="stream_wrapper_unregister" type="function" returnType="bool" version="">
		<attr name="protocol" type="string" status="required"></attr>
	</tag>
</tags>