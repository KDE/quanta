<!DOCTYPE tags>
<tags>
	<tag name="fam_cancel_monitor" type="function" returnType="bool">
		<attr name="fam" type="resource"></attr>
		<attr name="fam_monitor" type="resource"></attr>
	</tag>
	<tag name="fam_close" type="function" returnType="void">
		<attr name="fam" type="resource"></attr>
	</tag>
	<tag name="fam_monitor_collection" type="function" returnType="resource">
		<attr name="fam" type="resource"></attr>
		<attr name="dirname" type="string"></attr>
		<attr name="depth" type="int"></attr>
		<attr name="mask" type="string"></attr>
	</tag>
	<tag name="fam_monitor_directory" type="function" returnType="resource">
		<attr name="fam" type="resource"></attr>
		<attr name="dirname" type="string"></attr>
	</tag>
	<tag name="fam_monitor_file" type="function" returnType="resource">
		<attr name="fam" type="resource"></attr>
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="fam_next_event" type="function" returnType="array">
		<attr name="fam" type="resource"></attr>
	</tag>
	<tag name="fam_open" type="function" returnType="resource">
		<attr name="appname" type="string"></attr>
	</tag>
	<tag name="fam_pending" type="function" returnType="bool">
		<attr name="fam" type="resource"></attr>
	</tag>
	<tag name="fam_resume_monitor" type="function" returnType="bool">
		<attr name="fam" type="resource"></attr>
		<attr name="fam_monitor" type="resource"></attr>
	</tag>
	<tag name="fam_suspend_monitor" type="function" returnType="bool">
		<attr name="fam" type="resource"></attr>
		<attr name="fam_monitor" type="resource"></attr>
	</tag>
</tags>