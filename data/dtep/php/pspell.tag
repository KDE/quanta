<!DOCTYPE tags>
<tags>
	<tag name="pspell_add_to_personal" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="dictionary_link" type="int" status="required"></attr>
		<attr name="word" type="string" status="required"></attr>
	</tag>
	<tag name="pspell_add_to_session" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="dictionary_link" type="int" status="required"></attr>
		<attr name="word" type="string" status="required"></attr>
	</tag>
	<tag name="pspell_check" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="dictionary_link" type="int" status="required"></attr>
		<attr name="word" type="string" status="required"></attr>
	</tag>
	<tag name="pspell_clear_session" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="dictionary_link" type="int" status="required"></attr>
	</tag>
	<tag name="pspell_config_create" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="language" type="string" status="required"></attr>
		<attr name="spelling" type="string" status="optional"></attr>
		<attr name="jargon" type="string" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="pspell_config_data_dir" type="function" returnType="bool" version="PHP 5">
		<attr name="conf" type="int" status="required"></attr>
		<attr name="directory" type="string" status="required"></attr>
	</tag>
	<tag name="pspell_config_dict_dir" type="function" returnType="bool" version="PHP 5">
		<attr name="conf" type="int" status="required"></attr>
		<attr name="directory" type="string" status="required"></attr>
	</tag>
	<tag name="pspell_config_ignore" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="dictionary_link" type="int" status="required"></attr>
		<attr name="n" type="int" status="required"></attr>
	</tag>
	<tag name="pspell_config_mode" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="dictionary_link" type="int" status="required"></attr>
		<attr name="mode" type="int" status="required"></attr>
	</tag>
	<tag name="pspell_config_personal" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="dictionary_link" type="int" status="required"></attr>
		<attr name="file" type="string" status="required"></attr>
	</tag>
	<tag name="pspell_config_repl" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="dictionary_link" type="int" status="required"></attr>
		<attr name="file" type="string" status="required"></attr>
	</tag>
	<tag name="pspell_config_runtogether" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="dictionary_link" type="int" status="required"></attr>
		<attr name="flag" type="bool" status="required"></attr>
	</tag>
	<tag name="pspell_config_save_repl" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="dictionary_link" type="int" status="required"></attr>
		<attr name="flag" type="bool" status="required"></attr>
	</tag>
	<tag name="pspell_new_config" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="config" type="int" status="required"></attr>
	</tag>
	<tag name="pspell_new_personal" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="personal" type="string" status="required"></attr>
		<attr name="language" type="string" status="required"></attr>
		<attr name="spelling" type="string" status="optional"></attr>
		<attr name="jargon" type="string" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="pspell_new" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="language" type="string" status="required"></attr>
		<attr name="spelling" type="string" status="optional"></attr>
		<attr name="jargon" type="string" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="pspell_save_wordlist" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="dictionary_link" type="int" status="required"></attr>
	</tag>
	<tag name="pspell_store_replacement" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="dictionary_link" type="int" status="required"></attr>
		<attr name="misspelled" type="string" status="required"></attr>
		<attr name="correct" type="string" status="required"></attr>
	</tag>
	<tag name="pspell_suggest" type="function" returnType="array" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="dictionary_link" type="int" status="required"></attr>
		<attr name="word" type="string" status="required"></attr>
	</tag>
</tags>