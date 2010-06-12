<!DOCTYPE tags>
<tags>	<tag name="udm_add_search_limit" type="function" returnType="int">
		<attr name="agent" type="int" status="optional"></attr>
		<attr name="var" type="int" status="optional"></attr>
		<attr name="val" type="string" status="optional"></attr>
	</tag>
	<tag name="udm_alloc_agent" type="function" returnType="int">
		<attr name="dbaddr" type="string" status="optional"></attr>
		<attr name="dbmode" type="string" status="optional"></attr>
	</tag>
	<tag name="udm_api_version" type="function" returnType="int">
	</tag>
	<tag name="udm_cat_list" type="function" returnType="array">
		<attr name="agent" type="int" status="optional"></attr>
		<attr name="category" type="string" status="optional"></attr>
	</tag>
	<tag name="udm_cat_path" type="function" returnType="array">
		<attr name="agent" type="int" status="optional"></attr>
		<attr name="category" type="string" status="optional"></attr>
	</tag>
	<tag name="udm_check_charset" type="function" returnType="int">
		<attr name="agent" type="int" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="udm_check_stored" type="function" returnType="int">
		<attr name="agent" type="int" status="optional"></attr>
		<attr name="link" type="int" status="optional"></attr>
		<attr name="doc_id" type="string" status="optional"></attr>
	</tag>
	<tag name="udm_clear_search_limits" type="function" returnType="int">
		<attr name="agent" type="int" status="optional"></attr>
	</tag>
	<tag name="udm_close_stored" type="function" returnType="int">
		<attr name="agent" type="int" status="optional"></attr>
		<attr name="link" type="int" status="optional"></attr>
	</tag>
	<tag name="udm_crc32" type="function" returnType="int">
		<attr name="agent" type="int" status="optional"></attr>
		<attr name="str" type="string" status="optional"></attr>
	</tag>
	<tag name="udm_errno" type="function" returnType="int">
		<attr name="agent" type="int" status="optional"></attr>
	</tag>
	<tag name="udm_error" type="function" returnType="string">
		<attr name="agent" type="int" status="optional"></attr>
	</tag>
	<tag name="udm_find" type="function" returnType="int">
		<attr name="agent" type="int" status="optional"></attr>
		<attr name="query" type="string" status="optional"></attr>
	</tag>
	<tag name="udm_free_agent" type="function" returnType="int">
		<attr name="agent" type="int" status="optional"></attr>
	</tag>
	<tag name="udm_free_ispell_data" type="function" returnType="int">
		<attr name="agent" type="int" status="optional"></attr>
	</tag>
	<tag name="udm_free_res" type="function" returnType="int">
		<attr name="res" type="int" status="optional"></attr>
	</tag>
	<tag name="udm_get_doc_count" type="function" returnType="int">
		<attr name="agent" type="int" status="optional"></attr>
	</tag>
	<tag name="udm_get_res_field" type="function" returnType="string">
		<attr name="res" type="int" status="optional"></attr>
		<attr name="row" type="int" status="optional"></attr>
		<attr name="field" type="int" status="optional"></attr>
	</tag>
	<tag name="udm_get_res_param" type="function" returnType="string">
		<attr name="res" type="int" status="optional"></attr>
		<attr name="param" type="int" status="optional"></attr>
	</tag>
	<tag name="udm_load_ispell_data" type="function" returnType="int">
		<attr name="agent" type="int" status="optional"></attr>
		<attr name="var" type="int" status="optional"></attr>
		<attr name="val1" type="string" status="optional"></attr>
		<attr name="val2" type="string" status="optional"></attr>
		<attr name="flag" type="int" status="optional"></attr>
	</tag>
	<tag name="udm_open_stored" type="function" returnType="int">
		<attr name="agent" type="int" status="optional"></attr>
		<attr name="storedaddr" type="string" status="optional"></attr>
	</tag>
	<tag name="udm_set_agent_param" type="function" returnType="int">
		<attr name="agent" type="int" status="optional"></attr>
		<attr name="var" type="int" status="optional"></attr>
		<attr name="val" type="string" status="optional"></attr>
	</tag>
</tags>
