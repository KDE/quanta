<!DOCTYPE tags>
<tags>
	<tag name="posix_ctermid" type="function" returnType="string">
	</tag>
	<tag name="posix_get_last_error" type="function" returnType="int">
	</tag>
	<tag name="posix_getcwd" type="function" returnType="string">
	</tag>
	<tag name="posix_getegid" type="function" returnType="int">
	</tag>
	<tag name="posix_geteuid" type="function" returnType="int">
	</tag>
	<tag name="posix_getgid" type="function" returnType="int">
	</tag>
	<tag name="posix_getgrgid" type="function" returnType="array">
		<attr name="gid" type="int"></attr>
	</tag>
	<tag name="posix_getgrnam" type="function" returnType="array">
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="posix_getgroups" type="function" returnType="array">
	</tag>
	<tag name="posix_getlogin" type="function" returnType="string">
	</tag>
	<tag name="posix_getpgid" type="function" returnType="int">
		<attr name="pid" type="int"></attr>
	</tag>
	<tag name="posix_getpgrp" type="function" returnType="int">
	</tag>
	<tag name="posix_getpid" type="function" returnType="int">
	</tag>
	<tag name="posix_getppid" type="function" returnType="int">
	</tag>
	<tag name="posix_getpwnam" type="function" returnType="array">
		<attr name="username" type="string"></attr>
	</tag>
	<tag name="posix_getpwuid" type="function" returnType="array">
		<attr name="uid" type="int"></attr>
	</tag>
	<tag name="posix_getrlimit" type="function" returnType="array">
	</tag>
	<tag name="posix_getsid" type="function" returnType="int">
		<attr name="pid" type="int"></attr>
	</tag>
	<tag name="posix_getuid" type="function" returnType="int">
	</tag>
	<tag name="posix_isatty" type="function" returnType="bool">
		<attr name="fd" type="int"></attr>
	</tag>
	<tag name="posix_kill" type="function" returnType="bool">
		<attr name="pid" type="int"></attr>
		<attr name="sig" type="int"></attr>
	</tag>
	<tag name="posix_mkfifo" type="function" returnType="bool">
		<attr name="pathname" type="string"></attr>
		<attr name="mode" type="int"></attr>
	</tag>
	<tag name="posix_setegid" type="function" returnType="bool">
		<attr name="gid" type="int"></attr>
	</tag>
	<tag name="posix_seteuid" type="function" returnType="bool">
		<attr name="uid" type="int"></attr>
	</tag>
	<tag name="posix_setgid" type="function" returnType="bool">
		<attr name="gid" type="int"></attr>
	</tag>
	<tag name="posix_setpgid" type="function" returnType="int">
		<attr name="pid" type="int"></attr>
		<attr name="pgid" type="int"></attr>
	</tag>
	<tag name="posix_setsid" type="function" returnType="int">
	</tag>
	<tag name="posix_setuid" type="function" returnType="bool">
		<attr name="uid" type="int"></attr>
	</tag>
	<tag name="posix_strerror" type="function" returnType="string">
		<attr name="errno" type="int"></attr>
	</tag>
	<tag name="posix_times" type="function" returnType="array">
	</tag>
	<tag name="posix_ttyname" type="function" returnType="string">
		<attr name="fd" type="int"></attr>
	</tag>
	<tag name="posix_uname" type="function" returnType="array">
	</tag>
</tags>