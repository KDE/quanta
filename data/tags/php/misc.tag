<!DOCTYPE tags>
<tags>	<tag name="connection_aborted" type="function" returnType="int">
	</tag>
	<tag name="connection_status" type="function" returnType="int">
	</tag>
	<tag name="connection_timeout" type="function" returnType="bool">
	</tag>
	<tag name="constant" type="function" returnType="mixed">
		<attr name="name" type="string" status="optional"></attr>
	</tag>
	<tag name="define" type="function" returnType="bool">
		<attr name="name" type="string" status="optional"></attr>
		<attr name="value" type="mixed" status="optional"></attr>
		<attr name="case_insensitive" type="bool" status="optional"></attr>
	</tag>
	<tag name="defined" type="function" returnType="bool">
		<attr name="name" type="string" status="optional"></attr>
	</tag>
	<tag name="eval" type="function" returnType="mixed">
		<attr name="code_str" type="string" status="optional"></attr>
	</tag>
	<tag name="exit" type="function" returnType="void">
		<attr name="status" type="string" status="optional"></attr>
	</tag>
	<tag name="get_browser" type="function" returnType="object">
		<attr name="user_agent" type="string" status="optional"></attr>
	</tag>
	<tag name="highlight_file" type="function" returnType="mixed">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="return" type="bool" status="optional"></attr>
	</tag>
	<tag name="highlight_string" type="function" returnType="mixed">
		<attr name="str" type="string" status="optional"></attr>
		<attr name="return" type="bool" status="optional"></attr>
	</tag>
	<tag name="ignore_user_abort" type="function" returnType="int">
		<attr name="setting" type="int" status="optional"></attr>
	</tag>
	<tag name="leak" type="function" returnType="void">
		<attr name="bytes" type="int" status="optional"></attr>
	</tag>
	<tag name="pack" type="function" returnType="string">
		<attr name="format" type="string" status="optional"></attr>
		<attr name="args" type="mixed" status="optional"></attr>
	</tag>
	<tag name="show_source" type="function" returnType="bool">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="return" type="bool" status="optional"></attr>
	</tag>
	<tag name="sleep" type="function" returnType="void">
		<attr name="seconds" type="int" status="optional"></attr>
	</tag>
	<tag name="uniqid" type="function" returnType="string">
		<attr name="prefix" type="string" status="optional"></attr>
		<attr name="lcg" type="bool" status="optional"></attr>
	</tag>
	<tag name="unpack" type="function" returnType="array">
		<attr name="format" type="string" status="optional"></attr>
		<attr name="data" type="string" status="optional"></attr>
	</tag>
	<tag name="usleep" type="function" returnType="void">
		<attr name="micro_seconds" type="int" status="optional"></attr>
	</tag>
</tags>
