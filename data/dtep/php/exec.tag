<!DOCTYPE tags>
<tags>
	<tag name="escapeshellarg" type="function" returnType="string">
		<attr name="arg" type="string"></attr>
	</tag>
	<tag name="escapeshellcmd" type="function" returnType="string">
		<attr name="command" type="string"></attr>
	</tag>
	<tag name="exec" type="function" returnType="string">
		<attr name="command" type="string"></attr>
		<attr name="&amp;output" type="array" status="optional"></attr>
		<attr name="&amp;return_var" type="int" status="optional"></attr>
	</tag>
	<tag name="passthru" type="function" returnType="void">
		<attr name="command" type="string"></attr>
		<attr name="&amp;return_var" type="int" status="optional"></attr>
	</tag>
	<tag name="proc_close" type="function" returnType="int">
		<attr name="process" type="resource"></attr>
	</tag>
	<tag name="proc_get_status" type="function" returnType="array">
		<attr name="process" type="resource"></attr>
	</tag>
	<tag name="proc_nice" type="function" returnType="bool">
		<attr name="increment" type="int"></attr>
	</tag>
	<tag name="proc_open" type="function" returnType="resource">
		<attr name="cmd" type="string"></attr>
		<attr name="descriptorspec" type="array"></attr>
		<attr name="&amp;pipes" type="array"></attr>
		<attr name="cwd" type="string" status="optional"></attr>
		<attr name="env" type="array" status="optional"></attr>
		<attr name="other_options" type="array" status="optional"></attr>
	</tag>
	<tag name="proc_terminate" type="function" returnType="int">
		<attr name="process" type="resource"></attr>
		<attr name="signal" type="int" status="optional"></attr>
	</tag>
	<tag name="shell_exec" type="function" returnType="string">
		<attr name="cmd" type="string"></attr>
	</tag>
	<tag name="system" type="function" returnType="string">
		<attr name="command" type="string"></attr>
		<attr name="&amp;return_var" type="int" status="optional"></attr>
	</tag>
</tags>