<!DOCTYPE tags>
<tags>
	<tag name="readline_add_history" type="function" returnType="void" version="PHP 4 , PHP 5">
		<attr name="line" type="string" status="required"></attr>
	</tag>
	<tag name="readline_callback_handler_install" type="function" returnType="bool" version="">
		<attr name="prompt" type="string" status="required"></attr>
		<attr name="callback" type="callback" status="required"></attr>
	</tag>
	<tag name="readline_callback_handler_remove" type="function" returnType="bool" version="">
	</tag>
	<tag name="readline_callback_read_char" type="function" returnType="void" version="">
	</tag>
	<tag name="readline_clear_history" type="function" returnType="bool" version="PHP 4 , PHP 5">
	</tag>
	<tag name="readline_completion_function" type="function" returnType="bool" version="PHP 4 , PHP 5">
		<attr name="line" type="string" status="optional"></attr>
	</tag>
	<tag name="readline_info" type="function" returnType="mixed" version="PHP 4 , PHP 5">
		<attr name="varname" type="string" status="optional"></attr>
		<attr name="newvalue" type="string" status="optional"></attr>
	</tag>
	<tag name="readline_list_history" type="function" returnType="array" version="PHP 4 , PHP 5">
	</tag>
	<tag name="readline_on_new_line" type="function" returnType="void" version="">
	</tag>
	<tag name="readline_read_history" type="function" returnType="bool" version="PHP 4 , PHP 5">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="readline_redisplay" type="function" returnType="void" version="">
	</tag>
	<tag name="readline_write_history" type="function" returnType="bool" version="PHP 4 , PHP 5">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="readline" type="function" returnType="string" version="PHP 4 , PHP 5">
		<attr name="prompt" type="string" status="optional"></attr>
	</tag>
</tags>