<!DOCTYPE tags>
<tags>
	<tag name="connection_aborted" type="function" returnType="int" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
	</tag>
	<tag name="connection_status" type="function" returnType="int" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
	</tag>
	<tag name="connection_timeout" type="function" returnType="bool" version="PHP 3&gt;= 3.0.7, PHP 4  &lt;= 4.0.4">
	</tag>
	<tag name="constant" type="function" returnType="mixed" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="define" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="name" type="string" status="required"></attr>
		<attr name="value" type="mixed" status="required"></attr>
		<attr name="case_insensitive" type="bool" status="optional"></attr>
	</tag>
	<tag name="defined" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="eval" type="function" returnType="mixed" version="">
		<attr name="code_str" type="string" status="required"></attr>
	</tag>
	<tag name="exit" type="function" returnType="void" version="">
		<attr name="status" type="string" status="optional"></attr>
	</tag>
	<tag name="get_browser" type="function" returnType="object" version="PHP 3, PHP 4 , PHP 5">
		<attr name="user_agent" type="string" status="optional"></attr>
		<attr name="return_array" type="bool" status="optional"></attr>
	</tag>
	<tag name="highlight_file" type="function" returnType="mixed" version="PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="return" type="bool" status="required"></attr>
	</tag>
	<tag name="highlight_string" type="function" returnType="mixed" version="PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
		<attr name="return" type="bool" status="optional"></attr>
	</tag>
	<tag name="ignore_user_abort" type="function" returnType="int" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="setting" type="bool" status="optional"></attr>
	</tag>
	<tag name="pack" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="format" type="string" status="required"></attr>
		<attr name="args" type="mixed" status="optional"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="php_check_syntax" type="function" returnType="bool" version="PHP 5">
		<attr name="file_name" type="string" status="required"></attr>
		<attr name="&amp;error_message" type="string" status="optional"></attr>
	</tag>
	<tag name="php_strip_whitespace" type="function" returnType="string" version="PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="sleep" type="function" returnType="void" version="PHP 3, PHP 4 , PHP 5">
		<attr name="seconds" type="int" status="required"></attr>
	</tag>
	<tag name="time_nanosleep" type="function" returnType="mixed" version="PHP 5">
		<attr name="seconds" type="int" status="required"></attr>
		<attr name="nanoseconds" type="int" status="required"></attr>
	</tag>
	<tag name="uniqid" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="prefix" type="string" status="optional"></attr>
		<attr name="more_entropy" type="bool" status="optional"></attr>
	</tag>
	<tag name="unpack" type="function" returnType="array" version="PHP 3, PHP 4 , PHP 5">
		<attr name="format" type="string" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
	</tag>
	<tag name="usleep" type="function" returnType="void" version="PHP 3, PHP 4 , PHP 5">
		<attr name="micro_seconds" type="int" status="required"></attr>
	</tag>
</tags>