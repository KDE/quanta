<!DOCTYPE tags>
<tags>
	<tag name="shmop_close" type="function" returnType="int" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="shmid" type="int" status="required"></attr>
	</tag>
	<tag name="shmop_delete" type="function" returnType="int" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="shmid" type="int" status="required"></attr>
	</tag>
	<tag name="shmop_open" type="function" returnType="int" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="key" type="int" status="required"></attr>
		<attr name="flags" type="string" status="required"></attr>
		<attr name="mode" type="int" status="required"></attr>
		<attr name="size" type="int" status="required"></attr>
	</tag>
	<tag name="shmop_read" type="function" returnType="string" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="shmid" type="int" status="required"></attr>
		<attr name="start" type="int" status="required"></attr>
		<attr name="count" type="int" status="required"></attr>
	</tag>
	<tag name="shmop_size" type="function" returnType="int" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="shmid" type="int" status="required"></attr>
	</tag>
	<tag name="shmop_write" type="function" returnType="int" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="shmid" type="int" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
		<attr name="offset" type="int" status="required"></attr>
	</tag>
</tags>