<!DOCTYPE tags>
<tags>	<tag name="mhash_count" type="function" returnType="int">
	</tag>
	<tag name="mhash_get_block_size" type="function" returnType="int">
		<attr name="hash" type="int" status="optional"></attr>
	</tag>
	<tag name="mhash_get_hash_name" type="function" returnType="string">
		<attr name="hash" type="int" status="optional"></attr>
	</tag>
	<tag name="mhash_keygen_s2k" type="function" returnType="string">
		<attr name="hash" type="int" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="salt" type="string" status="optional"></attr>
		<attr name="bytes" type="int" status="optional"></attr>
	</tag>
	<tag name="mhash" type="function" returnType="string">
		<attr name="hash" type="int" status="optional"></attr>
		<attr name="data" type="string" status="optional"></attr>
		<attr name="key" type="string" status="optional"></attr>
	</tag>
</tags>
