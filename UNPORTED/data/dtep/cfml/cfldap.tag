<!DOCTYPE TAGS>
<TAGS>
<tag name="cfldap" single="1">
	<attr name="server" />
	<attr name="port" />
	<attr name="username" />
	<attr name="password" />
	<attr name="action" type="list">
		<items>
			<item>query</item>
			<item>add</item>
			<item>modify</item>
			<item>modifyDN</item>
			<item>delete</item>
		</items>
	</attr>
	<attr name="name" />
	<attr name="timeout" />
	<attr name="maxrows" />
	<attr name="start" />
	<attr name="scope" type="list">
		<items>
			<item>onelevel</item>
			<item>base</item>
			<item>subtree</item>
		</items>
	</attr>
	<attr name="attributes" />
	<attr name="filter" />
	<attr name="sort" />
	<attr name="sortcontrol" type="list">
		<items>
			<item>nocase</item>
			<item>asc</item>
			<item>desc</item>
			<item>nocase, asc</item>
			<item>nocase, desc</item>
		</items>
	</attr>
	<attr name="dn" />
	<attr name="startrow" />
	<attr name="modifytype" type="list">
		<items>
			<item>add</item>
			<item>delete</item>
			<item>replace</item>
		</items>
	</attr>
	<attr name="rebind" type="list">
		<items>
			<item>Yes</item>
			<item>No</item>
		</items>
	</attr>
	<attr name="referral" />
	<attr name="secure" />
	<attr name="separator" />
	<attr name="delimiter" />
</tag>
</TAGS>