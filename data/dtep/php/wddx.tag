<!DOCTYPE tags>
<tags>
	<tag name="wddx_add_vars" type="function" returnType="bool">
		<attr name="packet_id" type="int"></attr>
		<attr name="name_var" type="mixed"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="wddx_deserialize" type="function" returnType="mixed">
		<attr name="packet" type="string"></attr>
	</tag>
	<tag name="wddx_packet_end" type="function" returnType="string">
		<attr name="packet_id" type="int"></attr>
	</tag>
	<tag name="wddx_packet_start" type="function" returnType="int">
		<attr name="comment" type="string" status="optional"></attr>
	</tag>
	<tag name="wddx_serialize_value" type="function" returnType="string">
		<attr name="var" type="mixed"></attr>
		<attr name="comment" type="string" status="optional"></attr>
	</tag>
	<tag name="wddx_serialize_vars" type="function" returnType="string">
		<attr name="var_name" type="mixed"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
</tags>