<!DOCTYPE tags>
<tags>
	<tag name="msession_connect" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="host" type="string" status="required"></attr>
		<attr name="port" type="string" status="required"></attr>
	</tag>
	<tag name="msession_count" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
	</tag>
	<tag name="msession_create" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="session" type="string" status="required"></attr>
	</tag>
	<tag name="msession_destroy" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="msession_disconnect" type="function" returnType="void" version="PHP 4 &gt;= 4.2.0, PHP 5">
	</tag>
	<tag name="msession_find" type="function" returnType="array" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="name" type="string" status="required"></attr>
		<attr name="value" type="string" status="required"></attr>
	</tag>
	<tag name="msession_get_array" type="function" returnType="array" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="session" type="string" status="required"></attr>
	</tag>
	<tag name="msession_get_data" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="session" type="string" status="required"></attr>
	</tag>
	<tag name="msession_get" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="session" type="string" status="required"></attr>
		<attr name="name" type="string" status="required"></attr>
		<attr name="value" type="string" status="required"></attr>
	</tag>
	<tag name="msession_inc" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="session" type="string" status="required"></attr>
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="msession_list" type="function" returnType="array" version="PHP 4 &gt;= 4.2.0, PHP 5">
	</tag>
	<tag name="msession_listvar" type="function" returnType="array" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="msession_lock" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="msession_plugin" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="session" type="string" status="required"></attr>
		<attr name="val" type="string" status="required"></attr>
		<attr name="param" type="string" status="optional"></attr>
	</tag>
	<tag name="msession_randstr" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="param" type="int" status="required"></attr>
	</tag>
	<tag name="msession_set_array" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="session" type="string" status="required"></attr>
		<attr name="tuples" type="array" status="required"></attr>
	</tag>
	<tag name="msession_set_data" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="session" type="string" status="required"></attr>
		<attr name="value" type="string" status="required"></attr>
	</tag>
	<tag name="msession_set" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="session" type="string" status="required"></attr>
		<attr name="name" type="string" status="required"></attr>
		<attr name="value" type="string" status="required"></attr>
	</tag>
	<tag name="msession_timeout" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="session" type="string" status="required"></attr>
		<attr name="param" type="int" status="optional"></attr>
	</tag>
	<tag name="msession_uniq" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="param" type="int" status="required"></attr>
	</tag>
	<tag name="msession_unlock" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="session" type="string" status="required"></attr>
		<attr name="key" type="int" status="required"></attr>
	</tag>
</tags>