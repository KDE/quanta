<!DOCTYPE tags>
<tags>
	<tag name="apd_breakpoint" type="function" returnType="void">
		<attr name="debug_level" type="int"></attr>
	</tag>
	<tag name="apd_callstack" type="function" returnType="array">
	</tag>
	<tag name="apd_clunk" type="function" returnType="void">
		<attr name="warning" type="string"></attr>
		<attr name="delimiter" type="string" status="optional"></attr>
	</tag>
	<tag name="apd_continue" type="function" returnType="void">
		<attr name="debug_level" type="int"></attr>
	</tag>
	<tag name="apd_croak" type="function" returnType="void">
		<attr name="warning" type="string"></attr>
		<attr name="delimiter" type="string" status="optional"></attr>
	</tag>
	<tag name="apd_dump_function_table" type="function" returnType="void">
	</tag>
	<tag name="apd_dump_persistent_resources" type="function" returnType="array">
	</tag>
	<tag name="apd_dump_regular_resources" type="function" returnType="array">
	</tag>
	<tag name="apd_echo" type="function" returnType="void">
		<attr name="output" type="string"></attr>
	</tag>
	<tag name="apd_get_active_symbols" type="function" returnType="array">
		<attr name="" type=""></attr>
	</tag>
	<tag name="apd_set_pprof_trace" type="function" returnType="void">
		<attr name="dump_directory" type="string" status="optional"></attr>
	</tag>
	<tag name="apd_set_session_trace" type="function" returnType="void">
		<attr name="debug_level" type="int"></attr>
		<attr name="dump_directory" type="string" status="optional"></attr>
	</tag>
	<tag name="apd_set_session" type="function" returnType="void">
		<attr name="debug_level" type="int"></attr>
	</tag>
	<tag name="apd_set_socket_session_trace" type="function" returnType="bool">
		<attr name="(ip_address or unix socket file)" type="string"></attr>
		<attr name="socket_type" type="int"></attr>
		<attr name="port" type="int"></attr>
		<attr name="debug_level" type="int"></attr>
	</tag>
	<tag name="override_function" type="function" returnType="bool">
		<attr name="function_name" type="string"></attr>
		<attr name="function_args" type="string"></attr>
		<attr name="function_code" type="string"></attr>
	</tag>
	<tag name="rename_function" type="function" returnType="bool">
		<attr name="original_name" type="string"></attr>
		<attr name="new_name" type="string"></attr>
	</tag>
</tags>