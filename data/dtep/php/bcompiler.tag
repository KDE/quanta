<!DOCTYPE tags>
<tags>
	<tag name="bcompiler_load_exe" type="function" returnType="bool" version="">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="bcompiler_load" type="function" returnType="bool" version="">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="bcompiler_parse_class" type="function" returnType="bool" version="">
		<attr name="class" type="string" status="required"></attr>
		<attr name="callback" type="string" status="required"></attr>
	</tag>
	<tag name="bcompiler_read" type="function" returnType="bool" version="">
		<attr name="filehandle" type="resource" status="required"></attr>
	</tag>
	<tag name="bcompiler_write_class" type="function" returnType="bool" version="">
		<attr name="filehandle" type="resource" status="required"></attr>
		<attr name="className" type="string" status="required"></attr>
		<attr name="extends" type="string" status="optional"></attr>
	</tag>
	<tag name="bcompiler_write_constant" type="function" returnType="bool" version="">
		<attr name="filehandle" type="resource" status="required"></attr>
		<attr name="constantName" type="string" status="required"></attr>
	</tag>
	<tag name="bcompiler_write_exe_footer" type="function" returnType="bool" version="">
		<attr name="filehandle" type="resource" status="required"></attr>
		<attr name="startpos" type="int" status="required"></attr>
	</tag>
	<tag name="bcompiler_write_footer" type="function" returnType="bool" version="">
		<attr name="filehandle" type="resource" status="required"></attr>
	</tag>
	<tag name="bcompiler_write_function" type="function" returnType="bool" version="">
		<attr name="filehandle" type="resource" status="required"></attr>
		<attr name="functionName" type="string" status="required"></attr>
	</tag>
	<tag name="bcompiler_write_functions_from_file" type="function" returnType="bool" version="">
		<attr name="filehandle" type="resource" status="required"></attr>
		<attr name="fileName" type="string" status="required"></attr>
	</tag>
	<tag name="bcompiler_write_header" type="function" returnType="bool" version="">
		<attr name="filehandle" type="resource" status="required"></attr>
	</tag>
</tags>