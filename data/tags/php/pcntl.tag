<!DOCTYPE tags>
<tags>	<tag name="pcntl_exec" type="function" returnType="bool">
		<attr name="path" type="string" status="optional"></attr>
		<attr name="args" type="array" status="optional"></attr>
		<attr name="envs" type="array" status="optional"></attr>
	</tag>
	<tag name="pcntl_fork" type="function" returnType="int">
	</tag>
	<tag name="pcntl_signal" type="function" returnType="bool">
		<attr name="signo" type="int" status="optional"></attr>
		<attr name="handle" type="mixed" status="optional"></attr>
	</tag>
	<tag name="pcntl_waitpid" type="function" returnType="int">
		<attr name="pid" type="int" status="optional"></attr>
		<attr name="status" type="int" status="optional"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="pcntl_wexitstatus" type="function" returnType="int">
		<attr name="status" type="int" status="optional"></attr>
	</tag>
	<tag name="pcntl_wifexited" type="function" returnType="int">
		<attr name="status" type="int" status="optional"></attr>
	</tag>
	<tag name="pcntl_wifsignaled" type="function" returnType="int">
		<attr name="status" type="int" status="optional"></attr>
	</tag>
	<tag name="pcntl_wifstopped" type="function" returnType="int">
		<attr name="status" type="int" status="optional"></attr>
	</tag>
	<tag name="pcntl_wstopsig" type="function" returnType="int">
		<attr name="status" type="int" status="optional"></attr>
	</tag>
	<tag name="pcntl_wtermsig" type="function" returnType="int">
		<attr name="status" type="int" status="optional"></attr>
	</tag>
</tags>
