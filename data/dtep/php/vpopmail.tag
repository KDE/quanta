<!DOCTYPE tags>
<tags>	<tag name="vpopmail_add_alias_domain_ex" type="function" returnType="bool">
		<attr name="olddomain" type="string" status="optional"></attr>
		<attr name="newdomain" type="string" status="optional"></attr>
	</tag>
	<tag name="vpopmail_add_alias_domain" type="function" returnType="bool">
		<attr name="domain" type="string" status="optional"></attr>
		<attr name="aliasdomain" type="string" status="optional"></attr>
	</tag>
	<tag name="vpopmail_add_domain_ex" type="function" returnType="bool">
		<attr name="domain" type="string" status="optional"></attr>
		<attr name="passwd" type="string" status="optional"></attr>
		<attr name="quota" type="string" status="optional"></attr>
		<attr name="bounce" type="string" status="optional"></attr>
		<attr name="apop" type="bool" status="optional"></attr>
	</tag>
	<tag name="vpopmail_add_domain" type="function" returnType="bool">
		<attr name="domain" type="string" status="optional"></attr>
		<attr name="dir" type="string" status="optional"></attr>
		<attr name="uid" type="int" status="optional"></attr>
		<attr name="gid" type="int" status="optional"></attr>
	</tag>
	<tag name="vpopmail_add_user" type="function" returnType="bool">
		<attr name="user" type="string" status="optional"></attr>
		<attr name="domain" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="gecos" type="string" status="optional"></attr>
		<attr name="apop" type="bool" status="optional"></attr>
	</tag>
	<tag name="vpopmail_alias_add" type="function" returnType="bool">
		<attr name="user" type="string" status="optional"></attr>
		<attr name="domain" type="string" status="optional"></attr>
		<attr name="alias" type="string" status="optional"></attr>
	</tag>
	<tag name="vpopmail_alias_del_domain" type="function" returnType="bool">
		<attr name="domain" type="string" status="optional"></attr>
	</tag>
	<tag name="vpopmail_alias_del" type="function" returnType="bool">
		<attr name="user" type="string" status="optional"></attr>
		<attr name="domain" type="string" status="optional"></attr>
	</tag>
	<tag name="vpopmail_alias_get_all" type="function" returnType="array">
		<attr name="domain" type="string" status="optional"></attr>
	</tag>
	<tag name="vpopmail_alias_get" type="function" returnType="array">
		<attr name="alias" type="string" status="optional"></attr>
		<attr name="domain" type="string" status="optional"></attr>
	</tag>
	<tag name="vpopmail_auth_user" type="function" returnType="bool">
		<attr name="user" type="string" status="optional"></attr>
		<attr name="domain" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
		<attr name="apop" type="string" status="optional"></attr>
	</tag>
	<tag name="vpopmail_del_domain_ex" type="function" returnType="bool">
		<attr name="domain" type="string" status="optional"></attr>
	</tag>
	<tag name="vpopmail_del_domain" type="function" returnType="bool">
		<attr name="domain" type="string" status="optional"></attr>
	</tag>
	<tag name="vpopmail_del_user" type="function" returnType="bool">
		<attr name="user" type="string" status="optional"></attr>
		<attr name="domain" type="string" status="optional"></attr>
	</tag>
	<tag name="vpopmail_error" type="function" returnType="string">
	</tag>
	<tag name="vpopmail_passwd" type="function" returnType="bool">
		<attr name="user" type="string" status="optional"></attr>
		<attr name="domain" type="string" status="optional"></attr>
		<attr name="password" type="string" status="optional"></attr>
	</tag>
	<tag name="vpopmail_set_user_quota" type="function" returnType="bool">
		<attr name="user" type="string" status="optional"></attr>
		<attr name="domain" type="string" status="optional"></attr>
		<attr name="quota" type="string" status="optional"></attr>
	</tag>
</tags>
