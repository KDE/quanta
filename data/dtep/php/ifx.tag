<!DOCTYPE tags>
<tags>
	<tag name="ifx_affected_rows" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="result_id" type="int" status="required"></attr>
	</tag>
	<tag name="ifx_blobinfile_mode" type="function" returnType="void" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="mode" type="int" status="required"></attr>
	</tag>
	<tag name="ifx_byteasvarchar" type="function" returnType="void" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="mode" type="int" status="required"></attr>
	</tag>
	<tag name="ifx_close" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="link_identifier" type="int" status="optional"></attr>
	</tag>
	<tag name="ifx_connect" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="database" type="string" status="optional"></attr>
		<attr name="userid" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="ifx_copy_blob" type="function" returnType="int" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="bid" type="int" status="required"></attr>
	</tag>
	<tag name="ifx_create_blob" type="function" returnType="int" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="type" type="int" status="required"></attr>
		<attr name="mode" type="int" status="required"></attr>
		<attr name="param" type="string" status="required"></attr>
	</tag>
	<tag name="ifx_create_char" type="function" returnType="int" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="param" type="string" status="required"></attr>
	</tag>
	<tag name="ifx_do" type="function" returnType="int" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="result_id" type="int" status="required"></attr>
	</tag>
	<tag name="ifx_error" type="function" returnType="string" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
	</tag>
	<tag name="ifx_errormsg" type="function" returnType="string" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="errorcode" type="int" status="optional"></attr>
	</tag>
	<tag name="ifx_fetch_row" type="function" returnType="array" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="result_id" type="int" status="required"></attr>
		<attr name="position" type="mixed" status="optional"></attr>
	</tag>
	<tag name="ifx_fieldproperties" type="function" returnType="array" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="result_id" type="int" status="required"></attr>
	</tag>
	<tag name="ifx_fieldtypes" type="function" returnType="array" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="result_id" type="int" status="required"></attr>
	</tag>
	<tag name="ifx_free_blob" type="function" returnType="int" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="bid" type="int" status="required"></attr>
	</tag>
	<tag name="ifx_free_char" type="function" returnType="int" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="bid" type="int" status="required"></attr>
	</tag>
	<tag name="ifx_free_result" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="result_id" type="int" status="required"></attr>
	</tag>
	<tag name="ifx_get_blob" type="function" returnType="int" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="bid" type="int" status="required"></attr>
	</tag>
	<tag name="ifx_get_char" type="function" returnType="int" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="bid" type="int" status="required"></attr>
	</tag>
	<tag name="ifx_getsqlca" type="function" returnType="array" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="result_id" type="int" status="required"></attr>
	</tag>
	<tag name="ifx_htmltbl_result" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="result_id" type="int" status="required"></attr>
		<attr name="html_table_options" type="string" status="optional"></attr>
	</tag>
	<tag name="ifx_nullformat" type="function" returnType="void" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="mode" type="int" status="required"></attr>
	</tag>
	<tag name="ifx_num_fields" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="result_id" type="int" status="required"></attr>
	</tag>
	<tag name="ifx_num_rows" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="result_id" type="int" status="required"></attr>
	</tag>
	<tag name="ifx_pconnect" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="database" type="string" status="optional"></attr>
		<attr name="userid" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="ifx_prepare" type="function" returnType="int" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="query" type="string" status="required"></attr>
		<attr name="conn_id" type="int" status="required"></attr>
		<attr name="cursor_def" type="int" status="optional"></attr>
		<attr name="blobidarray" type="mixed" status="required"></attr>
	</tag>
	<tag name="ifx_query" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="query" type="string" status="required"></attr>
		<attr name="link_identifier" type="int" status="required"></attr>
		<attr name="cursor_type" type="int" status="optional"></attr>
		<attr name="blobidarray" type="mixed" status="optional"></attr>
	</tag>
	<tag name="ifx_textasvarchar" type="function" returnType="void" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="mode" type="int" status="required"></attr>
	</tag>
	<tag name="ifx_update_blob" type="function" returnType="bool" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="bid" type="int" status="required"></attr>
		<attr name="content" type="string" status="required"></attr>
	</tag>
	<tag name="ifx_update_char" type="function" returnType="int" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="bid" type="int" status="required"></attr>
		<attr name="content" type="string" status="required"></attr>
	</tag>
	<tag name="ifxus_close_slob" type="function" returnType="int" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="bid" type="int" status="required"></attr>
	</tag>
	<tag name="ifxus_create_slob" type="function" returnType="int" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="mode" type="int" status="required"></attr>
	</tag>
	<tag name="ifxus_free_slob" type="function" returnType="int" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="bid" type="int" status="required"></attr>
	</tag>
	<tag name="ifxus_open_slob" type="function" returnType="int" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="bid" type="int" status="required"></attr>
		<attr name="mode" type="int" status="required"></attr>
	</tag>
	<tag name="ifxus_read_slob" type="function" returnType="int" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="bid" type="int" status="required"></attr>
		<attr name="nbytes" type="int" status="required"></attr>
	</tag>
	<tag name="ifxus_seek_slob" type="function" returnType="int" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="bid" type="int" status="required"></attr>
		<attr name="mode" type="int" status="required"></attr>
		<attr name="offset" type="int" status="required"></attr>
	</tag>
	<tag name="ifxus_tell_slob" type="function" returnType="int" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="bid" type="int" status="required"></attr>
	</tag>
	<tag name="ifxus_write_slob" type="function" returnType="int" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="bid" type="int" status="required"></attr>
		<attr name="content" type="string" status="required"></attr>
	</tag>
</tags>