<!DOCTYPE tags>
<tags>
	<tag name="bind_textdomain_codeset" type="function" returnType="string">
		<attr name="domain" type="string"></attr>
		<attr name="codeset" type="string"></attr>
	</tag>
	<tag name="bindtextdomain" type="function" returnType="string">
		<attr name="domain" type="string"></attr>
		<attr name="directory" type="string"></attr>
	</tag>
	<tag name="dcgettext" type="function" returnType="string">
		<attr name="domain" type="string"></attr>
		<attr name="message" type="string"></attr>
		<attr name="category" type="int"></attr>
	</tag>
	<tag name="dcngettext" type="function" returnType="string">
		<attr name="domain" type="string"></attr>
		<attr name="msgid1" type="string"></attr>
		<attr name="msgid2" type="string"></attr>
		<attr name="n" type="int"></attr>
		<attr name="category" type="int"></attr>
	</tag>
	<tag name="dgettext" type="function" returnType="string">
		<attr name="domain" type="string"></attr>
		<attr name="message" type="string"></attr>
	</tag>
	<tag name="dngettext" type="function" returnType="string">
		<attr name="domain" type="string"></attr>
		<attr name="msgid1" type="string"></attr>
		<attr name="msgid2" type="string"></attr>
		<attr name="n" type="int"></attr>
	</tag>
	<tag name="gettext" type="function" returnType="string">
		<attr name="message" type="string"></attr>
	</tag>
	<tag name="ngettext" type="function" returnType="string">
		<attr name="msgid1" type="string"></attr>
		<attr name="msgid2" type="string"></attr>
		<attr name="n" type="int"></attr>
	</tag>
	<tag name="textdomain" type="function" returnType="string">
		<attr name="text_domain" type="string"></attr>
	</tag>
</tags>