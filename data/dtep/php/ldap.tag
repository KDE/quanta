<!DOCTYPE tags>
<tags>
	<tag name="ldap_8859_to_t61" type="function" returnType="string">
		<attr name="value" type="string"></attr>
	</tag>
	<tag name="ldap_add" type="function" returnType="bool">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="dn" type="string"></attr>
		<attr name="entry" type="array"></attr>
	</tag>
	<tag name="ldap_bind" type="function" returnType="bool">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="bind_rdn" type="string" status="optional"></attr>
		<attr name="bind_password" type="string" status="optional"></attr>
	</tag>
	<tag name="ldap_close" type="function" returnType="bool">
		<attr name="link_identifier" type="resource"></attr>
	</tag>
	<tag name="ldap_compare" type="function" returnType="bool">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="dn" type="string"></attr>
		<attr name="attribute" type="string"></attr>
		<attr name="value" type="string"></attr>
	</tag>
	<tag name="ldap_connect" type="function" returnType="resource">
		<attr name="hostname" type="string" status="optional"></attr>
		<attr name="port" type="int" status="optional"></attr>
	</tag>
	<tag name="ldap_count_entries" type="function" returnType="int">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="result_identifier" type="resource"></attr>
	</tag>
	<tag name="ldap_delete" type="function" returnType="bool">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="dn" type="string"></attr>
	</tag>
	<tag name="ldap_dn2ufn" type="function" returnType="string">
		<attr name="dn" type="string"></attr>
	</tag>
	<tag name="ldap_err2str" type="function" returnType="string">
		<attr name="errno" type="int"></attr>
	</tag>
	<tag name="ldap_errno" type="function" returnType="int">
		<attr name="link_identifier" type="resource"></attr>
	</tag>
	<tag name="ldap_error" type="function" returnType="string">
		<attr name="link_identifier" type="resource"></attr>
	</tag>
	<tag name="ldap_explode_dn" type="function" returnType="array">
		<attr name="dn" type="string"></attr>
		<attr name="with_attrib" type="int"></attr>
	</tag>
	<tag name="ldap_first_attribute" type="function" returnType="string">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="result_entry_identifier" type="resource"></attr>
		<attr name="&amp;ber_identifier" type="int"></attr>
	</tag>
	<tag name="ldap_first_entry" type="function" returnType="resource">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="result_identifier" type="resource"></attr>
	</tag>
	<tag name="ldap_first_reference" type="function" returnType="resource">
		<attr name="link" type="resource"></attr>
		<attr name="result" type="resource"></attr>
	</tag>
	<tag name="ldap_free_result" type="function" returnType="bool">
		<attr name="result_identifier" type="resource"></attr>
	</tag>
	<tag name="ldap_get_attributes" type="function" returnType="array">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="result_entry_identifier" type="resource"></attr>
	</tag>
	<tag name="ldap_get_dn" type="function" returnType="string">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="result_entry_identifier" type="resource"></attr>
	</tag>
	<tag name="ldap_get_entries" type="function" returnType="array">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="result_identifier" type="resource"></attr>
	</tag>
	<tag name="ldap_get_option" type="function" returnType="bool">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="option" type="int"></attr>
		<attr name="&amp;retval" type="mixed"></attr>
	</tag>
	<tag name="ldap_get_values_len" type="function" returnType="array">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="result_entry_identifier" type="resource"></attr>
		<attr name="attribute" type="string"></attr>
	</tag>
	<tag name="ldap_get_values" type="function" returnType="array">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="result_entry_identifier" type="resource"></attr>
		<attr name="attribute" type="string"></attr>
	</tag>
	<tag name="ldap_list" type="function" returnType="resource">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="base_dn" type="string"></attr>
		<attr name="filter" type="string"></attr>
		<attr name="attributes" type="array" status="optional"></attr>
		<attr name="attrsonly" type="int" status="optional"></attr>
		<attr name="sizelimit" type="int" status="optional"></attr>
		<attr name="timelimit" type="int" status="optional"></attr>
		<attr name="deref" type="int" status="optional"></attr>
	</tag>
	<tag name="ldap_mod_add" type="function" returnType="bool">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="dn" type="string"></attr>
		<attr name="entry" type="array"></attr>
	</tag>
	<tag name="ldap_mod_del" type="function" returnType="bool">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="dn" type="string"></attr>
		<attr name="entry" type="array"></attr>
	</tag>
	<tag name="ldap_mod_replace" type="function" returnType="bool">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="dn" type="string"></attr>
		<attr name="entry" type="array"></attr>
	</tag>
	<tag name="ldap_modify" type="function" returnType="bool">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="dn" type="string"></attr>
		<attr name="entry" type="array"></attr>
	</tag>
	<tag name="ldap_next_attribute" type="function" returnType="string">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="result_entry_identifier" type="resource"></attr>
		<attr name="&amp;ber_identifier" type="resource"></attr>
	</tag>
	<tag name="ldap_next_entry" type="function" returnType="resource">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="result_entry_identifier" type="resource"></attr>
	</tag>
	<tag name="ldap_next_reference" type="function" returnType="resource">
		<attr name="link" type="resource"></attr>
		<attr name="entry" type="resource"></attr>
	</tag>
	<tag name="ldap_parse_reference" type="function" returnType="bool">
		<attr name="link" type="resource"></attr>
		<attr name="entry" type="resource"></attr>
		<attr name="&amp;referrals" type="array"></attr>
	</tag>
	<tag name="ldap_parse_result" type="function" returnType="bool">
		<attr name="link" type="resource"></attr>
		<attr name="result" type="resource"></attr>
		<attr name="&amp;errcode" type="int"></attr>
		<attr name="&amp;matcheddn" type="string" status="optional"></attr>
		<attr name="&amp;errmsg" type="string" status="optional"></attr>
		<attr name="&amp;referrals" type="array" status="optional"></attr>
	</tag>
	<tag name="ldap_read" type="function" returnType="resource">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="base_dn" type="string"></attr>
		<attr name="filter" type="string"></attr>
		<attr name="attributes" type="array" status="optional"></attr>
		<attr name="attrsonly" type="int" status="optional"></attr>
		<attr name="sizelimit" type="int" status="optional"></attr>
		<attr name="timelimit" type="int" status="optional"></attr>
		<attr name="deref" type="int" status="optional"></attr>
	</tag>
	<tag name="ldap_rename" type="function" returnType="bool">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="dn" type="string"></attr>
		<attr name="newrdn" type="string"></attr>
		<attr name="newparent" type="string"></attr>
		<attr name="deleteoldrdn" type="bool"></attr>
	</tag>
	<tag name="ldap_sasl_bind" type="function" returnType="bool">
		<attr name="link" type="resource"></attr>
	</tag>
	<tag name="ldap_search" type="function" returnType="resource">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="base_dn" type="string"></attr>
		<attr name="filter" type="string"></attr>
		<attr name="attributes" type="array" status="optional"></attr>
		<attr name="attrsonly" type="int" status="optional"></attr>
		<attr name="sizelimit" type="int" status="optional"></attr>
		<attr name="timelimit" type="int" status="optional"></attr>
		<attr name="deref" type="int" status="optional"></attr>
	</tag>
	<tag name="ldap_set_option" type="function" returnType="bool">
		<attr name="link_identifier" type="resource"></attr>
		<attr name="option" type="int"></attr>
		<attr name="newval" type="mixed"></attr>
	</tag>
	<tag name="ldap_set_rebind_proc" type="function" returnType="bool">
		<attr name="link" type="resource"></attr>
		<attr name="callback" type="callback"></attr>
	</tag>
	<tag name="ldap_sort" type="function" returnType="bool">
		<attr name="link" type="resource"></attr>
		<attr name="result" type="resource"></attr>
		<attr name="sortfilter" type="string"></attr>
	</tag>
	<tag name="ldap_start_tls" type="function" returnType="bool">
		<attr name="link" type="resource"></attr>
	</tag>
	<tag name="ldap_t61_to_8859" type="function" returnType="string">
		<attr name="value" type="string"></attr>
	</tag>
	<tag name="ldap_unbind" type="function" returnType="bool">
		<attr name="link_identifier" type="resource"></attr>
	</tag>
</tags>