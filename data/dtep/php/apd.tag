<!DOCTYPE tags>
<tags>
	<tag name="apd_breakpoint" type="function" returnType="void" version="">
		<attr name="debug_level" type="int" status="required"></attr>
	</tag>
	<tag name="apd_callstack" type="function" returnType="array" version="">
	</tag>
	<tag name="apd_clunk" type="function" returnType="void" version="">
		<attr name="warning" type="string" status="required"></attr>
		<attr name="delimiter" type="string" status="optional"></attr>
	</tag>
	<tag name="apd_continue" type="function" returnType="void" version="">
		<attr name="debug_level" type="int" status="required"></attr>
	</tag>
	<tag name="apd_croak" type="function" returnType="void" version="">
		<attr name="warning" type="string" status="required"></attr>
		<attr name="delimiter" type="string" status="optional"></attr>
	</tag>
	<tag name="apd_dump_function_table" type="function" returnType="void" version="">
	</tag>
	<tag name="apd_dump_persistent_resources" type="function" returnType="array" version="">
	</tag>
	<tag name="apd_dump_regular_resources" type="function" returnType="array" version="">
	</tag>
	<tag name="apd_echo" type="function" returnType="void" version="">
		<attr name="output" type="string" status="required"></attr>
	</tag>
	<tag name="apd_get_active_symbols" type="function" returnType="array" version="">
		<attr name="&lt;/methodparam&gt;" type="" status="required"></attr>
	</tag>
	<tag name="apd_set_pprof_trace" type="function" returnType="void" version="">
		<attr name="dump_directory" type="string" status="optional"></attr>
	</tag>
	<tag name="apd_set_session_trace" type="function" returnType="void" version="">
		<attr name="debug_level" type="int" status="required"></attr>
		<attr name="dump_directory" type="string" status="optional"></attr>
	</tag>
	<tag name="apd_set_session" type="function" returnType="void" version="">
		<attr name="debug_level" type="int" status="required"></attr>
	</tag>
	<tag name="apd_set_socket_session_trace" type="function" returnType="bool" version="">
		<attr name="(ip_address or unix socket file)" type="string" status="required"></attr>
		<attr name="socket_type" type="int" status="required"></attr>
		<attr name="port" type="int" status="required"></attr>
		<attr name="debug_level" type="int" status="required"></attr>
	</tag>
	<tag name="override_function" type="function" returnType="bool" version="">
		<attr name="function_name" type="string" status="required"></attr>
		<attr name="function_args" type="string" status="required"></attr>
		<attr name="function_code" type="string" status="required"></attr>
	</tag>
	<tag name="rename_function" type="function" returnType="bool" version="">
		<attr name="original_name" type="string" status="required"></attr>
		<attr name="new_name" type="string" status="required"></attr>
	</tag>
</tags>