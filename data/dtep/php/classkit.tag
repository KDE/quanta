<!DOCTYPE tags>
<tags>
	<tag name="classkit_import" type="function" returnType="array" version="">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="classkit_method_add" type="function" returnType="bool" version="">
		<attr name="classname" type="string" status="required"></attr>
		<attr name="methodname" type="string" status="required"></attr>
		<attr name="args" type="string" status="required"></attr>
		<attr name="code" type="string" status="required"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="classkit_method_copy" type="function" returnType="bool" version="">
		<attr name="dClass" type="string" status="required"></attr>
		<attr name="dMethod" type="string" status="required"></attr>
		<attr name="sClass" type="string" status="required"></attr>
		<attr name="sMethod" type="string" status="optional"></attr>
	</tag>
	<tag name="classkit_method_redefine" type="function" returnType="bool" version="">
		<attr name="classname" type="string" status="required"></attr>
		<attr name="methodname" type="string" status="required"></attr>
		<attr name="args" type="string" status="required"></attr>
		<attr name="code" type="string" status="required"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="classkit_method_remove" type="function" returnType="bool" version="">
		<attr name="classname" type="string" status="required"></attr>
		<attr name="methodname" type="string" status="required"></attr>
	</tag>
	<tag name="classkit_method_rename" type="function" returnType="bool" version="">
		<attr name="classname" type="string" status="required"></attr>
		<attr name="methodname" type="string" status="required"></attr>
		<attr name="newname" type="string" status="required"></attr>
	</tag>
</tags>