<!DOCTYPE tags>
<tags>	<tag name="escapeshellarg" type="function" returnType="string">
		<attr name="arg" type="string" status="optional"></attr>
	</tag>
	<tag name="escapeshellcmd" type="function" returnType="string">
		<attr name="command" type="string" status="optional"></attr>
	</tag>
	<tag name="exec" type="function" returnType="string">
		<attr name="command" type="string" status="optional"></attr>
		<attr name="output" type="array" status="optional"></attr>
		<attr name="return_var" type="int" status="optional"></attr>
	</tag>
	<tag name="passthru" type="function" returnType="void">
		<attr name="command" type="string" status="optional"></attr>
		<attr name="return_var" type="int" status="optional"></attr>
	</tag>
	<tag name="proc_close" type="function" returnType="int">
		<attr name="process" type="resource" status="optional"></attr>
	</tag>
	<tag name="proc_open" type="function" returnType="resource">
		<attr name="cmd" type="string" status="optional"></attr>
		<attr name="descriptorspec" type="array" status="optional"></attr>
		<attr name="pipes" type="array" status="optional"></attr>
	</tag>
	<tag name="shell_exec" type="function" returnType="string">
		<attr name="cmd" type="string" status="optional"></attr>
	</tag>
	<tag name="system" type="function" returnType="string">
		<attr name="command" type="string" status="optional"></attr>
		<attr name="return_var" type="int" status="optional"></attr>
	</tag>
</tags>
