<!DOCTYPE tags>
<tags>
	<tag name="pcntl_alarm" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="seconds" type="int" status="required"></attr>
	</tag>
	<tag name="pcntl_exec" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="path" type="string" status="required"></attr>
		<attr name="args" type="array" status="optional"></attr>
		<attr name="envs" type="array" status="optional"></attr>
	</tag>
	<tag name="pcntl_fork" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="pcntl_getpriority" type="function" returnType="int" version="PHP 5">
		<attr name="pid" type="int" status="optional"></attr>
		<attr name="process_identifier" type="int" status="optional"></attr>
	</tag>
	<tag name="pcntl_setpriority" type="function" returnType="bool" version="PHP 5">
		<attr name="priority" type="int" status="required"></attr>
		<attr name="pid" type="int" status="optional"></attr>
		<attr name="process_identifier" type="int" status="optional"></attr>
	</tag>
	<tag name="pcntl_signal" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="signo" type="int" status="required"></attr>
		<attr name="handle" type="callback" status="required"></attr>
		<attr name="restart_syscalls" type="bool" status="optional"></attr>
	</tag>
	<tag name="pcntl_wait" type="function" returnType="int" version="PHP 5">
		<attr name="&amp;status" type="int" status="required"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="pcntl_waitpid" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="pid" type="int" status="required"></attr>
		<attr name="&amp;status" type="int" status="required"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="pcntl_wexitstatus" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="status" type="int" status="required"></attr>
	</tag>
	<tag name="pcntl_wifexited" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="status" type="int" status="required"></attr>
	</tag>
	<tag name="pcntl_wifsignaled" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="status" type="int" status="required"></attr>
	</tag>
	<tag name="pcntl_wifstopped" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="status" type="int" status="required"></attr>
	</tag>
	<tag name="pcntl_wstopsig" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="status" type="int" status="required"></attr>
	</tag>
	<tag name="pcntl_wtermsig" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="status" type="int" status="required"></attr>
	</tag>
</tags>