<!DOCTYPE tags>
<tags>
	<tag name="w32api_deftype" type="function" returnType="bool" version="4.2.0 - 4.2.3 only">
		<attr name="typename" type="string" status="required"></attr>
		<attr name="member1_type" type="string" status="required"></attr>
		<attr name="member1_name" type="string" status="required"></attr>
		<attr name="..." type="string" status="optional"></attr>
		<attr name="..." type="string" status="optional"></attr>
	</tag>
	<tag name="w32api_init_dtype" type="function" returnType="resource" version="4.2.0 - 4.2.3 only">
		<attr name="typename" type="string" status="required"></attr>
		<attr name="value" type="mixed" status="required"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="w32api_invoke_function" type="function" returnType="mixed" version="4.2.0 - 4.2.3 only">
		<attr name="funcname" type="string" status="required"></attr>
		<attr name="argument" type="mixed" status="required"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="w32api_register_function" type="function" returnType="bool" version="4.2.0 - 4.2.3 only">
		<attr name="library" type="string" status="required"></attr>
		<attr name="function_name" type="string" status="required"></attr>
		<attr name="return_type" type="string" status="required"></attr>
	</tag>
	<tag name="w32api_set_call_method" type="function" returnType="void" version="4.2.0 - 4.2.3 only">
		<attr name="method" type="int" status="required"></attr>
	</tag>
</tags>