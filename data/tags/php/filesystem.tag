<!DOCTYPE tags>
<tags>	<tag name="basename" type="function" returnType="string">
		<attr name="path" type="string" status="optional"></attr>
		<attr name="suffix" type="string" status="optional"></attr>
	</tag>
	<tag name="chgrp" type="function" returnType="int">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="group" type="mixed" status="optional"></attr>
	</tag>
	<tag name="chmod" type="function" returnType="int">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="chown" type="function" returnType="int">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="user" type="mixed" status="optional"></attr>
	</tag>
	<tag name="clearstatcache" type="function" returnType="void">
	</tag>
	<tag name="copy" type="function" returnType="int">
		<attr name="source" type="string" status="optional"></attr>
		<attr name="dest" type="string" status="optional"></attr>
	</tag>
	<tag name="delete" type="function" returnType="void">
		<attr name="file" type="string" status="optional"></attr>
	</tag>
	<tag name="dirname" type="function" returnType="string">
		<attr name="path" type="string" status="optional"></attr>
	</tag>
	<tag name="disk_free_space" type="function" returnType="float">
		<attr name="directory" type="string" status="optional"></attr>
	</tag>
	<tag name="disk_total_space" type="function" returnType="float">
		<attr name="directory" type="string" status="optional"></attr>
	</tag>
	<tag name="diskfreespace" type="function" returnType="float">
		<attr name="directory" type="string" status="optional"></attr>
	</tag>
	<tag name="fclose" type="function" returnType="bool">
		<attr name="fp" type="int" status="optional"></attr>
	</tag>
	<tag name="feof" type="function" returnType="int">
		<attr name="fp" type="int" status="optional"></attr>
	</tag>
	<tag name="fflush" type="function" returnType="int">
		<attr name="fp" type="int" status="optional"></attr>
	</tag>
	<tag name="fgetc" type="function" returnType="string">
		<attr name="fp" type="int" status="optional"></attr>
	</tag>
	<tag name="fgetcsv" type="function" returnType="array">
		<attr name="fp" type="int" status="optional"></attr>
		<attr name="length" type="int" status="optional"></attr>
		<attr name="delimiter" type="string" status="optional"></attr>
		<attr name="enclosure" type="string" status="optional"></attr>
	</tag>
	<tag name="fgets" type="function" returnType="string">
		<attr name="fp" type="int" status="optional"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="fgetss" type="function" returnType="string">
		<attr name="fp" type="int" status="optional"></attr>
		<attr name="length" type="int" status="optional"></attr>
		<attr name="allowable_tags" type="string" status="optional"></attr>
	</tag>
	<tag name="file_exists" type="function" returnType="bool">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="file_get_contents" type="function" returnType="string">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="use_include_path" type="int" status="optional"></attr>
	</tag>
	<tag name="file_get_wrapper_data" type="function" returnType="mixed">
		<attr name="fp" type="int" status="optional"></attr>
	</tag>
	<tag name="file_register_wrapper" type="function" returnType="boolean">
		<attr name="protocol" type="string" status="optional"></attr>
		<attr name="classname" type="string" status="optional"></attr>
	</tag>
	<tag name="file" type="function" returnType="array">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="use_include_path" type="int" status="optional"></attr>
	</tag>
	<tag name="fileatime" type="function" returnType="int">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="filectime" type="function" returnType="int">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="filegroup" type="function" returnType="int">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="fileinode" type="function" returnType="int">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="filemtime" type="function" returnType="int">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="fileowner" type="function" returnType="int">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="fileperms" type="function" returnType="int">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="filesize" type="function" returnType="int">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="filetype" type="function" returnType="string">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="flock" type="function" returnType="bool">
		<attr name="fp" type="int" status="optional"></attr>
		<attr name="operation" type="int" status="optional"></attr>
		<attr name="&wouldblock" type="int" status="optional"></attr>
	</tag>
	<tag name="fopen" type="function" returnType="int">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="mode" type="string" status="optional"></attr>
		<attr name="use_include_path" type="int" status="optional"></attr>
		<attr name="zcontext" type="resource" status="optional"></attr>
	</tag>
	<tag name="fpassthru" type="function" returnType="int">
		<attr name="fp" type="int" status="optional"></attr>
	</tag>
	<tag name="fputs" type="function" returnType="int">
		<attr name="fp" type="int" status="optional"></attr>
		<attr name="str" type="string" status="optional"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="fread" type="function" returnType="string">
		<attr name="fp" type="int" status="optional"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="fscanf" type="function" returnType="mixed">
		<attr name="handle" type="int" status="optional"></attr>
		<attr name="format" type="string" status="optional"></attr>
		<attr name="var1" type="string" status="optional"></attr>
	</tag>
	<tag name="fseek" type="function" returnType="int">
		<attr name="fp" type="int" status="optional"></attr>
		<attr name="offset" type="int" status="optional"></attr>
		<attr name="whence" type="int" status="optional"></attr>
	</tag>
	<tag name="fstat" type="function" returnType="array">
		<attr name="fp" type="int" status="optional"></attr>
	</tag>
	<tag name="ftell" type="function" returnType="int">
		<attr name="fp" type="int" status="optional"></attr>
	</tag>
	<tag name="ftruncate" type="function" returnType="int">
		<attr name="fp" type="int" status="optional"></attr>
		<attr name="size" type="int" status="optional"></attr>
	</tag>
	<tag name="fwrite" type="function" returnType="int">
		<attr name="fp" type="int" status="optional"></attr>
		<attr name="string" type="string" status="optional"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="glob" type="function" returnType="array">
		<attr name="pattern" type="string" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="is_dir" type="function" returnType="bool">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="is_executable" type="function" returnType="bool">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="is_file" type="function" returnType="bool">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="is_link" type="function" returnType="bool">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="is_readable" type="function" returnType="bool">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="is_uploaded_file" type="function" returnType="bool">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="is_writable" type="function" returnType="bool">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="is_writeable" type="function" returnType="bool">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="link" type="function" returnType="int">
		<attr name="target" type="string" status="optional"></attr>
		<attr name="link" type="string" status="optional"></attr>
	</tag>
	<tag name="linkinfo" type="function" returnType="int">
		<attr name="path" type="string" status="optional"></attr>
	</tag>
	<tag name="lstat" type="function" returnType="array">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="mkdir" type="function" returnType="int">
		<attr name="pathname" type="string" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="move_uploaded_file" type="function" returnType="bool">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="destination" type="string" status="optional"></attr>
	</tag>
	<tag name="parse_ini_file" type="function" returnType="array">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="process_sections" type="bool" status="optional"></attr>
	</tag>
	<tag name="pathinfo" type="function" returnType="array">
		<attr name="path" type="string" status="optional"></attr>
	</tag>
	<tag name="pclose" type="function" returnType="int">
		<attr name="fp" type="int" status="optional"></attr>
	</tag>
	<tag name="popen" type="function" returnType="int">
		<attr name="command" type="string" status="optional"></attr>
		<attr name="mode" type="string" status="optional"></attr>
	</tag>
	<tag name="readfile" type="function" returnType="int">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="use_include_path" type="int" status="optional"></attr>
	</tag>
	<tag name="readlink" type="function" returnType="string">
		<attr name="path" type="string" status="optional"></attr>
	</tag>
	<tag name="realpath" type="function" returnType="string">
		<attr name="path" type="string" status="optional"></attr>
	</tag>
	<tag name="rename" type="function" returnType="bool">
		<attr name="oldname" type="string" status="optional"></attr>
		<attr name="newname" type="string" status="optional"></attr>
	</tag>
	<tag name="rewind" type="function" returnType="int">
		<attr name="fp" type="int" status="optional"></attr>
	</tag>
	<tag name="rmdir" type="function" returnType="bool">
		<attr name="dirname" type="string" status="optional"></attr>
	</tag>
	<tag name="set_file_buffer" type="function" returnType="int">
		<attr name="fp" type="int" status="optional"></attr>
		<attr name="buffer" type="int" status="optional"></attr>
	</tag>
	<tag name="stat" type="function" returnType="array">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="symlink" type="function" returnType="int">
		<attr name="target" type="string" status="optional"></attr>
		<attr name="link" type="string" status="optional"></attr>
	</tag>
	<tag name="tempnam" type="function" returnType="string">
		<attr name="dir" type="string" status="optional"></attr>
		<attr name="prefix" type="string" status="optional"></attr>
	</tag>
	<tag name="tmpfile" type="function" returnType="int">
	</tag>
	<tag name="touch" type="function" returnType="int">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="time" type="int" status="optional"></attr>
		<attr name="atime" type="int" status="optional"></attr>
	</tag>
	<tag name="umask" type="function" returnType="int">
		<attr name="mask" type="int" status="optional"></attr>
	</tag>
	<tag name="unlink" type="function" returnType="int">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
</tags>
