<!DOCTYPE tags>
<tags>
	<tag name="aspell_check_raw" type="function" returnType="bool" comment="PHP 3&gt;= 3.0.7, PHP 4  &lt;= 4.2.3">
		<attr name="dictionary_link" type="int" status="required"></attr>
		<attr name="word" type="string" status="required"></attr>
	</tag>
	<tag name="aspell_check" type="function" returnType="bool" comment="PHP 3&gt;= 3.0.7, PHP 4  &lt;= 4.2.3">
		<attr name="dictionary_link" type="int" status="required"></attr>
		<attr name="word" type="string" status="required"></attr>
	</tag>
	<tag name="aspell_new" type="function" returnType="int" comment="PHP 3&gt;= 3.0.7, PHP 4  &lt;= 4.2.3">
		<attr name="master" type="string" status="required"></attr>
		<attr name="personal" type="string" status="optional"></attr>
	</tag>
	<tag name="aspell_suggest" type="function" returnType="array" comment="PHP 3&gt;= 3.0.7, PHP 4  &lt;= 4.2.3">
		<attr name="dictionary_link" type="int" status="required"></attr>
		<attr name="word" type="string" status="required"></attr>
	</tag>
</tags>