<!DOCTYPE tags>
<tags>
	<tag name="pspell_add_to_personal" type="function" returnType="int">
		<attr name="dictionary_link" type="int"></attr>
		<attr name="word" type="string"></attr>
	</tag>
	<tag name="pspell_add_to_session" type="function" returnType="int">
		<attr name="dictionary_link" type="int"></attr>
		<attr name="word" type="string"></attr>
	</tag>
	<tag name="pspell_check" type="function" returnType="bool">
		<attr name="dictionary_link" type="int"></attr>
		<attr name="word" type="string"></attr>
	</tag>
	<tag name="pspell_clear_session" type="function" returnType="int">
		<attr name="dictionary_link" type="int"></attr>
	</tag>
	<tag name="pspell_config_create" type="function" returnType="int">
		<attr name="language" type="string"></attr>
		<attr name="spelling" type="string" status="optional"></attr>
		<attr name="jargon" type="string" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="pspell_config_data_dir" type="function" returnType="bool">
		<attr name="conf" type="int"></attr>
		<attr name="directory" type="string"></attr>
	</tag>
	<tag name="pspell_config_dict_dir" type="function" returnType="bool">
		<attr name="conf" type="int"></attr>
		<attr name="directory" type="string"></attr>
	</tag>
	<tag name="pspell_config_ignore" type="function" returnType="int">
		<attr name="dictionary_link" type="int"></attr>
		<attr name="n" type="int"></attr>
	</tag>
	<tag name="pspell_config_mode" type="function" returnType="int">
		<attr name="dictionary_link" type="int"></attr>
		<attr name="mode" type="int"></attr>
	</tag>
	<tag name="pspell_config_personal" type="function" returnType="int">
		<attr name="dictionary_link" type="int"></attr>
		<attr name="file" type="string"></attr>
	</tag>
	<tag name="pspell_config_repl" type="function" returnType="int">
		<attr name="dictionary_link" type="int"></attr>
		<attr name="file" type="string"></attr>
	</tag>
	<tag name="pspell_config_runtogether" type="function" returnType="int">
		<attr name="dictionary_link" type="int"></attr>
		<attr name="flag" type="bool"></attr>
	</tag>
	<tag name="pspell_config_save_repl" type="function" returnType="int">
		<attr name="dictionary_link" type="int"></attr>
		<attr name="flag" type="bool"></attr>
	</tag>
	<tag name="pspell_new_config" type="function" returnType="int">
		<attr name="config" type="int"></attr>
	</tag>
	<tag name="pspell_new_personal" type="function" returnType="int">
		<attr name="personal" type="string"></attr>
		<attr name="language" type="string"></attr>
		<attr name="spelling" type="string" status="optional"></attr>
		<attr name="jargon" type="string" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="pspell_new" type="function" returnType="int">
		<attr name="language" type="string"></attr>
		<attr name="spelling" type="string" status="optional"></attr>
		<attr name="jargon" type="string" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="pspell_save_wordlist" type="function" returnType="int">
		<attr name="dictionary_link" type="int"></attr>
	</tag>
	<tag name="pspell_store_replacement" type="function" returnType="int">
		<attr name="dictionary_link" type="int"></attr>
		<attr name="misspelled" type="string"></attr>
		<attr name="correct" type="string"></attr>
	</tag>
	<tag name="pspell_suggest" type="function" returnType="array">
		<attr name="dictionary_link" type="int"></attr>
		<attr name="word" type="string"></attr>
	</tag>
</tags>