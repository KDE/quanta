<!DOCTYPE tags>
<tags>
	<tag name="call_user_func_array" type="function" returnType="mixed">
		<attr name="function" type="callback"></attr>
		<attr name="param_arr" type="array"></attr>
	</tag>
	<tag name="call_user_func" type="function" returnType="mixed">
		<attr name="function" type="callback"></attr>
		<attr name="parameter" type="mixed" status="optional"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="create_function" type="function" returnType="string">
		<attr name="args" type="string"></attr>
		<attr name="code" type="string"></attr>
	</tag>
	<tag name="func_get_arg" type="function" returnType="mixed">
		<attr name="arg_num" type="int"></attr>
	</tag>
	<tag name="func_get_args" type="function" returnType="array">
	</tag>
	<tag name="func_num_args" type="function" returnType="int">
	</tag>
	<tag name="function_exists" type="function" returnType="bool">
		<attr name="function_name" type="string"></attr>
	</tag>
	<tag name="get_defined_functions" type="function" returnType="array">
	</tag>
	<tag name="register_shutdown_function" type="function" returnType="void">
		<attr name="function" type="callback"></attr>
		<attr name="parameter" type="mixed" status="optional"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="register_tick_function" type="function" returnType="void">
		<attr name="function" type="callback"></attr>
		<attr name="arg" type="mixed" status="optional"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="unregister_tick_function" type="function" returnType="void">
		<attr name="function_name" type="string"></attr>
	</tag>
</tags>