<!DOCTYPE tags>
<tags>	<tag name="cal_days_in_month" type="function" returnType="int">
		<attr name="calendar" type="int" status="optional"></attr>
		<attr name="month" type="int" status="optional"></attr>
		<attr name="year" type="int" status="optional"></attr>
	</tag>
	<tag name="cal_from_jd" type="function" returnType="array">
		<attr name="jd" type="int" status="optional"></attr>
		<attr name="calendar" type="int" status="optional"></attr>
	</tag>
	<tag name="cal_info" type="function" returnType="array">
		<attr name="calendar" type="int" status="optional"></attr>
	</tag>
	<tag name="cal_to_jd" type="function" returnType="int">
		<attr name="calendar" type="int" status="optional"></attr>
		<attr name="month" type="int" status="optional"></attr>
		<attr name="day" type="int" status="optional"></attr>
		<attr name="year" type="int" status="optional"></attr>
	</tag>
	<tag name="easter_date" type="function" returnType="int">
		<attr name="year" type="int" status="optional"></attr>
	</tag>
	<tag name="easter_days" type="function" returnType="int">
		<attr name="year" type="int" status="optional"></attr>
		<attr name="method" type="int" status="optional"></attr>
	</tag>
	<tag name="frenchtojd" type="function" returnType="int">
		<attr name="month" type="int" status="optional"></attr>
		<attr name="day" type="int" status="optional"></attr>
		<attr name="year" type="int" status="optional"></attr>
	</tag>
	<tag name="gregoriantojd" type="function" returnType="int">
		<attr name="month" type="int" status="optional"></attr>
		<attr name="day" type="int" status="optional"></attr>
		<attr name="year" type="int" status="optional"></attr>
	</tag>
	<tag name="jddayofweek" type="function" returnType="mixed">
		<attr name="julianday" type="int" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="jdmonthname" type="function" returnType="string">
		<attr name="julianday" type="int" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="jdtofrench" type="function" returnType="string">
		<attr name="juliandaycount" type="int" status="optional"></attr>
	</tag>
	<tag name="jdtogregorian" type="function" returnType="string">
		<attr name="julianday" type="int" status="optional"></attr>
	</tag>
	<tag name="jdtojewish" type="function" returnType="string">
		<attr name="julianday" type="int" status="optional"></attr>
	</tag>
	<tag name="jdtojulian" type="function" returnType="string">
		<attr name="julianday" type="int" status="optional"></attr>
	</tag>
	<tag name="jdtounix" type="function" returnType="int">
		<attr name="jday" type="int" status="optional"></attr>
	</tag>
	<tag name="jewishtojd" type="function" returnType="int">
		<attr name="month" type="int" status="optional"></attr>
		<attr name="day" type="int" status="optional"></attr>
		<attr name="year" type="int" status="optional"></attr>
	</tag>
	<tag name="juliantojd" type="function" returnType="int">
		<attr name="month" type="int" status="optional"></attr>
		<attr name="day" type="int" status="optional"></attr>
		<attr name="year" type="int" status="optional"></attr>
	</tag>
	<tag name="unixtojd" type="function" returnType="int">
		<attr name="timestamp" type="int" status="optional"></attr>
	</tag>
</tags>
