<!DOCTYPE tags>
<tags>
	<tag name="stream_context_create" type="function" returnType="resource">
		<attr name="options" type="array" status="optional"></attr>
	</tag>
	<tag name="stream_context_get_default" type="function" returnType="resource">
		<attr name="options" type="array" status="optional"></attr>
	</tag>
	<tag name="stream_context_get_options" type="function" returnType="array">
		<attr name="stream|context" type="resource"></attr>
	</tag>
	<tag name="stream_context_set_option" type="function" returnType="bool">
		<attr name="context|stream" type="resource"></attr>
		<attr name="wrapper" type="string"></attr>
		<attr name="option" type="string"></attr>
		<attr name="value" type="mixed"></attr>
	</tag>
	<tag name="stream_context_set_params" type="function" returnType="bool">
		<attr name="stream|context" type="resource"></attr>
		<attr name="params" type="array"></attr>
	</tag>
	<tag name="stream_copy_to_stream" type="function" returnType="int">
		<attr name="source" type="resource"></attr>
		<attr name="dest" type="resource"></attr>
		<attr name="maxlength" type="int" status="optional"></attr>
	</tag>
	<tag name="stream_filter_append" type="function" returnType="resource">
		<attr name="stream" type="resource"></attr>
		<attr name="filtername" type="string"></attr>
		<attr name="read_write" type="int" status="optional"></attr>
		<attr name="params" type="mixed" status="optional"></attr>
	</tag>
	<tag name="stream_filter_prepend" type="function" returnType="resource">
		<attr name="stream" type="resource"></attr>
		<attr name="filtername" type="string"></attr>
		<attr name="read_write" type="int" status="optional"></attr>
		<attr name="params" type="mixed" status="optional"></attr>
	</tag>
	<tag name="stream_filter_register" type="function" returnType="bool">
		<attr name="filtername" type="string"></attr>
		<attr name="classname" type="string"></attr>
	</tag>
	<tag name="stream_filter_append" type="function" returnType="bool">
		<attr name="stream_filter" type="resource"></attr>
	</tag>
	<tag name="stream_get_contents" type="function" returnType="string">
		<attr name="handle" type="resource"></attr>
		<attr name="maxlength" type="int" status="optional"></attr>
	</tag>
	<tag name="stream_get_filters" type="function" returnType="array">
	</tag>
	<tag name="stream_get_line" type="function" returnType="string">
		<attr name="handle" type="resource"></attr>
		<attr name="length" type="int"></attr>
		<attr name="ending" type="string"></attr>
	</tag>
	<tag name="stream_get_meta_data" type="function" returnType="array">
		<attr name="stream" type="resource"></attr>
	</tag>
	<tag name="stream_get_transports" type="function" returnType="array">
	</tag>
	<tag name="stream_get_wrappers" type="function" returnType="array">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="stream_select" type="function" returnType="int">
		<attr name="&amp;read" type="array"></attr>
		<attr name="&amp;write" type="array"></attr>
		<attr name="&amp;except" type="array"></attr>
		<attr name="tv_sec" type="int"></attr>
		<attr name="tv_usec" type="int" status="optional"></attr>
	</tag>
	<tag name="stream_set_blocking" type="function" returnType="bool">
		<attr name="stream" type="resource"></attr>
		<attr name="mode" type="int"></attr>
	</tag>
	<tag name="stream_set_timeout" type="function" returnType="bool">
		<attr name="stream" type="resource"></attr>
		<attr name="seconds" type="int"></attr>
		<attr name="microseconds" type="int" status="optional"></attr>
	</tag>
	<tag name="stream_set_write_buffer" type="function" returnType="int">
		<attr name="stream" type="resource"></attr>
		<attr name="buffer" type="int"></attr>
	</tag>
	<tag name="stream_socket_accept" type="function" returnType="resource">
		<attr name="server_socket" type="resource"></attr>
		<attr name="timeout" type="float" status="optional"></attr>
		<attr name="&amp;peername" type="string" status="optional"></attr>
	</tag>
	<tag name="stream_socket_client" type="function" returnType="resource">
		<attr name="remote_socket" type="string"></attr>
		<attr name="&amp;errno" type="int" status="optional"></attr>
		<attr name="&amp;errstr" type="string" status="optional"></attr>
		<attr name="timeout" type="float" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="context" type="resource" status="optional"></attr>
	</tag>
	<tag name="stream_socket_enable_crypto" type="function" returnType="resource">
		<attr name="stream" type="resource"></attr>
		<attr name="enable" type="bool"></attr>
		<attr name="crypto_type" type="int" status="optional"></attr>
		<attr name="session_stream" type="resource" status="optional"></attr>
	</tag>
	<tag name="stream_socket_get_name" type="function" returnType="string">
		<attr name="handle" type="resource"></attr>
		<attr name="want_peer" type="bool"></attr>
	</tag>
	<tag name="stream_socket_recvfrom" type="function" returnType="string">
		<attr name="socket" type="resource"></attr>
		<attr name="length" type="int"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="&amp;address" type="string" status="optional"></attr>
	</tag>
	<tag name="stream_socket_sendto" type="function" returnType="int">
		<attr name="socket" type="resource"></attr>
		<attr name="data" type="string"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="address" type="string" status="optional"></attr>
	</tag>
	<tag name="stream_socket_server" type="function" returnType="resource">
		<attr name="local_socket" type="string"></attr>
		<attr name="&amp;errno" type="int" status="optional"></attr>
		<attr name="&amp;errstr" type="string" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="context" type="resource" status="optional"></attr>
	</tag>
	<tag name="stream_wrapper_register" type="function" returnType="bool">
		<attr name="protocol" type="string"></attr>
		<attr name="classname" type="string"></attr>
	</tag>
	<tag name="stream_wrapper_restore" type="function" returnType="bool">
		<attr name="protocol" type="string"></attr>
	</tag>
	<tag name="stream_wrapper_unregister" type="function" returnType="bool">
		<attr name="protocol" type="string"></attr>
	</tag>
</tags>