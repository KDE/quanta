<!DOCTYPE tags>
<tags>	<tag name="preg_grep" type="function" returnType="array">
		<attr name="pattern" type="string" status="optional"></attr>
		<attr name="input" type="array" status="optional"></attr>
	</tag>
	<tag name="preg_match_all" type="function" returnType="int">
		<attr name="pattern" type="string" status="optional"></attr>
		<attr name="subject" type="string" status="optional"></attr>
		<attr name="matches" type="array" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="preg_match" type="function" returnType="int">
		<attr name="pattern" type="string" status="optional"></attr>
		<attr name="subject" type="string" status="optional"></attr>
		<attr name="matches" type="array" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="preg_quote" type="function" returnType="string">
		<attr name="str" type="string" status="optional"></attr>
		<attr name="delimiter" type="string" status="optional"></attr>
	</tag>
	<tag name="preg_replace_callback" type="function" returnType="mixed">
		<attr name="pattern" type="mixed" status="optional"></attr>
		<attr name="callback" type="mixed" status="optional"></attr>
		<attr name="subject" type="mixed" status="optional"></attr>
		<attr name="limit" type="int" status="optional"></attr>
	</tag>
	<tag name="preg_replace" type="function" returnType="mixed">
		<attr name="pattern" type="mixed" status="optional"></attr>
		<attr name="replacement" type="mixed" status="optional"></attr>
		<attr name="subject" type="mixed" status="optional"></attr>
		<attr name="limit" type="int" status="optional"></attr>
	</tag>
	<tag name="preg_split" type="function" returnType="array">
		<attr name="pattern" type="string" status="optional"></attr>
		<attr name="subject" type="string" status="optional"></attr>
		<attr name="limit" type="int" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
</tags>
