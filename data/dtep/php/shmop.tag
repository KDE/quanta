<!DOCTYPE tags>
<tags>	<tag name="shmop_close" type="function" returnType="int">
		<attr name="shmid" type="int" status="optional"></attr>
	</tag>
	<tag name="shmop_delete" type="function" returnType="int">
		<attr name="shmid" type="int" status="optional"></attr>
	</tag>
	<tag name="shmop_open" type="function" returnType="int">
		<attr name="key" type="int" status="optional"></attr>
		<attr name="flags" type="string" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="size" type="int" status="optional"></attr>
	</tag>
	<tag name="shmop_read" type="function" returnType="string">
		<attr name="shmid" type="int" status="optional"></attr>
		<attr name="start" type="int" status="optional"></attr>
		<attr name="count" type="int" status="optional"></attr>
	</tag>
	<tag name="shmop_size" type="function" returnType="int">
		<attr name="shmid" type="int" status="optional"></attr>
	</tag>
	<tag name="shmop_write" type="function" returnType="int">
		<attr name="shmid" type="int" status="optional"></attr>
		<attr name="data" type="string" status="optional"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
</tags>
