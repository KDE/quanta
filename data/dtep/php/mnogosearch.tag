<!DOCTYPE tags>
<tags>
	<tag name="udm_add_search_limit" type="function" returnType="bool">
		<attr name="agent" type="resource"></attr>
		<attr name="var" type="int"></attr>
		<attr name="val" type="string"></attr>
	</tag>
	<tag name="udm_alloc_agent_array" type="function" returnType="resource">
		<attr name="databases" type="array"></attr>
	</tag>
	<tag name="udm_alloc_agent" type="function" returnType="resource">
		<attr name="dbaddr" type="string"></attr>
		<attr name="dbmode" type="string" status="optional"></attr>
	</tag>
	<tag name="udm_api_version" type="function" returnType="int">
	</tag>
	<tag name="udm_cat_list" type="function" returnType="array">
		<attr name="agent" type="resource"></attr>
		<attr name="category" type="string"></attr>
	</tag>
	<tag name="udm_cat_path" type="function" returnType="array">
		<attr name="agent" type="resource"></attr>
		<attr name="category" type="string"></attr>
	</tag>
	<tag name="udm_check_charset" type="function" returnType="bool">
		<attr name="agent" type="resource"></attr>
		<attr name="charset" type="string"></attr>
	</tag>
	<tag name="udm_check_stored" type="function" returnType="int">
		<attr name="agent" type="resource"></attr>
		<attr name="link" type="int"></attr>
		<attr name="doc_id" type="string"></attr>
	</tag>
	<tag name="udm_clear_search_limits" type="function" returnType="bool">
		<attr name="agent" type="resource"></attr>
	</tag>
	<tag name="udm_close_stored" type="function" returnType="int">
		<attr name="agent" type="resource"></attr>
		<attr name="link" type="int"></attr>
	</tag>
	<tag name="udm_crc32" type="function" returnType="int">
		<attr name="agent" type="resource"></attr>
		<attr name="str" type="string"></attr>
	</tag>
	<tag name="udm_errno" type="function" returnType="int">
		<attr name="agent" type="resource"></attr>
	</tag>
	<tag name="udm_error" type="function" returnType="string">
		<attr name="agent" type="resource"></attr>
	</tag>
	<tag name="udm_find" type="function" returnType="resource">
		<attr name="agent" type="resource"></attr>
		<attr name="query" type="string"></attr>
	</tag>
	<tag name="udm_free_agent" type="function" returnType="int">
		<attr name="agent" type="resource"></attr>
	</tag>
	<tag name="udm_free_ispell_data" type="function" returnType="bool">
		<attr name="agent" type="int"></attr>
	</tag>
	<tag name="udm_free_res" type="function" returnType="bool">
		<attr name="res" type="resource"></attr>
	</tag>
	<tag name="udm_get_doc_count" type="function" returnType="int">
		<attr name="agent" type="resource"></attr>
	</tag>
	<tag name="udm_get_res_field" type="function" returnType="string">
		<attr name="res" type="resource"></attr>
		<attr name="row" type="int"></attr>
		<attr name="field" type="int"></attr>
	</tag>
	<tag name="udm_get_res_param" type="function" returnType="string">
		<attr name="res" type="resource"></attr>
		<attr name="param" type="int"></attr>
	</tag>
	<tag name="udm_hash32" type="function" returnType="int">
		<attr name="agent" type="resource"></attr>
		<attr name="str" type="string"></attr>
	</tag>
	<tag name="udm_load_ispell_data" type="function" returnType="bool">
		<attr name="agent" type="resource"></attr>
		<attr name="var" type="int"></attr>
		<attr name="val1" type="string"></attr>
		<attr name="val2" type="string"></attr>
		<attr name="flag" type="int"></attr>
	</tag>
	<tag name="udm_open_stored" type="function" returnType="int">
		<attr name="agent" type="resource"></attr>
		<attr name="storedaddr" type="string"></attr>
	</tag>
	<tag name="udm_set_agent_param" type="function" returnType="bool">
		<attr name="agent" type="resource"></attr>
		<attr name="var" type="int"></attr>
		<attr name="val" type="string"></attr>
	</tag>
</tags>