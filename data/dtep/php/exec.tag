<!DOCTYPE tags>
<tags>
	<tag name="escapeshellarg" type="function" returnType="string" version="PHP 4 &gt;= 4.0.3, PHP 5">
		<attr name="arg" type="string" status="required"></attr>
	</tag>
	<tag name="escapeshellcmd" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="command" type="string" status="required"></attr>
	</tag>
	<tag name="exec" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="command" type="string" status="required"></attr>
		<attr name="&amp;output" type="array" status="optional"></attr>
		<attr name="&amp;return_var" type="int" status="optional"></attr>
	</tag>
	<tag name="passthru" type="function" returnType="void" version="PHP 3, PHP 4 , PHP 5">
		<attr name="command" type="string" status="required"></attr>
		<attr name="&amp;return_var" type="int" status="optional"></attr>
	</tag>
	<tag name="proc_close" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="process" type="resource" status="required"></attr>
	</tag>
	<tag name="proc_get_status" type="function" returnType="array" version="PHP 5">
		<attr name="process" type="resource" status="required"></attr>
	</tag>
	<tag name="proc_nice" type="function" returnType="bool" version="PHP 5">
		<attr name="increment" type="int" status="required"></attr>
	</tag>
	<tag name="proc_open" type="function" returnType="resource" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="cmd" type="string" status="required"></attr>
		<attr name="descriptorspec" type="array" status="required"></attr>
		<attr name="&amp;pipes" type="array" status="required"></attr>
		<attr name="cwd" type="string" status="optional"></attr>
		<attr name="env" type="array" status="optional"></attr>
		<attr name="other_options" type="array" status="optional"></attr>
	</tag>
	<tag name="proc_terminate" type="function" returnType="int" version="PHP 5">
		<attr name="process" type="resource" status="required"></attr>
		<attr name="signal" type="int" status="optional"></attr>
	</tag>
	<tag name="shell_exec" type="function" returnType="string" version="PHP 4 , PHP 5">
		<attr name="cmd" type="string" status="required"></attr>
	</tag>
	<tag name="system" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="command" type="string" status="required"></attr>
		<attr name="&amp;return_var" type="int" status="optional"></attr>
	</tag>
</tags>