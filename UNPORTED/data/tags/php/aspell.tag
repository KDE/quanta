<!DOCTYPE tags>
<tags>	<tag name="aspell_check_raw" type="function" returnType="bool">
		<attr name="dictionary_link" type="int" status="optional"></attr>
		<attr name="word" type="string" status="optional"></attr>
	</tag>
	<tag name="aspell_check" type="function" returnType="bool">
		<attr name="dictionary_link" type="int" status="optional"></attr>
		<attr name="word" type="string" status="optional"></attr>
	</tag>
	<tag name="aspell_new" type="function" returnType="int">
		<attr name="master" type="string" status="optional"></attr>
		<attr name="personal" type="string" status="optional"></attr>
	</tag>
	<tag name="aspell_suggest" type="function" returnType="array">
		<attr name="dictionary_link" type="int" status="optional"></attr>
		<attr name="word" type="string" status="optional"></attr>
	</tag>
</tags>
