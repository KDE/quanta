<!DOCTYPE tags>
<tags>
	<tag name="iconv_get_encoding" type="function" returnType="mixed" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="type" type="string" status="optional"></attr>
	</tag>
	<tag name="iconv_mime_decode_headers" type="function" returnType="array" version="PHP 5">
		<attr name="encoded_headers" type="string" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="iconv_mime_decode" type="function" returnType="string" version="PHP 5">
		<attr name="encoded_header" type="string" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="iconv_mime_encode" type="function" returnType="string" version="PHP 5">
		<attr name="field_name" type="string" status="required"></attr>
		<attr name="field_value" type="string" status="required"></attr>
		<attr name="preferences" type="array" status="optional"></attr>
	</tag>
	<tag name="iconv_set_encoding" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="type" type="string" status="required"></attr>
		<attr name="charset" type="string" status="required"></attr>
	</tag>
	<tag name="iconv_strlen" type="function" returnType="int" version="PHP 5">
		<attr name="str" type="string" status="required"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="iconv_strpos" type="function" returnType="int" version="PHP 5">
		<attr name="haystack" type="string" status="required"></attr>
		<attr name="needle" type="string" status="required"></attr>
		<attr name="offset" type="int" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="iconv_strrpos" type="function" returnType="string" version="PHP 5">
		<attr name="haystack" type="string" status="required"></attr>
		<attr name="needle" type="string" status="required"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="iconv_substr" type="function" returnType="string" version="PHP 5">
		<attr name="str" type="string" status="required"></attr>
		<attr name="offset" type="int" status="required"></attr>
		<attr name="length" type="int" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="iconv" type="function" returnType="string" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="in_charset" type="string" status="required"></attr>
		<attr name="out_charset" type="string" status="required"></attr>
		<attr name="str" type="string" status="required"></attr>
	</tag>
	<tag name="ob_iconv_handler" type="function" returnType="array" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="contents" type="string" status="required"></attr>
		<attr name="status" type="int" status="required"></attr>
	</tag>
</tags>