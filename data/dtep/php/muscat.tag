<!DOCTYPE tags>
<tags>
	<tag name="muscat_close" type="function" returnType="int">
		<attr name="muscat_handle" type="resource"></attr>
	</tag>
	<tag name="muscat_get" type="function" returnType="string">
		<attr name="muscat_handle" type="resource"></attr>
	</tag>
	<tag name="muscat_give" type="function" returnType="int">
		<attr name="muscat_handle" type="resource"></attr>
		<attr name="string" type="string"></attr>
	</tag>
	<tag name="muscat_setup_net" type="function" returnType="resource">
		<attr name="muscat_host" type="string"></attr>
	</tag>
	<tag name="muscat_setup" type="function" returnType="resource">
		<attr name="size" type="int"></attr>
		<attr name="muscat_dir" type="string" status="optional"></attr>
	</tag>
</tags>