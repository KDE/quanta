<!DOCTYPE tags>
<tags>
	<tag name="flush" type="function" returnType="void">
	</tag>
	<tag name="ob_clean" type="function" returnType="void">
	</tag>
	<tag name="ob_end_clean" type="function" returnType="bool">
	</tag>
	<tag name="ob_end_flush" type="function" returnType="bool">
	</tag>
	<tag name="ob_flush" type="function" returnType="void">
	</tag>
	<tag name="ob_get_clean" type="function" returnType="string">
	</tag>
	<tag name="ob_get_contents" type="function" returnType="string">
	</tag>
	<tag name="ob_get_flush" type="function" returnType="string">
	</tag>
	<tag name="ob_get_length" type="function" returnType="int">
	</tag>
	<tag name="ob_get_level" type="function" returnType="int">
	</tag>
	<tag name="ob_get_status" type="function" returnType="array">
		<attr name="full_status" type="bool" status="optional"></attr>
	</tag>
	<tag name="ob_gzhandler" type="function" returnType="string">
		<attr name="buffer" type="string"></attr>
		<attr name="mode" type="int"></attr>
	</tag>
	<tag name="ob_implicit_flush" type="function" returnType="void">
		<attr name="flag" type="int" status="optional"></attr>
	</tag>
	<tag name="ob_list_handlers" type="function" returnType="array">
	</tag>
	<tag name="ob_start" type="function" returnType="bool">
		<attr name="output_callback" type="callback" status="optional"></attr>
		<attr name="chunk_size" type="int" status="optional"></attr>
		<attr name="erase" type="bool" status="optional"></attr>
	</tag>
	<tag name="output_add_rewrite_var" type="function" returnType="bool">
		<attr name="name" type="string"></attr>
		<attr name="value" type="string"></attr>
	</tag>
	<tag name="output_reset_rewrite_vars" type="function" returnType="bool">
	</tag>
</tags>