<!DOCTYPE tags>
<tags>
	<tag name="mhash_count" type="function" returnType="int" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
	</tag>
	<tag name="mhash_get_block_size" type="function" returnType="int" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="hash" type="int" status="required"></attr>
	</tag>
	<tag name="mhash_get_hash_name" type="function" returnType="string" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="hash" type="int" status="required"></attr>
	</tag>
	<tag name="mhash_keygen_s2k" type="function" returnType="string" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="hash" type="int" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="salt" type="string" status="required"></attr>
		<attr name="bytes" type="int" status="required"></attr>
	</tag>
	<tag name="mhash" type="function" returnType="string" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="hash" type="int" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
		<attr name="key" type="string" status="optional"></attr>
	</tag>
</tags>