<!DOCTYPE tags>
<tags>
	<tag name="parsekit_compile_file" type="function" returnType="array" version="">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="&amp;errors" type="array" status="optional"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="parsekit_compile_string" type="function" returnType="array" version="">
		<attr name="phpcode" type="string" status="required"></attr>
		<attr name="&amp;errors" type="array" status="optional"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="parsekit_func_arginfo" type="function" returnType="array" version="">
		<attr name="function" type="mixed" status="required"></attr>
	</tag>
</tags>