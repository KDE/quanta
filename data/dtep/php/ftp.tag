<!DOCTYPE tags>
<tags>
	<tag name="ftp_alloc" type="function" returnType="bool" version="PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="filesize" type="int" status="required"></attr>
		<attr name="&amp;result" type="string" status="optional"></attr>
	</tag>
	<tag name="ftp_cdup" type="function" returnType="bool" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
	</tag>
	<tag name="ftp_chdir" type="function" returnType="bool" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="directory" type="string" status="required"></attr>
	</tag>
	<tag name="ftp_chmod" type="function" returnType="int" version="PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="mode" type="int" status="required"></attr>
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="ftp_close" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
	</tag>
	<tag name="ftp_connect" type="function" returnType="resource" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="host" type="string" status="required"></attr>
		<attr name="port" type="int" status="optional"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_delete" type="function" returnType="bool" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="path" type="string" status="required"></attr>
	</tag>
	<tag name="ftp_exec" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.3, PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="command" type="string" status="required"></attr>
	</tag>
	<tag name="ftp_fget" type="function" returnType="bool" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="remote_file" type="string" status="required"></attr>
		<attr name="mode" type="int" status="required"></attr>
		<attr name="resumepos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_fput" type="function" returnType="bool" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="remote_file" type="string" status="required"></attr>
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="mode" type="int" status="required"></attr>
		<attr name="startpos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_get_option" type="function" returnType="mixed" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="option" type="int" status="required"></attr>
	</tag>
	<tag name="ftp_get" type="function" returnType="bool" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="local_file" type="string" status="required"></attr>
		<attr name="remote_file" type="string" status="required"></attr>
		<attr name="mode" type="int" status="required"></attr>
		<attr name="resumepos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_login" type="function" returnType="bool" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
	</tag>
	<tag name="ftp_mdtm" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="remote_file" type="string" status="required"></attr>
	</tag>
	<tag name="ftp_mkdir" type="function" returnType="string" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="directory" type="string" status="required"></attr>
	</tag>
	<tag name="ftp_nb_continue" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
	</tag>
	<tag name="ftp_nb_fget" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="remote_file" type="string" status="required"></attr>
		<attr name="mode" type="int" status="required"></attr>
		<attr name="resumepos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_nb_fput" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="remote_file" type="string" status="required"></attr>
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="mode" type="int" status="required"></attr>
		<attr name="startpos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_nb_get" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="local_file" type="string" status="required"></attr>
		<attr name="remote_file" type="string" status="required"></attr>
		<attr name="mode" type="int" status="required"></attr>
		<attr name="resumepos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_nb_put" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="remote_file" type="string" status="required"></attr>
		<attr name="local_file" type="string" status="required"></attr>
		<attr name="mode" type="int" status="required"></attr>
		<attr name="startpos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_nlist" type="function" returnType="array" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="directory" type="string" status="required"></attr>
	</tag>
	<tag name="ftp_pasv" type="function" returnType="bool" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="pasv" type="bool" status="required"></attr>
	</tag>
	<tag name="ftp_put" type="function" returnType="bool" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="remote_file" type="string" status="required"></attr>
		<attr name="local_file" type="string" status="required"></attr>
		<attr name="mode" type="int" status="required"></attr>
		<attr name="startpos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_pwd" type="function" returnType="string" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
	</tag>
	<tag name="ftp_raw" type="function" returnType="array" version="PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="command" type="string" status="required"></attr>
	</tag>
	<tag name="ftp_rawlist" type="function" returnType="array" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="directory" type="string" status="required"></attr>
		<attr name="recursive" type="bool" status="optional"></attr>
	</tag>
	<tag name="ftp_rename" type="function" returnType="bool" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="from" type="string" status="required"></attr>
		<attr name="to" type="string" status="required"></attr>
	</tag>
	<tag name="ftp_rmdir" type="function" returnType="bool" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="directory" type="string" status="required"></attr>
	</tag>
	<tag name="ftp_set_option" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="option" type="int" status="required"></attr>
		<attr name="value" type="mixed" status="required"></attr>
	</tag>
	<tag name="ftp_site" type="function" returnType="bool" version="PHP 3&gt;= 3.0.15, PHP 4 , PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="cmd" type="string" status="required"></attr>
	</tag>
	<tag name="ftp_size" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
		<attr name="remote_file" type="string" status="required"></attr>
	</tag>
	<tag name="ftp_ssl_connect" type="function" returnType="resource" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="host" type="string" status="required"></attr>
		<attr name="port" type="int" status="optional"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_systype" type="function" returnType="string" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="ftp_stream" type="resource" status="required"></attr>
	</tag>
</tags>