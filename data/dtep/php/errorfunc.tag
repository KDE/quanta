<!DOCTYPE tags>
<tags>
	<tag name="debug_backtrace" type="function" returnType="array" version="PHP 4 &gt;= 4.3.0, PHP 5">
	</tag>
	<tag name="debug_print_backtrace" type="function" returnType="void" version="PHP 5">
	</tag>
	<tag name="error_log" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="message" type="string" status="required"></attr>
		<attr name="message_type" type="int" status="optional"></attr>
		<attr name="destination" type="string" status="optional"></attr>
		<attr name="extra_headers" type="string" status="optional"></attr>
	</tag>
	<tag name="error_reporting" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="level" type="int" status="optional"></attr>
	</tag>
	<tag name="restore_error_handler" type="function" returnType="void" version="PHP 4 &gt;= 4.0.1, PHP 5">
	</tag>
	<tag name="restore_exception_handler" type="function" returnType="void" version="PHP 5">
	</tag>
	<tag name="set_error_handler" type="function" returnType="mixed" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="error_handler" type="callback" status="required"></attr>
		<attr name="error_types" type="int" status="optional"></attr>
	</tag>
	<tag name="set_exception_handler" type="function" returnType="string" version="PHP 5">
		<attr name="exception_handler" type="callback" status="required"></attr>
	</tag>
	<tag name="trigger_error" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="error_msg" type="string" status="required"></attr>
		<attr name="error_type" type="int" status="optional"></attr>
	</tag>
</tags>