<!DOCTYPE tags>
<tags>
	<tag name="readline_add_history" type="function" returnType="void" comment="PHP 4 , PHP 5">
		<attr name="line" type="string" status="required"></attr>
	</tag>
	<tag name="readline_callback_handler_install" type="function" returnType="bool" comment="">
		<attr name="prompt" type="string" status="required"></attr>
		<attr name="callback" type="callback" status="required"></attr>
	</tag>
	<tag name="readline_callback_handler_remove" type="function" returnType="bool" comment="">
	</tag>
	<tag name="readline_callback_read_char" type="function" returnType="void" comment="">
	</tag>
	<tag name="readline_clear_history" type="function" returnType="bool" comment="PHP 4 , PHP 5">
	</tag>
	<tag name="readline_completion_function" type="function" returnType="bool" comment="PHP 4 , PHP 5">
		<attr name="line" type="string" status="optional"></attr>
	</tag>
	<tag name="readline_info" type="function" returnType="mixed" comment="PHP 4 , PHP 5">
		<attr name="varname" type="string" status="optional"></attr>
		<attr name="newvalue" type="string" status="optional"></attr>
	</tag>
	<tag name="readline_list_history" type="function" returnType="array" comment="PHP 4 , PHP 5">
	</tag>
	<tag name="readline_on_new_line" type="function" returnType="void" comment="">
	</tag>
	<tag name="readline_read_history" type="function" returnType="bool" comment="PHP 4 , PHP 5">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="readline_redisplay" type="function" returnType="void" comment="">
	</tag>
	<tag name="readline_write_history" type="function" returnType="bool" comment="PHP 4 , PHP 5">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="readline" type="function" returnType="string" comment="PHP 4 , PHP 5">
		<attr name="prompt" type="string" status="optional"></attr>
	</tag>
</tags>