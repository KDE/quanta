<!DOCTYPE TAGS>
<TAGS>
	<tag name="CreateDate" type="function" returnType="date">
		<attr name="year" type="int" status="required" />
		<attr name="month" type="int" status="required" />
		<attr name="day" type="int" status="required" />
	</tag>
	<tag name="CreateDateTime" type="function" returnType="date_time">
		<attr name="year" type="int" status="required" />
		<attr name="month" type="int" status="required" />
		<attr name="day" type="int" status="required" />
		<attr name="hour" type="int" status="required" />
		<attr name="minute" type="int" status="required" />
		<attr name="second" type="int" status="required" />
	</tag>
	<tag name="CreateODBCDate" type="function" returnType="string">
		<attr name="date" type="object" status="required" />
	</tag>
	<tag name="CreateODBCDateTime" type="function" returnType="string">
		<attr name="date" type="object" status="required" />
	</tag>
	<tag name="CreateODBCTime" type="function" returnType="string">
		<attr name="date" type="object" status="required" />
	</tag>
	<tag name="CreateTime" type="function" returnType="string">
		<attr name="hour" type="int" status="required" />
		<attr name="minute" type="int" status="required" />
		<attr name="second" type="int" status="required" />
	</tag>
	<tag name="CreateTimeSpan" type="function" returnType="string">
		<attr name="days" type="int" status="required" />
		<attr name="hour" type="int" status="required" />
		<attr name="minute" type="int" status="required" />
		<attr name="second" type="int" status="required" />
	</tag>
	<tag name="DateAdd" type="function" returnType="date">
		<attr name="datepart" type="string" status="required" />
		<attr name="number" type="int" status="required" />
		<attr name="date" type="object" status="required" />
	</tag>
	<tag name="DateCompare" type="function" returnType="int">
		<attr name="date1" type="object" status="required" />
		<attr name="date2" type="object" status="required" />
		<attr name="datepart" type="string" status="optional" />
	</tag>
	<tag name="DateConvert" type="function" returnType="int">
		<attr name="conversion-type {local2UTC|UTC2local}" type="string" status="required" />
		<attr name="date" type="object" status="required" />
	</tag>
	<tag name="DateDiff" type="function" returnType="int">
		<attr name="datepart" type="string" status="required" />
		<attr name="date1" type="object" status="required" />
		<attr name="date2" type="object" status="required" />
	</tag>
	<tag name="DateFormat" type="function" returnType="date">
		<attr name="date" type="object" status="required" />
		<attr name="mask" type="object" status="optional" />
	</tag>
	<tag name="DatePart" type="function" returnType="int">
		<attr name="datepart" type="string" status="required" />
		<attr name="date" type="object" status="required" />
	</tag>
	<tag name="Day" type="function" returnType="int">
		<attr name="date" type="object" status="required" />
	</tag>
	<tag name="DayOfWeek" type="function" returnType="int">
		<attr name="date" type="object" status="required" />
	</tag>
	<tag name="DayOfWeekAsString" type="function" returnType="string">
		<attr name="day_of_week" type="int" status="required" />
	</tag>
	<tag name="DayOfYear" type="function" returnType="int">
		<attr name="date" type="object" status="required" />
	</tag>
	<tag name="DaysInMonth" type="function" returnType="int">
		<attr name="date" type="object" status="required" />
	</tag>
	<tag name="DayInYear" type="function" returnType="int">
		<attr name="date" type="object" status="required" />
	</tag>
	<tag name="FirstDayOfMonth" type="function" returnType="int">
		<attr name="date" type="string" status="required" />
	</tag>
	<tag name="GetTimeZoneInfo" type="function" returnType="string">
	</tag>
	<tag name="Hour" type="function" returnType="int">
		<attr name="date" type="string" status="required" />
	</tag>
</TAGS>


