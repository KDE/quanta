<!DOCTYPE tags>
<tags>	<tag name="ftp_async_continue" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
	</tag>
	<tag name="ftp_async_fget" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
		<attr name="fp" type="resource" status="optional"></attr>
		<attr name="remote_file" type="string" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="resumepos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_async_fput" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
		<attr name="remote_file" type="string" status="optional"></attr>
		<attr name="fp" type="resource" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="startpos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_async_get" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
		<attr name="local_file" type="string" status="optional"></attr>
		<attr name="remote_file" type="string" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="resumepos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_async_put" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
		<attr name="remote_file" type="string" status="optional"></attr>
		<attr name="local_file" type="string" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="startpos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_cdup" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
	</tag>
	<tag name="ftp_chdir" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
		<attr name="directory" type="string" status="optional"></attr>
	</tag>
	<tag name="ftp_close" type="function" returnType="void">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
	</tag>
	<tag name="ftp_connect" type="function" returnType="resource">
		<attr name="host" type="string" status="optional"></attr>
		<attr name="port" type="int" status="optional"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_delete" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
		<attr name="path" type="string" status="optional"></attr>
	</tag>
	<tag name="ftp_exec" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
		<attr name="command" type="string" status="optional"></attr>
	</tag>
	<tag name="ftp_fget" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
		<attr name="fp" type="resource" status="optional"></attr>
		<attr name="remote_file" type="string" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="resumepos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_fput" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
		<attr name="remote_file" type="string" status="optional"></attr>
		<attr name="fp" type="resource" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="startpos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_get_option" type="function" returnType="mixed">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
		<attr name="option" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_get" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
		<attr name="local_file" type="string" status="optional"></attr>
		<attr name="remote_file" type="string" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="resumepos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_login" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="ftp_mdtm" type="function" returnType="int">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
		<attr name="remote_file" type="string" status="optional"></attr>
	</tag>
	<tag name="ftp_mkdir" type="function" returnType="string">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
		<attr name="directory" type="string" status="optional"></attr>
	</tag>
	<tag name="ftp_nlist" type="function" returnType="array">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
		<attr name="directory" type="string" status="optional"></attr>
	</tag>
	<tag name="ftp_pasv" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
		<attr name="pasv" type="bool" status="optional"></attr>
	</tag>
	<tag name="ftp_put" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
		<attr name="remote_file" type="string" status="optional"></attr>
		<attr name="local_file" type="string" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="startpos" type="int" status="optional"></attr>
	</tag>
	<tag name="ftp_pwd" type="function" returnType="string">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
	</tag>
	<tag name="ftp_quit" type="function" returnType="void">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
	</tag>
	<tag name="ftp_rawlist" type="function" returnType="array">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
		<attr name="directory" type="string" status="optional"></attr>
	</tag>
	<tag name="ftp_rename" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
		<attr name="from" type="string" status="optional"></attr>
		<attr name="to" type="string" status="optional"></attr>
	</tag>
	<tag name="ftp_rmdir" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
		<attr name="directory" type="string" status="optional"></attr>
	</tag>
	<tag name="ftp_set_option" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
		<attr name="option" type="int" status="optional"></attr>
		<attr name="value" type="mixed" status="optional"></attr>
	</tag>
	<tag name="ftp_site" type="function" returnType="bool">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
		<attr name="cmd" type="string" status="optional"></attr>
	</tag>
	<tag name="ftp_size" type="function" returnType="int">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
		<attr name="remote_file" type="string" status="optional"></attr>
	</tag>
	<tag name="ftp_systype" type="function" returnType="string">
		<attr name="ftp_stream" type="resource" status="optional"></attr>
	</tag>
</tags>
