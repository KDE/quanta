<!DOCTYPE tags>
<tags>
	<tag name="msql_affected_rows" type="function" returnType="int">
		<attr name="query_identifier" type="resource"></attr>
	</tag>
	<tag name="msql_close" type="function" returnType="int">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="msql_connect" type="function" returnType="int">
		<attr name="hostname" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="msql_create_db" type="function" returnType="bool">
		<attr name="database_name" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="msql_data_seek" type="function" returnType="bool">
		<attr name="query_identifier" type="resource"></attr>
		<attr name="row_number" type="int"></attr>
	</tag>
	<tag name="msql_db_query" type="function" returnType="resource">
		<attr name="database" type="string"></attr>
		<attr name="query" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="msql_drop_db" type="function" returnType="int">
		<attr name="database_name" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="msql_error" type="function" returnType="string">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="msql_fetch_array" type="function" returnType="int">
		<attr name="query_identifier" type="int"></attr>
		<attr name="result_type" type="int" status="optional"></attr>
	</tag>
	<tag name="msql_fetch_field" type="function" returnType="object">
		<attr name="query_identifier" type="resource"></attr>
		<attr name="field_offset" type="int" status="optional"></attr>
	</tag>
	<tag name="msql_fetch_object" type="function" returnType="int">
		<attr name="query_identifier" type="int"></attr>
	</tag>
	<tag name="msql_fetch_row" type="function" returnType="array">
		<attr name="query_identifier" type="resource"></attr>
	</tag>
	<tag name="msql_field_flags" type="function" returnType="string">
		<attr name="query_identifier" type="resource"></attr>
		<attr name="field_offset" type="int"></attr>
	</tag>
	<tag name="msql_field_len" type="function" returnType="int">
		<attr name="query_identifier" type="resource"></attr>
		<attr name="field_offset" type="int"></attr>
	</tag>
	<tag name="msql_field_name" type="function" returnType="string">
		<attr name="query_identifier" type="resource"></attr>
		<attr name="field" type="int"></attr>
	</tag>
	<tag name="msql_field_seek" type="function" returnType="int">
		<attr name="query_identifier" type="int"></attr>
		<attr name="field_offset" type="int"></attr>
	</tag>
	<tag name="msql_field_table" type="function" returnType="int">
		<attr name="query_identifier" type="int"></attr>
		<attr name="field" type="int"></attr>
	</tag>
	<tag name="msql_field_type" type="function" returnType="string">
		<attr name="query_identifier" type="resource"></attr>
		<attr name="field_offset" type="int"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="msql_free_result" type="function" returnType="int">
		<attr name="query_identifier" type="resource"></attr>
	</tag>
	<tag name="msql_list_dbs" type="function" returnType="resource">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="msql_list_fields" type="function" returnType="resource">
		<attr name="database" type="string"></attr>
		<attr name="tablename" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="msql_list_tables" type="function" returnType="resource">
		<attr name="database" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="msql_num_fields" type="function" returnType="int">
		<attr name="query_identifier" type="resource"></attr>
	</tag>
	<tag name="msql_num_rows" type="function" returnType="int">
		<attr name="query_identifier" type="resource"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="msql_pconnect" type="function" returnType="int">
		<attr name="server" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="msql_query" type="function" returnType="resource">
		<attr name="query" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="msql_result" type="function" returnType="string">
		<attr name="query_identifier" type="resource"></attr>
		<attr name="row" type="int"></attr>
		<attr name="field" type="mixed" status="optional"></attr>
	</tag>
	<tag name="msql_select_db" type="function" returnType="bool">
		<attr name="database_name" type="string"></attr>
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
</tags>