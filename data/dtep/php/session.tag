<!DOCTYPE tags>
<tags>
	<tag name="session_cache_expire" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="new_cache_expire" type="int" status="optional"></attr>
	</tag>
	<tag name="session_cache_limiter" type="function" returnType="string" version="PHP 4 &gt;= 4.0.3, PHP 5">
		<attr name="cache_limiter" type="string" status="optional"></attr>
	</tag>
	<tag name="session_decode" type="function" returnType="bool" version="PHP 4 , PHP 5">
		<attr name="data" type="string" status="required"></attr>
	</tag>
	<tag name="session_destroy" type="function" returnType="bool" version="PHP 4 , PHP 5">
	</tag>
	<tag name="session_encode" type="function" returnType="string" version="PHP 4 , PHP 5">
	</tag>
	<tag name="session_get_cookie_params" type="function" returnType="array" version="PHP 4 , PHP 5">
	</tag>
	<tag name="session_id" type="function" returnType="string" version="PHP 4 , PHP 5">
		<attr name="id" type="string" status="optional"></attr>
	</tag>
	<tag name="session_is_registered" type="function" returnType="bool" version="PHP 4 , PHP 5">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="session_module_name" type="function" returnType="string" version="PHP 4 , PHP 5">
		<attr name="module" type="string" status="optional"></attr>
	</tag>
	<tag name="session_name" type="function" returnType="string" version="PHP 4 , PHP 5">
		<attr name="name" type="string" status="optional"></attr>
	</tag>
	<tag name="session_regenerate_id" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.2, PHP 5">
	</tag>
	<tag name="session_register" type="function" returnType="bool" version="PHP 4 , PHP 5">
		<attr name="name" type="mixed" status="required"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="session_save_path" type="function" returnType="string" version="PHP 4 , PHP 5">
		<attr name="path" type="string" status="optional"></attr>
	</tag>
	<tag name="session_set_cookie_params" type="function" returnType="void" version="PHP 4 , PHP 5">
		<attr name="lifetime" type="int" status="required"></attr>
		<attr name="path" type="string" status="optional"></attr>
		<attr name="domain" type="string" status="optional"></attr>
		<attr name="secure" type="bool" status="optional"></attr>
	</tag>
	<tag name="session_set_save_handler" type="function" returnType="bool" version="PHP 4 , PHP 5">
		<attr name="open" type="string" status="required"></attr>
		<attr name="close" type="string" status="required"></attr>
		<attr name="read" type="string" status="required"></attr>
		<attr name="write" type="string" status="required"></attr>
		<attr name="destroy" type="string" status="required"></attr>
		<attr name="gc" type="string" status="required"></attr>
	</tag>
	<tag name="session_start" type="function" returnType="bool" version="PHP 4 , PHP 5">
	</tag>
	<tag name="session_unregister" type="function" returnType="bool" version="PHP 4 , PHP 5">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="session_unset" type="function" returnType="void" version="PHP 4 , PHP 5">
	</tag>
	<tag name="session_write_close" type="function" returnType="void" version="PHP 4 &gt;= 4.0.4, PHP 5">
	</tag>
</tags>