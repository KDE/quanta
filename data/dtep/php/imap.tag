<!DOCTYPE tags>
<tags>
	<tag name="imap_8bit" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="string" type="string" status="required"></attr>
	</tag>
	<tag name="imap_alerts" type="function" returnType="array" version="PHP 3&gt;= 3.0.12, PHP 4 , PHP 5">
	</tag>
	<tag name="imap_append" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="mbox" type="string" status="required"></attr>
		<attr name="message" type="string" status="required"></attr>
		<attr name="options" type="string" status="optional"></attr>
	</tag>
	<tag name="imap_base64" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="text" type="string" status="required"></attr>
	</tag>
	<tag name="imap_binary" type="function" returnType="string" version="PHP 3&gt;= 3.0.2, PHP 4 , PHP 5">
		<attr name="string" type="string" status="required"></attr>
	</tag>
	<tag name="imap_body" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="msg_number" type="int" status="required"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_bodystruct" type="function" returnType="object" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="stream_id" type="resource" status="required"></attr>
		<attr name="msg_no" type="int" status="required"></attr>
		<attr name="section" type="string" status="required"></attr>
	</tag>
	<tag name="imap_check" type="function" returnType="object" version="PHP 3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
	</tag>
	<tag name="imap_clearflag_full" type="function" returnType="bool" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="stream" type="resource" status="required"></attr>
		<attr name="sequence" type="string" status="required"></attr>
		<attr name="flag" type="string" status="required"></attr>
		<attr name="options" type="string" status="optional"></attr>
	</tag>
	<tag name="imap_close" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="flag" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_createmailbox" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="mbox" type="string" status="required"></attr>
	</tag>
	<tag name="imap_delete" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="int" status="required"></attr>
		<attr name="msg_number" type="int" status="required"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_deletemailbox" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="mbox" type="string" status="required"></attr>
	</tag>
	<tag name="imap_errors" type="function" returnType="array" version="PHP 3&gt;= 3.0.12, PHP 4 , PHP 5">
	</tag>
	<tag name="imap_expunge" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
	</tag>
	<tag name="imap_fetch_overview" type="function" returnType="array" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="sequence" type="string" status="required"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_fetchbody" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="msg_number" type="int" status="required"></attr>
		<attr name="part_number" type="string" status="required"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_fetchheader" type="function" returnType="string" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="msgno" type="int" status="required"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_fetchstructure" type="function" returnType="object" version="PHP 3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="msg_number" type="int" status="required"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_get_quota" type="function" returnType="array" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="quota_root" type="string" status="required"></attr>
	</tag>
	<tag name="imap_get_quotaroot" type="function" returnType="array" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="quota_root" type="string" status="required"></attr>
	</tag>
	<tag name="imap_getacl" type="function" returnType="array" version="PHP 5">
		<attr name="stream_id" type="resource" status="required"></attr>
		<attr name="mailbox" type="string" status="required"></attr>
	</tag>
	<tag name="imap_getmailboxes" type="function" returnType="array" version="PHP 3&gt;= 3.0.12, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="ref" type="string" status="required"></attr>
		<attr name="pattern" type="string" status="required"></attr>
	</tag>
	<tag name="imap_getsubscribed" type="function" returnType="array" version="PHP 3&gt;= 3.0.12, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="ref" type="string" status="required"></attr>
		<attr name="pattern" type="string" status="required"></attr>
	</tag>
	<tag name="imap_headerinfo" type="function" returnType="object" version="PHP 3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="msg_number" type="int" status="required"></attr>
		<attr name="fromlength" type="int" status="optional"></attr>
		<attr name="subjectlength" type="int" status="optional"></attr>
		<attr name="defaulthost" type="string" status="optional"></attr>
	</tag>
	<tag name="imap_headers" type="function" returnType="array" version="PHP 3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
	</tag>
	<tag name="imap_last_error" type="function" returnType="string" version="PHP 3&gt;= 3.0.12, PHP 4 , PHP 5">
	</tag>
	<tag name="imap_list" type="function" returnType="array" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="ref" type="string" status="required"></attr>
		<attr name="pattern" type="string" status="required"></attr>
	</tag>
	<tag name="imap_listscan" type="function" returnType="array" version="">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="ref" type="string" status="required"></attr>
		<attr name="pattern" type="string" status="required"></attr>
		<attr name="content" type="string" status="required"></attr>
	</tag>
	<tag name="imap_lsub" type="function" returnType="array" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="ref" type="string" status="required"></attr>
		<attr name="pattern" type="string" status="required"></attr>
	</tag>
	<tag name="imap_mail_compose" type="function" returnType="string" version="PHP 3&gt;= 3.0.5, PHP 4 , PHP 5">
		<attr name="envelope" type="array" status="required"></attr>
		<attr name="body" type="array" status="required"></attr>
	</tag>
	<tag name="imap_mail_copy" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="msglist" type="string" status="required"></attr>
		<attr name="mbox" type="string" status="required"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_mail_move" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="msglist" type="string" status="required"></attr>
		<attr name="mbox" type="string" status="required"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_mail" type="function" returnType="bool" version="PHP 3&gt;= 3.0.14, PHP 4 , PHP 5">
		<attr name="to" type="string" status="required"></attr>
		<attr name="subject" type="string" status="required"></attr>
		<attr name="message" type="string" status="required"></attr>
		<attr name="additional_headers" type="string" status="optional"></attr>
		<attr name="cc" type="string" status="optional"></attr>
		<attr name="bcc" type="string" status="optional"></attr>
		<attr name="rpath" type="string" status="optional"></attr>
	</tag>
	<tag name="imap_mailboxmsginfo" type="function" returnType="object" version="PHP 3&gt;= 3.0.2, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
	</tag>
	<tag name="imap_mime_header_decode" type="function" returnType="array" version="PHP 3&gt;= 3.0.17, PHP 4 , PHP 5">
		<attr name="text" type="string" status="required"></attr>
	</tag>
	<tag name="imap_msgno" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="uid" type="int" status="required"></attr>
	</tag>
	<tag name="imap_num_msg" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
	</tag>
	<tag name="imap_num_recent" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
	</tag>
	<tag name="imap_open" type="function" returnType="resource" version="PHP 3, PHP 4 , PHP 5">
		<attr name="mailbox" type="string" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_ping" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
	</tag>
	<tag name="imap_qprint" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="string" type="string" status="required"></attr>
	</tag>
	<tag name="imap_renamemailbox" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="old_mbox" type="string" status="required"></attr>
		<attr name="new_mbox" type="string" status="required"></attr>
	</tag>
	<tag name="imap_reopen" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="mailbox" type="string" status="required"></attr>
		<attr name="options" type="string" status="optional"></attr>
	</tag>
	<tag name="imap_rfc822_parse_adrlist" type="function" returnType="array" version="PHP 3&gt;= 3.0.2, PHP 4 , PHP 5">
		<attr name="address" type="string" status="required"></attr>
		<attr name="default_host" type="string" status="required"></attr>
	</tag>
	<tag name="imap_rfc822_parse_headers" type="function" returnType="object" version="PHP 4 , PHP 5">
		<attr name="headers" type="string" status="required"></attr>
		<attr name="defaulthost" type="string" status="optional"></attr>
	</tag>
	<tag name="imap_rfc822_write_address" type="function" returnType="string" version="PHP 3&gt;= 3.0.2, PHP 4 , PHP 5">
		<attr name="mailbox" type="string" status="required"></attr>
		<attr name="host" type="string" status="required"></attr>
		<attr name="personal" type="string" status="required"></attr>
	</tag>
	<tag name="imap_search" type="function" returnType="array" version="PHP 3&gt;= 3.0.12, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="criteria" type="string" status="required"></attr>
		<attr name="options" type="int" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="imap_set_quota" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="quota_root" type="string" status="required"></attr>
		<attr name="quota_limit" type="int" status="required"></attr>
	</tag>
	<tag name="imap_setacl" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="stream_id" type="resource" status="required"></attr>
		<attr name="mailbox" type="string" status="required"></attr>
		<attr name="id" type="string" status="required"></attr>
		<attr name="rights" type="string" status="required"></attr>
	</tag>
	<tag name="imap_setflag_full" type="function" returnType="bool" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="stream" type="resource" status="required"></attr>
		<attr name="sequence" type="string" status="required"></attr>
		<attr name="flag" type="string" status="required"></attr>
		<attr name="options" type="string" status="optional"></attr>
	</tag>
	<tag name="imap_sort" type="function" returnType="array" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="stream" type="resource" status="required"></attr>
		<attr name="criteria" type="int" status="required"></attr>
		<attr name="reverse" type="int" status="required"></attr>
		<attr name="options" type="int" status="optional"></attr>
		<attr name="search_criteria" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="imap_status" type="function" returnType="object" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="mailbox" type="string" status="required"></attr>
		<attr name="options" type="int" status="required"></attr>
	</tag>
	<tag name="imap_subscribe" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="mbox" type="string" status="required"></attr>
	</tag>
	<tag name="imap_thread" type="function" returnType="array" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="stream_id" type="resource" status="required"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_timeout" type="function" returnType="mixed" version="PHP 4 &gt;= 4.3.3, PHP 5">
		<attr name="timeout_type" type="int" status="required"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_uid" type="function" returnType="int" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="msgno" type="int" status="required"></attr>
	</tag>
	<tag name="imap_undelete" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="resource" status="required"></attr>
		<attr name="msg_number" type="int" status="required"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_unsubscribe" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="imap_stream" type="string" status="required"></attr>
		<attr name="mbox" type="string" status="required"></attr>
	</tag>
	<tag name="imap_utf7_decode" type="function" returnType="string" version="PHP 3&gt;= 3.0.15, PHP 4 , PHP 5">
		<attr name="text" type="string" status="required"></attr>
	</tag>
	<tag name="imap_utf7_encode" type="function" returnType="string" version="PHP 3&gt;= 3.0.15, PHP 4 , PHP 5">
		<attr name="data" type="string" status="required"></attr>
	</tag>
	<tag name="imap_utf8" type="function" returnType="string" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="mime_encoded_text" type="string" status="required"></attr>
	</tag>
</tags>