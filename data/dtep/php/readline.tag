<!DOCTYPE tags>
<tags>	<tag name="readline_add_history" type="function" returnType="void">
		<attr name="line" type="string" status="optional"></attr>
	</tag>
	<tag name="readline_clear_history" type="function" returnType="bool">
	</tag>
	<tag name="readline_completion_function" type="function" returnType="bool">
		<attr name="line" type="string" status="optional"></attr>
	</tag>
	<tag name="readline_info" type="function" returnType="mixed">
		<attr name="varname" type="string" status="optional"></attr>
		<attr name="newvalue" type="string" status="optional"></attr>
	</tag>
	<tag name="readline_list_history" type="function" returnType="array">
	</tag>
	<tag name="readline_read_history" type="function" returnType="bool">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="readline_write_history" type="function" returnType="bool">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="readline" type="function" returnType="string">
		<attr name="prompt" type="string" status="optional"></attr>
	</tag>
</tags>
