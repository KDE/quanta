<!DOCTYPE tags>
<tags>
	<tag name="checkdate" type="function" returnType="bool">
		<attr name="month" type="int"></attr>
		<attr name="day" type="int"></attr>
		<attr name="year" type="int"></attr>
	</tag>
	<tag name="date_sunrise" type="function" returnType="mixed">
		<attr name="timestamp" type="int"></attr>
		<attr name="format" type="int" status="optional"></attr>
		<attr name="latitude" type="float" status="optional"></attr>
		<attr name="longitude" type="float" status="optional"></attr>
		<attr name="zenith" type="float" status="optional"></attr>
		<attr name="gmt_offset" type="float" status="optional"></attr>
	</tag>
	<tag name="date_sunset" type="function" returnType="mixed">
		<attr name="timestamp" type="int"></attr>
		<attr name="format" type="int" status="optional"></attr>
		<attr name="latitude" type="float" status="optional"></attr>
		<attr name="longitude" type="float" status="optional"></attr>
		<attr name="zenith" type="float" status="optional"></attr>
		<attr name="gmt_offset" type="float" status="optional"></attr>
	</tag>
	<tag name="date" type="function" returnType="string">
		<attr name="format" type="string"></attr>
		<attr name="timestamp" type="int" status="optional"></attr>
	</tag>
	<tag name="getdate" type="function" returnType="array">
		<attr name="timestamp" type="int" status="optional"></attr>
	</tag>
	<tag name="gettimeofday" type="function" returnType="array">
	</tag>
	<tag name="gmdate" type="function" returnType="string">
		<attr name="format" type="string"></attr>
		<attr name="timestamp" type="int" status="optional"></attr>
	</tag>
	<tag name="gmmktime" type="function" returnType="int">
		<attr name="hour" type="int" status="optional"></attr>
		<attr name="minute" type="int" status="optional"></attr>
		<attr name="second" type="int" status="optional"></attr>
		<attr name="month" type="int" status="optional"></attr>
		<attr name="day" type="int" status="optional"></attr>
		<attr name="year" type="int" status="optional"></attr>
		<attr name="is_dst" type="int" status="optional"></attr>
	</tag>
	<tag name="gmstrftime" type="function" returnType="string">
		<attr name="format" type="string"></attr>
		<attr name="timestamp" type="int" status="optional"></attr>
	</tag>
	<tag name="idate" type="function" returnType="int">
		<attr name="format" type="string"></attr>
		<attr name="timestamp" type="int" status="optional"></attr>
	</tag>
	<tag name="localtime" type="function" returnType="array">
		<attr name="timestamp" type="int" status="optional"></attr>
		<attr name="is_associative" type="bool" status="optional"></attr>
	</tag>
	<tag name="microtime" type="function" returnType="mixed">
		<attr name="get_as_float" type="bool" status="optional"></attr>
	</tag>
	<tag name="mktime" type="function" returnType="int">
		<attr name="hour" type="int" status="optional"></attr>
		<attr name="minute" type="int" status="optional"></attr>
		<attr name="second" type="int" status="optional"></attr>
		<attr name="month" type="int" status="optional"></attr>
		<attr name="day" type="int" status="optional"></attr>
		<attr name="year" type="int" status="optional"></attr>
		<attr name="is_dst" type="int" status="optional"></attr>
	</tag>
	<tag name="strftime" type="function" returnType="string">
		<attr name="format" type="string"></attr>
		<attr name="timestamp" type="int" status="optional"></attr>
	</tag>
	<tag name="strptime" type="function" returnType="array">
		<attr name="timestamp" type="string"></attr>
		<attr name="format" type="string"></attr>
	</tag>
	<tag name="strtotime" type="function" returnType="int">
		<attr name="time" type="string"></attr>
		<attr name="now" type="int" status="optional"></attr>
	</tag>
	<tag name="time" type="function" returnType="int">
	</tag>
</tags>