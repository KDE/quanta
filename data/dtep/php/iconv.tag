<!DOCTYPE tags>
<tags>
	<tag name="iconv_get_encoding" type="function" returnType="mixed">
		<attr name="type" type="string" status="optional"></attr>
	</tag>
	<tag name="iconv_mime_decode_headers" type="function" returnType="array">
		<attr name="encoded_headers" type="string"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="iconv_mime_decode" type="function" returnType="string">
		<attr name="encoded_header" type="string"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="iconv_mime_encode" type="function" returnType="string">
		<attr name="field_name" type="string"></attr>
		<attr name="field_value" type="string"></attr>
		<attr name="preferences" type="array" status="optional"></attr>
	</tag>
	<tag name="iconv_set_encoding" type="function" returnType="bool">
		<attr name="type" type="string"></attr>
		<attr name="charset" type="string"></attr>
	</tag>
	<tag name="iconv_strlen" type="function" returnType="int">
		<attr name="str" type="string"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="iconv_strpos" type="function" returnType="int">
		<attr name="haystack" type="string"></attr>
		<attr name="needle" type="string"></attr>
		<attr name="offset" type="int" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="iconv_strrpos" type="function" returnType="string">
		<attr name="haystack" type="string"></attr>
		<attr name="needle" type="string"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="iconv_substr" type="function" returnType="string">
		<attr name="str" type="string"></attr>
		<attr name="offset" type="int"></attr>
		<attr name="length" type="int" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="iconv" type="function" returnType="string">
		<attr name="in_charset" type="string"></attr>
		<attr name="out_charset" type="string"></attr>
		<attr name="str" type="string"></attr>
	</tag>
	<tag name="ob_iconv_handler" type="function" returnType="array">
		<attr name="contents" type="string"></attr>
		<attr name="status" type="int"></attr>
	</tag>
</tags>