<!DOCTYPE tags>
<tags>
	<tag name="vpopmail_add_alias_domain_ex" type="function" returnType="bool" version="4.0.5 - 4.2.3 only">
		<attr name="olddomain" type="string" status="required"></attr>
		<attr name="newdomain" type="string" status="required"></attr>
	</tag>
	<tag name="vpopmail_add_alias_domain" type="function" returnType="bool" version="4.0.5 - 4.2.3 only">
		<attr name="domain" type="string" status="required"></attr>
		<attr name="aliasdomain" type="string" status="required"></attr>
	</tag>
	<tag name="vpopmail_add_domain_ex" type="function" returnType="bool" version="4.0.5 - 4.2.3 only">
		<attr name="domain" type="string" status="required"></attr>
		<attr name="passwd" type="string" status="required"></attr>
		<attr name="quota" type="string" status="optional"></attr>
		<attr name="bounce" type="string" status="optional"></attr>
		<attr name="apop" type="bool" status="optional"></attr>
	</tag>
	<tag name="vpopmail_add_domain" type="function" returnType="bool" version="4.0.5 - 4.2.3 only">
		<attr name="domain" type="string" status="required"></attr>
		<attr name="dir" type="string" status="required"></attr>
		<attr name="uid" type="int" status="required"></attr>
		<attr name="gid" type="int" status="required"></attr>
	</tag>
	<tag name="vpopmail_add_user" type="function" returnType="bool" version="4.0.5 - 4.2.3 only">
		<attr name="user" type="string" status="required"></attr>
		<attr name="domain" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="gecos" type="string" status="optional"></attr>
		<attr name="apop" type="bool" status="optional"></attr>
	</tag>
	<tag name="vpopmail_alias_add" type="function" returnType="bool" version="4.1.0 - 4.2.3 only">
		<attr name="user" type="string" status="required"></attr>
		<attr name="domain" type="string" status="required"></attr>
		<attr name="alias" type="string" status="required"></attr>
	</tag>
	<tag name="vpopmail_alias_del_domain" type="function" returnType="bool" version="4.1.0 - 4.2.3 only">
		<attr name="domain" type="string" status="required"></attr>
	</tag>
	<tag name="vpopmail_alias_del" type="function" returnType="bool" version="4.1.0 - 4.2.3 only">
		<attr name="user" type="string" status="required"></attr>
		<attr name="domain" type="string" status="required"></attr>
	</tag>
	<tag name="vpopmail_alias_get_all" type="function" returnType="array" version="4.1.0 - 4.2.3 only">
		<attr name="domain" type="string" status="required"></attr>
	</tag>
	<tag name="vpopmail_alias_get" type="function" returnType="array" version="4.1.0 - 4.2.3 only">
		<attr name="alias" type="string" status="required"></attr>
		<attr name="domain" type="string" status="required"></attr>
	</tag>
	<tag name="vpopmail_auth_user" type="function" returnType="bool" version="4.0.5 - 4.2.3 only">
		<attr name="user" type="string" status="required"></attr>
		<attr name="domain" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="apop" type="string" status="optional"></attr>
	</tag>
	<tag name="vpopmail_del_domain_ex" type="function" returnType="bool" version="4.0.5 - 4.2.3 only">
		<attr name="domain" type="string" status="required"></attr>
	</tag>
	<tag name="vpopmail_del_domain" type="function" returnType="bool" version="4.0.5 - 4.2.3 only">
		<attr name="domain" type="string" status="required"></attr>
	</tag>
	<tag name="vpopmail_del_user" type="function" returnType="bool" version="4.0.5 - 4.2.3 only">
		<attr name="user" type="string" status="required"></attr>
		<attr name="domain" type="string" status="required"></attr>
	</tag>
	<tag name="vpopmail_error" type="function" returnType="string" version="4.0.5 - 4.2.3 only">
	</tag>
	<tag name="vpopmail_passwd" type="function" returnType="bool" version="4.0.5 - 4.2.3 only">
		<attr name="user" type="string" status="required"></attr>
		<attr name="domain" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="apop" type="bool" status="optional"></attr>
	</tag>
	<tag name="vpopmail_set_user_quota" type="function" returnType="bool" version="4.0.5 - 4.2.3 only">
		<attr name="user" type="string" status="required"></attr>
		<attr name="domain" type="string" status="required"></attr>
		<attr name="quota" type="string" status="required"></attr>
	</tag>
</tags>