<!DOCTYPE tags>
<tags>
	<tag name="xattr_get" type="function" returnType="string" langVersion="">
		<attr name="path" type="string" status="required"></attr>
		<attr name="name" type="string" status="required"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="xattr_list" type="function" returnType="array" langVersion="">
		<attr name="path" type="string" status="required"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="xattr_remove" type="function" returnType="bool" langVersion="">
		<attr name="path" type="string" status="required"></attr>
		<attr name="name" type="string" status="required"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="xattr_set" type="function" returnType="bool" langVersion="">
		<attr name="path" type="string" status="required"></attr>
		<attr name="name" type="string" status="required"></attr>
		<attr name="value" type="string" status="required"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="xattr_supported" type="function" returnType="bool" langVersion="">
		<attr name="path" type="string" status="required"></attr>
	</tag>
</tags>