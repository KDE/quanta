<!DOCTYPE tags>
<tags>
	<tag name="dblist" type="function" returnType="string">
	</tag>
	<tag name="dbmclose" type="function" returnType="bool">
		<attr name="dbm_identifier" type="resource"></attr>
	</tag>
	<tag name="dbmdelete" type="function" returnType="bool">
		<attr name="dbm_identifier" type="resource"></attr>
		<attr name="key" type="string"></attr>
	</tag>
	<tag name="dbmexists" type="function" returnType="bool">
		<attr name="dbm_identifier" type="resource"></attr>
		<attr name="key" type="string"></attr>
	</tag>
	<tag name="dbmfetch" type="function" returnType="string">
		<attr name="dbm_identifier" type="resource"></attr>
		<attr name="key" type="string"></attr>
	</tag>
	<tag name="dbmfirstkey" type="function" returnType="string">
		<attr name="dbm_identifier" type="resource"></attr>
	</tag>
	<tag name="dbminsert" type="function" returnType="int">
		<attr name="dbm_identifier" type="resource"></attr>
		<attr name="key" type="string"></attr>
		<attr name="value" type="string"></attr>
	</tag>
	<tag name="dbmnextkey" type="function" returnType="string">
		<attr name="dbm_identifier" type="resource"></attr>
		<attr name="key" type="string"></attr>
	</tag>
	<tag name="dbmopen" type="function" returnType="resource">
		<attr name="filename" type="string"></attr>
		<attr name="flags" type="string"></attr>
	</tag>
	<tag name="dbmreplace" type="function" returnType="int">
		<attr name="dbm_identifier" type="resource"></attr>
		<attr name="key" type="string"></attr>
		<attr name="value" type="string"></attr>
	</tag>
</tags>