<!DOCTYPE tags>
<tags>	<tag name="apache_child_terminate" type="function" returnType="bool">
	</tag>
	<tag name="apache_lookup_uri" type="function" returnType="object">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="apache_note" type="function" returnType="string">
		<attr name="note_name" type="string"></attr>
		<attr name="note_value" type="string" status="optional"></attr>
	</tag>
	<tag name="apache_request_headers" type="function" returnType="array">
	</tag>
	<tag name="apache_response_headers" type="function" returnType="array">
	</tag>
	<tag name="apache_setenv" type="function" returnType="int">
		<attr name="variable" type="string" status="optional"></attr>
		<attr name="value" type="string" status="optional"></attr>
		<attr name="walk_to_top" type="bool" status="optional"></attr>
	</tag>
	<tag name="ascii2ebcdic" type="function" returnType="int">
		<attr name="ascii_str" type="string" status="optional"></attr>
	</tag>
	<tag name="ebcdic2ascii" type="function" returnType="int">
		<attr name="ebcdic_str" type="string" status="optional"></attr>
	</tag>
	<tag name="getallheaders" type="function" returnType="array">
	</tag>
	<tag name="virtual" type="function" returnType="int">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
</tags>
