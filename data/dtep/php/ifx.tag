<!DOCTYPE tags>
<tags>
	<tag name="ifx_affected_rows" type="function" returnType="int">
		<attr name="result_id" type="int"></attr>
	</tag>
	<tag name="ifx_blobinfile_mode" type="function" returnType="void">
		<attr name="mode" type="int"></attr>
	</tag>
	<tag name="ifx_byteasvarchar" type="function" returnType="void">
		<attr name="mode" type="int"></attr>
	</tag>
	<tag name="ifx_close" type="function" returnType="int">
		<attr name="link_identifier" type="int" status="optional"></attr>
	</tag>
	<tag name="ifx_connect" type="function" returnType="int">
		<attr name="database" type="string" status="optional"></attr>
		<attr name="userid" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="ifx_copy_blob" type="function" returnType="int">
		<attr name="bid" type="int"></attr>
	</tag>
	<tag name="ifx_create_blob" type="function" returnType="int">
		<attr name="type" type="int"></attr>
		<attr name="mode" type="int"></attr>
		<attr name="param" type="string"></attr>
	</tag>
	<tag name="ifx_create_char" type="function" returnType="int">
		<attr name="param" type="string"></attr>
	</tag>
	<tag name="ifx_do" type="function" returnType="int">
		<attr name="result_id" type="int"></attr>
	</tag>
	<tag name="ifx_error" type="function" returnType="string">
	</tag>
	<tag name="ifx_errormsg" type="function" returnType="string">
		<attr name="errorcode" type="int" status="optional"></attr>
	</tag>
	<tag name="ifx_fetch_row" type="function" returnType="array">
		<attr name="result_id" type="int"></attr>
		<attr name="position" type="mixed" status="optional"></attr>
	</tag>
	<tag name="ifx_fieldproperties" type="function" returnType="array">
		<attr name="result_id" type="int"></attr>
	</tag>
	<tag name="ifx_fieldtypes" type="function" returnType="array">
		<attr name="result_id" type="int"></attr>
	</tag>
	<tag name="ifx_free_blob" type="function" returnType="int">
		<attr name="bid" type="int"></attr>
	</tag>
	<tag name="ifx_free_char" type="function" returnType="int">
		<attr name="bid" type="int"></attr>
	</tag>
	<tag name="ifx_free_result" type="function" returnType="int">
		<attr name="result_id" type="int"></attr>
	</tag>
	<tag name="ifx_get_blob" type="function" returnType="int">
		<attr name="bid" type="int"></attr>
	</tag>
	<tag name="ifx_get_char" type="function" returnType="int">
		<attr name="bid" type="int"></attr>
	</tag>
	<tag name="ifx_getsqlca" type="function" returnType="array">
		<attr name="result_id" type="int"></attr>
	</tag>
	<tag name="ifx_htmltbl_result" type="function" returnType="int">
		<attr name="result_id" type="int"></attr>
		<attr name="html_table_options" type="string" status="optional"></attr>
	</tag>
	<tag name="ifx_nullformat" type="function" returnType="void">
		<attr name="mode" type="int"></attr>
	</tag>
	<tag name="ifx_num_fields" type="function" returnType="int">
		<attr name="result_id" type="int"></attr>
	</tag>
	<tag name="ifx_num_rows" type="function" returnType="int">
		<attr name="result_id" type="int"></attr>
	</tag>
	<tag name="ifx_pconnect" type="function" returnType="int">
		<attr name="database" type="string" status="optional"></attr>
		<attr name="userid" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="ifx_prepare" type="function" returnType="int">
		<attr name="query" type="string"></attr>
		<attr name="conn_id" type="int"></attr>
		<attr name="cursor_def" type="int" status="optional"></attr>
		<attr name="blobidarray" type="mixed" status="optional"></attr>
	</tag>
	<tag name="ifx_query" type="function" returnType="int">
		<attr name="query" type="string"></attr>
		<attr name="link_identifier" type="int"></attr>
		<attr name="cursor_type" type="int" status="optional"></attr>
		<attr name="blobidarray" type="mixed" status="optional"></attr>
	</tag>
	<tag name="ifx_textasvarchar" type="function" returnType="void">
		<attr name="mode" type="int"></attr>
	</tag>
	<tag name="ifx_update_blob" type="function" returnType="bool">
		<attr name="bid" type="int"></attr>
		<attr name="content" type="string"></attr>
	</tag>
	<tag name="ifx_update_char" type="function" returnType="int">
		<attr name="bid" type="int"></attr>
		<attr name="content" type="string"></attr>
	</tag>
	<tag name="ifxus_close_slob" type="function" returnType="int">
		<attr name="bid" type="int"></attr>
	</tag>
	<tag name="ifxus_create_slob" type="function" returnType="int">
		<attr name="mode" type="int"></attr>
	</tag>
	<tag name="ifxus_free_slob" type="function" returnType="int">
		<attr name="bid" type="int"></attr>
	</tag>
	<tag name="ifxus_open_slob" type="function" returnType="int">
		<attr name="bid" type="int"></attr>
		<attr name="mode" type="int"></attr>
	</tag>
	<tag name="ifxus_read_slob" type="function" returnType="int">
		<attr name="bid" type="int"></attr>
		<attr name="nbytes" type="int"></attr>
	</tag>
	<tag name="ifxus_seek_slob" type="function" returnType="int">
		<attr name="bid" type="int"></attr>
		<attr name="mode" type="int"></attr>
		<attr name="offset" type="int"></attr>
	</tag>
	<tag name="ifxus_tell_slob" type="function" returnType="int">
		<attr name="bid" type="int"></attr>
	</tag>
	<tag name="ifxus_write_slob" type="function" returnType="int">
		<attr name="bid" type="int"></attr>
		<attr name="content" type="string"></attr>
	</tag>
</tags>