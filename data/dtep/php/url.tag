<!DOCTYPE tags>
<tags>
	<tag name="base64_decode" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="encoded_data" type="string" status="required"></attr>
	</tag>
	<tag name="base64_encode" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="data" type="string" status="required"></attr>
	</tag>
	<tag name="get_headers" type="function" returnType="array" version="PHP 5">
		<attr name="url" type="string" status="required"></attr>
		<attr name="format" type="int" status="optional"></attr>
	</tag>
	<tag name="get_meta_tags" type="function" returnType="array" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="use_include_path" type="bool" status="optional"></attr>
	</tag>
	<tag name="http_build_query" type="function" returnType="string" version="PHP 5">
		<attr name="formdata" type="array" status="required"></attr>
		<attr name="numeric_prefix" type="string" status="optional"></attr>
	</tag>
	<tag name="parse_url" type="function" returnType="array" version="PHP 3, PHP 4 , PHP 5">
		<attr name="url" type="string" status="required"></attr>
	</tag>
	<tag name="rawurldecode" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
	</tag>
	<tag name="rawurlencode" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
	</tag>
	<tag name="urldecode" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
	</tag>
	<tag name="urlencode" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
	</tag>
</tags>