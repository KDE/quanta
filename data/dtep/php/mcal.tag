<!DOCTYPE tags>
<tags>	<tag name="mcal_append_event" type="function" returnType="int">
		<attr name="mcal_stream" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_close" type="function" returnType="int">
		<attr name="mcal_stream" type="int" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_create_calendar" type="function" returnType="string">
		<attr name="stream" type="int" status="optional"></attr>
		<attr name="calendar" type="string" status="optional"></attr>
	</tag>
	<tag name="mcal_date_compare" type="function" returnType="int">
		<attr name="a_year" type="int" status="optional"></attr>
		<attr name="a_month" type="int" status="optional"></attr>
		<attr name="a_day" type="int" status="optional"></attr>
		<attr name="b_year" type="int" status="optional"></attr>
		<attr name="b_month" type="int" status="optional"></attr>
		<attr name="b_day" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_date_valid" type="function" returnType="int">
		<attr name="year" type="int" status="optional"></attr>
		<attr name="month" type="int" status="optional"></attr>
		<attr name="day" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_day_of_week" type="function" returnType="int">
		<attr name="year" type="int" status="optional"></attr>
		<attr name="month" type="int" status="optional"></attr>
		<attr name="day" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_" type="function" returnType="int">
		<attr name="year" type="int" status="optional"></attr>
		<attr name="month" type="int" status="optional"></attr>
		<attr name="day" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_days_in_month" type="function" returnType="int">
		<attr name="month" type="int" status="optional"></attr>
		<attr name="leap year" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_delete_calendar" type="function" returnType="string">
		<attr name="stream" type="int" status="optional"></attr>
		<attr name="calendar" type="string" status="optional"></attr>
	</tag>
	<tag name="mcal_delete_event" type="function" returnType="int">
		<attr name="mcal_stream" type="int" status="optional"></attr>
		<attr name="event_id" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_event_add_attribute" type="function" returnType="void">
		<attr name="stream" type="int" status="optional"></attr>
		<attr name="attribute" type="string" status="optional"></attr>
		<attr name="value" type="string" status="optional"></attr>
	</tag>
	<tag name="mcal_event_init" type="function" returnType="int">
		<attr name="stream" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_event_set_alarm" type="function" returnType="int">
		<attr name="stream" type="int" status="optional"></attr>
		<attr name="alarm" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_event_set_category" type="function" returnType="int">
		<attr name="stream" type="int" status="optional"></attr>
		<attr name="category" type="string" status="optional"></attr>
	</tag>
	<tag name="mcal_event_set_class" type="function" returnType="int">
		<attr name="stream" type="int" status="optional"></attr>
		<attr name="class" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_event_set_description" type="function" returnType="int">
		<attr name="stream" type="int" status="optional"></attr>
		<attr name="description" type="string" status="optional"></attr>
	</tag>
	<tag name="mcal_event_set_end" type="function" returnType="int">
		<attr name="stream" type="int" status="optional"></attr>
		<attr name="year" type="int" status="optional"></attr>
		<attr name="month" type="int" status="optional"></attr>
		<attr name="day" type="int" status="optional"></attr>
		<attr name="hour" type="int" status="optional"></attr>
		<attr name="min" type="int" status="optional"></attr>
		<attr name="sec" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_event_set_recur_daily" type="function" returnType="int">
		<attr name="stream" type="int" status="optional"></attr>
		<attr name="year" type="int" status="optional"></attr>
		<attr name="month" type="int" status="optional"></attr>
		<attr name="day" type="int" status="optional"></attr>
		<attr name="interval" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_event_set_recur_monthly_mday" type="function" returnType="int">
		<attr name="stream" type="int" status="optional"></attr>
		<attr name="year" type="int" status="optional"></attr>
		<attr name="month" type="int" status="optional"></attr>
		<attr name="day" type="int" status="optional"></attr>
		<attr name="interval" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_event_set_recur_monthly_wday" type="function" returnType="int">
		<attr name="stream" type="int" status="optional"></attr>
		<attr name="year" type="int" status="optional"></attr>
		<attr name="month" type="int" status="optional"></attr>
		<attr name="day" type="int" status="optional"></attr>
		<attr name="interval" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_event_set_recur_none" type="function" returnType="int">
		<attr name="stream" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_event_set_recur_weekly" type="function" returnType="int">
		<attr name="stream" type="int" status="optional"></attr>
		<attr name="year" type="int" status="optional"></attr>
		<attr name="month" type="int" status="optional"></attr>
		<attr name="day" type="int" status="optional"></attr>
		<attr name="interval" type="int" status="optional"></attr>
		<attr name="weekdays" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_event_set_recur_yearly" type="function" returnType="int">
		<attr name="stream" type="int" status="optional"></attr>
		<attr name="year" type="int" status="optional"></attr>
		<attr name="month" type="int" status="optional"></attr>
		<attr name="day" type="int" status="optional"></attr>
		<attr name="interval" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_event_set_start" type="function" returnType="int">
		<attr name="stream" type="int" status="optional"></attr>
		<attr name="year" type="int" status="optional"></attr>
		<attr name="month" type="int" status="optional"></attr>
		<attr name="day" type="int" status="optional"></attr>
		<attr name="hour" type="int" status="optional"></attr>
		<attr name="min" type="int" status="optional"></attr>
		<attr name="sec" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_event_set_title" type="function" returnType="int">
		<attr name="stream" type="int" status="optional"></attr>
		<attr name="title" type="string" status="optional"></attr>
	</tag>
	<tag name="mcal_expunge" type="function" returnType="int">
		<attr name="stream" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_fetch_current_stream_event" type="function" returnType="object">
		<attr name="stream" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_fetch_event" type="function" returnType="object">
		<attr name="mcal_stream" type="int" status="optional"></attr>
		<attr name="event_id" type="int" status="optional"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_is_leap_year" type="function" returnType="int">
		<attr name="year" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_list_alarms" type="function" returnType="array">
		<attr name="mcal_stream" type="int" status="optional"></attr>
		<attr name="begin_year" type="int" status="optional"></attr>
		<attr name="begin_month" type="int" status="optional"></attr>
		<attr name="begin_day" type="int" status="optional"></attr>
		<attr name="end_year" type="int" status="optional"></attr>
		<attr name="end_month" type="int" status="optional"></attr>
		<attr name="end_day" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_list_events" type="function" returnType="array">
		<attr name="mcal_stream" type="int" status="optional"></attr>
		<attr name="begin_date" type="object" status="optional"></attr>
		<attr name="end_date" type="object" status="optional"></attr>
	</tag>
	<tag name="mcal_next_recurrence" type="function" returnType="int">
		<attr name="stream" type="int" status="optional"></attr>
		<attr name="weekstart" type="int" status="optional"></attr>
		<attr name="next" type="array" status="optional"></attr>
	</tag>
	<tag name="mcal_open" type="function" returnType="int">
		<attr name="calendar" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_popen" type="function" returnType="int">
		<attr name="calendar" type="string" status="optional"></attr>
		<attr name="username" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_rename_calendar" type="function" returnType="string">
		<attr name="stream" type="int" status="optional"></attr>
		<attr name="old_name" type="string" status="optional"></attr>
		<attr name="new_name" type="string" status="optional"></attr>
	</tag>
	<tag name="mcal_reopen" type="function" returnType="int">
		<attr name="calendar" type="string" status="optional"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_snooze" type="function" returnType="int">
		<attr name="id" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_store_event" type="function" returnType="int">
		<attr name="mcal_stream" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_time_valid" type="function" returnType="int">
		<attr name="hour" type="int" status="optional"></attr>
		<attr name="minutes" type="int" status="optional"></attr>
		<attr name="seconds" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_week_of_year" type="function" returnType="int">
		<attr name="day" type="int" status="optional"></attr>
		<attr name="month" type="int" status="optional"></attr>
		<attr name="year" type="int" status="optional"></attr>
	</tag>
</tags>
