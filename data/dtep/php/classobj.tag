<!DOCTYPE tags>
<tags>
	<tag name="call_user_method_array" type="function" returnType="mixed" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="method_name" type="string" status="required"></attr>
		<attr name="&amp;obj" type="object" status="required"></attr>
		<attr name="paramarr" type="array" status="required"></attr>
	</tag>
	<tag name="call_user_method" type="function" returnType="mixed" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="method_name" type="string" status="required"></attr>
		<attr name="&amp;obj" type="object" status="required"></attr>
		<attr name="parameter" type="mixed" status="optional"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="class_exists" type="function" returnType="bool" version="PHP 4 , PHP 5">
		<attr name="class_name" type="string" status="required"></attr>
		<attr name="autoload" type="bool" status="optional"></attr>
	</tag>
	<tag name="get_class_methods" type="function" returnType="array" version="PHP 4 , PHP 5">
		<attr name="class_name" type="mixed" status="required"></attr>
	</tag>
	<tag name="get_class_vars" type="function" returnType="array" version="PHP 4 , PHP 5">
		<attr name="class_name" type="string" status="required"></attr>
	</tag>
	<tag name="get_class" type="function" returnType="string" version="PHP 4 , PHP 5">
		<attr name="obj" type="object" status="required"></attr>
	</tag>
	<tag name="get_declared_classes" type="function" returnType="array" version="PHP 4 , PHP 5">
	</tag>
	<tag name="get_declared_interfaces" type="function" returnType="array" version="PHP 5">
	</tag>
	<tag name="get_object_vars" type="function" returnType="array" version="PHP 4 , PHP 5">
		<attr name="obj" type="object" status="required"></attr>
	</tag>
	<tag name="get_parent_class" type="function" returnType="string" version="PHP 4 , PHP 5">
		<attr name="obj" type="mixed" status="required"></attr>
	</tag>
	<tag name="interface_exists" type="function" returnType="bool" version="">
		<attr name="interface_name" type="string" status="required"></attr>
		<attr name="autoload" type="bool" status="optional"></attr>
	</tag>
	<tag name="is_a" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="object" type="object" status="required"></attr>
		<attr name="class_name" type="string" status="required"></attr>
	</tag>
	<tag name="is_subclass_of" type="function" returnType="bool" version="PHP 4 , PHP 5">
		<attr name="object" type="object" status="required"></attr>
		<attr name="class_name" type="string" status="required"></attr>
	</tag>
	<tag name="method_exists" type="function" returnType="bool" version="PHP 4 , PHP 5">
		<attr name="object" type="object" status="required"></attr>
		<attr name="method_name" type="string" status="required"></attr>
	</tag>
</tags>