<!DOCTYPE tags>
<tags>
	<tag name="mb_convert_case" type="function" returnType="string">
		<attr name="str" type="string"></attr>
		<attr name="mode" type="int"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_convert_encoding" type="function" returnType="string">
		<attr name="str" type="string"></attr>
		<attr name="to-encoding" type="string"></attr>
		<attr name="from-encoding" type="mixed" status="optional"></attr>
	</tag>
	<tag name="mb_convert_kana" type="function" returnType="string">
		<attr name="str" type="string"></attr>
		<attr name="option" type="string" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_convert_variables" type="function" returnType="string">
		<attr name="to-encoding" type="string"></attr>
		<attr name="from-encoding" type="mixed"></attr>
		<attr name="&amp;vars" type="mixed"></attr>
		<attr name="&amp;..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="mb_decode_mimeheader" type="function" returnType="string">
		<attr name="str" type="string"></attr>
	</tag>
	<tag name="mb_decode_numericentity" type="function" returnType="string">
		<attr name="str" type="string"></attr>
		<attr name="convmap" type="array"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_detect_encoding" type="function" returnType="string">
		<attr name="str" type="string"></attr>
		<attr name="encoding_list" type="mixed" status="optional"></attr>
		<attr name="strict" type="bool" status="optional"></attr>
	</tag>
	<tag name="mb_detect_order" type="function" returnType="array">
		<attr name="encoding-list" type="mixed" status="optional"></attr>
	</tag>
	<tag name="mb_encode_mimeheader" type="function" returnType="string">
		<attr name="str" type="string"></attr>
		<attr name="charset" type="string" status="optional"></attr>
		<attr name="transfer-encoding" type="string" status="optional"></attr>
		<attr name="linefeed" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_encode_numericentity" type="function" returnType="string">
		<attr name="str" type="string"></attr>
		<attr name="convmap" type="array"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_ereg_match" type="function" returnType="bool">
		<attr name="pattern" type="string"></attr>
		<attr name="string" type="string"></attr>
		<attr name="option" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_ereg_replace" type="function" returnType="string">
		<attr name="pattern" type="string"></attr>
		<attr name="replacement" type="string"></attr>
		<attr name="string" type="string"></attr>
		<attr name="option" type="array" status="optional"></attr>
	</tag>
	<tag name="mb_ereg_search_getpos" type="function" returnType="array">
	</tag>
	<tag name="mb_ereg_search_getregs" type="function" returnType="array">
	</tag>
	<tag name="mb_ereg_search_init" type="function" returnType="array">
		<attr name="string" type="string"></attr>
		<attr name="pattern" type="string" status="optional"></attr>
		<attr name="option" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_ereg_search_pos" type="function" returnType="array">
		<attr name="pattern" type="string" status="optional"></attr>
		<attr name="option" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_ereg_search_regs" type="function" returnType="array">
		<attr name="pattern" type="string" status="optional"></attr>
		<attr name="option" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_ereg_search_setpos" type="function" returnType="array">
		<attr name="position" type="int"></attr>
	</tag>
	<tag name="mb_ereg_search" type="function" returnType="bool">
		<attr name="pattern" type="string" status="optional"></attr>
		<attr name="option" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_ereg" type="function" returnType="int">
		<attr name="pattern" type="string"></attr>
		<attr name="string" type="string"></attr>
		<attr name="regs" type="array" status="optional"></attr>
	</tag>
	<tag name="mb_eregi_replace" type="function" returnType="string">
		<attr name="pattern" type="string"></attr>
		<attr name="replace" type="string"></attr>
		<attr name="string" type="string"></attr>
	</tag>
	<tag name="mb_eregi" type="function" returnType="int">
		<attr name="pattern" type="string"></attr>
		<attr name="string" type="string"></attr>
		<attr name="regs" type="array" status="optional"></attr>
	</tag>
	<tag name="mb_get_info" type="function" returnType="string">
		<attr name="type" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_http_input" type="function" returnType="string">
		<attr name="type" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_http_output" type="function" returnType="string">
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_internal_encoding" type="function" returnType="mixed">
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_language" type="function" returnType="string">
		<attr name="language" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_list_encodings" type="function" returnType="array">
	</tag>
	<tag name="mb_output_handler" type="function" returnType="string">
		<attr name="contents" type="string"></attr>
		<attr name="status" type="int"></attr>
	</tag>
	<tag name="mb_parse_str" type="function" returnType="bool">
		<attr name="encoded_string" type="string"></attr>
		<attr name="&amp;result" type="array" status="optional"></attr>
	</tag>
	<tag name="mb_preferred_mime_name" type="function" returnType="string">
		<attr name="encoding" type="string"></attr>
	</tag>
	<tag name="mb_regex_encoding" type="function" returnType="string">
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_regex_set_options" type="function" returnType="string">
		<attr name="options" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_send_mail" type="function" returnType="bool">
		<attr name="to" type="string"></attr>
		<attr name="subject" type="string"></attr>
		<attr name="message" type="string"></attr>
		<attr name="additional_headers" type="string" status="optional"></attr>
		<attr name="additional_parameter" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_split" type="function" returnType="array">
		<attr name="pattern" type="string"></attr>
		<attr name="string" type="string"></attr>
		<attr name="limit" type="int" status="optional"></attr>
	</tag>
	<tag name="mb_strcut" type="function" returnType="string">
		<attr name="str" type="string"></attr>
		<attr name="start" type="int"></attr>
		<attr name="length" type="int" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_strimwidth" type="function" returnType="string">
		<attr name="str" type="string"></attr>
		<attr name="start" type="int"></attr>
		<attr name="width" type="int"></attr>
		<attr name="trimmarker" type="string" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_strlen" type="function" returnType="string">
		<attr name="str" type="string"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_strpos" type="function" returnType="int">
		<attr name="haystack" type="string"></attr>
		<attr name="needle" type="string"></attr>
		<attr name="offset" type="int" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_strrpos" type="function" returnType="int">
		<attr name="haystack" type="string"></attr>
		<attr name="needle" type="string"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_strtolower" type="function" returnType="string">
		<attr name="str" type="string"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_strtoupper" type="function" returnType="string">
		<attr name="str" type="string"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_strwidth" type="function" returnType="int">
		<attr name="str" type="string"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_substitute_character" type="function" returnType="mixed">
		<attr name="substrchar" type="mixed" status="optional"></attr>
	</tag>
	<tag name="mb_substr_count" type="function" returnType="int">
		<attr name="haystack" type="string"></attr>
		<attr name="needle" type="string"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="mb_substr" type="function" returnType="string">
		<attr name="str" type="string"></attr>
		<attr name="start" type="int"></attr>
		<attr name="length" type="int" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
</tags>