<!DOCTYPE tags>
<tags>
	<tag name="checkdate" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="month" type="int" status="required"></attr>
		<attr name="day" type="int" status="required"></attr>
		<attr name="year" type="int" status="required"></attr>
	</tag>
	<tag name="date_sunrise" type="function" returnType="mixed" version="PHP 5">
		<attr name="timestamp" type="int" status="required"></attr>
		<attr name="format" type="int" status="optional"></attr>
		<attr name="latitude" type="float" status="optional"></attr>
		<attr name="longitude" type="float" status="optional"></attr>
		<attr name="zenith" type="float" status="optional"></attr>
		<attr name="gmt_offset" type="float" status="optional"></attr>
	</tag>
	<tag name="date_sunset" type="function" returnType="mixed" version="PHP 5">
		<attr name="timestamp" type="int" status="required"></attr>
		<attr name="format" type="int" status="optional"></attr>
		<attr name="latitude" type="float" status="optional"></attr>
		<attr name="longitude" type="float" status="optional"></attr>
		<attr name="zenith" type="float" status="optional"></attr>
		<attr name="gmt_offset" type="float" status="optional"></attr>
	</tag>
	<tag name="date" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="format" type="string" status="required"></attr>
		<attr name="timestamp" type="int" status="optional"></attr>
	</tag>
	<tag name="getdate" type="function" returnType="array" version="PHP 3, PHP 4 , PHP 5">
		<attr name="timestamp" type="int" status="optional"></attr>
	</tag>
	<tag name="gettimeofday" type="function" returnType="array" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
	</tag>
	<tag name="gmdate" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="format" type="string" status="required"></attr>
		<attr name="timestamp" type="int" status="optional"></attr>
	</tag>
	<tag name="gmmktime" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="hour" type="int" status="optional"></attr>
		<attr name="minute" type="int" status="optional"></attr>
		<attr name="second" type="int" status="optional"></attr>
		<attr name="month" type="int" status="optional"></attr>
		<attr name="day" type="int" status="optional"></attr>
		<attr name="year" type="int" status="optional"></attr>
		<attr name="is_dst" type="int" status="optional"></attr>
	</tag>
	<tag name="gmstrftime" type="function" returnType="string" version="PHP 3&gt;= 3.0.12, PHP 4 , PHP 5">
		<attr name="format" type="string" status="required"></attr>
		<attr name="timestamp" type="int" status="optional"></attr>
	</tag>
	<tag name="idate" type="function" returnType="int" version="PHP 5">
		<attr name="format" type="string" status="required"></attr>
		<attr name="timestamp" type="int" status="optional"></attr>
	</tag>
	<tag name="localtime" type="function" returnType="array" version="PHP 4 , PHP 5">
		<attr name="timestamp" type="int" status="optional"></attr>
		<attr name="is_associative" type="bool" status="optional"></attr>
	</tag>
	<tag name="microtime" type="function" returnType="mixed" version="PHP 3, PHP 4 , PHP 5">
		<attr name="get_as_float" type="bool" status="optional"></attr>
	</tag>
	<tag name="mktime" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="hour" type="int" status="optional"></attr>
		<attr name="minute" type="int" status="optional"></attr>
		<attr name="second" type="int" status="optional"></attr>
		<attr name="month" type="int" status="optional"></attr>
		<attr name="day" type="int" status="optional"></attr>
		<attr name="year" type="int" status="optional"></attr>
		<attr name="is_dst" type="int" status="optional"></attr>
	</tag>
	<tag name="strftime" type="function" returnType="string" version="PHP 3, PHP 4 , PHP 5">
		<attr name="format" type="string" status="required"></attr>
		<attr name="timestamp" type="int" status="optional"></attr>
	</tag>
	<tag name="strptime" type="function" returnType="array" version="">
		<attr name="timestamp" type="string" status="required"></attr>
		<attr name="format" type="string" status="required"></attr>
	</tag>
	<tag name="strtotime" type="function" returnType="int" version="PHP 3&gt;= 3.0.12, PHP 4 , PHP 5">
		<attr name="time" type="string" status="required"></attr>
		<attr name="now" type="int" status="optional"></attr>
	</tag>
	<tag name="time" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
	</tag>
</tags>