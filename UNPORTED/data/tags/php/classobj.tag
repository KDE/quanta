<!DOCTYPE tags>
<tags>	<tag name="call_user_method_array" type="function" returnType="mixed">
		<attr name="method_name" type="string" status="optional"></attr>
		<attr name="obj" type="object" status="optional"></attr>
		<attr name="paramarr" type="array" status="optional"></attr>
	</tag>
	<tag name="call_user_method" type="function" returnType="mixed">
		<attr name="method_name" type="string" status="optional"></attr>
		<attr name="obj" type="object" status="optional"></attr>
		<attr name="parameter" type="mixed" status="optional"></attr>
		<attr name="mixedN" type="mixed" status="optional"></attr>
	</tag>
	<tag name="class_exists" type="function" returnType="bool">
		<attr name="class_name" type="string" status="optional"></attr>
	</tag>
	<tag name="get_class_methods" type="function" returnType="array">
		<attr name="class_name" type="mixed" status="optional"></attr>
	</tag>
	<tag name="get_class_vars" type="function" returnType="array">
		<attr name="class_name" type="string" status="optional"></attr>
	</tag>
	<tag name="get_class" type="function" returnType="string">
		<attr name="obj" type="object" status="optional"></attr>
	</tag>
	<tag name="get_declared_classes" type="function" returnType="array">
	</tag>
	<tag name="get_object_vars" type="function" returnType="array">
		<attr name="obj" type="object" status="optional"></attr>
	</tag>
	<tag name="get_parent_class" type="function" returnType="string">
		<attr name="obj" type="mixed" status="optional"></attr>
	</tag>
	<tag name="is_a" type="function" returnType="bool">
		<attr name="object" type="object" status="optional"></attr>
		<attr name="class_name" type="string" status="optional"></attr>
	</tag>
	<tag name="is_subclass_of" type="function" returnType="bool">
		<attr name="object" type="object" status="optional"></attr>
		<attr name="class_name" type="string" status="optional"></attr>
	</tag>
	<tag name="method_exists" type="function" returnType="bool">
		<attr name="object" type="object" status="optional"></attr>
		<attr name="method_name" type="string" status="optional"></attr>
	</tag>
</tags>
