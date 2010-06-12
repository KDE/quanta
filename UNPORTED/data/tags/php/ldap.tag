<!DOCTYPE tags>
<tags>	<tag name="ldap_8859_to_t61" type="function" returnType="string">
		<attr name="value" type="string" status="optional"></attr>
	</tag>
	<tag name="ldap_add" type="function" returnType="bool">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="dn" type="string" status="optional"></attr>
		<attr name="entry" type="array" status="optional"></attr>
	</tag>
	<tag name="ldap_bind" type="function" returnType="bool">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="bind_rdn" type="string" status="optional"></attr>
		<attr name="bind_password" type="string" status="optional"></attr>
	</tag>
	<tag name="ldap_close" type="function" returnType="bool">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ldap_compare" type="function" returnType="bool">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="dn" type="string" status="optional"></attr>
		<attr name="attribute" type="string" status="optional"></attr>
		<attr name="value" type="string" status="optional"></attr>
	</tag>
	<tag name="ldap_connect" type="function" returnType="resource">
		<attr name="hostname" type="string" status="optional"></attr>
		<attr name="port" type="int" status="optional"></attr>
	</tag>
	<tag name="ldap_count_entries" type="function" returnType="int">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="result_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ldap_delete" type="function" returnType="bool">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="dn" type="string" status="optional"></attr>
	</tag>
	<tag name="ldap_dn2ufn" type="function" returnType="string">
		<attr name="dn" type="string" status="optional"></attr>
	</tag>
	<tag name="ldap_err2str" type="function" returnType="string">
		<attr name="errno" type="int" status="optional"></attr>
	</tag>
	<tag name="ldap_errno" type="function" returnType="int">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ldap_error" type="function" returnType="string">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ldap_explode_dn" type="function" returnType="array">
		<attr name="dn" type="string" status="optional"></attr>
		<attr name="with_attrib" type="int" status="optional"></attr>
	</tag>
	<tag name="ldap_first_attribute" type="function" returnType="string">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="result_entry_identifier" type="resource" status="optional"></attr>
		<attr name="ber_identifier" type="int" status="optional"></attr>
	</tag>
	<tag name="ldap_first_entry" type="function" returnType="resource">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="result_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ldap_first_reference" type="function" returnType="resource">
		<attr name="link" type="resource" status="optional"></attr>
		<attr name="result" type="resource" status="optional"></attr>
	</tag>
	<tag name="ldap_free_result" type="function" returnType="bool">
		<attr name="result_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ldap_get_attributes" type="function" returnType="array">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="result_entry_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ldap_get_dn" type="function" returnType="string">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="result_entry_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ldap_get_entries" type="function" returnType="array">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="result_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ldap_get_option" type="function" returnType="bool">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="option" type="int" status="optional"></attr>
		<attr name="retval" type="mixed" status="optional"></attr>
	</tag>
	<tag name="ldap_get_values_len" type="function" returnType="array">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="result_entry_identifier" type="resource" status="optional"></attr>
		<attr name="attribute" type="string" status="optional"></attr>
	</tag>
	<tag name="ldap_get_values" type="function" returnType="array">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="result_entry_identifier" type="resource" status="optional"></attr>
		<attr name="attribute" type="string" status="optional"></attr>
	</tag>
	<tag name="ldap_list" type="function" returnType="resource">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="base_dn" type="string" status="optional"></attr>
		<attr name="filter" type="string" status="optional"></attr>
		<attr name="attributes" type="array" status="optional"></attr>
		<attr name="attrsonly" type="int" status="optional"></attr>
		<attr name="sizelimit" type="int" status="optional"></attr>
		<attr name="timelimit" type="int" status="optional"></attr>
		<attr name="deref" type="int" status="optional"></attr>
	</tag>
	<tag name="ldap_mod_add" type="function" returnType="bool">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="dn" type="string" status="optional"></attr>
		<attr name="entry" type="array" status="optional"></attr>
	</tag>
	<tag name="ldap_mod_del" type="function" returnType="bool">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="dn" type="string" status="optional"></attr>
		<attr name="entry" type="array" status="optional"></attr>
	</tag>
	<tag name="ldap_mod_replace" type="function" returnType="bool">
		<attr name="link_identifier" type="resoure" status="optional"></attr>
		<attr name="dn" type="string" status="optional"></attr>
		<attr name="entry" type="array" status="optional"></attr>
	</tag>
	<tag name="ldap_modify" type="function" returnType="bool">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="dn" type="string" status="optional"></attr>
		<attr name="entry" type="array" status="optional"></attr>
	</tag>
	<tag name="ldap_next_attribute" type="function" returnType="string">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="result_entry_identifier" type="resource" status="optional"></attr>
		<attr name="ber_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ldap_next_entry" type="function" returnType="resource">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="result_entry_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="ldap_next_reference" type="function" returnType="resource">
		<attr name="link" type="resource" status="optional"></attr>
		<attr name="entry" type="resource" status="optional"></attr>
	</tag>
	<tag name="ldap_parse_reference" type="function" returnType="bool">
		<attr name="link" type="resource" status="optional"></attr>
		<attr name="entry" type="resource" status="optional"></attr>
		<attr name="referrals" type="array" status="optional"></attr>
	</tag>
	<tag name="ldap_parse_result" type="function" returnType="bool">
		<attr name="link" type="resource" status="optional"></attr>
		<attr name="result" type="resource" status="optional"></attr>
		<attr name="errcode" type="int" status="optional"></attr>
		<attr name="matcheddn" type="string" status="optional"></attr>
		<attr name="errmsg" type="string" status="optional"></attr>
		<attr name="referrals" type="array" status="optional"></attr>
	</tag>
	<tag name="ldap_read" type="function" returnType="resource">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="base_dn" type="string" status="optional"></attr>
		<attr name="filter" type="string" status="optional"></attr>
		<attr name="attributes" type="array" status="optional"></attr>
		<attr name="attrsonly" type="int" status="optional"></attr>
		<attr name="sizelimit" type="int" status="optional"></attr>
		<attr name="timelimit" type="int" status="optional"></attr>
		<attr name="deref" type="int" status="optional"></attr>
	</tag>
	<tag name="ldap_rename" type="function" returnType="bool">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="dn" type="string" status="optional"></attr>
		<attr name="newrdn" type="string" status="optional"></attr>
		<attr name="newparent" type="string" status="optional"></attr>
		<attr name="deleteoldrdn" type="bool" status="optional"></attr>
	</tag>
	<tag name="ldap_search" type="function" returnType="resource">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="base_dn" type="string" status="optional"></attr>
		<attr name="filter" type="string" status="optional"></attr>
		<attr name="attributes" type="array" status="optional"></attr>
		<attr name="attrsonly" type="int" status="optional"></attr>
		<attr name="sizelimit" type="int" status="optional"></attr>
		<attr name="timelimit" type="int" status="optional"></attr>
		<attr name="deref" type="int" status="optional"></attr>
	</tag>
	<tag name="ldap_set_option" type="function" returnType="bool">
		<attr name="link_identifier" type="resource" status="optional"></attr>
		<attr name="option" type="int" status="optional"></attr>
		<attr name="newval" type="mixed" status="optional"></attr>
	</tag>
	<tag name="ldap_set_rebind_proc" type="function" returnType="bool">
		<attr name="link" type="resource" status="optional"></attr>
		<attr name="callback" type="string" status="optional"></attr>
	</tag>
	<tag name="ldap_sort" type="function" returnType="bool">
		<attr name="link" type="resource" status="optional"></attr>
		<attr name="result" type="resource" status="optional"></attr>
		<attr name="sortfilter" type="string" status="optional"></attr>
	</tag>
	<tag name="ldap_start_tls" type="function" returnType="bool">
		<attr name="link" type="resource" status="optional"></attr>
	</tag>
	<tag name="ldap_t61_to_8859" type="function" returnType="string">
		<attr name="value" type="string" status="optional"></attr>
	</tag>
	<tag name="ldap_unbind" type="function" returnType="bool">
		<attr name="link_identifier" type="resource" status="optional"></attr>
	</tag>
</tags>
