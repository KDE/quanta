<!DOCTYPE tags>
<tags>
	<tag name="muscat_close" type="function" returnType="int" version="4.0.5 - 4.2.3 only">
		<attr name="muscat_handle" type="resource" status="required"></attr>
	</tag>
	<tag name="muscat_get" type="function" returnType="string" version="4.0.5 - 4.2.3 only">
		<attr name="muscat_handle" type="resource" status="required"></attr>
	</tag>
	<tag name="muscat_give" type="function" returnType="int" version="4.0.5 - 4.2.3 only">
		<attr name="muscat_handle" type="resource" status="required"></attr>
		<attr name="string" type="string" status="required"></attr>
	</tag>
	<tag name="muscat_setup_net" type="function" returnType="resource" version="4.0.5 - 4.2.3 only">
		<attr name="muscat_host" type="string" status="required"></attr>
	</tag>
	<tag name="muscat_setup" type="function" returnType="resource" version="4.0.5 - 4.2.3 only">
		<attr name="size" type="int" status="required"></attr>
		<attr name="muscat_dir" type="string" status="optional"></attr>
	</tag>
</tags>