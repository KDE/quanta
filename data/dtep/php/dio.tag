<!DOCTYPE tags>
<tags>	<tag name="dio_close" type="function" returnType="void">
		<attr name="fd" type="resource" status="optional"></attr>
	</tag>
	<tag name="dio_fcntl" type="function" returnType="mixed">
		<attr name="fd" type="resource" status="optional"></attr>
		<attr name="cmd" type="int" status="optional"></attr>
		<attr name="arg" type="mixed" status="optional"></attr>
	</tag>
	<tag name="dio_open" type="function" returnType="resource">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="dio_read" type="function" returnType="string">
		<attr name="fd" type="resource" status="optional"></attr>
		<attr name="n" type="int" status="optional"></attr>
	</tag>
	<tag name="dio_seek" type="function" returnType="int">
		<attr name="fd" type="resource" status="optional"></attr>
		<attr name="pos" type="int" status="optional"></attr>
		<attr name="whence" type="int" status="optional"></attr>
	</tag>
	<tag name="dio_stat" type="function" returnType="array">
		<attr name="fd" type="resource" status="optional"></attr>
	</tag>
	<tag name="dio_tcsetattr" type="function" returnType="none">
		<attr name="fd" type="resource" status="optional"></attr>
		<attr name="options" type="array" status="optional"></attr>
	</tag>
	<tag name="dio_truncate" type="function" returnType="bool">
		<attr name="fd" type="resource" status="optional"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="dio_write" type="function" returnType="int">
		<attr name="fd" type="resource" status="optional"></attr>
		<attr name="data" type="string" status="optional"></attr>
		<attr name="len" type="int" status="optional"></attr>
	</tag>
</tags>
