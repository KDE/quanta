<!DOCTYPE tags>
<tags>	<tag name="w32api_deftype" type="function" returnType="bool">
		<attr name="typename" type="string" status="optional"></attr>
		<attr name="member1_type" type="string" status="optional"></attr>
		<attr name="member1_name" type="string" status="optional"></attr>
		<attr name="stringN" type="string" status="optional"></attr>
		<attr name="stringN" type="string" status="optional"></attr>
	</tag>
	<tag name="w32api_init_dtype" type="function" returnType="resource">
		<attr name="typename" type="string" status="optional"></attr>
		<attr name="value" type="mixed" status="optional"></attr>
		<attr name="mixedN" type="mixed" status="optional"></attr>
	</tag>
	<tag name="w32api_invoke_function" type="function" returnType="mixed">
		<attr name="funcname" type="string" status="optional"></attr>
		<attr name="argument" type="mixed" status="optional"></attr>
		<attr name="mixedN" type="mixed" status="optional"></attr>
	</tag>
	<tag name="w32api_register_function" type="function" returnType="bool">
		<attr name="libary" type="string" status="optional"></attr>
		<attr name="function_name" type="string" status="optional"></attr>
		<attr name="return_type" type="string" status="optional"></attr>
	</tag>
	<tag name="w32api_set_call_method" type="function" returnType="void">
		<attr name="method" type="int" status="optional"></attr>
	</tag>
</tags>
