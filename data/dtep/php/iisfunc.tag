<!DOCTYPE tags>
<tags>
	<tag name="iis_add_server" type="function" returnType="int" version="">
		<attr name="path" type="string" status="required"></attr>
		<attr name="comment" type="string" status="required"></attr>
		<attr name="server_ip" type="string" status="required"></attr>
		<attr name="port" type="int" status="required"></attr>
		<attr name="host_name" type="string" status="required"></attr>
		<attr name="rights" type="int" status="required"></attr>
		<attr name="start_server" type="int" status="required"></attr>
	</tag>
	<tag name="iis_get_dir_security" type="function" returnType="int" version="">
		<attr name="server_instance" type="int" status="required"></attr>
		<attr name="virtual_path" type="string" status="required"></attr>
	</tag>
	<tag name="iis_get_script_map" type="function" returnType="int" version="">
		<attr name="server_instance" type="int" status="required"></attr>
		<attr name="virtual_path" type="string" status="required"></attr>
		<attr name="script_extension" type="string" status="required"></attr>
	</tag>
	<tag name="iis_get_server_by_comment" type="function" returnType="int" version="">
		<attr name="comment" type="string" status="required"></attr>
	</tag>
	<tag name="iis_get_server_by_path" type="function" returnType="int" version="">
		<attr name="path" type="string" status="required"></attr>
	</tag>
	<tag name="iis_get_server_rights" type="function" returnType="int" version="">
		<attr name="server_instance" type="int" status="required"></attr>
		<attr name="virtual_path" type="string" status="required"></attr>
	</tag>
	<tag name="iis_get_service_state" type="function" returnType="int" version="">
		<attr name="service_id" type="string" status="required"></attr>
	</tag>
	<tag name="iis_remove_server" type="function" returnType="int" version="">
		<attr name="server_instance" type="int" status="required"></attr>
	</tag>
	<tag name="iis_set_app_settings" type="function" returnType="int" version="">
		<attr name="server_instance" type="int" status="required"></attr>
		<attr name="virtual_path" type="string" status="required"></attr>
		<attr name="application_scope" type="string" status="required"></attr>
	</tag>
	<tag name="iis_set_dir_security" type="function" returnType="int" version="">
		<attr name="server_instance" type="int" status="required"></attr>
		<attr name="virtual_path" type="string" status="required"></attr>
		<attr name="directory_flags" type="int" status="required"></attr>
	</tag>
	<tag name="iis_set_script_map" type="function" returnType="int" version="">
		<attr name="server_instance" type="int" status="required"></attr>
		<attr name="virtual_path" type="string" status="required"></attr>
		<attr name="script_extension" type="string" status="required"></attr>
		<attr name="engine_path" type="string" status="required"></attr>
		<attr name="allow_scripting" type="int" status="required"></attr>
	</tag>
	<tag name="iis_set_server_rights" type="function" returnType="int" version="">
		<attr name="server_instance" type="int" status="required"></attr>
		<attr name="virtual_path" type="string" status="required"></attr>
		<attr name="directory_flags" type="int" status="required"></attr>
	</tag>
	<tag name="iis_start_server" type="function" returnType="int" version="">
		<attr name="server_instance" type="int" status="required"></attr>
	</tag>
	<tag name="iis_start_service" type="function" returnType="int" version="">
		<attr name="service_id" type="string" status="required"></attr>
	</tag>
	<tag name="iis_stop_server" type="function" returnType="int" version="">
		<attr name="server_instance" type="int" status="required"></attr>
	</tag>
	<tag name="iis_stop_service" type="function" returnType="int" version="">
		<attr name="service_id" type="string" status="required"></attr>
	</tag>
</tags>