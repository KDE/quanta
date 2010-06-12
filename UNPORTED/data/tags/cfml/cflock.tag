<!DOCTYPE TAGS>
<TAGS>
<tag name="cflock">
	<attr name="timeout" />
	<attr name="scope" type="list">
		<items>
			<item>Application</item>
			<item>Server</item>
			<item>Session</item>
		</items>
	</attr>
	<attr name="name" />
	<attr name="throwontimeout" type="list">
		<items>
			<item>Yes</item>
			<item>No</item>
		</items>
	</attr>
	<attr name="type" type="list">
		<items>
			<item>read-only</item>
			<item>exclusive</item>
		</items>
	</attr>
</tag>
</TAGS>