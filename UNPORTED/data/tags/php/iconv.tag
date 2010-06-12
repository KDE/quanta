<!DOCTYPE tags>
<tags>	<tag name="iconv_get_encoding" type="function" returnType="array">
		<attr name="type" type="string" status="optional"></attr>
	</tag>
	<tag name="iconv_set_encoding" type="function" returnType="array">
		<attr name="type" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="iconv" type="function" returnType="string">
		<attr name="in_charset" type="string" status="optional"></attr>
		<attr name="out_charset" type="string" status="optional"></attr>
		<attr name="str" type="string" status="optional"></attr>
	</tag>
	<tag name="ob_iconv_handler" type="function" returnType="array">
		<attr name="contents" type="string" status="optional"></attr>
		<attr name="status" type="int" status="optional"></attr>
	</tag>
</tags>
