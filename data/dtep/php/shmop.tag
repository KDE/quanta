<!DOCTYPE tags>
<tags>
	<tag name="shmop_close" type="function" returnType="int">
		<attr name="shmid" type="int"></attr>
	</tag>
	<tag name="shmop_delete" type="function" returnType="int">
		<attr name="shmid" type="int"></attr>
	</tag>
	<tag name="shmop_open" type="function" returnType="int">
		<attr name="key" type="int"></attr>
		<attr name="flags" type="string"></attr>
		<attr name="mode" type="int"></attr>
		<attr name="size" type="int"></attr>
	</tag>
	<tag name="shmop_read" type="function" returnType="string">
		<attr name="shmid" type="int"></attr>
		<attr name="start" type="int"></attr>
		<attr name="count" type="int"></attr>
	</tag>
	<tag name="shmop_size" type="function" returnType="int">
		<attr name="shmid" type="int"></attr>
	</tag>
	<tag name="shmop_write" type="function" returnType="int">
		<attr name="shmid" type="int"></attr>
		<attr name="data" type="string"></attr>
		<attr name="offset" type="int"></attr>
	</tag>
</tags>