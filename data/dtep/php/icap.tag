<!DOCTYPE tags>
<tags>
	<tag name="icap_close" type="function" returnType="int">
		<attr name="icap_stream" type="int"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="icap_create_calendar" type="function" returnType="string">
		<attr name="stream_id" type="int"></attr>
		<attr name="calendar" type="string"></attr>
	</tag>
	<tag name="icap_delete_calendar" type="function" returnType="string">
		<attr name="stream_id" type="int"></attr>
		<attr name="calendar" type="string"></attr>
	</tag>
	<tag name="icap_delete_event" type="function" returnType="string">
		<attr name="stream_id" type="int"></attr>
		<attr name="uid" type="int"></attr>
	</tag>
	<tag name="icap_fetch_event" type="function" returnType="int">
		<attr name="stream_id" type="int"></attr>
		<attr name="event_id" type="int"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="icap_list_alarms" type="function" returnType="int">
		<attr name="stream_id" type="int"></attr>
		<attr name="date" type="array"></attr>
		<attr name="time" type="array"></attr>
	</tag>
	<tag name="icap_list_events" type="function" returnType="array">
		<attr name="stream_id" type="int"></attr>
		<attr name="begin_date" type="int"></attr>
		<attr name="end_date" type="int" status="optional"></attr>
	</tag>
	<tag name="icap_open" type="function" returnType="resource">
		<attr name="calendar" type="string"></attr>
		<attr name="username" type="string"></attr>
		<attr name="password" type="string"></attr>
		<attr name="options" type="string"></attr>
	</tag>
	<tag name="icap_rename_calendar" type="function" returnType="string">
		<attr name="stream_id" type="int"></attr>
		<attr name="old_name" type="string"></attr>
		<attr name="new_name" type="string"></attr>
	</tag>
	<tag name="icap_reopen" type="function" returnType="int">
		<attr name="stream_id" type="int"></attr>
		<attr name="calendar" type="string"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="icap_snooze" type="function" returnType="string">
		<attr name="stream_id" type="int"></attr>
		<attr name="uid" type="int"></attr>
	</tag>
	<tag name="icap_store_event" type="function" returnType="string">
		<attr name="stream_id" type="int"></attr>
		<attr name="event" type="object"></attr>
	</tag>
</tags>