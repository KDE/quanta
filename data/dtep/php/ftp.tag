<!DOCTYPE tags>
<tags>
	<tag name="ftp_alloc" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="filesize" type="int"></attr>
		<attr name="&amp;result" type="string" status="optional"></attr>
	</tag>
	<tag name="ftp_cdup" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource"></attr>
	</tag>
	<tag name="ftp_chdir" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="directory" type="string"></attr>
	</tag>
	<tag name="ftp_chmod" type="function" returnType="int">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="mode" type="int"></attr>
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="ftp_close" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource"></attr>
	</tag>
	<tag name="ftp_connect" type="function" returnType="resource">
		<attr name="host" type="string"></attr>
		<attr name="port" type="int" status="optional"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_delete" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="path" type="string"></attr>
	</tag>
	<tag name="ftp_exec" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="command" type="string"></attr>
	</tag>
	<tag name="ftp_fget" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="handle" type="resource"></attr>
		<attr name="remote_file" type="string"></attr>
		<attr name="mode" type="int"></attr>
		<attr name="resumepos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_fput" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="remote_file" type="string"></attr>
		<attr name="handle" type="resource"></attr>
		<attr name="mode" type="int"></attr>
		<attr name="startpos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_get_option" type="function" returnType="mixed">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="option" type="int"></attr>
	</tag>
	<tag name="ftp_get" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="local_file" type="string"></attr>
		<attr name="remote_file" type="string"></attr>
		<attr name="mode" type="int"></attr>
		<attr name="resumepos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_login" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="username" type="string"></attr>
		<attr name="password" type="string"></attr>
	</tag>
	<tag name="ftp_mdtm" type="function" returnType="int">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="remote_file" type="string"></attr>
	</tag>
	<tag name="ftp_mkdir" type="function" returnType="string">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="directory" type="string"></attr>
	</tag>
	<tag name="ftp_nb_continue" type="function" returnType="int">
		<attr name="ftp_stream" type="resource"></attr>
	</tag>
	<tag name="ftp_nb_fget" type="function" returnType="int">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="handle" type="resource"></attr>
		<attr name="remote_file" type="string"></attr>
		<attr name="mode" type="int"></attr>
		<attr name="resumepos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_nb_fput" type="function" returnType="int">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="remote_file" type="string"></attr>
		<attr name="handle" type="resource"></attr>
		<attr name="mode" type="int"></attr>
		<attr name="startpos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_nb_get" type="function" returnType="int">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="local_file" type="string"></attr>
		<attr name="remote_file" type="string"></attr>
		<attr name="mode" type="int"></attr>
		<attr name="resumepos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_nb_put" type="function" returnType="int">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="remote_file" type="string"></attr>
		<attr name="local_file" type="string"></attr>
		<attr name="mode" type="int"></attr>
		<attr name="startpos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_nlist" type="function" returnType="array">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="directory" type="string"></attr>
	</tag>
	<tag name="ftp_pasv" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="pasv" type="bool"></attr>
	</tag>
	<tag name="ftp_put" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="remote_file" type="string"></attr>
		<attr name="local_file" type="string"></attr>
		<attr name="mode" type="int"></attr>
		<attr name="startpos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_pwd" type="function" returnType="string">
		<attr name="ftp_stream" type="resource"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="ftp_raw" type="function" returnType="array">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="command" type="string"></attr>
	</tag>
	<tag name="ftp_rawlist" type="function" returnType="array">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="directory" type="string"></attr>
		<attr name="recursive" type="bool" status="optional"></attr>
	</tag>
	<tag name="ftp_rename" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="from" type="string"></attr>
		<attr name="to" type="string"></attr>
	</tag>
	<tag name="ftp_rmdir" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="directory" type="string"></attr>
	</tag>
	<tag name="ftp_set_option" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="option" type="int"></attr>
		<attr name="value" type="mixed"></attr>
	</tag>
	<tag name="ftp_site" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="cmd" type="string"></attr>
	</tag>
	<tag name="ftp_size" type="function" returnType="int">
		<attr name="ftp_stream" type="resource"></attr>
		<attr name="remote_file" type="string"></attr>
	</tag>
	<tag name="ftp_ssl_connect" type="function" returnType="resource">
		<attr name="host" type="string"></attr>
		<attr name="port" type="int" status="optional"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_systype" type="function" returnType="string">
		<attr name="ftp_stream" type="resource"></attr>
	</tag>
</tags>