<!DOCTYPE tags>
<tags>
	<tag name="yaz_addinfo" type="function" returnType="string" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="id" type="resource" status="required"></attr>
	</tag>
	<tag name="yaz_ccl_conf" type="function" returnType="int" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="id" type="resource" status="required"></attr>
		<attr name="config" type="array" status="required"></attr>
	</tag>
	<tag name="yaz_ccl_parse" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="id" type="resource" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
		<attr name="&amp;result" type="array" status="required"></attr>
	</tag>
	<tag name="yaz_close" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="id" type="resource" status="required"></attr>
	</tag>
	<tag name="yaz_connect" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="zurl" type="string" status="required"></attr>
		<attr name="options" type="mixed" status="optional"></attr>
	</tag>
	<tag name="yaz_database" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="id" type="resource" status="required"></attr>
		<attr name="databases" type="string" status="required"></attr>
	</tag>
	<tag name="yaz_element" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="id" type="resource" status="required"></attr>
		<attr name="elementset" type="string" status="required"></attr>
	</tag>
	<tag name="yaz_errno" type="function" returnType="int" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="id" type="resource" status="required"></attr>
	</tag>
	<tag name="yaz_error" type="function" returnType="string" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="id" type="resource" status="required"></attr>
	</tag>
	<tag name="yaz_es_result" type="function" returnType="array" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="id" type="resource" status="required"></attr>
	</tag>
	<tag name="yaz_get_option" type="function" returnType="string" version="PHP 5">
		<attr name="id" type="resource" status="required"></attr>
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="yaz_hits" type="function" returnType="int" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="id" type="resource" status="required"></attr>
	</tag>
	<tag name="yaz_itemorder" type="function" returnType="int" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="id" type="resource" status="required"></attr>
		<attr name="args" type="array" status="required"></attr>
	</tag>
	<tag name="yaz_present" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="id" type="resource" status="required"></attr>
	</tag>
	<tag name="yaz_range" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="id" type="resource" status="required"></attr>
		<attr name="start" type="int" status="required"></attr>
		<attr name="number" type="int" status="required"></attr>
	</tag>
	<tag name="yaz_record" type="function" returnType="string" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="id" type="resource" status="required"></attr>
		<attr name="pos" type="int" status="required"></attr>
		<attr name="type" type="string" status="required"></attr>
	</tag>
	<tag name="yaz_scan_result" type="function" returnType="array" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="id" type="resource" status="required"></attr>
		<attr name="&amp;result" type="array" status="optional"></attr>
	</tag>
	<tag name="yaz_scan" type="function" returnType="int" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="id" type="resource" status="required"></attr>
		<attr name="type" type="string" status="required"></attr>
		<attr name="startterm" type="string" status="required"></attr>
		<attr name="flags" type="array" status="optional"></attr>
	</tag>
	<tag name="yaz_schema" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="id" type="resource" status="required"></attr>
		<attr name="schema" type="string" status="required"></attr>
	</tag>
	<tag name="yaz_search" type="function" returnType="int" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="id" type="resource" status="required"></attr>
		<attr name="type" type="string" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
	</tag>
	<tag name="yaz_set_option" type="function" returnType="string" version="PHP 5">
		<attr name="id" type="resource" status="required"></attr>
		<attr name="name" type="string" status="required"></attr>
		<attr name="value" type="string" status="required"></attr>
	</tag>
	<tag name="yaz_sort" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="id" type="resource" status="required"></attr>
		<attr name="criteria" type="string" status="required"></attr>
	</tag>
	<tag name="yaz_syntax" type="function" returnType="int" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="id" type="resource" status="required"></attr>
		<attr name="syntax" type="string" status="required"></attr>
	</tag>
	<tag name="yaz_wait" type="function" returnType="int" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="&amp;options" type="array" status="optional"></attr>
	</tag>
</tags>