<!DOCTYPE tags>
<tags>
	<tag name="msession_connect" type="function" returnType="bool">
		<attr name="host" type="string"></attr>
		<attr name="port" type="string"></attr>
	</tag>
	<tag name="msession_count" type="function" returnType="int">
	</tag>
	<tag name="msession_create" type="function" returnType="bool">
		<attr name="session" type="string"></attr>
	</tag>
	<tag name="msession_destroy" type="function" returnType="bool">
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="msession_disconnect" type="function" returnType="void">
	</tag>
	<tag name="msession_find" type="function" returnType="array">
		<attr name="name" type="string"></attr>
		<attr name="value" type="string"></attr>
	</tag>
	<tag name="msession_get_array" type="function" returnType="array">
		<attr name="session" type="string"></attr>
	</tag>
	<tag name="msession_get_data" type="function" returnType="string">
		<attr name="session" type="string"></attr>
	</tag>
	<tag name="msession_get" type="function" returnType="string">
		<attr name="session" type="string"></attr>
		<attr name="name" type="string"></attr>
		<attr name="value" type="string"></attr>
	</tag>
	<tag name="msession_inc" type="function" returnType="string">
		<attr name="session" type="string"></attr>
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="msession_list" type="function" returnType="array">
	</tag>
	<tag name="msession_listvar" type="function" returnType="array">
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="msession_lock" type="function" returnType="int">
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="msession_plugin" type="function" returnType="string">
		<attr name="session" type="string"></attr>
		<attr name="val" type="string"></attr>
		<attr name="param" type="string" status="optional"></attr>
	</tag>
	<tag name="msession_randstr" type="function" returnType="string">
		<attr name="param" type="int"></attr>
	</tag>
	<tag name="msession_set_array" type="function" returnType="bool">
		<attr name="session" type="string"></attr>
		<attr name="tuples" type="array"></attr>
	</tag>
	<tag name="msession_set_data" type="function" returnType="bool">
		<attr name="session" type="string"></attr>
		<attr name="value" type="string"></attr>
	</tag>
	<tag name="msession_set" type="function" returnType="bool">
		<attr name="session" type="string"></attr>
		<attr name="name" type="string"></attr>
		<attr name="value" type="string"></attr>
	</tag>
	<tag name="msession_timeout" type="function" returnType="int">
		<attr name="session" type="string"></attr>
		<attr name="param" type="int" status="optional"></attr>
	</tag>
	<tag name="msession_uniq" type="function" returnType="string">
		<attr name="param" type="int"></attr>
	</tag>
	<tag name="msession_unlock" type="function" returnType="int">
		<attr name="session" type="string"></attr>
		<attr name="key" type="int"></attr>
	</tag>
</tags>