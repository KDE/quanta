<!DOCTYPE tags>
<tags>
	<tag name="aggregate_info" type="function" returnType="array" version="">
		<attr name="object" type="object" status="required"></attr>
	</tag>
	<tag name="aggregate_methods_by_list" type="function" returnType="void" version="PHP 4 &gt;= 4.2.0">
		<attr name="object" type="object" status="required"></attr>
		<attr name="class_name" type="string" status="required"></attr>
		<attr name="methods_list" type="array" status="required"></attr>
		<attr name="exclude" type="bool" status="optional"></attr>
	</tag>
	<tag name="aggregate_methods_by_regexp" type="function" returnType="void" version="PHP 4 &gt;= 4.2.0">
		<attr name="object" type="object" status="required"></attr>
		<attr name="class_name" type="string" status="required"></attr>
		<attr name="regexp" type="string" status="required"></attr>
		<attr name="exclude" type="bool" status="optional"></attr>
	</tag>
	<tag name="aggregate_methods" type="function" returnType="void" version="PHP 4 &gt;= 4.2.0">
		<attr name="object" type="object" status="required"></attr>
		<attr name="class_name" type="string" status="required"></attr>
	</tag>
	<tag name="aggregate_properties_by_list" type="function" returnType="void" version="PHP 4 &gt;= 4.2.0">
		<attr name="object" type="object" status="required"></attr>
		<attr name="class_name" type="string" status="required"></attr>
		<attr name="properties_list" type="array" status="required"></attr>
		<attr name="exclude" type="bool" status="optional"></attr>
	</tag>
	<tag name="aggregate_properties_by_regexp" type="function" returnType="void" version="PHP 4 &gt;= 4.2.0">
		<attr name="object" type="object" status="required"></attr>
		<attr name="class_name" type="string" status="required"></attr>
		<attr name="regexp" type="string" status="required"></attr>
		<attr name="exclude" type="bool" status="optional"></attr>
	</tag>
	<tag name="aggregate_properties" type="function" returnType="void" version="PHP 4 &gt;= 4.2.0">
		<attr name="object" type="object" status="required"></attr>
		<attr name="class_name" type="string" status="required"></attr>
	</tag>
	<tag name="aggregate" type="function" returnType="void" version="PHP 4 &gt;= 4.2.0">
		<attr name="object" type="object" status="required"></attr>
		<attr name="class_name" type="string" status="required"></attr>
	</tag>
	<tag name="deaggregate" type="function" returnType="void" version="PHP 4 &gt;= 4.2.0">
		<attr name="object" type="object" status="required"></attr>
		<attr name="class_name" type="string" status="optional"></attr>
	</tag>
</tags>