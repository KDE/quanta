<!DOCTYPE tags>
<tags>
	<tag name="xmlrpc_decode_request" type="function" returnType="array" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="xml" type="string" status="required"></attr>
		<attr name="&amp;method" type="string" status="required"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="xmlrpc_decode" type="function" returnType="array" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="xml" type="string" status="required"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="xmlrpc_encode_request" type="function" returnType="string" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="method" type="string" status="required"></attr>
		<attr name="params" type="mixed" status="required"></attr>
		<attr name="output_options" type="array" status="optional"></attr>
	</tag>
	<tag name="xmlrpc_encode" type="function" returnType="string" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="value" type="mixed" status="required"></attr>
	</tag>
	<tag name="xmlrpc_get_type" type="function" returnType="string" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="value" type="mixed" status="required"></attr>
	</tag>
	<tag name="xmlrpc_is_fault" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="arg" type="array" status="required"></attr>
	</tag>
	<tag name="xmlrpc_parse_method_descriptions" type="function" returnType="array" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="xml" type="string" status="required"></attr>
	</tag>
	<tag name="xmlrpc_server_add_introspection_data" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="server" type="resource" status="required"></attr>
		<attr name="desc" type="array" status="required"></attr>
	</tag>
	<tag name="xmlrpc_server_call_method" type="function" returnType="mixed" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="server" type="resource" status="required"></attr>
		<attr name="xml" type="string" status="required"></attr>
		<attr name="user_data" type="mixed" status="required"></attr>
		<attr name="output_options" type="array" status="optional"></attr>
	</tag>
	<tag name="xmlrpc_server_create" type="function" returnType="resource" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="xmlrpc_server_destroy" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="server" type="resource" status="required"></attr>
	</tag>
	<tag name="xmlrpc_server_register_introspection_callback" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="server" type="resource" status="required"></attr>
		<attr name="function" type="string" status="required"></attr>
	</tag>
	<tag name="xmlrpc_server_register_method" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="server" type="resource" status="required"></attr>
		<attr name="method_name" type="string" status="required"></attr>
		<attr name="function" type="string" status="required"></attr>
	</tag>
	<tag name="xmlrpc_set_type" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="&amp;value" type="string" status="required"></attr>
		<attr name="type" type="string" status="required"></attr>
	</tag>
</tags>