<!DOCTYPE tags>
<tags>
	<tag name="SimpleXMLElement->asXML" type="function" returnType="string" comment="">
	</tag>
	<tag name="simplexml_element->attributes" type="function" returnType="object SimpleXMLElement" comment="">
		<attr name="data" type="string" status="required"></attr>
	</tag>
	<tag name="simplexml_element->children" type="function" returnType="object SimpleXMLElement" comment="">
	</tag>
	<tag name="SimpleXMLElement->xpath" type="function" returnType="array" comment="">
		<attr name="path" type="string" status="required"></attr>
	</tag>
	<tag name="simplexml_import_dom" type="function" returnType="object" comment="PHP 5">
		<attr name="node" type="object" status="required"></attr>
		<attr name="class_name" type="string" status="optional"></attr>
	</tag>
	<tag name="simplexml_load_file" type="function" returnType="object" comment="PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="class_name" type="string" status="optional"></attr>
	</tag>
	<tag name="simplexml_load_string" type="function" returnType="object" comment="PHP 5">
		<attr name="data" type="string" status="required"></attr>
		<attr name="class_name" type="string" status="optional"></attr>
	</tag>
</tags>