<!DOCTYPE tags>
<tags>
	<tag name="bzclose" type="function" returnType="int">
		<attr name="bz" type="resource"></attr>
	</tag>
	<tag name="bzcompress" type="function" returnType="string">
		<attr name="source" type="string"></attr>
		<attr name="blocksize" type="int" status="optional"></attr>
		<attr name="workfactor" type="int" status="optional"></attr>
	</tag>
	<tag name="bzdecompress" type="function" returnType="string">
		<attr name="source" type="string"></attr>
		<attr name="small" type="int" status="optional"></attr>
	</tag>
	<tag name="bzerrno" type="function" returnType="int">
		<attr name="bz" type="resource"></attr>
	</tag>
	<tag name="bzerror" type="function" returnType="array">
		<attr name="bz" type="resource"></attr>
	</tag>
	<tag name="bzerrstr" type="function" returnType="string">
		<attr name="bz" type="resource"></attr>
	</tag>
	<tag name="bzflush" type="function" returnType="int">
		<attr name="bz" type="resource"></attr>
	</tag>
	<tag name="bzopen" type="function" returnType="resource">
		<attr name="filename" type="string"></attr>
		<attr name="mode" type="string"></attr>
	</tag>
	<tag name="bzread" type="function" returnType="string">
		<attr name="bz" type="resource"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="bzwrite" type="function" returnType="int">
		<attr name="bz" type="resource"></attr>
		<attr name="data" type="string"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
</tags>