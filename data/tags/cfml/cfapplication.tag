<!DOCTYPE TAGS>
<TAGS>
<tag name="cfapplication" single="1">
	<attr name="name" type="input">
		<text>Name:</text>
		<textlocation row="0" col="0" />
		<location row="0" col="1" />
	</attr>
	<attr name="clientmanagement" type="list">
		<text>Client Management</text>
		<textlocation row="1" col="0" />
		<location row="1" col="1" />
		<items>
			<item>Yes</item>
			<item>No</item>
		</items>
	</attr>
	<attr name="setclientcookies" type="list">
		<text>Session Management</text>
		<textlocation row="2" col="0" />
		<location row="2" col="1" />
		<items>
			<item>Yes</item>
			<item>No</item>
		</items>
	</attr>
	<attr name="setclientcookies" type="list">
		<text>Set Client Cookies</text>
		<textlocation row="3" col="0" />
		<location row="3" col="1" />
		<items>
			<item>Yes</item>
			<item>No</item>
		</items>
	</attr>
	<attr name="setdomaincookies" type="list">
		<text>Set Domain Cookies</text>
		<textlocation row="4" col="0" />
		<location row="4" col="1" />
		<items>
			<item>Yes</item>
			<item>No</item>
		</items>
	</attr>
	<attr name="clientstorage" type="list">
		<text>Client Storage</text>
		<textlocation row="5" col="0" />
		<location row="5" col="1" />
		<items>
			<item>Registry</item>
			<item>Cookie</item>
			<item>"datasource_name"</item>
		</items>
	</attr>
	<attr name="applicationtimeout" type="input">
		<text>Application Timeout</text>
		<textlocation row="6" col="0" />
		<location row="6" col="1" />
	</attr>
	<attr name="sessiontimeout" type="input">
		<text>Session Timeout</text>
		<textlocation row="7" col="0" />
		<location row="7" col="1" />
	</attr>
    <spacer orientation="v">
        <location col="0" row="9" colspan="2" />
    </spacer>
</tag>
</TAGS>
