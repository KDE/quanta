<!DOCTYPE tags>
<tags>
	<tag name="bzclose" type="function" returnType="int" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="bz" type="resource" status="required"></attr>
	</tag>
	<tag name="bzcompress" type="function" returnType="string" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="source" type="string" status="required"></attr>
		<attr name="blocksize" type="int" status="optional"></attr>
		<attr name="workfactor" type="int" status="optional"></attr>
	</tag>
	<tag name="bzdecompress" type="function" returnType="string" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="source" type="string" status="required"></attr>
		<attr name="small" type="int" status="optional"></attr>
	</tag>
	<tag name="bzerrno" type="function" returnType="int" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="bz" type="resource" status="required"></attr>
	</tag>
	<tag name="bzerror" type="function" returnType="array" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="bz" type="resource" status="required"></attr>
	</tag>
	<tag name="bzerrstr" type="function" returnType="string" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="bz" type="resource" status="required"></attr>
	</tag>
	<tag name="bzflush" type="function" returnType="int" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="bz" type="resource" status="required"></attr>
	</tag>
	<tag name="bzopen" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="mode" type="string" status="required"></attr>
	</tag>
	<tag name="bzread" type="function" returnType="string" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="bz" type="resource" status="required"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="bzwrite" type="function" returnType="int" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="bz" type="resource" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
</tags>