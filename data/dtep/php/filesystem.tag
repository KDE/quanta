<!DOCTYPE tags>
<tags>
	<tag name="basename" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="path" type="string" status="required"></attr>
		<attr name="suffix" type="string" status="optional"></attr>
	</tag>
	<tag name="chgrp" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="group" type="mixed" status="required"></attr>
	</tag>
	<tag name="chmod" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="mode" type="int" status="required"></attr>
	</tag>
	<tag name="chown" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="user" type="mixed" status="required"></attr>
	</tag>
	<tag name="clearstatcache" type="function" returnType="void" version="PHP 3, PHP 4 , PHP 5">
	</tag>
	<tag name="copy" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="source" type="string" status="required"></attr>
		<attr name="dest" type="string" status="required"></attr>
	</tag>
	<tag name="delete" type="function" returnType="void" version="">
		<attr name="file" type="string" status="required"></attr>
	</tag>
	<tag name="dirname" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="path" type="string" status="required"></attr>
	</tag>
	<tag name="disk_free_space" type="function" returnType="float" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="directory" type="string" status="required"></attr>
	</tag>
	<tag name="disk_total_space" type="function" returnType="float" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="directory" type="string" status="required"></attr>
	</tag>
	<tag name="fclose" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="feof" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="fflush" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="fgetc" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="fgetcsv" type="function" returnType="array" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="length" type="int" status="optional"></attr>
		<attr name="delimiter" type="string" status="optional"></attr>
		<attr name="enclosure" type="string" status="optional"></attr>
	</tag>
	<tag name="fgets" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="fgetss" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="length" type="int" status="optional"></attr>
		<attr name="allowable_tags" type="string" status="optional"></attr>
	</tag>
	<tag name="file_exists" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="file_get_contents" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="use_include_path" type="bool" status="optional"></attr>
		<attr name="context" type="resource" status="optional"></attr>
	</tag>
	<tag name="file_put_contents" type="function" returnType="int" version="PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="context" type="resource" status="optional"></attr>
	</tag>
	<tag name="file" type="function" returnType="array" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="use_include_path" type="int" status="optional"></attr>
		<attr name="context" type="resource" status="optional"></attr>
	</tag>
	<tag name="fileatime" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="filectime" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="filegroup" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="fileinode" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="filemtime" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="fileowner" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="fileperms" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="filesize" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="filetype" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="flock" type="function" returnType="bool" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="operation" type="int" status="required"></attr>
		<attr name="&amp;wouldblock" type="int" status="optional"></attr>
	</tag>
	<tag name="fnmatch" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="pattern" type="string" status="required"></attr>
		<attr name="string" type="string" status="required"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="fopen" type="function" returnType="resource" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="mode" type="string" status="required"></attr>
		<attr name="use_include_path" type="bool" status="optional"></attr>
		<attr name="zcontext" type="resource" status="optional"></attr>
	</tag>
	<tag name="fpassthru" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="fputcsv" type="function" returnType="int" version="">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="fields" type="array" status="required"></attr>
		<attr name="delimiter" type="string" status="optional"></attr>
		<attr name="enclosure" type="string" status="optional"></attr>
	</tag>
	<tag name="fread" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="length" type="int" status="required"></attr>
	</tag>
	<tag name="fscanf" type="function" returnType="mixed" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="format" type="string" status="required"></attr>
		<attr name="&amp;..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="fseek" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="offset" type="int" status="required"></attr>
		<attr name="whence" type="int" status="optional"></attr>
	</tag>
	<tag name="fstat" type="function" returnType="array" version="PHP 4 , PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="ftell" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="ftruncate" type="function" returnType="bool" version="PHP 4 , PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="size" type="int" status="required"></attr>
	</tag>
	<tag name="fwrite" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="string" type="string" status="required"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="glob" type="function" returnType="array" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="pattern" type="string" status="required"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="is_dir" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="is_executable" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="is_file" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="is_link" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="is_readable" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="is_uploaded_file" type="function" returnType="bool" version="PHP 3&gt;= 3.0.17, PHP 4 &gt;= 4.0.3, PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="is_writable" type="function" returnType="bool" version="PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="link" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="target" type="string" status="required"></attr>
		<attr name="link" type="string" status="required"></attr>
	</tag>
	<tag name="linkinfo" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="path" type="string" status="required"></attr>
	</tag>
	<tag name="lstat" type="function" returnType="array" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="mkdir" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="pathname" type="string" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="recursive" type="bool" status="optional"></attr>
		<attr name="context" type="resource" status="optional"></attr>
	</tag>
	<tag name="move_uploaded_file" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.3, PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="destination" type="string" status="required"></attr>
	</tag>
	<tag name="parse_ini_file" type="function" returnType="array" version="PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="process_sections" type="bool" status="optional"></attr>
	</tag>
	<tag name="pathinfo" type="function" returnType="array" version="PHP 4 &gt;= 4.0.3, PHP 5">
		<attr name="path" type="string" status="required"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="pclose" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="popen" type="function" returnType="resource" version="PHP 3, PHP 4 , PHP 5">
		<attr name="command" type="string" status="required"></attr>
		<attr name="mode" type="string" status="required"></attr>
	</tag>
	<tag name="readfile" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="use_include_path" type="bool" status="optional"></attr>
		<attr name="context" type="resource" status="optional"></attr>
	</tag>
	<tag name="readlink" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="path" type="string" status="required"></attr>
	</tag>
	<tag name="realpath" type="function" returnType="string" version="PHP 4 , PHP 5">
		<attr name="path" type="string" status="required"></attr>
	</tag>
	<tag name="rename" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="oldname" type="string" status="required"></attr>
		<attr name="newname" type="string" status="required"></attr>
		<attr name="context" type="resource" status="optional"></attr>
	</tag>
	<tag name="rewind" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="rmdir" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="dirname" type="string" status="required"></attr>
		<attr name="context" type="resource" status="optional"></attr>
	</tag>
	<tag name="stat" type="function" returnType="array" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="symlink" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="target" type="string" status="required"></attr>
		<attr name="link" type="string" status="required"></attr>
	</tag>
	<tag name="tempnam" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="dir" type="string" status="required"></attr>
		<attr name="prefix" type="string" status="required"></attr>
	</tag>
	<tag name="tmpfile" type="function" returnType="resource" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
	</tag>
	<tag name="touch" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="time" type="int" status="optional"></attr>
		<attr name="atime" type="int" status="optional"></attr>
	</tag>
	<tag name="umask" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="mask" type="int" status="optional"></attr>
	</tag>
	<tag name="unlink" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="context" type="resource" status="optional"></attr>
	</tag>
</tags>