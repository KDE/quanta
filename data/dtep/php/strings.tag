<!DOCTYPE tags>
<tags>
	<tag name="addcslashes" type="function" returnType="string" version="PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
		<attr name="charlist" type="string" status="required"></attr>
	</tag>
	<tag name="addslashes" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
	</tag>
	<tag name="bin2hex" type="function" returnType="string" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
	</tag>
	<tag name="chr" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="ascii" type="int" status="required"></attr>
	</tag>
	<tag name="chunk_split" type="function" returnType="string" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="body" type="string" status="required"></attr>
		<attr name="chunklen" type="int" status="optional"></attr>
		<attr name="end" type="string" status="optional"></attr>
	</tag>
	<tag name="convert_cyr_string" type="function" returnType="string" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
		<attr name="from" type="string" status="required"></attr>
		<attr name="to" type="string" status="required"></attr>
	</tag>
	<tag name="convert_uudecode" type="function" returnType="string" version="PHP 5">
		<attr name="data" type="string" status="required"></attr>
	</tag>
	<tag name="convert_uuencode" type="function" returnType="string" version="PHP 5">
		<attr name="data" type="string" status="required"></attr>
	</tag>
	<tag name="count_chars" type="function" returnType="mixed" version="PHP 4 , PHP 5">
		<attr name="string" type="string" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="crc32" type="function" returnType="int" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="str" type="string" status="required"></attr>
	</tag>
	<tag name="crypt" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
		<attr name="salt" type="string" status="optional"></attr>
	</tag>
	<tag name="echo" type="function" returnType="void" version="">
		<attr name="arg1" type="string" status="required"></attr>
		<attr name="argn..." type="string" status="optional"></attr>
	</tag>
	<tag name="explode" type="function" returnType="array" version="PHP 3, PHP 4 , PHP 5">
		<attr name="separator" type="string" status="required"></attr>
		<attr name="string" type="string" status="required"></attr>
		<attr name="limit" type="int" status="optional"></attr>
	</tag>
	<tag name="fprintf" type="function" returnType="int" version="PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="format" type="string" status="required"></attr>
		<attr name="args" type="mixed" status="optional"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="get_html_translation_table" type="function" returnType="array" version="PHP 4 , PHP 5">
		<attr name="table" type="int" status="optional"></attr>
		<attr name="quote_style" type="int" status="optional"></attr>
	</tag>
	<tag name="hebrev" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="hebrew_text" type="string" status="required"></attr>
		<attr name="max_chars_per_line" type="int" status="optional"></attr>
	</tag>
	<tag name="hebrevc" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="hebrew_text" type="string" status="required"></attr>
		<attr name="max_chars_per_line" type="int" status="optional"></attr>
	</tag>
	<tag name="html_entity_decode" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="string" type="string" status="required"></attr>
		<attr name="quote_style" type="int" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="htmlentities" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="string" type="string" status="required"></attr>
		<attr name="quote_style" type="int" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="htmlspecialchars" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="string" type="string" status="required"></attr>
		<attr name="quote_style" type="int" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="implode" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="glue" type="string" status="required"></attr>
		<attr name="pieces" type="array" status="required"></attr>
	</tag>
	<tag name="levenshtein" type="function" returnType="int" version="PHP 3&gt;= 3.0.17, PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="str1" type="string" status="required"></attr>
		<attr name="str2" type="string" status="required"></attr>
		<attr name="cost_ins" type="int" status="optional"></attr>
		<attr name="cost_rep" type="int" status="optional"></attr>
		<attr name="cost_del" type="int" status="required"></attr>
	</tag>
	<tag name="localeconv" type="function" returnType="array" version="PHP 4 &gt;= 4.0.5, PHP 5">
	</tag>
	<tag name="ltrim" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
		<attr name="charlist" type="string" status="optional"></attr>
	</tag>
	<tag name="md5_file" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="raw_output" type="bool" status="required"></attr>
	</tag>
	<tag name="md5" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
		<attr name="raw_output" type="bool" status="required"></attr>
	</tag>
	<tag name="metaphone" type="function" returnType="string" version="PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
		<attr name="phones" type="int" status="optional"></attr>
	</tag>
	<tag name="money_format" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="format" type="string" status="required"></attr>
		<attr name="number" type="float" status="required"></attr>
	</tag>
	<tag name="nl_langinfo" type="function" returnType="string" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="item" type="int" status="required"></attr>
	</tag>
	<tag name="nl2br" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="string" type="string" status="required"></attr>
	</tag>
	<tag name="number_format" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="number" type="float" status="required"></attr>
		<attr name="decimals" type="int" status="optional"></attr>
		<attr name="dec_point" type="string" status="optional"></attr>
		<attr name="thousands_sep" type="string" status="required"></attr>
	</tag>
	<tag name="ord" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="string" type="string" status="required"></attr>
	</tag>
	<tag name="parse_str" type="function" returnType="void" version="PHP 3, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
		<attr name="&amp;arr" type="array" status="optional"></attr>
	</tag>
	<tag name="print" type="function" returnType="int" version="">
		<attr name="arg" type="string" status="required"></attr>
	</tag>
	<tag name="printf" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="format" type="string" status="required"></attr>
		<attr name="args" type="mixed" status="optional"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="quoted_printable_decode" type="function" returnType="string" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
	</tag>
	<tag name="quotemeta" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
	</tag>
	<tag name="rtrim" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
		<attr name="charlist" type="string" status="optional"></attr>
	</tag>
	<tag name="setlocale" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="category" type="mixed" status="required"></attr>
		<attr name="locale" type="string" status="required"></attr>
		<attr name="..." type="string" status="optional"></attr>
	</tag>
	<tag name="sha1_file" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="raw_output" type="bool" status="required"></attr>
	</tag>
	<tag name="sha1" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="str" type="string" status="required"></attr>
		<attr name="raw_output" type="bool" status="required"></attr>
	</tag>
	<tag name="similar_text" type="function" returnType="int" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="first" type="string" status="required"></attr>
		<attr name="second" type="string" status="required"></attr>
		<attr name="&amp;percent" type="float" status="optional"></attr>
	</tag>
	<tag name="soundex" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
	</tag>
	<tag name="sprintf" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="format" type="string" status="required"></attr>
		<attr name="args" type="mixed" status="optional"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="sscanf" type="function" returnType="mixed" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="str" type="string" status="required"></attr>
		<attr name="format" type="string" status="required"></attr>
		<attr name="&amp;..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="str_ireplace" type="function" returnType="mixed" version="PHP 5">
		<attr name="search" type="mixed" status="required"></attr>
		<attr name="replace" type="mixed" status="required"></attr>
		<attr name="subject" type="mixed" status="required"></attr>
		<attr name="&amp;count" type="int" status="optional"></attr>
	</tag>
	<tag name="str_pad" type="function" returnType="string" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="input" type="string" status="required"></attr>
		<attr name="pad_length" type="int" status="required"></attr>
		<attr name="pad_string" type="string" status="optional"></attr>
		<attr name="pad_type" type="int" status="optional"></attr>
	</tag>
	<tag name="str_repeat" type="function" returnType="string" version="PHP 4 , PHP 5">
		<attr name="input" type="string" status="required"></attr>
		<attr name="multiplier" type="int" status="required"></attr>
	</tag>
	<tag name="str_replace" type="function" returnType="mixed" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="search" type="mixed" status="required"></attr>
		<attr name="replace" type="mixed" status="required"></attr>
		<attr name="subject" type="mixed" status="required"></attr>
		<attr name="&amp;count" type="int" status="optional"></attr>
	</tag>
	<tag name="str_rot13" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="str" type="string" status="required"></attr>
	</tag>
	<tag name="str_shuffle" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="str" type="string" status="required"></attr>
	</tag>
	<tag name="str_split" type="function" returnType="array" version="PHP 5">
		<attr name="string" type="string" status="required"></attr>
		<attr name="split_length" type="int" status="optional"></attr>
	</tag>
	<tag name="str_word_count" type="function" returnType="mixed" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="string" type="string" status="required"></attr>
		<attr name="format" type="int" status="optional"></attr>
	</tag>
	<tag name="strcasecmp" type="function" returnType="int" version="PHP 3&gt;= 3.0.2, PHP 4 , PHP 5">
		<attr name="str1" type="string" status="required"></attr>
		<attr name="str2" type="string" status="required"></attr>
	</tag>
	<tag name="strcmp" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="str1" type="string" status="required"></attr>
		<attr name="str2" type="string" status="required"></attr>
	</tag>
	<tag name="strcoll" type="function" returnType="int" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="str1" type="string" status="required"></attr>
		<attr name="str2" type="string" status="required"></attr>
	</tag>
	<tag name="strcspn" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="str1" type="string" status="required"></attr>
		<attr name="str2" type="string" status="required"></attr>
		<attr name="start" type="int" status="optional"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="strip_tags" type="function" returnType="string" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
		<attr name="allowable_tags" type="string" status="optional"></attr>
	</tag>
	<tag name="stripcslashes" type="function" returnType="string" version="PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
	</tag>
	<tag name="stripos" type="function" returnType="int" version="PHP 5">
		<attr name="haystack" type="string" status="required"></attr>
		<attr name="needle" type="string" status="required"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="stripslashes" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
	</tag>
	<tag name="stristr" type="function" returnType="string" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="haystack" type="string" status="required"></attr>
		<attr name="needle" type="string" status="required"></attr>
	</tag>
	<tag name="strlen" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="string" type="string" status="required"></attr>
	</tag>
	<tag name="strnatcasecmp" type="function" returnType="int" version="PHP 4 , PHP 5">
		<attr name="str1" type="string" status="required"></attr>
		<attr name="str2" type="string" status="required"></attr>
	</tag>
	<tag name="strnatcmp" type="function" returnType="int" version="PHP 4 , PHP 5">
		<attr name="str1" type="string" status="required"></attr>
		<attr name="str2" type="string" status="required"></attr>
	</tag>
	<tag name="strncasecmp" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="str1" type="string" status="required"></attr>
		<attr name="str2" type="string" status="required"></attr>
		<attr name="len" type="int" status="required"></attr>
	</tag>
	<tag name="strncmp" type="function" returnType="int" version="PHP 4 , PHP 5">
		<attr name="str1" type="string" status="required"></attr>
		<attr name="str2" type="string" status="required"></attr>
		<attr name="len" type="int" status="required"></attr>
	</tag>
	<tag name="strpbrk" type="function" returnType="string" version="PHP 5">
		<attr name="haystack" type="string" status="required"></attr>
		<attr name="char_list" type="string" status="required"></attr>
	</tag>
	<tag name="strpos" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="haystack" type="string" status="required"></attr>
		<attr name="needle" type="string" status="required"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="strrchr" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="haystack" type="string" status="required"></attr>
		<attr name="needle" type="string" status="required"></attr>
	</tag>
	<tag name="strrev" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="string" type="string" status="required"></attr>
	</tag>
	<tag name="strripos" type="function" returnType="int" version="PHP 5">
		<attr name="haystack" type="string" status="required"></attr>
		<attr name="needle" type="string" status="required"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="strrpos" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="haystack" type="string" status="required"></attr>
		<attr name="needle" type="string" status="required"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="strspn" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="str1" type="string" status="required"></attr>
		<attr name="str2" type="string" status="required"></attr>
		<attr name="start" type="int" status="optional"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="strstr" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="haystack" type="string" status="required"></attr>
		<attr name="needle" type="string" status="required"></attr>
	</tag>
	<tag name="strtok" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
		<attr name="token" type="string" status="required"></attr>
	</tag>
	<tag name="strtolower" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
	</tag>
	<tag name="strtoupper" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="string" type="string" status="required"></attr>
	</tag>
	<tag name="strtr" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
		<attr name="from" type="string" status="required"></attr>
		<attr name="to" type="string" status="required"></attr>
	</tag>
	<tag name="substr_compare" type="function" returnType="int" version="PHP 5">
		<attr name="main_str" type="string" status="required"></attr>
		<attr name="str" type="string" status="required"></attr>
		<attr name="offset" type="int" status="required"></attr>
		<attr name="length" type="int" status="optional"></attr>
		<attr name="case_insensitivity" type="bool" status="optional"></attr>
	</tag>
	<tag name="substr_count" type="function" returnType="int" version="PHP 4 , PHP 5">
		<attr name="haystack" type="string" status="required"></attr>
		<attr name="needle" type="string" status="required"></attr>
	</tag>
	<tag name="substr_replace" type="function" returnType="string" version="PHP 4 , PHP 5">
		<attr name="string" type="string" status="required"></attr>
		<attr name="replacement" type="string" status="required"></attr>
		<attr name="start" type="int" status="required"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="substr" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="string" type="string" status="required"></attr>
		<attr name="start" type="int" status="required"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="trim" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
		<attr name="charlist" type="string" status="optional"></attr>
	</tag>
	<tag name="ucfirst" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
	</tag>
	<tag name="ucwords" type="function" returnType="string" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="str" type="string" status="required"></attr>
	</tag>
	<tag name="vfprintf" type="function" returnType="int" version="PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="format" type="string" status="required"></attr>
		<attr name="args" type="array" status="required"></attr>
	</tag>
	<tag name="vprintf" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="format" type="string" status="required"></attr>
		<attr name="args" type="array" status="required"></attr>
	</tag>
	<tag name="vsprintf" type="function" returnType="string" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="format" type="string" status="required"></attr>
		<attr name="args" type="array" status="required"></attr>
	</tag>
	<tag name="wordwrap" type="function" returnType="string" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="str" type="string" status="required"></attr>
		<attr name="width" type="int" status="optional"></attr>
		<attr name="break" type="string" status="optional"></attr>
		<attr name="cut" type="bool" status="optional"></attr>
	</tag>
</tags>