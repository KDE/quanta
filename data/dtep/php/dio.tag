<!DOCTYPE tags>
<tags>
	<tag name="dio_close" type="function" returnType="void">
		<attr name="fd" type="resource"></attr>
	</tag>
	<tag name="dio_fcntl" type="function" returnType="mixed">
		<attr name="fd" type="resource"></attr>
		<attr name="cmd" type="int"></attr>
		<attr name="args" type="mixed" status="optional"></attr>
	</tag>
	<tag name="dio_open" type="function" returnType="resource">
		<attr name="filename" type="string"></attr>
		<attr name="flags" type="int"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="dio_read" type="function" returnType="string">
		<attr name="fd" type="resource"></attr>
		<attr name="n" type="int" status="optional"></attr>
	</tag>
	<tag name="dio_seek" type="function" returnType="int">
		<attr name="fd" type="resource"></attr>
		<attr name="pos" type="int"></attr>
		<attr name="whence" type="int" status="optional"></attr>
	</tag>
	<tag name="dio_stat" type="function" returnType="array">
		<attr name="fd" type="resource"></attr>
	</tag>
	<tag name="dio_tcsetattr" type="function" returnType="void">
		<attr name="fd" type="resource"></attr>
		<attr name="options" type="array"></attr>
	</tag>
	<tag name="dio_truncate" type="function" returnType="bool">
		<attr name="fd" type="resource"></attr>
		<attr name="offset" type="int"></attr>
	</tag>
	<tag name="dio_write" type="function" returnType="int">
		<attr name="fd" type="resource"></attr>
		<attr name="data" type="string"></attr>
		<attr name="len" type="int" status="optional"></attr>
	</tag>
</tags>