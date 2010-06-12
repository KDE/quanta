<!DOCTYPE tags>
<tags>	<tag name="dblist" type="function" returnType="string">
	</tag>
	<tag name="dbmclose" type="function" returnType="bool">
		<attr name="dbm_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="dbmdelete" type="function" returnType="bool">
		<attr name="dbm_identifier" type="resource" status="optional"></attr>
		<attr name="key" type="string" status="optional"></attr>
	</tag>
	<tag name="dbmexists" type="function" returnType="bool">
		<attr name="dbm_identifier" type="resource" status="optional"></attr>
		<attr name="key" type="string" status="optional"></attr>
	</tag>
	<tag name="dbmfetch" type="function" returnType="string">
		<attr name="dbm_identifier" type="resource" status="optional"></attr>
		<attr name="key" type="string" status="optional"></attr>
	</tag>
	<tag name="dbmfirstkey" type="function" returnType="string">
		<attr name="dbm_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="dbminsert" type="function" returnType="int">
		<attr name="dbm_identifier" type="resource" status="optional"></attr>
		<attr name="key" type="string" status="optional"></attr>
		<attr name="value" type="string" status="optional"></attr>
	</tag>
	<tag name="dbmnextkey" type="function" returnType="string">
		<attr name="dbm_identifier" type="resource" status="optional"></attr>
		<attr name="key" type="string" status="optional"></attr>
	</tag>
	<tag name="dbmopen" type="function" returnType="resource">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="flags" type="string" status="optional"></attr>
	</tag>
	<tag name="dbmreplace" type="function" returnType="int">
		<attr name="dbm_identifier" type="resource" status="optional"></attr>
		<attr name="key" type="string" status="optional"></attr>
		<attr name="value" type="string" status="optional"></attr>
	</tag>
</tags>
