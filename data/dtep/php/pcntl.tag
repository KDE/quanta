<!DOCTYPE tags>
<tags>
	<tag name="pcntl_alarm" type="function" returnType="int">
		<attr name="seconds" type="int"></attr>
	</tag>
	<tag name="pcntl_exec" type="function" returnType="bool">
		<attr name="path" type="string"></attr>
		<attr name="args" type="array" status="optional"></attr>
		<attr name="envs" type="array" status="optional"></attr>
	</tag>
	<tag name="pcntl_fork" type="function" returnType="int">
	</tag>
	<tag name="pcntl_getpriority" type="function" returnType="int">
		<attr name="pid" type="int" status="optional"></attr>
		<attr name="process_identifier" type="int" status="optional"></attr>
	</tag>
	<tag name="pcntl_setpriority" type="function" returnType="bool">
		<attr name="priority" type="int"></attr>
		<attr name="pid" type="int" status="optional"></attr>
		<attr name="process_identifier" type="int" status="optional"></attr>
	</tag>
	<tag name="pcntl_signal" type="function" returnType="bool">
		<attr name="signo" type="int"></attr>
		<attr name="handle" type="callback"></attr>
		<attr name="restart_syscalls" type="bool" status="optional"></attr>
	</tag>
	<tag name="pcntl_wait" type="function" returnType="int">
		<attr name="&amp;status" type="int"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="pcntl_waitpid" type="function" returnType="int">
		<attr name="pid" type="int"></attr>
		<attr name="&amp;status" type="int"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="pcntl_wexitstatus" type="function" returnType="int">
		<attr name="status" type="int"></attr>
	</tag>
	<tag name="pcntl_wifexited" type="function" returnType="int">
		<attr name="status" type="int"></attr>
	</tag>
	<tag name="pcntl_wifsignaled" type="function" returnType="int">
		<attr name="status" type="int"></attr>
	</tag>
	<tag name="pcntl_wifstopped" type="function" returnType="int">
		<attr name="status" type="int"></attr>
	</tag>
	<tag name="pcntl_wstopsig" type="function" returnType="int">
		<attr name="status" type="int"></attr>
	</tag>
	<tag name="pcntl_wtermsig" type="function" returnType="int">
		<attr name="status" type="int"></attr>
	</tag>
</tags>