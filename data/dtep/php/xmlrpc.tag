<!DOCTYPE tags>
<tags>
	<tag name="xmlrpc_decode_request" type="function" returnType="array">
		<attr name="xml" type="string"></attr>
		<attr name="&amp;method" type="string"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="xmlrpc_decode" type="function" returnType="array">
		<attr name="xml" type="string"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="xmlrpc_encode_request" type="function" returnType="string">
		<attr name="method" type="string"></attr>
		<attr name="params" type="mixed"></attr>
		<attr name="output_options" type="array" status="optional"></attr>
	</tag>
	<tag name="xmlrpc_encode" type="function" returnType="string">
		<attr name="value" type="mixed"></attr>
	</tag>
	<tag name="xmlrpc_get_type" type="function" returnType="string">
		<attr name="value" type="mixed"></attr>
	</tag>
	<tag name="xmlrpc_is_fault" type="function" returnType="bool">
		<attr name="arg" type="array"></attr>
	</tag>
	<tag name="xmlrpc_parse_method_descriptions" type="function" returnType="array">
		<attr name="xml" type="string"></attr>
	</tag>
	<tag name="xmlrpc_server_add_introspection_data" type="function" returnType="int">
		<attr name="server" type="resource"></attr>
		<attr name="desc" type="array"></attr>
	</tag>
	<tag name="xmlrpc_server_call_method" type="function" returnType="mixed">
		<attr name="server" type="resource"></attr>
		<attr name="xml" type="string"></attr>
		<attr name="user_data" type="mixed"></attr>
		<attr name="output_options" type="array" status="optional"></attr>
	</tag>
	<tag name="xmlrpc_server_create" type="function" returnType="resource">
	</tag>
	<tag name="xmlrpc_server_destroy" type="function" returnType="int">
		<attr name="server" type="resource"></attr>
	</tag>
	<tag name="xmlrpc_server_register_introspection_callback" type="function" returnType="bool">
		<attr name="server" type="resource"></attr>
		<attr name="function" type="string"></attr>
	</tag>
	<tag name="xmlrpc_server_register_method" type="function" returnType="bool">
		<attr name="server" type="resource"></attr>
		<attr name="method_name" type="string"></attr>
		<attr name="function" type="string"></attr>
	</tag>
	<tag name="xmlrpc_set_type" type="function" returnType="bool">
		<attr name="&amp;value" type="string"></attr>
		<attr name="type" type="string"></attr>
	</tag>
</tags>