<!DOCTYPE tags>
<tags>
	<tag name="yaz_addinfo" type="function" returnType="string">
		<attr name="id" type="resource"></attr>
	</tag>
	<tag name="yaz_ccl_conf" type="function" returnType="int">
		<attr name="id" type="resource"></attr>
		<attr name="config" type="array"></attr>
	</tag>
	<tag name="yaz_ccl_parse" type="function" returnType="bool">
		<attr name="id" type="resource"></attr>
		<attr name="query" type="string"></attr>
		<attr name="&amp;result" type="array"></attr>
	</tag>
	<tag name="yaz_close" type="function" returnType="bool">
		<attr name="id" type="resource"></attr>
	</tag>
	<tag name="yaz_connect" type="function" returnType="resource">
		<attr name="zurl" type="string"></attr>
		<attr name="options" type="mixed" status="optional"></attr>
	</tag>
	<tag name="yaz_database" type="function" returnType="bool">
		<attr name="id" type="resource"></attr>
		<attr name="databases" type="string"></attr>
	</tag>
	<tag name="yaz_element" type="function" returnType="bool">
		<attr name="id" type="resource"></attr>
		<attr name="elementset" type="string"></attr>
	</tag>
	<tag name="yaz_errno" type="function" returnType="int">
		<attr name="id" type="resource"></attr>
	</tag>
	<tag name="yaz_error" type="function" returnType="string">
		<attr name="id" type="resource"></attr>
	</tag>
	<tag name="yaz_es_result" type="function" returnType="array">
		<attr name="id" type="resource"></attr>
	</tag>
	<tag name="yaz_get_option" type="function" returnType="string">
		<attr name="id" type="resource"></attr>
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="yaz_hits" type="function" returnType="int">
		<attr name="id" type="resource"></attr>
	</tag>
	<tag name="yaz_itemorder" type="function" returnType="int">
		<attr name="id" type="resource"></attr>
		<attr name="args" type="array"></attr>
	</tag>
	<tag name="yaz_present" type="function" returnType="bool">
		<attr name="id" type="resource"></attr>
	</tag>
	<tag name="yaz_range" type="function" returnType="bool">
		<attr name="id" type="resource"></attr>
		<attr name="start" type="int"></attr>
		<attr name="number" type="int"></attr>
	</tag>
	<tag name="yaz_record" type="function" returnType="string">
		<attr name="id" type="resource"></attr>
		<attr name="pos" type="int"></attr>
		<attr name="type" type="string"></attr>
	</tag>
	<tag name="yaz_scan_result" type="function" returnType="array">
		<attr name="id" type="resource"></attr>
		<attr name="&amp;result" type="array" status="optional"></attr>
	</tag>
	<tag name="yaz_scan" type="function" returnType="int">
		<attr name="id" type="resource"></attr>
		<attr name="type" type="string"></attr>
		<attr name="startterm" type="string"></attr>
		<attr name="flags" type="array" status="optional"></attr>
	</tag>
	<tag name="yaz_schema" type="function" returnType="int">
		<attr name="id" type="resource"></attr>
		<attr name="schema" type="string"></attr>
	</tag>
	<tag name="yaz_search" type="function" returnType="int">
		<attr name="id" type="resource"></attr>
		<attr name="type" type="string"></attr>
		<attr name="query" type="string"></attr>
	</tag>
	<tag name="yaz_set_option" type="function" returnType="string">
		<attr name="id" type="resource"></attr>
		<attr name="name" type="string"></attr>
		<attr name="value" type="string"></attr>
	</tag>
	<tag name="yaz_sort" type="function" returnType="int">
		<attr name="id" type="resource"></attr>
		<attr name="criteria" type="string"></attr>
	</tag>
	<tag name="yaz_syntax" type="function" returnType="int">
		<attr name="id" type="resource"></attr>
		<attr name="syntax" type="string"></attr>
	</tag>
	<tag name="yaz_wait" type="function" returnType="int">
		<attr name="&amp;options" type="array" status="optional"></attr>
	</tag>
</tags>