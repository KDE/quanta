<!DOCTYPE tags>
<tags>
	<tag name="empty" type="function" returnType="boolean">
		<attr name="var" type="mixed" status="optional"></attr>
	</tag>
	<tag name="floatval" type="function" returnType="float">
		<attr name="var" type="mixed" status="optional"></attr>
	</tag>
	<tag name="get_defined_vars" type="function" returnType="array">
	</tag>
	<tag name="get_resource_type" type="function" returnType="string">
		<attr name="handle" type="resource" status="optional"></attr>
	</tag>
	<tag name="gettype" type="function" returnType="string">
		<attr name="var" type="mixed" status="optional"></attr>
	</tag>
	<tag name="import_request_variables" type="function" returnType="bool">
		<attr name="types" type="string" status="optional"></attr>
		<attr name="prefix" type="string" status="optional"></attr>
	</tag>
	<tag name="intval" type="function" returnType="int">
		<attr name="var" type="mixed" status="optional"></attr>
		<attr name="base" type="int" status="optional"></attr>
	</tag>
	<tag name="is_array" type="function" returnType="bool">
		<attr name="var" type="mixed" status="optional"></attr>
	</tag>
	<tag name="is_bool" type="function" returnType="bool">
		<attr name="var" type="mixed" status="optional"></attr>
	</tag>
	<tag name="is_callable" type="function" returnType="bool">
		<attr name="var" type="mixed" status="optional"></attr>
		<attr name="syntax_only" type="bool" status="optional"></attr>
		<attr name="callable_name" type="string" status="optional"></attr>
	</tag>
	<tag name="is_float" type="function" returnType="bool">
		<attr name="var" type="mixed" status="optional"></attr>
	</tag>
	<tag name="is_int" type="function" returnType="bool">
		<attr name="var" type="mixed" status="optional"></attr>
	</tag>
	<tag name="is_null" type="function" returnType="bool">
		<attr name="var" type="mixed" status="optional"></attr>
	</tag>
	<tag name="is_numeric" type="function" returnType="bool">
		<attr name="var" type="mixed" status="optional"></attr>
	</tag>
	<tag name="is_object" type="function" returnType="bool">
		<attr name="var" type="mixed" status="optional"></attr>
	</tag>
	<tag name="is_resource" type="function" returnType="bool">
		<attr name="var" type="mixed" status="optional"></attr>
	</tag>
	<tag name="is_scalar" type="function" returnType="bool">
		<attr name="var" type="mixed" status="optional"></attr>
	</tag>
	<tag name="is_string" type="function" returnType="bool">
		<attr name="var" type="mixed" status="optional"></attr>
	</tag>
	<tag name="isset" type="function" returnType="bool">
		<attr name="var" type="mixed" status="optional"></attr>
		<attr name="var" type="mixed" status="optional"></attr>
	</tag>
	<tag name="print_r" type="function" returnType="void">
		<attr name="expression" type="mixed" status="optional"></attr>
	</tag>
	<tag name="serialize" type="function" returnType="string">
		<attr name="value" type="mixed" status="optional"></attr>
	</tag>
	<tag name="settype" type="function" returnType="bool">
		<attr name="var" type="mixed" status="optional"></attr>
		<attr name="type" type="string" status="optional"></attr>
	</tag>
	<tag name="strval" type="function" returnType="string">
		<attr name="var" type="mixed" status="optional"></attr>
	</tag>
	<tag name="unserialize" type="function" returnType="mixed">
		<attr name="str" type="string" status="optional"></attr>
	</tag>
	<tag name="unset" type="function" returnType="void">
		<attr name="var" type="mixed" status="optional"></attr>
		<attr name="var" type="mixed" status="optional"></attr>
	</tag>
	<tag name="var_dump" type="function" returnType="void">
		<attr name="expression" type="mixed" status="optional"></attr>
		<attr name="expression" type="mixed" status="optional"></attr>
	</tag>
	<tag name="var_export" type="function" returnType="mixed">
		<attr name="expression" type="mixed" status="optional"></attr>
		<attr name="return" type="bool" status="optional"></attr>
	</tag>
</tags>
