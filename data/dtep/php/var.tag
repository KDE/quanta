<!DOCTYPE tags>
<tags>
	<tag name="debug_zval_dump" type="function" returnType="void" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="variable" type="mixed" status="required"></attr>
	</tag>
	<tag name="empty" type="function" returnType="bool" version="">
		<attr name="var" type="mixed" status="required"></attr>
	</tag>
	<tag name="floatval" type="function" returnType="float" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="var" type="mixed" status="required"></attr>
	</tag>
	<tag name="get_defined_vars" type="function" returnType="array" version="PHP 4 &gt;= 4.0.4, PHP 5">
	</tag>
	<tag name="get_resource_type" type="function" returnType="string" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="gettype" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="var" type="mixed" status="required"></attr>
	</tag>
	<tag name="import_request_variables" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="types" type="string" status="required"></attr>
		<attr name="prefix" type="string" status="optional"></attr>
	</tag>
	<tag name="intval" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="var" type="mixed" status="required"></attr>
		<attr name="base" type="int" status="optional"></attr>
	</tag>
	<tag name="is_array" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="var" type="mixed" status="required"></attr>
	</tag>
	<tag name="is_bool" type="function" returnType="bool" version="PHP 4 , PHP 5">
		<attr name="var" type="mixed" status="required"></attr>
	</tag>
	<tag name="is_callable" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="var" type="mixed" status="required"></attr>
		<attr name="syntax_only" type="bool" status="optional"></attr>
		<attr name="&amp;callable_name" type="string" status="optional"></attr>
	</tag>
	<tag name="is_float" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="var" type="mixed" status="required"></attr>
	</tag>
	<tag name="is_int" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="var" type="mixed" status="required"></attr>
	</tag>
	<tag name="is_null" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="var" type="mixed" status="required"></attr>
	</tag>
	<tag name="is_numeric" type="function" returnType="bool" version="PHP 4 , PHP 5">
		<attr name="var" type="mixed" status="required"></attr>
	</tag>
	<tag name="is_object" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="var" type="mixed" status="required"></attr>
	</tag>
	<tag name="is_resource" type="function" returnType="bool" version="PHP 4 , PHP 5">
		<attr name="var" type="mixed" status="required"></attr>
	</tag>
	<tag name="is_scalar" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="var" type="mixed" status="required"></attr>
	</tag>
	<tag name="is_string" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="var" type="mixed" status="required"></attr>
	</tag>
	<tag name="isset" type="function" returnType="bool" version="">
		<attr name="var" type="mixed" status="required"></attr>
		<attr name="var" type="mixed" status="optional"></attr>
		<attr name="..." type="" status="optional"></attr>
	</tag>
	<tag name="print_r" type="function" returnType="bool" version="PHP 4 , PHP 5">
		<attr name="expression" type="mixed" status="required"></attr>
		<attr name="return" type="bool" status="optional"></attr>
	</tag>
	<tag name="serialize" type="function" returnType="string" version="PHP 3&gt;= 3.0.5, PHP 4 , PHP 5">
		<attr name="value" type="mixed" status="required"></attr>
	</tag>
	<tag name="settype" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="&amp;var" type="mixed" status="required"></attr>
		<attr name="type" type="string" status="required"></attr>
	</tag>
	<tag name="strval" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="var" type="mixed" status="required"></attr>
	</tag>
	<tag name="unserialize" type="function" returnType="mixed" version="PHP 3&gt;= 3.0.5, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
	</tag>
	<tag name="unset" type="function" returnType="void" version="">
		<attr name="var" type="mixed" status="required"></attr>
		<attr name="var" type="mixed" status="optional"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="var_dump" type="function" returnType="void" version="PHP 3&gt;= 3.0.5, PHP 4 , PHP 5">
		<attr name="expression" type="mixed" status="required"></attr>
		<attr name="expression" type="mixed" status="optional"></attr>
		<attr name="..." type="" status="optional"></attr>
	</tag>
	<tag name="var_export" type="function" returnType="mixed" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="expression" type="mixed" status="required"></attr>
		<attr name="return" type="bool" status="optional"></attr>
	</tag>
</tags>