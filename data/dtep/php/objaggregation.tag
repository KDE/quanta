<!DOCTYPE tags>
<tags>
	<tag name="aggregate_info" type="function" returnType="array">
		<attr name="object" type="object"></attr>
	</tag>
	<tag name="aggregate_methods_by_list" type="function" returnType="void">
		<attr name="object" type="object"></attr>
		<attr name="class_name" type="string"></attr>
		<attr name="methods_list" type="array"></attr>
		<attr name="exclude" type="bool" status="optional"></attr>
	</tag>
	<tag name="aggregate_methods_by_regexp" type="function" returnType="void">
		<attr name="object" type="object"></attr>
		<attr name="class_name" type="string"></attr>
		<attr name="regexp" type="string"></attr>
		<attr name="exclude" type="bool" status="optional"></attr>
	</tag>
	<tag name="aggregate_methods" type="function" returnType="void">
		<attr name="object" type="object"></attr>
		<attr name="class_name" type="string"></attr>
	</tag>
	<tag name="aggregate_properties_by_list" type="function" returnType="void">
		<attr name="object" type="object"></attr>
		<attr name="class_name" type="string"></attr>
		<attr name="properties_list" type="array"></attr>
		<attr name="exclude" type="bool" status="optional"></attr>
	</tag>
	<tag name="aggregate_properties_by_regexp" type="function" returnType="void">
		<attr name="object" type="object"></attr>
		<attr name="class_name" type="string"></attr>
		<attr name="regexp" type="string"></attr>
		<attr name="exclude" type="bool" status="optional"></attr>
	</tag>
	<tag name="aggregate_properties" type="function" returnType="void">
		<attr name="object" type="object"></attr>
		<attr name="class_name" type="string"></attr>
	</tag>
	<tag name="aggregate" type="function" returnType="void">
		<attr name="object" type="object"></attr>
		<attr name="class_name" type="string"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="deaggregate" type="function" returnType="void">
		<attr name="object" type="object"></attr>
		<attr name="class_name" type="string" status="optional"></attr>
	</tag>
</tags>