<!DOCTYPE tags>
<tags>	<tag name="error_log" type="function" returnType="int">
		<attr name="message" type="string" status="optional"></attr>
		<attr name="message_type" type="int" status="optional"></attr>
		<attr name="destination" type="string" status="optional"></attr>
		<attr name="extra_headers" type="string" status="optional"></attr>
	</tag>
	<tag name="error_reporting" type="function" returnType="int">
		<attr name="level" type="int" status="optional"></attr>
	</tag>
	<tag name="restore_error_handler" type="function" returnType="void">
	</tag>
	<tag name="set_error_handler" type="function" returnType="string">
		<attr name="error_handler" type="string" status="optional"></attr>
	</tag>
	<tag name="trigger_error" type="function" returnType="void">
		<attr name="error_msg" type="string" status="optional"></attr>
		<attr name="error_type" type="int" status="optional"></attr>
	</tag>
	<tag name="user_error" type="function" returnType="void">
		<attr name="error_msg" type="string" status="optional"></attr>
		<attr name="error_type" type="int" status="optional"></attr>
	</tag>
</tags>
