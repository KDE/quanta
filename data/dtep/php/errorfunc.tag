<!DOCTYPE tags>
<tags>
	<tag name="debug_backtrace" type="function" returnType="array">
	</tag>
	<tag name="debug_print_backtrace" type="function" returnType="void">
	</tag>
	<tag name="error_log" type="function" returnType="int">
		<attr name="message" type="string"></attr>
		<attr name="message_type" type="int" status="optional"></attr>
		<attr name="destination" type="string" status="optional"></attr>
		<attr name="extra_headers" type="string" status="optional"></attr>
	</tag>
	<tag name="error_reporting" type="function" returnType="int">
		<attr name="level" type="int" status="optional"></attr>
	</tag>
	<tag name="restore_error_handler" type="function" returnType="void">
	</tag>
	<tag name="restore_exception_handler" type="function" returnType="void">
	</tag>
	<tag name="set_error_handler" type="function" returnType="mixed">
		<attr name="error_handler" type="callback"></attr>
		<attr name="error_types" type="int" status="optional"></attr>
	</tag>
	<tag name="set_exception_handler" type="function" returnType="string">
		<attr name="exception_handler" type="callback"></attr>
	</tag>
	<tag name="trigger_error" type="function" returnType="bool">
		<attr name="error_msg" type="string"></attr>
		<attr name="error_type" type="int" status="optional"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
</tags>