<!DOCTYPE tags>
<tags>
	<tag name="COM::COM" type="function" returnType="object">
		<attr name="module_name" type="string"></attr>
		<attr name="server_name" type="mixed" status="optional"></attr>
		<attr name="codepage" type="int" status="optional"></attr>
		<attr name="typelib" type="string" status="optional"></attr>
	</tag>
	<tag name="DOTNET::DOTNET" type="function" returnType="string">
		<attr name="assembly name" type="string"></attr>
		<attr name="class_name" type="string"></attr>
		<attr name="codepage" type="int" status="optional"></attr>
	</tag>
	<tag name="VARIANT::VARIANT" type="function" returnType="object">
		<attr name="value" type="mixed" status="optional"></attr>
		<attr name="type" type="int" status="optional"></attr>
		<attr name="codepage" type="int" status="optional"></attr>
	</tag>
	<tag name="com_addref" type="function" returnType="void">
	</tag>
	<tag name="com_create_guid" type="function" returnType="string">
	</tag>
	<tag name="com_event_sink" type="function" returnType="bool">
		<attr name="comobject" type="object"></attr>
		<attr name="sinkobject" type="object"></attr>
		<attr name="sinkinterface" type="mixed" status="optional"></attr>
	</tag>
	<tag name="com_get_active_object" type="function" returnType="object">
		<attr name="progid" type="string"></attr>
		<attr name="code_page" type="int" status="optional"></attr>
	</tag>
	<tag name="com_get" type="function" returnType="mixed">
		<attr name="com_object" type="resource"></attr>
		<attr name="property" type="string"></attr>
	</tag>
	<tag name="com_invoke" type="function" returnType="mixed">
		<attr name="com_object" type="resource"></attr>
		<attr name="function_name" type="string"></attr>
		<attr name="function_parameters" type="mixed" status="optional"></attr>
	</tag>
	<tag name="com_isenum" type="function" returnType="bool">
		<attr name="com_module" type="object"></attr>
	</tag>
	<tag name="com_load_typelib" type="function" returnType="bool">
		<attr name="typelib_name" type="string"></attr>
		<attr name="case_insensitive" type="bool" status="optional"></attr>
	</tag>
	<tag name="com_load" type="function" returnType="resource">
		<attr name="module_name" type="string"></attr>
		<attr name="server_name" type="string" status="optional"></attr>
		<attr name="codepage" type="int" status="optional"></attr>
	</tag>
	<tag name="com_message_pump" type="function" returnType="bool">
		<attr name="timeoutms" type="int" status="optional"></attr>
	</tag>
	<tag name="com_print_typeinfo" type="function" returnType="bool">
		<attr name="comobject" type="object"></attr>
		<attr name="dispinterface" type="string" status="optional"></attr>
		<attr name="wantsink" type="bool" status="optional"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="com_release" type="function" returnType="void">
	</tag>
	<tag name="com_set" type="function" returnType="void">
		<attr name="com_object" type="resource"></attr>
		<attr name="property" type="string"></attr>
		<attr name="value" type="mixed"></attr>
	</tag>
	<tag name="variant_abs" type="function" returnType="mixed">
		<attr name="val" type="mixed"></attr>
	</tag>
	<tag name="variant_add" type="function" returnType="mixed">
		<attr name="left" type="mixed"></attr>
		<attr name="right" type="mixed"></attr>
	</tag>
	<tag name="variant_and" type="function" returnType="mixed">
		<attr name="left" type="mixed"></attr>
		<attr name="right" type="mixed"></attr>
	</tag>
	<tag name="variant_cast" type="function" returnType="object">
		<attr name="variant" type="object"></attr>
		<attr name="type" type="int"></attr>
	</tag>
	<tag name="variant_cat" type="function" returnType="mixed">
		<attr name="left" type="mixed"></attr>
		<attr name="right" type="mixed"></attr>
	</tag>
	<tag name="variant_cmp" type="function" returnType="int">
		<attr name="left" type="mixed"></attr>
		<attr name="right" type="mixed"></attr>
		<attr name="lcid" type="int" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="variant_date_from_timestamp" type="function" returnType="object">
		<attr name="timestamp" type="int"></attr>
	</tag>
	<tag name="variant_date_to_timestamp" type="function" returnType="int">
		<attr name="variant" type="object"></attr>
	</tag>
	<tag name="variant_div" type="function" returnType="mixed">
		<attr name="left" type="mixed"></attr>
		<attr name="right" type="mixed"></attr>
	</tag>
	<tag name="variant_eqv" type="function" returnType="mixed">
		<attr name="left" type="mixed"></attr>
		<attr name="right" type="mixed"></attr>
	</tag>
	<tag name="variant_fix" type="function" returnType="mixed">
		<attr name="variant" type="mixed"></attr>
	</tag>
	<tag name="variant_get_type" type="function" returnType="int">
		<attr name="variant" type="object"></attr>
	</tag>
	<tag name="variant_idiv" type="function" returnType="mixed">
		<attr name="left" type="mixed"></attr>
		<attr name="right" type="mixed"></attr>
	</tag>
	<tag name="variant_imp" type="function" returnType="mixed">
		<attr name="left" type="mixed"></attr>
		<attr name="right" type="mixed"></attr>
	</tag>
	<tag name="variant_int" type="function" returnType="mixed">
		<attr name="variant" type="mixed"></attr>
	</tag>
	<tag name="variant_mod" type="function" returnType="mixed">
		<attr name="left" type="mixed"></attr>
		<attr name="right" type="mixed"></attr>
	</tag>
	<tag name="variant_mul" type="function" returnType="mixed">
		<attr name="left" type="mixed"></attr>
		<attr name="right" type="mixed"></attr>
	</tag>
	<tag name="variant_neg" type="function" returnType="mixed">
		<attr name="variant" type="mixed"></attr>
	</tag>
	<tag name="variant_not" type="function" returnType="mixed">
		<attr name="variant" type="mixed"></attr>
	</tag>
	<tag name="variant_or" type="function" returnType="mixed">
		<attr name="left" type="mixed"></attr>
		<attr name="right" type="mixed"></attr>
	</tag>
	<tag name="variant_pow" type="function" returnType="mixed">
		<attr name="left" type="mixed"></attr>
		<attr name="right" type="mixed"></attr>
	</tag>
	<tag name="variant_round" type="function" returnType="mixed">
		<attr name="variant" type="mixed"></attr>
		<attr name="decimals" type="int"></attr>
	</tag>
	<tag name="variant_set_type" type="function" returnType="void">
		<attr name="variant" type="object"></attr>
		<attr name="type" type="int"></attr>
	</tag>
	<tag name="variant_set" type="function" returnType="void">
		<attr name="variant" type="object"></attr>
		<attr name="value" type="mixed"></attr>
	</tag>
	<tag name="variant_sub" type="function" returnType="mixed">
		<attr name="left" type="mixed"></attr>
		<attr name="right" type="mixed"></attr>
	</tag>
	<tag name="variant_xor" type="function" returnType="mixed">
		<attr name="left" type="mixed"></attr>
		<attr name="right" type="mixed"></attr>
	</tag>
</tags>