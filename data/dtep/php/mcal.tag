<!DOCTYPE tags>
<tags>
	<tag name="mcal_append_event" type="function" returnType="int" version="PHP 4 ">
		<attr name="mcal_stream" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_close" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="mcal_stream" type="int" status="required"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_create_calendar" type="function" returnType="bool" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="stream" type="int" status="required"></attr>
		<attr name="calendar" type="string" status="required"></attr>
	</tag>
	<tag name="mcal_date_compare" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="a_year" type="int" status="required"></attr>
		<attr name="a_month" type="int" status="required"></attr>
		<attr name="a_day" type="int" status="required"></attr>
		<attr name="b_year" type="int" status="required"></attr>
		<attr name="b_month" type="int" status="required"></attr>
		<attr name="b_day" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_date_valid" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="year" type="int" status="required"></attr>
		<attr name="month" type="int" status="required"></attr>
		<attr name="day" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_day_of_week" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="year" type="int" status="required"></attr>
		<attr name="month" type="int" status="required"></attr>
		<attr name="day" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_day_of_year" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="year" type="int" status="required"></attr>
		<attr name="month" type="int" status="required"></attr>
		<attr name="day" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_days_in_month" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="month" type="int" status="required"></attr>
		<attr name="leap_year" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_delete_calendar" type="function" returnType="string" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="stream" type="int" status="required"></attr>
		<attr name="calendar" type="string" status="required"></attr>
	</tag>
	<tag name="mcal_delete_event" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="mcal_stream" type="int" status="required"></attr>
		<attr name="event_id" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_event_add_attribute" type="function" returnType="void" version="PHP 3&gt;= 3.0.15, PHP 4 ">
		<attr name="stream" type="int" status="required"></attr>
		<attr name="attribute" type="string" status="required"></attr>
		<attr name="value" type="string" status="required"></attr>
	</tag>
	<tag name="mcal_event_init" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="stream" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_event_set_alarm" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="stream" type="int" status="required"></attr>
		<attr name="alarm" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_event_set_category" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="stream" type="int" status="required"></attr>
		<attr name="category" type="string" status="required"></attr>
	</tag>
	<tag name="mcal_event_set_class" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="stream" type="int" status="required"></attr>
		<attr name="class" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_event_set_description" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="stream" type="int" status="required"></attr>
		<attr name="description" type="string" status="required"></attr>
	</tag>
	<tag name="mcal_event_set_end" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="stream" type="int" status="required"></attr>
		<attr name="year" type="int" status="required"></attr>
		<attr name="month" type="int" status="required"></attr>
		<attr name="day" type="int" status="required"></attr>
		<attr name="hour" type="int" status="optional"></attr>
		<attr name="min" type="int" status="optional"></attr>
		<attr name="sec" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_event_set_recur_daily" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="stream" type="int" status="required"></attr>
		<attr name="year" type="int" status="required"></attr>
		<attr name="month" type="int" status="required"></attr>
		<attr name="day" type="int" status="required"></attr>
		<attr name="interval" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_event_set_recur_monthly_mday" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="stream" type="int" status="required"></attr>
		<attr name="year" type="int" status="required"></attr>
		<attr name="month" type="int" status="required"></attr>
		<attr name="day" type="int" status="required"></attr>
		<attr name="interval" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_event_set_recur_monthly_wday" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="stream" type="int" status="required"></attr>
		<attr name="year" type="int" status="required"></attr>
		<attr name="month" type="int" status="required"></attr>
		<attr name="day" type="int" status="required"></attr>
		<attr name="interval" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_event_set_recur_none" type="function" returnType="int" version="PHP 3&gt;= 3.0.15, PHP 4 ">
		<attr name="stream" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_event_set_recur_weekly" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="stream" type="int" status="required"></attr>
		<attr name="year" type="int" status="required"></attr>
		<attr name="month" type="int" status="required"></attr>
		<attr name="day" type="int" status="required"></attr>
		<attr name="interval" type="int" status="required"></attr>
		<attr name="weekdays" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_event_set_recur_yearly" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="stream" type="int" status="required"></attr>
		<attr name="year" type="int" status="required"></attr>
		<attr name="month" type="int" status="required"></attr>
		<attr name="day" type="int" status="required"></attr>
		<attr name="interval" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_event_set_start" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="stream" type="int" status="required"></attr>
		<attr name="year" type="int" status="required"></attr>
		<attr name="month" type="int" status="required"></attr>
		<attr name="day" type="int" status="required"></attr>
		<attr name="hour" type="int" status="optional"></attr>
		<attr name="min" type="int" status="optional"></attr>
		<attr name="sec" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_event_set_title" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="stream" type="int" status="required"></attr>
		<attr name="title" type="string" status="required"></attr>
	</tag>
	<tag name="mcal_expunge" type="function" returnType="int" version="">
		<attr name="stream" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_fetch_current_stream_event" type="function" returnType="object" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="stream" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_fetch_event" type="function" returnType="object" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="mcal_stream" type="int" status="required"></attr>
		<attr name="event_id" type="int" status="required"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_is_leap_year" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="year" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_list_alarms" type="function" returnType="array" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="mcal_stream" type="int" status="required"></attr>
		<attr name="begin_year" type="int" status="optional"></attr>
		<attr name="begin_month" type="int" status="required"></attr>
		<attr name="begin_day" type="int" status="required"></attr>
		<attr name="end_year" type="int" status="required"></attr>
		<attr name="end_month" type="int" status="required"></attr>
		<attr name="end_day" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_list_events" type="function" returnType="array" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="mcal_stream" type="int" status="required"></attr>
		<attr name="begin_year" type="int" status="optional"></attr>
		<attr name="begin_month" type="int" status="required"></attr>
		<attr name="begin_day" type="int" status="required"></attr>
		<attr name="end_year" type="int" status="required"></attr>
		<attr name="end_month" type="int" status="required"></attr>
		<attr name="end_day" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_next_recurrence" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="stream" type="int" status="required"></attr>
		<attr name="weekstart" type="int" status="required"></attr>
		<attr name="next" type="array" status="required"></attr>
	</tag>
	<tag name="mcal_open" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="calendar" type="string" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_popen" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="calendar" type="string" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_rename_calendar" type="function" returnType="string" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="stream" type="int" status="required"></attr>
		<attr name="old_name" type="string" status="required"></attr>
		<attr name="new_name" type="string" status="required"></attr>
	</tag>
	<tag name="mcal_reopen" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="mcal_stream" type="int" status="required"></attr>
		<attr name="calendar" type="string" status="required"></attr>
		<attr name="options" type="int" status="optional"></attr>
	</tag>
	<tag name="mcal_snooze" type="function" returnType="bool" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="stream_id" type="int" status="required"></attr>
		<attr name="event_id" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_store_event" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="mcal_stream" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_time_valid" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 ">
		<attr name="hour" type="int" status="required"></attr>
		<attr name="minutes" type="int" status="required"></attr>
		<attr name="seconds" type="int" status="required"></attr>
	</tag>
	<tag name="mcal_week_of_year" type="function" returnType="int" version="PHP 4 ">
		<attr name="day" type="int" status="required"></attr>
		<attr name="month" type="int" status="required"></attr>
		<attr name="year" type="int" status="required"></attr>
	</tag>
</tags>