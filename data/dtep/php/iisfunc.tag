<!DOCTYPE tags>
<tags>
	<tag name="iis_add_server" type="function" returnType="int">
		<attr name="path" type="string"></attr>
		<attr name="comment" type="string"></attr>
		<attr name="server_ip" type="string"></attr>
		<attr name="port" type="int"></attr>
		<attr name="host_name" type="string"></attr>
		<attr name="rights" type="int"></attr>
		<attr name="start_server" type="int"></attr>
	</tag>
	<tag name="iis_get_dir_security" type="function" returnType="int">
		<attr name="server_instance" type="int"></attr>
		<attr name="virtual_path" type="string"></attr>
	</tag>
	<tag name="iis_get_script_map" type="function" returnType="int">
		<attr name="server_instance" type="int"></attr>
		<attr name="virtual_path" type="string"></attr>
		<attr name="script_extension" type="string"></attr>
	</tag>
	<tag name="iis_get_server_by_comment" type="function" returnType="int">
		<attr name="comment" type="string"></attr>
	</tag>
	<tag name="iis_get_server_by_path" type="function" returnType="int">
		<attr name="path" type="string"></attr>
	</tag>
	<tag name="iis_get_server_rights" type="function" returnType="int">
		<attr name="server_instance" type="int"></attr>
		<attr name="virtual_path" type="string"></attr>
	</tag>
	<tag name="iis_get_service_state" type="function" returnType="int">
		<attr name="service_id" type="string"></attr>
	</tag>
	<tag name="iis_remove_server" type="function" returnType="int">
		<attr name="server_instance" type="int"></attr>
	</tag>
	<tag name="iis_set_app_settings" type="function" returnType="int">
		<attr name="server_instance" type="int"></attr>
		<attr name="virtual_path" type="string"></attr>
		<attr name="application_scope" type="string"></attr>
	</tag>
	<tag name="iis_set_dir_security" type="function" returnType="int">
		<attr name="server_instance" type="int"></attr>
		<attr name="virtual_path" type="string"></attr>
		<attr name="directory_flags" type="int"></attr>
	</tag>
	<tag name="iis_set_script_map" type="function" returnType="int">
		<attr name="server_instance" type="int"></attr>
		<attr name="virtual_path" type="string"></attr>
		<attr name="script_extension" type="string"></attr>
		<attr name="engine_path" type="string"></attr>
		<attr name="allow_scripting" type="int"></attr>
	</tag>
	<tag name="iis_set_server_rights" type="function" returnType="int">
		<attr name="server_instance" type="int"></attr>
		<attr name="virtual_path" type="string"></attr>
		<attr name="directory_flags" type="int"></attr>
	</tag>
	<tag name="iis_start_server" type="function" returnType="int">
		<attr name="server_instance" type="int"></attr>
	</tag>
	<tag name="iis_start_service" type="function" returnType="int">
		<attr name="service_id" type="string"></attr>
	</tag>
	<tag name="iis_stop_server" type="function" returnType="int">
		<attr name="server_instance" type="int"></attr>
	</tag>
	<tag name="iis_stop_service" type="function" returnType="int">
		<attr name="service_id" type="string"></attr>
	</tag>
</tags>