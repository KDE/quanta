<!DOCTYPE tags>
<tags>
	<tag name="xattr_get" type="function" returnType="string">
		<attr name="path" type="string"></attr>
		<attr name="name" type="string"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="xattr_list" type="function" returnType="array">
		<attr name="path" type="string"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="xattr_remove" type="function" returnType="bool">
		<attr name="path" type="string"></attr>
		<attr name="name" type="string"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="xattr_set" type="function" returnType="bool">
		<attr name="path" type="string"></attr>
		<attr name="name" type="string"></attr>
		<attr name="value" type="string"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="xattr_supported" type="function" returnType="bool">
		<attr name="path" type="string"></attr>
	</tag>
</tags>