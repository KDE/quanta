<!DOCTYPE tags>
<tags>
	<tag name="mailparse_determine_best_xfer_encoding" type="function" returnType="int">
		<attr name="fp" type="resource"></attr>
	</tag>
	<tag name="mailparse_msg_create" type="function" returnType="int">
	</tag>
	<tag name="mailparse_msg_extract_part_file" type="function" returnType="string">
		<attr name="rfc2045" type="resource"></attr>
		<attr name="filename" type="string"></attr>
		<attr name="callbackfunc" type="callback" status="optional"></attr>
	</tag>
	<tag name="mailparse_msg_extract_part" type="function" returnType="void">
		<attr name="rfc2045" type="resource"></attr>
		<attr name="msgbody" type="string"></attr>
		<attr name="callbackfunc" type="callback" status="optional"></attr>
	</tag>
	<tag name="mailparse_msg_free" type="function" returnType="void">
		<attr name="rfc2045buf" type="resource"></attr>
	</tag>
	<tag name="mailparse_msg_get_part_data" type="function" returnType="array">
		<attr name="rfc2045" type="resource"></attr>
	</tag>
	<tag name="mailparse_msg_get_part" type="function" returnType="int">
		<attr name="rfc2045" type="resource"></attr>
		<attr name="mimesection" type="string"></attr>
	</tag>
	<tag name="mailparse_msg_get_structure" type="function" returnType="array">
		<attr name="rfc2045" type="resource"></attr>
	</tag>
	<tag name="mailparse_msg_parse_file" type="function" returnType="resource">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="mailparse_msg_parse" type="function" returnType="void">
		<attr name="rfc2045buf" type="resource"></attr>
		<attr name="data" type="string"></attr>
	</tag>
	<tag name="mailparse_rfc822_parse_addresses" type="function" returnType="array">
		<attr name="addresses" type="string"></attr>
	</tag>
	<tag name="mailparse_stream_encode" type="function" returnType="bool">
		<attr name="sourcefp" type="resource"></attr>
		<attr name="destfp" type="resource"></attr>
		<attr name="encoding" type="string"></attr>
	</tag>
	<tag name="mailparse_uudecode_all" type="function" returnType="array">
		<attr name="fp" type="resource"></attr>
	</tag>
</tags>