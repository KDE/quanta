<!DOCTYPE tags>
<tags>	<tag name="addcslashes" type="function" returnType="string">
		<attr name="str" type="string" status="optional"></attr>
		<attr name="charlist" type="string" status="optional"></attr>
	</tag>
	<tag name="addslashes" type="function" returnType="string">
		<attr name="str" type="string" status="optional"></attr>
	</tag>
	<tag name="bin2hex" type="function" returnType="string">
		<attr name="str" type="string" status="optional"></attr>
	</tag>
	<tag name="chr" type="function" returnType="string">
		<attr name="ascii" type="int" status="optional"></attr>
	</tag>
	<tag name="chunk_split" type="function" returnType="string">
		<attr name="body" type="string" status="optional"></attr>
		<attr name="chunklen" type="int" status="optional"></attr>
		<attr name="end" type="string" status="optional"></attr>
	</tag>
	<tag name="convert_cyr_string" type="function" returnType="string">
		<attr name="str" type="string" status="optional"></attr>
		<attr name="from" type="string" status="optional"></attr>
		<attr name="to" type="string" status="optional"></attr>
	</tag>
	<tag name="count_chars" type="function" returnType="mixed">
		<attr name="string" type="string" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="crc32" type="function" returnType="int">
		<attr name="str" type="string" status="optional"></attr>
	</tag>
	<tag name="crypt" type="function" returnType="string">
		<attr name="str" type="string" status="optional"></attr>
		<attr name="salt" type="string" status="optional"></attr>
	</tag>
	<tag name="echo" type="function" returnType="none">
		<attr name="arg1" type="string" status="optional"></attr>
		<attr name="stringN" type="string" status="optional"></attr>
	</tag>
	<tag name="explode" type="function" returnType="array">
		<attr name="separator" type="string" status="optional"></attr>
		<attr name="string" type="string" status="optional"></attr>
		<attr name="limit" type="int" status="optional"></attr>
	</tag>
	<tag name="get_html_translation_table" type="function" returnType="string">
		<attr name="table" type="int" status="optional"></attr>
		<attr name="quote_style" type="int" status="optional"></attr>
	</tag>
	<tag name="get_meta_tags" type="function" returnType="array">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="use_include_path" type="int" status="optional"></attr>
	</tag>
	<tag name="hebrev" type="function" returnType="string">
		<attr name="hebrew_text" type="string" status="optional"></attr>
		<attr name="max_chars_per_line" type="int" status="optional"></attr>
	</tag>
	<tag name="hebrevc" type="function" returnType="string">
		<attr name="hebrew_text" type="string" status="optional"></attr>
		<attr name="max_chars_per_line" type="int" status="optional"></attr>
	</tag>
	<tag name="htmlentities" type="function" returnType="string">
		<attr name="string" type="string" status="optional"></attr>
		<attr name="quote_style" type="int" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="htmlspecialchars" type="function" returnType="string">
		<attr name="string" type="string" status="optional"></attr>
		<attr name="quote_style" type="int" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="implode" type="function" returnType="string">
		<attr name="glue" type="string" status="optional"></attr>
		<attr name="pieces" type="array" status="optional"></attr>
	</tag>
	<tag name="join" type="function" returnType="string">
		<attr name="glue" type="string" status="optional"></attr>
		<attr name="pieces" type="array" status="optional"></attr>
	</tag>
	<tag name="levenshtein" type="function" returnType="int">
		<attr name="str1" type="string" status="optional"></attr>
		<attr name="str2" type="string" status="optional"></attr>
	</tag>
	<tag name="localeconv" type="function" returnType="array">
	</tag>
	<tag name="ltrim" type="function" returnType="string">
		<attr name="str" type="string" status="optional"></attr>
		<attr name="charlist" type="string" status="optional"></attr>
	</tag>
	<tag name="md5_file" type="function" returnType="string">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="md5" type="function" returnType="string">
		<attr name="str" type="string" status="optional"></attr>
	</tag>
	<tag name="metaphone" type="function" returnType="string">
		<attr name="str" type="string" status="optional"></attr>
	</tag>
	<tag name="nl_langinfo" type="function" returnType="string">
		<attr name="item" type="int" status="optional"></attr>
	</tag>
	<tag name="nl2br" type="function" returnType="string">
		<attr name="string" type="string" status="optional"></attr>
	</tag>
	<tag name="ord" type="function" returnType="int">
		<attr name="string" type="string" status="optional"></attr>
	</tag>
	<tag name="parse_str" type="function" returnType="void">
		<attr name="str" type="string" status="optional"></attr>
		<attr name="arr" type="array" status="optional"></attr>
	</tag>
	<tag name="print" type="function" returnType="none">
		<attr name="arg" type="string" status="optional"></attr>
	</tag>
	<tag name="printf" type="function" returnType="void">
		<attr name="format" type="string" status="optional"></attr>
		<attr name="args" type="mixed" status="optional"></attr>
	</tag>
	<tag name="quoted_printable_decode" type="function" returnType="string">
		<attr name="str" type="string" status="optional"></attr>
	</tag>
	<tag name="quotemeta" type="function" returnType="string">
		<attr name="str" type="string" status="optional"></attr>
	</tag>
	<tag name="rtrim" type="function" returnType="string">
		<attr name="str" type="string" status="optional"></attr>
		<attr name="charlist" type="string" status="optional"></attr>
	</tag>
	<tag name="setlocale" type="function" returnType="string">
		<attr name="category" type="mixed" status="optional"></attr>
		<attr name="locale" type="string" status="optional"></attr>
	</tag>
	<tag name="similar_text" type="function" returnType="int">
		<attr name="first" type="string" status="optional"></attr>
		<attr name="second" type="string" status="optional"></attr>
		<attr name="percent" type="float" status="optional"></attr>
	</tag>
	<tag name="soundex" type="function" returnType="string">
		<attr name="str" type="string" status="optional"></attr>
	</tag>
	<tag name="sprintf" type="function" returnType="string">
		<attr name="format" type="string" status="optional"></attr>
		<attr name="args" type="mixed" status="optional"></attr>
	</tag>
	<tag name="sscanf" type="function" returnType="mixed">
		<attr name="str" type="string" status="optional"></attr>
		<attr name="format" type="string" status="optional"></attr>
		<attr name="var1" type="string" status="optional"></attr>
	</tag>
	<tag name="str_pad" type="function" returnType="string">
		<attr name="input" type="string" status="optional"></attr>
		<attr name="pad_length" type="int" status="optional"></attr>
		<attr name="pad_string" type="string" status="optional"></attr>
		<attr name="pad_type" type="int" status="optional"></attr>
	</tag>
	<tag name="str_repeat" type="function" returnType="string">
		<attr name="input" type="string" status="optional"></attr>
		<attr name="multiplier" type="int" status="optional"></attr>
	</tag>
	<tag name="str_replace" type="function" returnType="mixed">
		<attr name="search" type="mixed" status="optional"></attr>
		<attr name="replace" type="mixed" status="optional"></attr>
		<attr name="subject" type="mixed" status="optional"></attr>
	</tag>
	<tag name="str_rot13" type="function" returnType="string">
		<attr name="str" type="string" status="optional"></attr>
	</tag>
	<tag name="strcasecmp" type="function" returnType="int">
		<attr name="str1" type="string" status="optional"></attr>
		<attr name="str2" type="string" status="optional"></attr>
	</tag>
	<tag name="strchr" type="function" returnType="string">
		<attr name="haystack" type="string" status="optional"></attr>
		<attr name="needle" type="string" status="optional"></attr>
	</tag>
	<tag name="strcmp" type="function" returnType="int">
		<attr name="str1" type="string" status="optional"></attr>
		<attr name="str2" type="string" status="optional"></attr>
	</tag>
	<tag name="strcoll" type="function" returnType="int">
		<attr name="str1" type="string" status="optional"></attr>
		<attr name="str2" type="string" status="optional"></attr>
	</tag>
	<tag name="strcspn" type="function" returnType="int">
		<attr name="str1" type="string" status="optional"></attr>
		<attr name="str2" type="string" status="optional"></attr>
	</tag>
	<tag name="strip_tags" type="function" returnType="string">
		<attr name="str" type="string" status="optional"></attr>
		<attr name="allowable_tags" type="string" status="optional"></attr>
	</tag>
	<tag name="stripcslashes" type="function" returnType="string">
		<attr name="str" type="string" status="optional"></attr>
	</tag>
	<tag name="stripslashes" type="function" returnType="string">
		<attr name="str" type="string" status="optional"></attr>
	</tag>
	<tag name="stristr" type="function" returnType="string">
		<attr name="haystack" type="string" status="optional"></attr>
		<attr name="needle" type="string" status="optional"></attr>
	</tag>
	<tag name="strlen" type="function" returnType="int">
		<attr name="str" type="string" status="optional"></attr>
	</tag>
	<tag name="strnatcasecmp" type="function" returnType="int">
		<attr name="str1" type="string" status="optional"></attr>
		<attr name="str2" type="string" status="optional"></attr>
	</tag>
	<tag name="strnatcmp" type="function" returnType="int">
		<attr name="str1" type="string" status="optional"></attr>
		<attr name="str2" type="string" status="optional"></attr>
	</tag>
	<tag name="strncasecmp" type="function" returnType="int">
		<attr name="str1" type="string" status="optional"></attr>
		<attr name="str2" type="string" status="optional"></attr>
		<attr name="len" type="int" status="optional"></attr>
	</tag>
	<tag name="strncmp" type="function" returnType="int">
		<attr name="str1" type="string" status="optional"></attr>
		<attr name="str2" type="string" status="optional"></attr>
		<attr name="len" type="int" status="optional"></attr>
	</tag>
	<tag name="strpos" type="function" returnType="int">
		<attr name="haystack" type="string" status="optional"></attr>
		<attr name="needle" type="string" status="optional"></attr>
		<attr name="offset" type="int" status="optional"></attr>
	</tag>
	<tag name="strrchr" type="function" returnType="string">
		<attr name="haystack" type="string" status="optional"></attr>
		<attr name="needle" type="string" status="optional"></attr>
	</tag>
	<tag name="strrev" type="function" returnType="string">
		<attr name="string" type="string" status="optional"></attr>
	</tag>
	<tag name="strrpos" type="function" returnType="int">
		<attr name="haystack" type="string" status="optional"></attr>
		<attr name="needle" type="char" status="optional"></attr>
	</tag>
	<tag name="strspn" type="function" returnType="int">
		<attr name="str1" type="string" status="optional"></attr>
		<attr name="str2" type="string" status="optional"></attr>
	</tag>
	<tag name="strstr" type="function" returnType="string">
		<attr name="haystack" type="string" status="optional"></attr>
		<attr name="needle" type="string" status="optional"></attr>
	</tag>
	<tag name="strtok" type="function" returnType="string">
		<attr name="arg1" type="string" status="optional"></attr>
		<attr name="arg2" type="string" status="optional"></attr>
	</tag>
	<tag name="strtolower" type="function" returnType="string">
		<attr name="str" type="string" status="optional"></attr>
	</tag>
	<tag name="strtoupper" type="function" returnType="string">
		<attr name="string" type="string" status="optional"></attr>
	</tag>
	<tag name="strtr" type="function" returnType="string">
		<attr name="str" type="string" status="optional"></attr>
		<attr name="from" type="string" status="optional"></attr>
		<attr name="to" type="string" status="optional"></attr>
	</tag>
	<tag name="substr_count" type="function" returnType="int">
		<attr name="haystack" type="string" status="optional"></attr>
		<attr name="needle" type="string" status="optional"></attr>
	</tag>
	<tag name="substr_replace" type="function" returnType="string">
		<attr name="string" type="string" status="optional"></attr>
		<attr name="replacement" type="string" status="optional"></attr>
		<attr name="start" type="int" status="optional"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="substr" type="function" returnType="string">
		<attr name="string" type="string" status="optional"></attr>
		<attr name="start" type="int" status="optional"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="trim" type="function" returnType="string">
		<attr name="str" type="string" status="optional"></attr>
		<attr name="charlist" type="string" status="optional"></attr>
	</tag>
	<tag name="ucfirst" type="function" returnType="string">
		<attr name="str" type="string" status="optional"></attr>
	</tag>
	<tag name="ucwords" type="function" returnType="string">
		<attr name="str" type="string" status="optional"></attr>
	</tag>
	<tag name="vprintf" type="function" returnType="void">
		<attr name="format" type="string" status="optional"></attr>
		<attr name="args" type="array" status="optional"></attr>
	</tag>
	<tag name="vsprintf" type="function" returnType="string">
		<attr name="format" type="string" status="optional"></attr>
		<attr name="args" type="array" status="optional"></attr>
	</tag>
	<tag name="wordwrap" type="function" returnType="string">
		<attr name="str" type="string" status="optional"></attr>
		<attr name="width" type="int" status="optional"></attr>
		<attr name="break" type="string" status="optional"></attr>
		<attr name="cut" type="int" status="optional"></attr>
	</tag>
</tags>
