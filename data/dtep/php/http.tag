<!DOCTYPE tags>
<tags>
	<tag name="header" type="function" returnType="void" version="PHP 3, PHP 4 , PHP 5">
		<attr name="string" type="string" status="required"></attr>
		<attr name="replace" type="bool" status="optional"></attr>
		<attr name="http_response_code" type="int" status="optional"></attr>
	</tag>
	<tag name="headers_list" type="function" returnType="array" version="PHP 5">
	</tag>
	<tag name="headers_sent" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="&amp;file" type="string" status="optional"></attr>
		<attr name="&amp;line" type="int" status="optional"></attr>
	</tag>
	<tag name="setcookie" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="name" type="string" status="required"></attr>
		<attr name="value" type="string" status="optional"></attr>
		<attr name="expire" type="int" status="optional"></attr>
		<attr name="path" type="string" status="optional"></attr>
		<attr name="domain" type="string" status="optional"></attr>
		<attr name="secure" type="bool" status="optional"></attr>
	</tag>
	<tag name="setrawcookie" type="function" returnType="bool" version="PHP 5">
		<attr name="name" type="string" status="required"></attr>
		<attr name="value" type="string" status="optional"></attr>
		<attr name="expire" type="int" status="optional"></attr>
		<attr name="path" type="string" status="optional"></attr>
		<attr name="domain" type="string" status="optional"></attr>
		<attr name="secure" type="bool" status="optional"></attr>
	</tag>
</tags>