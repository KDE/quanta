<!DOCTYPE tags>
<tags>
	<tag name="hw_api_attribute->key" type="function" returnType="string" version="">
	</tag>
	<tag name="hw_api_attribute->langdepvalue" type="function" returnType="string" version="">
		<attr name="language" type="string" status="required"></attr>
	</tag>
	<tag name="hw_api_attribute->value" type="function" returnType="string" version="">
	</tag>
	<tag name="hw_api_attribute->values" type="function" returnType="array" version="">
	</tag>
	<tag name="hw_api_attribute" type="function" returnType="object" version="">
		<attr name="name" type="string" status="optional"></attr>
		<attr name="value" type="string" status="optional"></attr>
	</tag>
	<tag name="hw_api->checkin" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->checkout" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->children" type="function" returnType="array" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api_content->mimetype" type="function" returnType="string" version="">
	</tag>
	<tag name="hw_api_content->read" type="function" returnType="string" version="">
		<attr name="buffer" type="string" status="required"></attr>
		<attr name="len" type="integer" status="required"></attr>
	</tag>
	<tag name="hw_api->content" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->copy" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->dbstat" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->dcstat" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->dstanchors" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->dstofsrcanchors" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api_error->count" type="function" returnType="int" version="">
	</tag>
	<tag name="hw_api_error->reason" type="function" returnType="object" version="">
	</tag>
	<tag name="hw_api->find" type="function" returnType="array" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->ftstat" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hwapi_hgcsp" type="function" returnType="object" version="">
		<attr name="hostname" type="string" status="required"></attr>
		<attr name="port" type="int" status="optional"></attr>
	</tag>
	<tag name="hw_api->hwstat" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->identify" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->info" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->insert" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->insertanchor" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->insertcollection" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->insertdocument" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->link" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->lock" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->move" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api_content" type="function" returnType="object" version="">
		<attr name="content" type="string" status="required"></attr>
		<attr name="mimetype" type="string" status="required"></attr>
	</tag>
	<tag name="hw_api_object->assign" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api_object->attreditable" type="function" returnType="bool" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api_object->count" type="function" returnType="int" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api_object->insert" type="function" returnType="bool" version="">
		<attr name="attribute" type="object" status="required"></attr>
	</tag>
	<tag name="hw_api_object" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api_object->remove" type="function" returnType="bool" version="">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="hw_api_object->title" type="function" returnType="string" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api_object->value" type="function" returnType="string" version="">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="hw_api->object" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->objectbyanchor" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->parents" type="function" returnType="array" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api_reason->description" type="function" returnType="string" version="">
	</tag>
	<tag name="hw_api_reason->type" type="function" returnType="object" version="">
	</tag>
	<tag name="hw_api->remove" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->replace" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->setcommitedversion" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->srcanchors" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->srcsofdst" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->unlock" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->user" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
	<tag name="hw_api->userlist" type="function" returnType="object" version="">
		<attr name="parameter" type="array" status="required"></attr>
	</tag>
</tags>