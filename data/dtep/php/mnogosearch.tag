<!DOCTYPE tags>
<tags>
	<tag name="udm_add_search_limit" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="agent" type="resource" status="required"></attr>
		<attr name="var" type="int" status="required"></attr>
		<attr name="val" type="string" status="required"></attr>
	</tag>
	<tag name="udm_alloc_agent_array" type="function" returnType="resource" version="PHP 4 &gt;= 4.3.3, PHP 5">
		<attr name="databases" type="array" status="required"></attr>
	</tag>
	<tag name="udm_alloc_agent" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="dbaddr" type="string" status="required"></attr>
		<attr name="dbmode" type="string" status="optional"></attr>
	</tag>
	<tag name="udm_api_version" type="function" returnType="int" version="PHP 4 &gt;= 4.0.5, PHP 5">
	</tag>
	<tag name="udm_cat_list" type="function" returnType="array" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="agent" type="resource" status="required"></attr>
		<attr name="category" type="string" status="required"></attr>
	</tag>
	<tag name="udm_cat_path" type="function" returnType="array" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="agent" type="resource" status="required"></attr>
		<attr name="category" type="string" status="required"></attr>
	</tag>
	<tag name="udm_check_charset" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="agent" type="resource" status="required"></attr>
		<attr name="charset" type="string" status="required"></attr>
	</tag>
	<tag name="udm_check_stored" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="agent" type="resource" status="required"></attr>
		<attr name="link" type="int" status="required"></attr>
		<attr name="doc_id" type="string" status="required"></attr>
	</tag>
	<tag name="udm_clear_search_limits" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="agent" type="resource" status="required"></attr>
	</tag>
	<tag name="udm_close_stored" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="agent" type="resource" status="required"></attr>
		<attr name="link" type="int" status="required"></attr>
	</tag>
	<tag name="udm_crc32" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="agent" type="resource" status="required"></attr>
		<attr name="str" type="string" status="required"></attr>
	</tag>
	<tag name="udm_errno" type="function" returnType="int" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="agent" type="resource" status="required"></attr>
	</tag>
	<tag name="udm_error" type="function" returnType="string" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="agent" type="resource" status="required"></attr>
	</tag>
	<tag name="udm_find" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="agent" type="resource" status="required"></attr>
		<attr name="query" type="string" status="required"></attr>
	</tag>
	<tag name="udm_free_agent" type="function" returnType="int" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="agent" type="resource" status="required"></attr>
	</tag>
	<tag name="udm_free_ispell_data" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="agent" type="int" status="required"></attr>
	</tag>
	<tag name="udm_free_res" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="res" type="resource" status="required"></attr>
	</tag>
	<tag name="udm_get_doc_count" type="function" returnType="int" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="agent" type="resource" status="required"></attr>
	</tag>
	<tag name="udm_get_res_field" type="function" returnType="string" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="res" type="resource" status="required"></attr>
		<attr name="row" type="int" status="required"></attr>
		<attr name="field" type="int" status="required"></attr>
	</tag>
	<tag name="udm_get_res_param" type="function" returnType="string" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="res" type="resource" status="required"></attr>
		<attr name="param" type="int" status="required"></attr>
	</tag>
	<tag name="udm_hash32" type="function" returnType="int" version="PHP 4 &gt;= 4.3.3, PHP 5">
		<attr name="agent" type="resource" status="required"></attr>
		<attr name="str" type="string" status="required"></attr>
	</tag>
	<tag name="udm_load_ispell_data" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="agent" type="resource" status="required"></attr>
		<attr name="var" type="int" status="required"></attr>
		<attr name="val1" type="string" status="required"></attr>
		<attr name="val2" type="string" status="required"></attr>
		<attr name="flag" type="int" status="required"></attr>
	</tag>
	<tag name="udm_open_stored" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="agent" type="resource" status="required"></attr>
		<attr name="storedaddr" type="string" status="required"></attr>
	</tag>
	<tag name="udm_set_agent_param" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="agent" type="resource" status="required"></attr>
		<attr name="var" type="int" status="required"></attr>
		<attr name="val" type="string" status="required"></attr>
	</tag>
</tags>