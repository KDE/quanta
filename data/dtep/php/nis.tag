<!DOCTYPE tags>
<tags>
	<tag name="yp_all" type="function" returnType="void" langVersion="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="domain" type="string" status="required"></attr>
		<attr name="map" type="string" status="required"></attr>
		<attr name="callback" type="string" status="required"></attr>
	</tag>
	<tag name="yp_cat" type="function" returnType="array" langVersion="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="domain" type="string" status="required"></attr>
		<attr name="map" type="string" status="required"></attr>
	</tag>
	<tag name="yp_err_string" type="function" returnType="string" langVersion="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="errorcode" type="int" status="required"></attr>
	</tag>
	<tag name="yp_errno" type="function" returnType="int" langVersion="PHP 4 &gt;= 4.0.6, PHP 5">
	</tag>
	<tag name="yp_first" type="function" returnType="array" langVersion="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="domain" type="string" status="required"></attr>
		<attr name="map" type="string" status="required"></attr>
	</tag>
	<tag name="yp_get_default_domain" type="function" returnType="int" langVersion="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
	</tag>
	<tag name="yp_master" type="function" returnType="string" langVersion="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="domain" type="string" status="required"></attr>
		<attr name="map" type="string" status="required"></attr>
	</tag>
	<tag name="yp_match" type="function" returnType="string" langVersion="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="domain" type="string" status="required"></attr>
		<attr name="map" type="string" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
	</tag>
	<tag name="yp_next" type="function" returnType="array" langVersion="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="domain" type="string" status="required"></attr>
		<attr name="map" type="string" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
	</tag>
	<tag name="yp_order" type="function" returnType="int" langVersion="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="domain" type="string" status="required"></attr>
		<attr name="map" type="string" status="required"></attr>
	</tag>
</tags>