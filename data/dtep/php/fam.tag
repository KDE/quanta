<!DOCTYPE tags>
<tags>
	<tag name="fam_cancel_monitor" type="function" returnType="bool" version="PHP 5">
		<attr name="fam" type="resource" status="required"></attr>
		<attr name="fam_monitor" type="resource" status="required"></attr>
	</tag>
	<tag name="fam_close" type="function" returnType="void" version="PHP 5">
		<attr name="fam" type="resource" status="required"></attr>
	</tag>
	<tag name="fam_monitor_collection" type="function" returnType="resource" version="PHP 5">
		<attr name="fam" type="resource" status="required"></attr>
		<attr name="dirname" type="string" status="required"></attr>
		<attr name="depth" type="int" status="required"></attr>
		<attr name="mask" type="string" status="required"></attr>
	</tag>
	<tag name="fam_monitor_directory" type="function" returnType="resource" version="PHP 5">
		<attr name="fam" type="resource" status="required"></attr>
		<attr name="dirname" type="string" status="required"></attr>
	</tag>
	<tag name="fam_monitor_file" type="function" returnType="resource" version="PHP 5">
		<attr name="fam" type="resource" status="required"></attr>
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="fam_next_event" type="function" returnType="array" version="PHP 5">
		<attr name="fam" type="resource" status="required"></attr>
	</tag>
	<tag name="fam_open" type="function" returnType="resource" version="PHP 5">
		<attr name="appname" type="string"></attr>
	</tag>
	<tag name="fam_pending" type="function" returnType="bool" version="PHP 5">
		<attr name="fam" type="resource" status="required"></attr>
	</tag>
	<tag name="fam_resume_monitor" type="function" returnType="bool" version="PHP 5">
		<attr name="fam" type="resource" status="required"></attr>
		<attr name="fam_monitor" type="resource" status="required"></attr>
	</tag>
	<tag name="fam_suspend_monitor" type="function" returnType="bool" version="PHP 5">
		<attr name="fam" type="resource" status="required"></attr>
		<attr name="fam_monitor" type="resource" status="required"></attr>
	</tag>
</tags>