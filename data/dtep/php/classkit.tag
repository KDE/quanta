<!DOCTYPE tags>
<tags>
	<tag name="classkit_import" type="function" returnType="array">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="classkit_method_add" type="function" returnType="bool">
		<attr name="classname" type="string"></attr>
		<attr name="methodname" type="string"></attr>
		<attr name="args" type="string"></attr>
		<attr name="code" type="string"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="classkit_method_copy" type="function" returnType="bool">
		<attr name="dClass" type="string"></attr>
		<attr name="dMethod" type="string"></attr>
		<attr name="sClass" type="string"></attr>
		<attr name="sMethod" type="string" status="optional"></attr>
	</tag>
	<tag name="classkit_method_redefine" type="function" returnType="bool">
		<attr name="classname" type="string"></attr>
		<attr name="methodname" type="string"></attr>
		<attr name="args" type="string"></attr>
		<attr name="code" type="string"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="classkit_method_remove" type="function" returnType="bool">
		<attr name="classname" type="string"></attr>
		<attr name="methodname" type="string"></attr>
	</tag>
	<tag name="classkit_method_rename" type="function" returnType="bool">
		<attr name="classname" type="string"></attr>
		<attr name="methodname" type="string"></attr>
		<attr name="newname" type="string"></attr>
	</tag>
</tags>