<!DOCTYPE tags>
<tags>
	<tag name="gzclose" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="zp" type="resource" status="required"></attr>
	</tag>
	<tag name="gzcompress" type="function" returnType="string" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="data" type="string" status="required"></attr>
		<attr name="level" type="int" status="optional"></attr>
	</tag>
	<tag name="gzdeflate" type="function" returnType="string" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="data" type="string" status="required"></attr>
		<attr name="level" type="int" status="optional"></attr>
	</tag>
	<tag name="gzencode" type="function" returnType="string" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="data" type="string" status="required"></attr>
		<attr name="level" type="int" status="optional"></attr>
		<attr name="encoding_mode" type="int" status="optional"></attr>
	</tag>
	<tag name="gzeof" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="zp" type="resource" status="required"></attr>
	</tag>
	<tag name="gzfile" type="function" returnType="array" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="use_include_path" type="int" status="optional"></attr>
	</tag>
	<tag name="gzgetc" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="zp" type="resource" status="required"></attr>
	</tag>
	<tag name="gzgets" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="zp" type="resource" status="required"></attr>
		<attr name="length" type="int" status="required"></attr>
	</tag>
	<tag name="gzgetss" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="zp" type="resource" status="required"></attr>
		<attr name="length" type="int" status="required"></attr>
		<attr name="allowable_tags" type="string" status="optional"></attr>
	</tag>
	<tag name="gzinflate" type="function" returnType="string" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="data" type="string" status="required"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="gzopen" type="function" returnType="resource" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="mode" type="string" status="required"></attr>
		<attr name="use_include_path" type="int" status="optional"></attr>
	</tag>
	<tag name="gzpassthru" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="zp" type="resource" status="required"></attr>
	</tag>
	<tag name="gzread" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="zp" type="resource" status="required"></attr>
		<attr name="length" type="int" status="required"></attr>
	</tag>
	<tag name="gzrewind" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="zp" type="resource" status="required"></attr>
	</tag>
	<tag name="gzseek" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="zp" type="resource" status="required"></attr>
		<attr name="offset" type="int" status="required"></attr>
	</tag>
	<tag name="gztell" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="zp" type="resource" status="required"></attr>
	</tag>
	<tag name="gzuncompress" type="function" returnType="string" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="data" type="string" status="required"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="gzwrite" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="zp" type="resource" status="required"></attr>
		<attr name="string" type="string" status="required"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="readgzfile" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="use_include_path" type="int" status="optional"></attr>
	</tag>
	<tag name="zlib_get_coding_type" type="function" returnType="string" version="PHP 4 &gt;= 4.3.2, PHP 5">
	</tag>
</tags>