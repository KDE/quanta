<!DOCTYPE tags>
<tags>
	<tag name="base64_decode" type="function" returnType="string">
		<attr name="encoded_data" type="string"></attr>
	</tag>
	<tag name="base64_encode" type="function" returnType="string">
		<attr name="data" type="string"></attr>
	</tag>
	<tag name="get_headers" type="function" returnType="array">
		<attr name="url" type="string"></attr>
		<attr name="format" type="int" status="optional"></attr>
	</tag>
	<tag name="get_meta_tags" type="function" returnType="array">
		<attr name="filename" type="string"></attr>
		<attr name="use_include_path" type="bool" status="optional"></attr>
	</tag>
	<tag name="http_build_query" type="function" returnType="string">
		<attr name="formdata" type="array"></attr>
		<attr name="numeric_prefix" type="string" status="optional"></attr>
	</tag>
	<tag name="parse_url" type="function" returnType="array">
		<attr name="url" type="string"></attr>
	</tag>
	<tag name="rawurldecode" type="function" returnType="string">
		<attr name="str" type="string"></attr>
	</tag>
	<tag name="rawurlencode" type="function" returnType="string">
		<attr name="str" type="string"></attr>
	</tag>
	<tag name="urldecode" type="function" returnType="string">
		<attr name="str" type="string"></attr>
	</tag>
	<tag name="urlencode" type="function" returnType="string">
		<attr name="str" type="string"></attr>
	</tag>
</tags>