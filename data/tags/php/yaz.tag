<!DOCTYPE tags>
<tags>	<tag name="yaz_addinfo" type="function" returnType="int">
		<attr name="id" type="int" status="optional"></attr>
	</tag>
	<tag name="yaz_ccl_conf" type="function" returnType="int">
		<attr name="id" type="int" status="optional"></attr>
		<attr name="config" type="array" status="optional"></attr>
	</tag>
	<tag name="yaz_ccl_parse" type="function" returnType="int">
		<attr name="id" type="int" status="optional"></attr>
		<attr name="query" type="string" status="optional"></attr>
		<attr name="& result" type="array" status="optional"></attr>
	</tag>
	<tag name="yaz_close" type="function" returnType="int">
		<attr name="id" type="int" status="optional"></attr>
	</tag>
	<tag name="yaz_connect" type="function" returnType="int">
		<attr name="zurl" type="string" status="optional"></attr>
		<attr name="options" type="mixed" status="optional"></attr>
	</tag>
	<tag name="yaz_database" type="function" returnType="int">
		<attr name="id" type="int" status="optional"></attr>
		<attr name="databases" type="string" status="optional"></attr>
	</tag>
	<tag name="yaz_element" type="function" returnType="int">
		<attr name="id" type="int" status="optional"></attr>
		<attr name="elementset" type="string" status="optional"></attr>
	</tag>
	<tag name="yaz_errno" type="function" returnType="int">
		<attr name="id" type="int" status="optional"></attr>
	</tag>
	<tag name="yaz_error" type="function" returnType="string">
		<attr name="id" type="int" status="optional"></attr>
	</tag>
	<tag name="yaz_hits" type="function" returnType="int">
		<attr name="id" type="int" status="optional"></attr>
	</tag>
	<tag name="yaz_itemorder" type="function" returnType="int">
		<attr name="args" type="array" status="optional"></attr>
	</tag>
	<tag name="yaz_present" type="function" returnType="int">
	</tag>
	<tag name="yaz_range" type="function" returnType="int">
		<attr name="id" type="int" status="optional"></attr>
		<attr name="start" type="int" status="optional"></attr>
		<attr name="number" type="int" status="optional"></attr>
	</tag>
	<tag name="yaz_record" type="function" returnType="int">
		<attr name="id" type="int" status="optional"></attr>
		<attr name="pos" type="int" status="optional"></attr>
		<attr name="type" type="string" status="optional"></attr>
	</tag>
	<tag name="yaz_scan_result" type="function" returnType="array">
		<attr name="id" type="int" status="optional"></attr>
		<attr name="& result" type="array" status="optional"></attr>
	</tag>
	<tag name="yaz_scan" type="function" returnType="int">
		<attr name="id" type="int" status="optional"></attr>
		<attr name="type" type="string" status="optional"></attr>
		<attr name="startterm" type="string" status="optional"></attr>
		<attr name="flags" type="array" status="optional"></attr>
	</tag>
	<tag name="yaz_search" type="function" returnType="int">
		<attr name="id" type="int" status="optional"></attr>
		<attr name="type" type="string" status="optional"></attr>
		<attr name="query" type="string" status="optional"></attr>
	</tag>
	<tag name="yaz_sort" type="function" returnType="int">
		<attr name="id" type="int" status="optional"></attr>
		<attr name="criteria" type="string" status="optional"></attr>
	</tag>
	<tag name="yaz_syntax" type="function" returnType="int">
		<attr name="id" type="int" status="optional"></attr>
		<attr name="syntax" type="string" status="optional"></attr>
	</tag>
	<tag name="yaz_wait" type="function" returnType="int">
		<attr name="options" type="array" status="optional"></attr>
	</tag>
</tags>
