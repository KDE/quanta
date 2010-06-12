<!DOCTYPE tags>
<tags>	<tag name="session_cache_expire" type="function" returnType="int">
		<attr name="new_cache_expire" type="int" status="optional"></attr>
	</tag>
	<tag name="session_cache_limiter" type="function" returnType="string">
		<attr name="cache_limiter" type="string" status="optional"></attr>
	</tag>
	<tag name="session_decode" type="function" returnType="bool">
		<attr name="data" type="string" status="optional"></attr>
	</tag>
	<tag name="session_destroy" type="function" returnType="bool">
	</tag>
	<tag name="session_encode" type="function" returnType="string">
	</tag>
	<tag name="session_get_cookie_params" type="function" returnType="array">
	</tag>
	<tag name="session_id" type="function" returnType="string">
		<attr name="id" type="string" status="optional"></attr>
	</tag>
	<tag name="session_is_registered" type="function" returnType="bool">
		<attr name="name" type="string" status="optional"></attr>
	</tag>
	<tag name="session_module_name" type="function" returnType="string">
		<attr name="module" type="string" status="optional"></attr>
	</tag>
	<tag name="session_name" type="function" returnType="string">
		<attr name="name" type="string" status="optional"></attr>
	</tag>
	<tag name="session_readonly" type="function" returnType="void">
	</tag>
	<tag name="session_register" type="function" returnType="bool">
		<attr name="name" type="mixed" status="optional"></attr>
		<attr name="mixedN" type="mixed" status="optional"></attr>
	</tag>
	<tag name="session_save_path" type="function" returnType="string">
		<attr name="path" type="string" status="optional"></attr>
	</tag>
	<tag name="session_set_cookie_params" type="function" returnType="void">
		<attr name="lifetime" type="int" status="optional"></attr>
		<attr name="path" type="string" status="optional"></attr>
		<attr name="domain" type="string" status="optional"></attr>
		<attr name="secure" type="bool" status="optional"></attr>
	</tag>
	<tag name="session_set_save_handler" type="function" returnType="bool">
		<attr name="open" type="string" status="optional"></attr>
		<attr name="close" type="string" status="optional"></attr>
		<attr name="read" type="string" status="optional"></attr>
		<attr name="write" type="string" status="optional"></attr>
		<attr name="destroy" type="string" status="optional"></attr>
		<attr name="gc" type="string" status="optional"></attr>
	</tag>
	<tag name="session_start" type="function" returnType="bool">
	</tag>
	<tag name="session_unregister" type="function" returnType="bool">
		<attr name="name" type="string" status="optional"></attr>
	</tag>
	<tag name="session_unset" type="function" returnType="void">
	</tag>
	<tag name="session_write_close" type="function" returnType="void">
	</tag>
</tags>
