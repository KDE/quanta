<!DOCTYPE tags>
<tags>
	<tag name="ircg_channel_mode" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="channel" type="string" status="required"></attr>
		<attr name="mode_spec" type="string" status="required"></attr>
		<attr name="nick" type="string" status="required"></attr>
	</tag>
	<tag name="ircg_disconnect" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="reason" type="string" status="required"></attr>
	</tag>
	<tag name="ircg_eval_ecmascript_params" type="function" returnType="array" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="params" type="string" status="required"></attr>
	</tag>
	<tag name="ircg_fetch_error_msg" type="function" returnType="array" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
	</tag>
	<tag name="ircg_get_username" type="function" returnType="string" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
	</tag>
	<tag name="ircg_html_encode" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="html_string" type="string" status="required"></attr>
		<attr name="auto_links" type="bool" status="optional"></attr>
		<attr name="conv_br" type="bool" status="optional"></attr>
	</tag>
	<tag name="ircg_ignore_add" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="nick" type="string" status="required"></attr>
	</tag>
	<tag name="ircg_ignore_del" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="nick" type="string" status="required"></attr>
	</tag>
	<tag name="ircg_invite" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.3, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="channel" type="string" status="required"></attr>
		<attr name="nickname" type="string" status="required"></attr>
	</tag>
	<tag name="ircg_is_conn_alive" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
	</tag>
	<tag name="ircg_join" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="channel" type="string" status="required"></attr>
		<attr name="key" type="string" status="optional"></attr>
	</tag>
	<tag name="ircg_kick" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="channel" type="string" status="required"></attr>
		<attr name="nick" type="string" status="required"></attr>
		<attr name="reason" type="string" status="required"></attr>
	</tag>
	<tag name="ircg_list" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.3, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="channel" type="string" status="required"></attr>
	</tag>
	<tag name="ircg_lookup_format_messages" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="ircg_lusers" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.3, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
	</tag>
	<tag name="ircg_msg" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="recipient" type="string" status="required"></attr>
		<attr name="message" type="string" status="required"></attr>
		<attr name="suppress" type="boolean" status="optional"></attr>
	</tag>
	<tag name="ircg_names" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.3, PHP 5">
		<attr name="connection" type="int" status="required"></attr>
		<attr name="channel" type="string" status="required"></attr>
		<attr name="target" type="string" status="optional"></attr>
	</tag>
	<tag name="ircg_nick" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="nick" type="string" status="required"></attr>
	</tag>
	<tag name="ircg_nickname_escape" type="function" returnType="string" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="nick" type="string" status="required"></attr>
	</tag>
	<tag name="ircg_nickname_unescape" type="function" returnType="string" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="nick" type="string" status="required"></attr>
	</tag>
	<tag name="ircg_notice" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="recipient" type="string" status="required"></attr>
		<attr name="message" type="string" status="required"></attr>
	</tag>
	<tag name="ircg_oper" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.3, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="name" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
	</tag>
	<tag name="ircg_part" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="channel" type="string" status="required"></attr>
	</tag>
	<tag name="ircg_pconnect" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="username" type="string" status="required"></attr>
		<attr name="server_ip" type="string" status="optional"></attr>
		<attr name="server_port" type="int" status="optional"></attr>
		<attr name="msg_format" type="string" status="optional"></attr>
		<attr name="ctcp_messages" type="array" status="optional"></attr>
		<attr name="user_settings" type="array" status="optional"></attr>
		<attr name="bailout_on_trivial" type="bool" status="optional"></attr>
	</tag>
	<tag name="ircg_register_format_messages" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="name" type="string" status="required"></attr>
		<attr name="messages" type="array" status="required"></attr>
	</tag>
	<tag name="ircg_set_current" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
	</tag>
	<tag name="ircg_set_file" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="path" type="string" status="required"></attr>
	</tag>
	<tag name="ircg_set_on_die" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="host" type="string" status="required"></attr>
		<attr name="port" type="int" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
	</tag>
	<tag name="ircg_topic" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="channel" type="string" status="required"></attr>
		<attr name="new_topic" type="string" status="required"></attr>
	</tag>
	<tag name="ircg_who" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.3, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="mask" type="string" status="required"></attr>
		<attr name="ops_only" type="bool" status="optional"></attr>
	</tag>
	<tag name="ircg_whois" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="connection" type="resource" status="required"></attr>
		<attr name="nick" type="string" status="required"></attr>
	</tag>
</tags>