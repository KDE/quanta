<!DOCTYPE tags>
<tags>
	<tag name="icap_close" type="function" returnType="int" version="">
		<attr name="icap_stream" type="int" status="required"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="icap_create_calendar" type="function" returnType="string" version="PHP 4  &lt;= 4.2.3">
		<attr name="stream_id" type="int" status="required"></attr>
		<attr name="calendar" type="string" status="required"></attr>
	</tag>
	<tag name="icap_delete_calendar" type="function" returnType="string" version="PHP 4  &lt;= 4.2.3">
		<attr name="stream_id" type="int" status="required"></attr>
		<attr name="calendar" type="string" status="required"></attr>
	</tag>
	<tag name="icap_delete_event" type="function" returnType="string" version="PHP 4  &lt;= 4.2.3">
		<attr name="stream_id" type="int" status="required"></attr>
		<attr name="uid" type="int" status="required"></attr>
	</tag>
	<tag name="icap_fetch_event" type="function" returnType="int" version="PHP 4  &lt;= 4.2.3">
		<attr name="stream_id" type="int" status="required"></attr>
		<attr name="event_id" type="int" status="required"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="icap_list_alarms" type="function" returnType="int" version="PHP 4  &lt;= 4.2.3">
		<attr name="stream_id" type="int" status="required"></attr>
		<attr name="date" type="array" status="required"></attr>
		<attr name="time" type="array" status="required"></attr>
	</tag>
	<tag name="icap_list_events" type="function" returnType="array" version="PHP 4  &lt;= 4.2.3">
		<attr name="stream_id" type="int" status="required"></attr>
		<attr name="begin_date" type="int" status="required"></attr>
		<attr name="end_date" type="int" status="optional"></attr>
	</tag>
	<tag name="icap_open" type="function" returnType="resource" version="PHP 4  &lt;= 4.2.3">
		<attr name="calendar" type="string" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="options" type="string" status="required"></attr>
	</tag>
	<tag name="icap_rename_calendar" type="function" returnType="string" version="PHP 4  &lt;= 4.2.3">
		<attr name="stream_id" type="int" status="required"></attr>
		<attr name="old_name" type="string" status="required"></attr>
		<attr name="new_name" type="string" status="required"></attr>
	</tag>
	<tag name="icap_reopen" type="function" returnType="int" version="PHP 4  &lt;= 4.2.3">
		<attr name="stream_id" type="int" status="required"></attr>
		<attr name="calendar" type="string" status="required"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="icap_snooze" type="function" returnType="string" version="PHP 4  &lt;= 4.2.3">
		<attr name="stream_id" type="int" status="required"></attr>
		<attr name="uid" type="int" status="required"></attr>
	</tag>
	<tag name="icap_store_event" type="function" returnType="string" version="PHP 4  &lt;= 4.2.3">
		<attr name="stream_id" type="int" status="required"></attr>
		<attr name="event" type="object" status="required"></attr>
	</tag>
</tags>