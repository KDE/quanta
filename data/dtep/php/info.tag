<!DOCTYPE tags>
<tags>
	<tag name="assert_options" type="function" returnType="mixed">
		<attr name="what" type="int"></attr>
		<attr name="value" type="mixed" status="optional"></attr>
	</tag>
	<tag name="assert" type="function" returnType="int">
		<attr name="assertion" type="mixed"></attr>
	</tag>
	<tag name="dl" type="function" returnType="int">
		<attr name="library" type="string"></attr>
	</tag>
	<tag name="extension_loaded" type="function" returnType="bool">
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="get_cfg_var" type="function" returnType="string">
		<attr name="varname" type="string"></attr>
	</tag>
	<tag name="get_current_user" type="function" returnType="string">
	</tag>
	<tag name="get_defined_constants" type="function" returnType="array">
		<attr name="categorize" type="mixed" status="optional"></attr>
	</tag>
	<tag name="get_extension_funcs" type="function" returnType="array">
		<attr name="module_name" type="string"></attr>
	</tag>
	<tag name="get_include_path" type="function" returnType="string">
	</tag>
	<tag name="get_included_files" type="function" returnType="array">
	</tag>
	<tag name="get_loaded_extensions" type="function" returnType="array">
	</tag>
	<tag name="get_magic_quotes_gpc" type="function" returnType="int">
	</tag>
	<tag name="get_magic_quotes_runtime" type="function" returnType="int">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="getenv" type="function" returnType="string">
		<attr name="varname" type="string"></attr>
	</tag>
	<tag name="getlastmod" type="function" returnType="int">
	</tag>
	<tag name="getmygid" type="function" returnType="int">
	</tag>
	<tag name="getmyinode" type="function" returnType="int">
	</tag>
	<tag name="getmypid" type="function" returnType="int">
	</tag>
	<tag name="getmyuid" type="function" returnType="int">
	</tag>
	<tag name="getopt" type="function" returnType="array">
		<attr name="options" type="string"></attr>
		<attr name="longopts" type="array" status="optional"></attr>
	</tag>
	<tag name="getrusage" type="function" returnType="array">
		<attr name="who" type="int" status="optional"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="ini_get_all" type="function" returnType="array">
		<attr name="extension" type="string" status="optional"></attr>
	</tag>
	<tag name="ini_get" type="function" returnType="string">
		<attr name="varname" type="string"></attr>
	</tag>
	<tag name="ini_restore" type="function" returnType="void">
		<attr name="varname" type="string"></attr>
	</tag>
	<tag name="ini_set" type="function" returnType="string">
		<attr name="varname" type="string"></attr>
		<attr name="newvalue" type="string"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="memory_get_usage" type="function" returnType="int">
	</tag>
	<tag name="php_ini_scanned_files" type="function" returnType="string">
	</tag>
	<tag name="php_logo_guid" type="function" returnType="string">
	</tag>
	<tag name="php_sapi_name" type="function" returnType="string">
	</tag>
	<tag name="php_uname" type="function" returnType="string">
		<attr name="mode" type="string" status="optional"></attr>
	</tag>
	<tag name="phpcredits" type="function" returnType="void">
		<attr name="flag" type="int" status="optional"></attr>
	</tag>
	<tag name="phpinfo" type="function" returnType="int">
		<attr name="what" type="int" status="optional"></attr>
	</tag>
	<tag name="phpversion" type="function" returnType="string">
	</tag>
	<tag name="putenv" type="function" returnType="void">
		<attr name="setting" type="string"></attr>
	</tag>
	<tag name="restore_include_path" type="function" returnType="void">
	</tag>
	<tag name="set_include_path" type="function" returnType="string">
		<attr name="new_include_path" type="string"></attr>
	</tag>
	<tag name="set_magic_quotes_runtime" type="function" returnType="bool">
		<attr name="new_setting" type="int"></attr>
	</tag>
	<tag name="set_time_limit" type="function" returnType="void">
		<attr name="seconds" type="int"></attr>
	</tag>
	<tag name="version_compare" type="function" returnType="int">
		<attr name="version1" type="string"></attr>
		<attr name="version2" type="string"></attr>
		<attr name="operator" type="string" status="optional"></attr>
	</tag>
	<tag name="zend_logo_guid" type="function" returnType="string">
	</tag>
	<tag name="zend_version" type="function" returnType="string">
	</tag>
</tags>