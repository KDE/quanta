<!DOCTYPE tags>
<tags>
	<tag name="bcompiler_load_exe" type="function" returnType="bool">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="bcompiler_load" type="function" returnType="bool">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="bcompiler_parse_class" type="function" returnType="bool">
		<attr name="class" type="string"></attr>
		<attr name="callback" type="string"></attr>
	</tag>
	<tag name="bcompiler_read" type="function" returnType="bool">
		<attr name="filehandle" type="resource"></attr>
	</tag>
	<tag name="bcompiler_write_class" type="function" returnType="bool">
		<attr name="filehandle" type="resource"></attr>
		<attr name="className" type="string"></attr>
		<attr name="extends" type="string" status="optional"></attr>
	</tag>
	<tag name="bcompiler_write_constant" type="function" returnType="bool">
		<attr name="filehandle" type="resource"></attr>
		<attr name="constantName" type="string"></attr>
	</tag>
	<tag name="bcompiler_write_exe_footer" type="function" returnType="bool">
		<attr name="filehandle" type="resource"></attr>
		<attr name="startpos" type="int"></attr>
	</tag>
	<tag name="bcompiler_write_footer" type="function" returnType="bool">
		<attr name="filehandle" type="resource"></attr>
	</tag>
	<tag name="bcompiler_write_function" type="function" returnType="bool">
		<attr name="filehandle" type="resource"></attr>
		<attr name="functionName" type="string"></attr>
	</tag>
	<tag name="bcompiler_write_functions_from_file" type="function" returnType="bool">
		<attr name="filehandle" type="resource"></attr>
		<attr name="fileName" type="string"></attr>
	</tag>
	<tag name="bcompiler_write_header" type="function" returnType="bool">
		<attr name="filehandle" type="resource"></attr>
	</tag>
</tags>