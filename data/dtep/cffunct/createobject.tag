<!DOCTYPE TAGS>
<TAGS>
	<tag name="CreateObject" type="function" returnType="object">
		<attr name="type {COM|CORBA|JAVA}" type="int" status="required" />
		<attr name="class" type="string" status="required" />
		<attr name="context {COM=InProc|Local|Remote CORBA=IOR|NameService}" type="string" status="optional" />
		<attr name="serverName {for COM}" type="string" status="optional" />
		<attr name="locale {for CORBA}" type="string" status="optional" />
	</tag>
</TAGS>
