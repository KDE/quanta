<!DOCTYPE tags>
<tags>
	<tag name="preg_grep" type="function" returnType="array">
		<attr name="pattern" type="string"></attr>
		<attr name="input" type="array"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="preg_match_all" type="function" returnType="int">
		<attr name="pattern" type="string"></attr>
		<attr name="subject" type="string"></attr>
		<attr name="&amp;matches" type="array"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="preg_match" type="function" returnType="mixed">
		<attr name="pattern" type="string"></attr>
		<attr name="subject" type="string"></attr>
		<attr name="&amp;matches" type="array" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="preg_quote" type="function" returnType="string">
		<attr name="str" type="string"></attr>
		<attr name="delimiter" type="string" status="optional"></attr>
	</tag>
	<tag name="preg_replace_callback" type="function" returnType="mixed">
		<attr name="pattern" type="mixed"></attr>
		<attr name="callback" type="callback"></attr>
		<attr name="subject" type="mixed"></attr>
		<attr name="limit" type="int" status="optional"></attr>
	</tag>
	<tag name="preg_replace" type="function" returnType="mixed">
		<attr name="pattern" type="mixed"></attr>
		<attr name="replacement" type="mixed"></attr>
		<attr name="subject" type="mixed"></attr>
		<attr name="limit" type="int" status="optional"></attr>
	</tag>
	<tag name="preg_split" type="function" returnType="array">
		<attr name="pattern" type="string"></attr>
		<attr name="subject" type="string"></attr>
		<attr name="limit" type="int" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
</tags>