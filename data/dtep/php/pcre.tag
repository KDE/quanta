<!DOCTYPE tags>
<tags>
	<tag name="preg_grep" type="function" returnType="array" comment="PHP 4 , PHP 5">
		<attr name="pattern" type="string" status="required"></attr>
		<attr name="input" type="array" status="required"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="preg_match_all" type="function" returnType="int" comment="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="pattern" type="string" status="required"></attr>
		<attr name="subject" type="string" status="required"></attr>
		<attr name="&amp;matches" type="array" status="required"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="preg_match" type="function" returnType="mixed" comment="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="pattern" type="string" status="required"></attr>
		<attr name="subject" type="string" status="required"></attr>
		<attr name="&amp;matches" type="array" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="preg_quote" type="function" returnType="string" comment="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
		<attr name="delimiter" type="string" status="optional"></attr>
	</tag>
	<tag name="preg_replace_callback" type="function" returnType="mixed" comment="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pattern" type="mixed" status="required"></attr>
		<attr name="callback" type="callback" status="required"></attr>
		<attr name="subject" type="mixed" status="required"></attr>
		<attr name="limit" type="int" status="optional"></attr>
	</tag>
	<tag name="preg_replace" type="function" returnType="mixed" comment="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="pattern" type="mixed" status="required"></attr>
		<attr name="replacement" type="mixed" status="required"></attr>
		<attr name="subject" type="mixed" status="required"></attr>
		<attr name="limit" type="int" status="optional"></attr>
	</tag>
	<tag name="preg_split" type="function" returnType="array" comment="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="pattern" type="string" status="required"></attr>
		<attr name="subject" type="string" status="required"></attr>
		<attr name="limit" type="int" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
</tags>