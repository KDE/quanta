<!DOCTYPE tags>
<tags>
	<tag name="assert_options" type="function" returnType="mixed" version="PHP 4 , PHP 5">
		<attr name="what" type="int" status="required"></attr>
		<attr name="value" type="mixed" status="optional"></attr>
	</tag>
	<tag name="assert" type="function" returnType="int" version="PHP 4 , PHP 5">
		<attr name="assertion" type="mixed" status="required"></attr>
	</tag>
	<tag name="dl" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="library" type="string" status="required"></attr>
	</tag>
	<tag name="extension_loaded" type="function" returnType="bool" version="PHP 3&gt;= 3.0.10, PHP 4 , PHP 5">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="get_cfg_var" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="varname" type="string" status="required"></attr>
	</tag>
	<tag name="get_current_user" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
	</tag>
	<tag name="get_defined_constants" type="function" returnType="array" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="categorize" type="mixed" status="optional"></attr>
	</tag>
	<tag name="get_extension_funcs" type="function" returnType="array" version="PHP 4 , PHP 5">
		<attr name="module_name" type="string" status="required"></attr>
	</tag>
	<tag name="get_include_path" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
	</tag>
	<tag name="get_included_files" type="function" returnType="array" version="PHP 4 , PHP 5">
	</tag>
	<tag name="get_loaded_extensions" type="function" returnType="array" version="PHP 4 , PHP 5">
	</tag>
	<tag name="get_magic_quotes_gpc" type="function" returnType="int" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
	</tag>
	<tag name="get_magic_quotes_runtime" type="function" returnType="int" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
	</tag>
	<tag name="getenv" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="varname" type="string" status="required"></attr>
	</tag>
	<tag name="getlastmod" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
	</tag>
	<tag name="getmygid" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="getmyinode" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
	</tag>
	<tag name="getmypid" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
	</tag>
	<tag name="getmyuid" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
	</tag>
	<tag name="getopt" type="function" returnType="array" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="options" type="string" status="required"></attr>
		<attr name="longopts" type="array" status="optional"></attr>
	</tag>
	<tag name="getrusage" type="function" returnType="array" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="who" type="int" status="optional"></attr>
	</tag>
	<tag name="ini_get_all" type="function" returnType="array" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="extension" type="string" status="optional"></attr>
	</tag>
	<tag name="ini_get" type="function" returnType="string" version="PHP 4 , PHP 5">
		<attr name="varname" type="string" status="required"></attr>
	</tag>
	<tag name="ini_restore" type="function" returnType="void" version="PHP 4 , PHP 5">
		<attr name="varname" type="string" status="required"></attr>
	</tag>
	<tag name="ini_set" type="function" returnType="string" version="PHP 4 , PHP 5">
		<attr name="varname" type="string" status="required"></attr>
		<attr name="newvalue" type="string" status="required"></attr>
	</tag>
	<tag name="memory_get_usage" type="function" returnType="int" version="PHP 4 &gt;= 4.3.2, PHP 5">
	</tag>
	<tag name="php_ini_scanned_files" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
	</tag>
	<tag name="php_logo_guid" type="function" returnType="string" version="PHP 4 , PHP 5">
	</tag>
	<tag name="php_sapi_name" type="function" returnType="string" version="PHP 4 &gt;= 4.0.1, PHP 5">
	</tag>
	<tag name="php_uname" type="function" returnType="string" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="mode" type="string" status="optional"></attr>
	</tag>
	<tag name="phpcredits" type="function" returnType="void" version="PHP 4 , PHP 5">
		<attr name="flag" type="int" status="optional"></attr>
	</tag>
	<tag name="phpinfo" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="what" type="int" status="optional"></attr>
	</tag>
	<tag name="phpversion" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
	</tag>
	<tag name="putenv" type="function" returnType="void" version="PHP 3, PHP 4 , PHP 5">
		<attr name="setting" type="string" status="required"></attr>
	</tag>
	<tag name="restore_include_path" type="function" returnType="void" version="PHP 4 &gt;= 4.3.0, PHP 5">
	</tag>
	<tag name="set_include_path" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="new_include_path" type="string" status="required"></attr>
	</tag>
	<tag name="set_magic_quotes_runtime" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="new_setting" type="int" status="required"></attr>
	</tag>
	<tag name="set_time_limit" type="function" returnType="void" version="PHP 3, PHP 4 , PHP 5">
		<attr name="seconds" type="int" status="required"></attr>
	</tag>
	<tag name="version_compare" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="version1" type="string" status="required"></attr>
		<attr name="version2" type="string" status="required"></attr>
		<attr name="operator" type="string" status="optional"></attr>
	</tag>
	<tag name="zend_logo_guid" type="function" returnType="string" version="PHP 4 , PHP 5">
	</tag>
	<tag name="zend_version" type="function" returnType="string" version="PHP 4 , PHP 5">
	</tag>
</tags>