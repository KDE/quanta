<!DOCTYPE tags>
<tags>	<tag name="ircg_channel_mode" type="function" returnType="boolean">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="channel" type="string" status="optional"></attr>
		<attr name="mode_spec" type="string" status="optional"></attr>
		<attr name="nick" type="string" status="optional"></attr>
	</tag>
	<tag name="ircg_disconnect" type="function" returnType="boolean">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="reason" type="string" status="optional"></attr>
	</tag>
	<tag name="ircg_fetch_error_msg" type="function" returnType="array">
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="ircg_get_username" type="function" returnType="string">
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="ircg_html_encode" type="function" returnType="boolean">
		<attr name="html_string" type="string" status="optional"></attr>
	</tag>
	<tag name="ircg_ignore_add" type="function" returnType="boolean">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="nick" type="string" status="optional"></attr>
	</tag>
	<tag name="ircg_ignore_del" type="function" returnType="boolean">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="nick" type="string" status="optional"></attr>
	</tag>
	<tag name="ircg_is_conn_alive" type="function" returnType="boolean">
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="ircg_join" type="function" returnType="boolean">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="channel" type="string" status="optional"></attr>
	</tag>
	<tag name="ircg_kick" type="function" returnType="boolean">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="channel" type="string" status="optional"></attr>
		<attr name="nick" type="string" status="optional"></attr>
		<attr name="reason" type="string" status="optional"></attr>
	</tag>
	<tag name="ircg_lookup_format_messages" type="function" returnType="boolean">
		<attr name="name" type="string" status="optional"></attr>
	</tag>
	<tag name="ircg_msg" type="function" returnType="boolean">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="recipient" type="string" status="optional"></attr>
		<attr name="message" type="string" status="optional"></attr>
		<attr name="suppress" type="boolean" status="optional"></attr>
	</tag>
	<tag name="ircg_nick" type="function" returnType="boolean">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="nick" type="string" status="optional"></attr>
	</tag>
	<tag name="ircg_nickname_escape" type="function" returnType="string">
		<attr name="nick" type="string" status="optional"></attr>
	</tag>
	<tag name="ircg_nickname_unescape" type="function" returnType="string">
		<attr name="nick" type="string" status="optional"></attr>
	</tag>
	<tag name="ircg_notice" type="function" returnType="boolean">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="nick" type="string" status="optional"></attr>
		<attr name="message" type="string" status="optional"></attr>
	</tag>
	<tag name="ircg_part" type="function" returnType="boolean">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="channel" type="string" status="optional"></attr>
	</tag>
	<tag name="ircg_pconnect" type="function" returnType="resource">
		<attr name="username" type="string" status="optional"></attr>
		<attr name="server_ip" type="string" status="optional"></attr>
		<attr name="server_port" type="int" status="optional"></attr>
		<attr name="msg_format" type="string" status="optional"></attr>
		<attr name="ctcp_messages" type="array" status="optional"></attr>
		<attr name="user_settings" type="array" status="optional"></attr>
	</tag>
	<tag name="ircg_register_format_messages" type="function" returnType="boolean">
		<attr name="name" type="string" status="optional"></attr>
		<attr name="messages" type="array" status="optional"></attr>
	</tag>
	<tag name="ircg_set_current" type="function" returnType="boolean">
		<attr name="connection" type="resource" status="optional"></attr>
	</tag>
	<tag name="ircg_set_file" type="function" returnType="bool">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="path" type="string" status="optional"></attr>
	</tag>
	<tag name="ircg_set_on_die" type="function" returnType="bool">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="host" type="string" status="optional"></attr>
		<attr name="port" type="int" status="optional"></attr>
		<attr name="data" type="string" status="optional"></attr>
	</tag>
	<tag name="ircg_topic" type="function" returnType="boolean">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="channel" type="string" status="optional"></attr>
		<attr name="new_topic" type="string" status="optional"></attr>
	</tag>
	<tag name="ircg_whois" type="function" returnType="boolean">
		<attr name="connection" type="resource" status="optional"></attr>
		<attr name="nick" type="string" status="optional"></attr>
	</tag>
</tags>
