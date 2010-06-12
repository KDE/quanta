<!DOCTYPE tags>
<tags>
	<tag name="wddx_add_vars" type="function" returnType="none">
		<attr name="packet_id" type="int" status="optional"></attr>
		<attr name="name_var" type="mixed" status="optional"></attr>
		<attr name="mixedN" type="mixed" status="optional"></attr>
	</tag>
	<tag name="wddx_deserialize" type="function" returnType="mixed">
		<attr name="packet" type="string" status="optional"></attr>
	</tag>
	<tag name="wddx_packet_end" type="function" returnType="string">
		<attr name="packet_id" type="int" status="optional"></attr>
	</tag>
	<tag name="wddx_packet_start" type="function" returnType="int">
		<attr name="comment" type="string" status="optional"></attr>
	</tag>
	<tag name="wddx_serialize_value" type="function" returnType="string">
		<attr name="var" type="mixed" status="optional"></attr>
		<attr name="comment" type="string" status="optional"></attr>
	</tag>
	<tag name="wddx_serialize_vars" type="function" returnType="string">
		<attr name="var_name" type="mixed" status="optional"></attr>
		<attr name="mixedN" type="mixed" status="optional"></attr>
	</tag>
</tags>
