<!DOCTYPE tags>
<tags>
	<tag name="call_user_func_array" type="function" returnType="mixed" comment="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="function" type="callback" status="required"></attr>
		<attr name="param_arr" type="array" status="required"></attr>
	</tag>
	<tag name="call_user_func" type="function" returnType="mixed" comment="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="function" type="callback" status="required"></attr>
		<attr name="parameter" type="mixed" status="optional"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="create_function" type="function" returnType="string" comment="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="args" type="string" status="required"></attr>
		<attr name="code" type="string" status="required"></attr>
	</tag>
	<tag name="func_get_arg" type="function" returnType="mixed" comment="PHP 4 , PHP 5">
		<attr name="arg_num" type="int" status="required"></attr>
	</tag>
	<tag name="func_get_args" type="function" returnType="array" comment="PHP 4 , PHP 5">
	</tag>
	<tag name="func_num_args" type="function" returnType="int" comment="PHP 4 , PHP 5">
	</tag>
	<tag name="function_exists" type="function" returnType="bool" comment="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="function_name" type="string" status="required"></attr>
	</tag>
	<tag name="get_defined_functions" type="function" returnType="array" comment="PHP 4 &gt;= 4.0.4, PHP 5">
	</tag>
	<tag name="register_shutdown_function" type="function" returnType="void" comment="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="function" type="callback" status="required"></attr>
		<attr name="parameter" type="mixed" status="optional"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="register_tick_function" type="function" returnType="void" comment="PHP 4 &gt;= 4.0.3, PHP 5">
		<attr name="function" type="callback" status="required"></attr>
		<attr name="arg" type="mixed" status="optional"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="unregister_tick_function" type="function" returnType="void" comment="PHP 4 &gt;= 4.0.3, PHP 5">
		<attr name="function_name" type="string" status="required"></attr>
	</tag>
</tags>