<!DOCTYPE tags>
<tags>
	<tag name="wddx_add_vars" type="function" returnType="bool" langVersion="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="packet_id" type="int" status="required"></attr>
		<attr name="name_var" type="mixed" status="required"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="wddx_deserialize" type="function" returnType="mixed" langVersion="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="packet" type="string" status="required"></attr>
	</tag>
	<tag name="wddx_packet_end" type="function" returnType="string" langVersion="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="packet_id" type="int" status="required"></attr>
	</tag>
	<tag name="wddx_packet_start" type="function" returnType="int" langVersion="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="comment" type="string" status="optional"></attr>
	</tag>
	<tag name="wddx_serialize_value" type="function" returnType="string" langVersion="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="var" type="mixed" status="required"></attr>
		<attr name="comment" type="string" status="optional"></attr>
	</tag>
	<tag name="wddx_serialize_vars" type="function" returnType="string" langVersion="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="var_name" type="mixed" status="required"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
</tags>