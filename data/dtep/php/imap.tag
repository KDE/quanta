<!DOCTYPE tags>
<tags>
	<tag name="imap_8bit" type="function" returnType="string">
		<attr name="string" type="string"></attr>
	</tag>
	<tag name="imap_alerts" type="function" returnType="array">
	</tag>
	<tag name="imap_append" type="function" returnType="bool">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="mbox" type="string"></attr>
		<attr name="message" type="string"></attr>
		<attr name="options" type="string" status="optional"></attr>
	</tag>
	<tag name="imap_base64" type="function" returnType="string">
		<attr name="text" type="string"></attr>
	</tag>
	<tag name="imap_binary" type="function" returnType="string">
		<attr name="string" type="string"></attr>
	</tag>
	<tag name="imap_body" type="function" returnType="string">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="msg_number" type="int"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_bodystruct" type="function" returnType="object">
		<attr name="stream_id" type="resource"></attr>
		<attr name="msg_no" type="int"></attr>
		<attr name="section" type="string"></attr>
	</tag>
	<tag name="imap_check" type="function" returnType="object">
		<attr name="imap_stream" type="resource"></attr>
	</tag>
	<tag name="imap_clearflag_full" type="function" returnType="bool">
		<attr name="stream" type="resource"></attr>
		<attr name="sequence" type="string"></attr>
		<attr name="flag" type="string"></attr>
		<attr name="options" type="string" status="optional"></attr>
	</tag>
	<tag name="imap_close" type="function" returnType="bool">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="flag" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_createmailbox" type="function" returnType="bool">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="mbox" type="string"></attr>
	</tag>
	<tag name="imap_delete" type="function" returnType="bool">
		<attr name="imap_stream" type="int"></attr>
		<attr name="msg_number" type="int"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_deletemailbox" type="function" returnType="bool">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="mbox" type="string"></attr>
	</tag>
	<tag name="imap_errors" type="function" returnType="array">
	</tag>
	<tag name="imap_expunge" type="function" returnType="bool">
		<attr name="imap_stream" type="resource"></attr>
	</tag>
	<tag name="imap_fetch_overview" type="function" returnType="array">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="sequence" type="string"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_fetchbody" type="function" returnType="string">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="msg_number" type="int"></attr>
		<attr name="part_number" type="string"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_fetchheader" type="function" returnType="string">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="msgno" type="int"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_fetchstructure" type="function" returnType="object">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="msg_number" type="int"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_get_quota" type="function" returnType="array">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="quota_root" type="string"></attr>
	</tag>
	<tag name="imap_get_quotaroot" type="function" returnType="array">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="quota_root" type="string"></attr>
	</tag>
	<tag name="imap_getacl" type="function" returnType="array">
		<attr name="stream_id" type="resource"></attr>
		<attr name="mailbox" type="string"></attr>
	</tag>
	<tag name="imap_getmailboxes" type="function" returnType="array">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="ref" type="string"></attr>
		<attr name="pattern" type="string"></attr>
	</tag>
	<tag name="imap_getsubscribed" type="function" returnType="array">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="ref" type="string"></attr>
		<attr name="pattern" type="string"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="imap_headerinfo" type="function" returnType="object">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="msg_number" type="int"></attr>
		<attr name="fromlength" type="int" status="optional"></attr>
		<attr name="subjectlength" type="int" status="optional"></attr>
		<attr name="defaulthost" type="string" status="optional"></attr>
	</tag>
	<tag name="imap_headers" type="function" returnType="array">
		<attr name="imap_stream" type="resource"></attr>
	</tag>
	<tag name="imap_last_error" type="function" returnType="string">
	</tag>
	<tag name="imap_list" type="function" returnType="array">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="ref" type="string"></attr>
		<attr name="pattern" type="string"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="imap_listscan" type="function" returnType="array">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="ref" type="string"></attr>
		<attr name="pattern" type="string"></attr>
		<attr name="content" type="string"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="imap_lsub" type="function" returnType="array">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="ref" type="string"></attr>
		<attr name="pattern" type="string"></attr>
	</tag>
	<tag name="imap_mail_compose" type="function" returnType="string">
		<attr name="envelope" type="array"></attr>
		<attr name="body" type="array"></attr>
	</tag>
	<tag name="imap_mail_copy" type="function" returnType="bool">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="msglist" type="string"></attr>
		<attr name="mbox" type="string"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_mail_move" type="function" returnType="bool">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="msglist" type="string"></attr>
		<attr name="mbox" type="string"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_mail" type="function" returnType="bool">
		<attr name="to" type="string"></attr>
		<attr name="subject" type="string"></attr>
		<attr name="message" type="string"></attr>
		<attr name="additional_headers" type="string" status="optional"></attr>
		<attr name="cc" type="string" status="optional"></attr>
		<attr name="bcc" type="string" status="optional"></attr>
		<attr name="rpath" type="string" status="optional"></attr>
	</tag>
	<tag name="imap_mailboxmsginfo" type="function" returnType="object">
		<attr name="imap_stream" type="resource"></attr>
	</tag>
	<tag name="imap_mime_header_decode" type="function" returnType="array">
		<attr name="text" type="string"></attr>
	</tag>
	<tag name="imap_msgno" type="function" returnType="int">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="uid" type="int"></attr>
	</tag>
	<tag name="imap_num_msg" type="function" returnType="int">
		<attr name="imap_stream" type="resource"></attr>
	</tag>
	<tag name="imap_num_recent" type="function" returnType="int">
		<attr name="imap_stream" type="resource"></attr>
	</tag>
	<tag name="imap_open" type="function" returnType="resource">
		<attr name="mailbox" type="string"></attr>
		<attr name="username" type="string"></attr>
		<attr name="password" type="string"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_ping" type="function" returnType="bool">
		<attr name="imap_stream" type="resource"></attr>
	</tag>
	<tag name="imap_qprint" type="function" returnType="string">
		<attr name="string" type="string"></attr>
	</tag>
	<tag name="imap_renamemailbox" type="function" returnType="bool">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="old_mbox" type="string"></attr>
		<attr name="new_mbox" type="string"></attr>
	</tag>
	<tag name="imap_reopen" type="function" returnType="bool">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="mailbox" type="string"></attr>
		<attr name="options" type="string" status="optional"></attr>
	</tag>
	<tag name="imap_rfc822_parse_adrlist" type="function" returnType="array">
		<attr name="address" type="string"></attr>
		<attr name="default_host" type="string"></attr>
	</tag>
	<tag name="imap_rfc822_parse_headers" type="function" returnType="object">
		<attr name="headers" type="string"></attr>
		<attr name="defaulthost" type="string" status="optional"></attr>
	</tag>
	<tag name="imap_rfc822_write_address" type="function" returnType="string">
		<attr name="mailbox" type="string"></attr>
		<attr name="host" type="string"></attr>
		<attr name="personal" type="string"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="imap_search" type="function" returnType="array">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="criteria" type="string"></attr>
		<attr name="options" type="int" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="imap_set_quota" type="function" returnType="bool">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="quota_root" type="string"></attr>
		<attr name="quota_limit" type="int"></attr>
	</tag>
	<tag name="imap_setacl" type="function" returnType="bool">
		<attr name="stream_id" type="resource"></attr>
		<attr name="mailbox" type="string"></attr>
		<attr name="id" type="string"></attr>
		<attr name="rights" type="string"></attr>
	</tag>
	<tag name="imap_setflag_full" type="function" returnType="bool">
		<attr name="stream" type="resource"></attr>
		<attr name="sequence" type="string"></attr>
		<attr name="flag" type="string"></attr>
		<attr name="options" type="string" status="optional"></attr>
	</tag>
	<tag name="imap_sort" type="function" returnType="array">
		<attr name="stream" type="resource"></attr>
		<attr name="criteria" type="int"></attr>
		<attr name="reverse" type="int"></attr>
		<attr name="options" type="int" status="optional"></attr>
		<attr name="search_criteria" type="string" status="optional"></attr>
		<attr name="charset" type="string" status="optional"></attr>
	</tag>
	<tag name="imap_status" type="function" returnType="object">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="mailbox" type="string"></attr>
		<attr name="options" type="int"></attr>
	</tag>
	<tag name="imap_subscribe" type="function" returnType="bool">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="mbox" type="string"></attr>
	</tag>
	<tag name="imap_thread" type="function" returnType="array">
		<attr name="stream_id" type="resource"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_timeout" type="function" returnType="mixed">
		<attr name="timeout_type" type="int"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_uid" type="function" returnType="int">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="msgno" type="int"></attr>
	</tag>
	<tag name="imap_undelete" type="function" returnType="bool">
		<attr name="imap_stream" type="resource"></attr>
		<attr name="msg_number" type="int"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="imap_unsubscribe" type="function" returnType="bool">
		<attr name="imap_stream" type="string"></attr>
		<attr name="mbox" type="string"></attr>
	</tag>
	<tag name="imap_utf7_decode" type="function" returnType="string">
		<attr name="text" type="string"></attr>
	</tag>
	<tag name="imap_utf7_encode" type="function" returnType="string">
		<attr name="data" type="string"></attr>
	</tag>
	<tag name="imap_utf8" type="function" returnType="string">
		<attr name="mime_encoded_text" type="string"></attr>
	</tag>
</tags>