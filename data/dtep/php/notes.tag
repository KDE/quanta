<!DOCTYPE tags>
<tags>
	<tag name="notes_body" type="function" returnType="array" version="PHP 4 &gt;= 4.0.5">
		<attr name="server" type="string" status="required"></attr>
		<attr name="mailbox" type="string" status="required"></attr>
		<attr name="msg_number" type="int" status="required"></attr>
	</tag>
	<tag name="notes_copy_db" type="function" returnType="string" version="PHP 4 &gt;= 4.0.5">
		<attr name="from_database_name" type="string" status="required"></attr>
		<attr name="to_database_name" type="string" status="required"></attr>
	</tag>
	<tag name="notes_create_db" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5">
		<attr name="database_name" type="string" status="required"></attr>
	</tag>
	<tag name="notes_create_note" type="function" returnType="string" version="PHP 4 &gt;= 4.0.5">
		<attr name="database_name" type="string" status="required"></attr>
		<attr name="form_name" type="string" status="required"></attr>
	</tag>
	<tag name="notes_drop_db" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5">
		<attr name="database_name" type="string" status="required"></attr>
	</tag>
	<tag name="notes_find_note" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5">
		<attr name="database_name" type="string" status="required"></attr>
		<attr name="name" type="string" status="required"></attr>
		<attr name="type" type="string" status="optional"></attr>
	</tag>
	<tag name="notes_header_info" type="function" returnType="object" version="PHP 4 &gt;= 4.0.5">
		<attr name="server" type="string" status="required"></attr>
		<attr name="mailbox" type="string" status="required"></attr>
		<attr name="msg_number" type="int" status="required"></attr>
	</tag>
	<tag name="notes_list_msgs" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5">
		<attr name="db" type="string" status="required"></attr>
	</tag>
	<tag name="notes_mark_read" type="function" returnType="string" version="PHP 4 &gt;= 4.0.5">
		<attr name="database_name" type="string" status="required"></attr>
		<attr name="user_name" type="string" status="required"></attr>
		<attr name="note_id" type="string" status="required"></attr>
	</tag>
	<tag name="notes_mark_unread" type="function" returnType="string" version="PHP 4 &gt;= 4.0.5">
		<attr name="database_name" type="string" status="required"></attr>
		<attr name="user_name" type="string" status="required"></attr>
		<attr name="note_id" type="string" status="required"></attr>
	</tag>
	<tag name="notes_nav_create" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5">
		<attr name="database_name" type="string" status="required"></attr>
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="notes_search" type="function" returnType="string" version="PHP 4 &gt;= 4.0.5">
		<attr name="database_name" type="string" status="required"></attr>
		<attr name="keywords" type="string" status="required"></attr>
	</tag>
	<tag name="notes_unread" type="function" returnType="string" version="PHP 4 &gt;= 4.0.5">
		<attr name="database_name" type="string" status="required"></attr>
		<attr name="user_name" type="string" status="required"></attr>
	</tag>
	<tag name="notes_version" type="function" returnType="string" version="PHP 4 &gt;= 4.0.5">
		<attr name="database_name" type="string" status="required"></attr>
	</tag>
</tags>