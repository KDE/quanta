<!DOCTYPE tags>
<tags>
	<tag name="mailparse_determine_best_xfer_encoding" type="function" returnType="int" version="4.1.0 - 4.1.2 only">
		<attr name="fp" type="resource" status="required"></attr>
	</tag>
	<tag name="mailparse_msg_create" type="function" returnType="int" version="4.1.0 - 4.1.2 only">
	</tag>
	<tag name="mailparse_msg_extract_part_file" type="function" returnType="string" version="4.1.0 - 4.1.2 only">
		<attr name="rfc2045" type="resource" status="required"></attr>
		<attr name="filename" type="string" status="required"></attr>
		<attr name="callbackfunc" type="callback" status="optional"></attr>
	</tag>
	<tag name="mailparse_msg_extract_part" type="function" returnType="void" version="4.1.0 - 4.1.2 only">
		<attr name="rfc2045" type="resource" status="required"></attr>
		<attr name="msgbody" type="string" status="required"></attr>
		<attr name="callbackfunc" type="callback" status="optional"></attr>
	</tag>
	<tag name="mailparse_msg_free" type="function" returnType="void" version="4.1.0 - 4.1.2 only">
		<attr name="rfc2045buf" type="resource" status="required"></attr>
	</tag>
	<tag name="mailparse_msg_get_part_data" type="function" returnType="array" version="4.1.0 - 4.1.2 only">
		<attr name="rfc2045" type="resource" status="required"></attr>
	</tag>
	<tag name="mailparse_msg_get_part" type="function" returnType="int" version="4.1.0 - 4.1.2 only">
		<attr name="rfc2045" type="resource" status="required"></attr>
		<attr name="mimesection" type="string" status="required"></attr>
	</tag>
	<tag name="mailparse_msg_get_structure" type="function" returnType="array" version="4.1.0 - 4.1.2 only">
		<attr name="rfc2045" type="resource" status="required"></attr>
	</tag>
	<tag name="mailparse_msg_parse_file" type="function" returnType="resource" version="4.1.0 - 4.1.2 only">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="mailparse_msg_parse" type="function" returnType="void" version="4.1.0 - 4.1.2 only">
		<attr name="rfc2045buf" type="resource" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
	</tag>
	<tag name="mailparse_rfc822_parse_addresses" type="function" returnType="array" version="4.1.0 - 4.1.2 only">
		<attr name="addresses" type="string" status="required"></attr>
	</tag>
	<tag name="mailparse_stream_encode" type="function" returnType="bool" version="4.1.0 - 4.1.2 only">
		<attr name="sourcefp" type="resource" status="required"></attr>
		<attr name="destfp" type="resource" status="required"></attr>
		<attr name="encoding" type="string" status="required"></attr>
	</tag>
	<tag name="mailparse_uudecode_all" type="function" returnType="array" version="">
		<attr name="fp" type="resource" status="required"></attr>
	</tag>
</tags>