<!DOCTYPE tags>
<tags>	<tag name="com_addref" type="function" returnType="void">
	</tag>
	<tag name="com_get" type="function" returnType="mixed">
		<attr name="com_object" type="resource" status="optional"></attr>
		<attr name="property" type="string" status="optional"></attr>
	</tag>
	<tag name="com_invoke" type="function" returnType="mixed">
		<attr name="com_object" type="resource" status="optional"></attr>
		<attr name="function_name" type="string" status="optional"></attr>
		<attr name="function parameters" type="mixed" status="optional"></attr>
	</tag>
	<tag name="com_isenum" type="function" returnType="void">
		<attr name="com_module" type="object" status="optional"></attr>
	</tag>
	<tag name="com_load_typelib" type="function" returnType="void">
		<attr name="typelib_name" type="string" status="optional"></attr>
		<attr name="case_insensitive" type="int" status="optional"></attr>
	</tag>
	<tag name="com_load" type="function" returnType="string">
		<attr name="module name" type="string" status="optional"></attr>
		<attr name="server name" type="string" status="optional"></attr>
		<attr name="codepage" type="int" status="optional"></attr>
	</tag>
	<tag name="com_propget" type="function" returnType="mixed">
		<attr name="com_object" type="resource" status="optional"></attr>
		<attr name="property" type="string" status="optional"></attr>
	</tag>
	<tag name="com_propput" type="function" returnType="void">
		<attr name="com_object" type="resource" status="optional"></attr>
		<attr name="property" type="string" status="optional"></attr>
		<attr name="value" type="mixed" status="optional"></attr>
	</tag>
	<tag name="com_propset" type="function" returnType="void">
		<attr name="com_object" type="resource" status="optional"></attr>
		<attr name="property" type="string" status="optional"></attr>
		<attr name="value" type="mixed" status="optional"></attr>
	</tag>
	<tag name="com_release" type="function" returnType="void">
	</tag>
	<tag name="com_set" type="function" returnType="void">
		<attr name="com_object" type="resource" status="optional"></attr>
		<attr name="property" type="string" status="optional"></attr>
		<attr name="value" type="mixed" status="optional"></attr>
	</tag>
</tags>
