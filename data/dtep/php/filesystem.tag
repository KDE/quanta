<!DOCTYPE tags>
<tags>
	<tag name="basename" type="function" returnType="string">
		<attr name="path" type="string"></attr>
		<attr name="suffix" type="string" status="optional"></attr>
	</tag>
	<tag name="chgrp" type="function" returnType="bool">
		<attr name="filename" type="string"></attr>
		<attr name="group" type="mixed"></attr>
	</tag>
	<tag name="chmod" type="function" returnType="bool">
		<attr name="filename" type="string"></attr>
		<attr name="mode" type="int"></attr>
	</tag>
	<tag name="chown" type="function" returnType="bool">
		<attr name="filename" type="string"></attr>
		<attr name="user" type="mixed"></attr>
	</tag>
	<tag name="clearstatcache" type="function" returnType="void">
	</tag>
	<tag name="copy" type="function" returnType="bool">
		<attr name="source" type="string"></attr>
		<attr name="dest" type="string"></attr>
	</tag>
	<tag name="delete" type="function" returnType="void">
		<attr name="file" type="string"></attr>
	</tag>
	<tag name="dirname" type="function" returnType="string">
		<attr name="path" type="string"></attr>
	</tag>
	<tag name="disk_free_space" type="function" returnType="float">
		<attr name="directory" type="string"></attr>
	</tag>
	<tag name="disk_total_space" type="function" returnType="float">
		<attr name="directory" type="string"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="fclose" type="function" returnType="bool">
		<attr name="handle" type="resource"></attr>
	</tag>
	<tag name="feof" type="function" returnType="bool">
		<attr name="handle" type="resource"></attr>
	</tag>
	<tag name="fflush" type="function" returnType="bool">
		<attr name="handle" type="resource"></attr>
	</tag>
	<tag name="fgetc" type="function" returnType="string">
		<attr name="handle" type="resource"></attr>
	</tag>
	<tag name="fgetcsv" type="function" returnType="array">
		<attr name="handle" type="resource"></attr>
		<attr name="length" type="int" status="optional"></attr>
		<attr name="delimiter" type="string" status="optional"></attr>
		<attr name="enclosure" type="string" status="optional"></attr>
	</tag>
	<tag name="fgets" type="function" returnType="string">
		<attr name="handle" type="resource"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="fgetss" type="function" returnType="string">
		<attr name="handle" type="resource"></attr>
		<attr name="length" type="int" status="optional"></attr>
		<attr name="allowable_tags" type="string" status="optional"></attr>
	</tag>
	<tag name="file_exists" type="function" returnType="bool">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="file_get_contents" type="function" returnType="string">
		<attr name="filename" type="string"></attr>
		<attr name="use_include_path" type="bool" status="optional"></attr>
		<attr name="context" type="resource" status="optional"></attr>
	</tag>
	<tag name="file_put_contents" type="function" returnType="int">
		<attr name="filename" type="string"></attr>
		<attr name="data" type="string"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="context" type="resource" status="optional"></attr>
	</tag>
	<tag name="file" type="function" returnType="array">
		<attr name="filename" type="string"></attr>
		<attr name="use_include_path" type="int" status="optional"></attr>
		<attr name="context" type="resource" status="optional"></attr>
	</tag>
	<tag name="fileatime" type="function" returnType="int">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="filectime" type="function" returnType="int">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="filegroup" type="function" returnType="int">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="fileinode" type="function" returnType="int">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="filemtime" type="function" returnType="int">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="fileowner" type="function" returnType="int">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="fileperms" type="function" returnType="int">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="filesize" type="function" returnType="int">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="filetype" type="function" returnType="string">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="flock" type="function" returnType="bool">
		<attr name="handle" type="resource"></attr>
		<attr name="operation" type="int"></attr>
		<attr name="&amp;wouldblock" type="int" status="optional"></attr>
	</tag>
	<tag name="fnmatch" type="function" returnType="bool">
		<attr name="pattern" type="string"></attr>
		<attr name="string" type="string"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="fopen" type="function" returnType="resource">
		<attr name="filename" type="string"></attr>
		<attr name="mode" type="string"></attr>
		<attr name="use_include_path" type="bool" status="optional"></attr>
		<attr name="zcontext" type="resource" status="optional"></attr>
	</tag>
	<tag name="fpassthru" type="function" returnType="int">
		<attr name="handle" type="resource"></attr>
	</tag>
	<tag name="fputcsv" type="function" returnType="int">
		<attr name="handle" type="resource"></attr>
		<attr name="fields" type="array"></attr>
		<attr name="delimiter" type="string" status="optional"></attr>
		<attr name="enclosure" type="string" status="optional"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="fread" type="function" returnType="string">
		<attr name="handle" type="resource"></attr>
		<attr name="length" type="int"></attr>
	</tag>
	<tag name="fscanf" type="function" returnType="mixed">
		<attr name="handle" type="resource"></attr>
		<attr name="format" type="string"></attr>
		<attr name="&amp;..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="fseek" type="function" returnType="int">
		<attr name="handle" type="resource"></attr>
		<attr name="offset" type="int"></attr>
		<attr name="whence" type="int" status="optional"></attr>
	</tag>
	<tag name="fstat" type="function" returnType="array">
		<attr name="handle" type="resource"></attr>
	</tag>
	<tag name="ftell" type="function" returnType="int">
		<attr name="handle" type="resource"></attr>
	</tag>
	<tag name="ftruncate" type="function" returnType="bool">
		<attr name="handle" type="resource"></attr>
		<attr name="size" type="int"></attr>
	</tag>
	<tag name="fwrite" type="function" returnType="int">
		<attr name="handle" type="resource"></attr>
		<attr name="string" type="string"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="glob" type="function" returnType="array">
		<attr name="pattern" type="string"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="is_dir" type="function" returnType="bool">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="is_executable" type="function" returnType="bool">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="is_file" type="function" returnType="bool">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="is_link" type="function" returnType="bool">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="is_readable" type="function" returnType="bool">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="is_uploaded_file" type="function" returnType="bool">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="is_writable" type="function" returnType="bool">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="link" type="function" returnType="bool">
		<attr name="target" type="string"></attr>
		<attr name="link" type="string"></attr>
	</tag>
	<tag name="linkinfo" type="function" returnType="int">
		<attr name="path" type="string"></attr>
	</tag>
	<tag name="lstat" type="function" returnType="array">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="mkdir" type="function" returnType="bool">
		<attr name="pathname" type="string"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="recursive" type="bool" status="optional"></attr>
		<attr name="context" type="resource" status="optional"></attr>
	</tag>
	<tag name="move_uploaded_file" type="function" returnType="bool">
		<attr name="filename" type="string"></attr>
		<attr name="destination" type="string"></attr>
	</tag>
	<tag name="parse_ini_file" type="function" returnType="array">
		<attr name="filename" type="string"></attr>
		<attr name="process_sections" type="bool" status="optional"></attr>
	</tag>
	<tag name="pathinfo" type="function" returnType="array">
		<attr name="path" type="string"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="pclose" type="function" returnType="int">
		<attr name="handle" type="resource"></attr>
	</tag>
	<tag name="popen" type="function" returnType="resource">
		<attr name="command" type="string"></attr>
		<attr name="mode" type="string"></attr>
	</tag>
	<tag name="readfile" type="function" returnType="int">
		<attr name="filename" type="string"></attr>
		<attr name="use_include_path" type="bool" status="optional"></attr>
		<attr name="context" type="resource" status="optional"></attr>
	</tag>
	<tag name="readlink" type="function" returnType="string">
		<attr name="path" type="string"></attr>
	</tag>
	<tag name="realpath" type="function" returnType="string">
		<attr name="path" type="string"></attr>
	</tag>
	<tag name="rename" type="function" returnType="bool">
		<attr name="oldname" type="string"></attr>
		<attr name="newname" type="string"></attr>
		<attr name="context" type="resource" status="optional"></attr>
	</tag>
	<tag name="rewind" type="function" returnType="bool">
		<attr name="handle" type="resource"></attr>
	</tag>
	<tag name="rmdir" type="function" returnType="bool">
		<attr name="dirname" type="string"></attr>
		<attr name="context" type="resource" status="optional"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="stat" type="function" returnType="array">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="symlink" type="function" returnType="bool">
		<attr name="target" type="string"></attr>
		<attr name="link" type="string"></attr>
	</tag>
	<tag name="tempnam" type="function" returnType="string">
		<attr name="dir" type="string"></attr>
		<attr name="prefix" type="string"></attr>
	</tag>
	<tag name="tmpfile" type="function" returnType="resource">
	</tag>
	<tag name="touch" type="function" returnType="bool">
		<attr name="filename" type="string"></attr>
		<attr name="time" type="int" status="optional"></attr>
		<attr name="atime" type="int" status="optional"></attr>
	</tag>
	<tag name="umask" type="function" returnType="int">
		<attr name="mask" type="int" status="optional"></attr>
	</tag>
	<tag name="unlink" type="function" returnType="bool">
		<attr name="filename" type="string"></attr>
		<attr name="context" type="resource" status="optional"></attr>
	</tag>
</tags>