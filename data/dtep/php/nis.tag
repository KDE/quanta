<!DOCTYPE tags>
<tags>
	<tag name="yp_all" type="function" returnType="void">
		<attr name="domain" type="string"></attr>
		<attr name="map" type="string"></attr>
		<attr name="callback" type="string"></attr>
	</tag>
	<tag name="yp_cat" type="function" returnType="array">
		<attr name="domain" type="string"></attr>
		<attr name="map" type="string"></attr>
	</tag>
	<tag name="yp_err_string" type="function" returnType="string">
		<attr name="errorcode" type="int"></attr>
	</tag>
	<tag name="yp_errno" type="function" returnType="int">
	</tag>
	<tag name="yp_first" type="function" returnType="array">
		<attr name="domain" type="string"></attr>
		<attr name="map" type="string"></attr>
	</tag>
	<tag name="yp_get_default_domain" type="function" returnType="int">
	</tag>
	<tag name="yp_master" type="function" returnType="string">
		<attr name="domain" type="string"></attr>
		<attr name="map" type="string"></attr>
	</tag>
	<tag name="yp_match" type="function" returnType="string">
		<attr name="domain" type="string"></attr>
		<attr name="map" type="string"></attr>
		<attr name="key" type="string"></attr>
	</tag>
	<tag name="yp_next" type="function" returnType="array">
		<attr name="domain" type="string"></attr>
		<attr name="map" type="string"></attr>
		<attr name="key" type="string"></attr>
	</tag>
	<tag name="yp_order" type="function" returnType="int">
		<attr name="domain" type="string"></attr>
		<attr name="map" type="string"></attr>
	</tag>
</tags>