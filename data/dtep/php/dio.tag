<!DOCTYPE tags>
<tags>
	<tag name="dio_close" type="function" returnType="void" langVersion="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="fd" type="resource" status="required"></attr>
	</tag>
	<tag name="dio_fcntl" type="function" returnType="mixed" langVersion="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="fd" type="resource" status="required"></attr>
		<attr name="cmd" type="int" status="required"></attr>
		<attr name="args" type="mixed" status="optional"></attr>
	</tag>
	<tag name="dio_open" type="function" returnType="resource" langVersion="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="flags" type="int" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="dio_read" type="function" returnType="string" langVersion="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="fd" type="resource" status="required"></attr>
		<attr name="n" type="int" status="optional"></attr>
	</tag>
	<tag name="dio_seek" type="function" returnType="int" langVersion="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="fd" type="resource" status="required"></attr>
		<attr name="pos" type="int" status="required"></attr>
		<attr name="whence" type="int" status="optional"></attr>
	</tag>
	<tag name="dio_stat" type="function" returnType="array" langVersion="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="fd" type="resource" status="required"></attr>
	</tag>
	<tag name="dio_tcsetattr" type="function" returnType="void" langVersion="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="fd" type="resource" status="required"></attr>
		<attr name="options" type="array" status="required"></attr>
	</tag>
	<tag name="dio_truncate" type="function" returnType="bool" langVersion="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="fd" type="resource" status="required"></attr>
		<attr name="offset" type="int" status="required"></attr>
	</tag>
	<tag name="dio_write" type="function" returnType="int" langVersion="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="fd" type="resource" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
		<attr name="len" type="int" status="optional"></attr>
	</tag>
</tags>