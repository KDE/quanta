<!DOCTYPE tags>
<tags>
	<tag name="DOMAttr->isId" type="function" returnType="bool" version="">
	</tag>
	<tag name="DOMCharacterData->appendData" type="function" returnType="void" version="">
		<attr name="data" type="string" status="required"></attr>
	</tag>
	<tag name="DOMCharacterData->deleteData" type="function" returnType="void" version="">
		<attr name="offset" type="int" status="required"></attr>
		<attr name="count" type="int" status="required"></attr>
	</tag>
	<tag name="DOMCharacterData->insertData" type="function" returnType="void" version="">
		<attr name="offset" type="int" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
	</tag>
	<tag name="DOMCharacterData->replaceData" type="function" returnType="void" version="">
		<attr name="offset" type="int" status="required"></attr>
		<attr name="count" type="int" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
	</tag>
	<tag name="DOMCharacterData->substringData" type="function" returnType="string" version="">
		<attr name="offset" type="int" status="required"></attr>
		<attr name="count" type="int" status="required"></attr>
	</tag>
	<tag name="DOMDocument->createAttribute" type="function" returnType="object" version="">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="DOMDocument->createAttributeNS" type="function" returnType="object" version="">
		<attr name="namespaceURI" type="string" status="required"></attr>
		<attr name="qualifiedName" type="string" status="required"></attr>
	</tag>
	<tag name="DOMDocument->createCDATASection" type="function" returnType="object" version="">
		<attr name="data" type="string" status="required"></attr>
	</tag>
	<tag name="DOMDocument->createComment" type="function" returnType="object" version="">
		<attr name="data" type="string" status="required"></attr>
	</tag>
	<tag name="DOMDocument->createDocumentFragment" type="function" returnType="object" version="">
	</tag>
	<tag name="DOMDocument->createElement" type="function" returnType="object" version="">
		<attr name="name" type="string" status="required"></attr>
		<attr name="value" type="string" status="optional"></attr>
	</tag>
	<tag name="DomDocument->createElementNS" type="function" returnType="object" version="">
		<attr name="namespaceURI" type="string" status="required"></attr>
		<attr name="qualifiedName" type="string" status="required"></attr>
	</tag>
	<tag name="DOMDocument->createEntityReference" type="function" returnType="object" version="">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="DOMDocument->createProcessingInstruction" type="function" returnType="object" version="">
		<attr name="target" type="string" status="required"></attr>
		<attr name="data" type="string" status="optional"></attr>
	</tag>
	<tag name="DOMDocument->createTextNode" type="function" returnType="object" version="">
		<attr name="content" type="string" status="required"></attr>
	</tag>
	<tag name="DOMDocument->getElementById" type="function" returnType="object" version="">
		<attr name="elementId" type="string" status="required"></attr>
	</tag>
	<tag name="DOMDocument->getElementsByTagName" type="function" returnType="object" version="">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="DOMDocument->getElementsByTagNameNS" type="function" returnType="object" version="">
		<attr name="namespaceURI" type="string" status="required"></attr>
		<attr name="localName" type="string" status="required"></attr>
	</tag>
	<tag name="DOMDocument->importNode" type="function" returnType="object" version="">
		<attr name="importedNode" type="object" status="required"></attr>
		<attr name="deep" type="bool" status="optional"></attr>
	</tag>
	<tag name="DOMDocument->load" type="function" returnType="mixed" version="">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="DOMDocument->loadHTML" type="function" returnType="mixed" version="">
		<attr name="source" type="string" status="required"></attr>
	</tag>
	<tag name="DOMDocument->loadHTMLFile" type="function" returnType="mixed" version="">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="DOMDocument->loadXML" type="function" returnType="mixed" version="">
		<attr name="source" type="string" status="required"></attr>
	</tag>
	<tag name="DOMDocument->normalize" type="function" returnType="void" version="">
	</tag>
	<tag name="DOMDocument->relaxNGValidate" type="function" returnType="bool" version="">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="DOMDocument->relaxNGValidateSource" type="function" returnType="bool" version="">
		<attr name="source" type="string" status="required"></attr>
	</tag>
	<tag name="DOMDocument->save" type="function" returnType="int" version="">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="DOMDocument->saveHTML" type="function" returnType="string" version="">
	</tag>
	<tag name="DOMDocument->saveHTMLFile" type="function" returnType="string" version="">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="DOMDocument->saveXML" type="function" returnType="string" version="">
		<attr name="node" type="object" status="optional"></attr>
	</tag>
	<tag name="DOMDocument->schemaValidate" type="function" returnType="bool" version="">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="DOMDocument->schemaValidateSource" type="function" returnType="bool" version="">
		<attr name="source" type="string" status="required"></attr>
	</tag>
	<tag name="DOMDocument->validate" type="function" returnType="bool" version="">
	</tag>
	<tag name="DOMDocument->xinclude" type="function" returnType="int" version="">
	</tag>
	<tag name="DOMElement->getAttribute" type="function" returnType="string" version="">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="DOMElement->getAttributeNode" type="function" returnType="object" version="">
		<attr name="name" type="object" status="required"></attr>
	</tag>
	<tag name="DOMElement->getAttributeNodeNS" type="function" returnType="object" version="">
		<attr name="namespaceURI" type="string" status="required"></attr>
		<attr name="localName" type="string" status="required"></attr>
	</tag>
	<tag name="DOMElement->getAttributeNS" type="function" returnType="string" version="">
		<attr name="namespaceURI" type="string" status="required"></attr>
		<attr name="localName" type="string" status="required"></attr>
	</tag>
	<tag name="DOMElement->getElementsByTagName" type="function" returnType="object" version="">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="DOMElement->getElementsByTagNameNS" type="function" returnType="object" version="">
		<attr name="namespaceURI" type="string" status="required"></attr>
		<attr name="localName" type="string" status="required"></attr>
	</tag>
	<tag name="DOMElement->hasAttribute" type="function" returnType="bool" version="">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="DOMElement->hasAttributeNS" type="function" returnType="bool" version="">
		<attr name="namespaceURI" type="string" status="required"></attr>
		<attr name="localName" type="string" status="required"></attr>
	</tag>
	<tag name="DOMElement->removeAttribute" type="function" returnType="bool" version="">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="DOMElement->removeAttributeNode" type="function" returnType="bool" version="">
		<attr name="oldnode" type="object" status="required"></attr>
	</tag>
	<tag name="DOMElement->removeAttributeNS" type="function" returnType="bool" version="">
		<attr name="namespaceURI" type="string" status="required"></attr>
		<attr name="localName" type="string" status="required"></attr>
	</tag>
	<tag name="DOMElement->setAttribute" type="function" returnType="bool" version="">
		<attr name="name" type="string" status="required"></attr>
		<attr name="value" type="string" status="required"></attr>
	</tag>
	<tag name="DOMElement->setAttributeNode" type="function" returnType="bool" version="">
		<attr name="attr" type="object" status="required"></attr>
	</tag>
	<tag name="DOMElement->setAttributeNodeNS" type="function" returnType="bool" version="">
		<attr name="attr" type="object" status="required"></attr>
	</tag>
	<tag name="DOMElement->setAttributeNS" type="function" returnType="void" version="">
		<attr name="namespaceURI" type="string" status="required"></attr>
		<attr name="qualifiedName" type="string" status="required"></attr>
		<attr name="value" type="string" status="required"></attr>
	</tag>
	<tag name="DOMImplementation->createDocument" type="function" returnType="object" version="">
		<attr name="namespaceURI" type="string" status="optional"></attr>
		<attr name="qualifiedName" type="string" status="optional"></attr>
		<attr name="doctype" type="object" status="optional"></attr>
	</tag>
	<tag name="DOMImplementation->createDocumentType" type="function" returnType="object" version="">
		<attr name="qualifiedName" type="string" status="optional"></attr>
		<attr name="publicId" type="string" status="optional"></attr>
		<attr name="systemId" type="string" status="optional"></attr>
	</tag>
	<tag name="DOMImplementation->hasFeature" type="function" returnType="bool" version="">
		<attr name="feature" type="string" status="required"></attr>
		<attr name="version" type="string" status="required"></attr>
	</tag>
	<tag name="DOMNamedNodeMap->getNamedItem" type="function" returnType="object" version="">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="DOMNamedNodeMap->getNamedItemNS" type="function" returnType="object" version="">
		<attr name="namespaceURI" type="string" status="required"></attr>
		<attr name="localName" type="string" status="required"></attr>
	</tag>
	<tag name="DOMNamedNodeMap->item" type="function" returnType="object" version="">
		<attr name="index" type="int" status="required"></attr>
	</tag>
	<tag name="DOMNode->appendChild" type="function" returnType="object" version="">
		<attr name="newnode" type="object" status="required"></attr>
	</tag>
	<tag name="DOMNode->cloneNode" type="function" returnType="object" version="">
		<attr name="deep" type="bool" status="optional"></attr>
	</tag>
	<tag name="DOMNode->hasAttributes" type="function" returnType="bool" version="">
	</tag>
	<tag name="DOMNode->hasChildNodes" type="function" returnType="bool" version="">
	</tag>
	<tag name="DOMNode->insertBefore" type="function" returnType="object" version="">
		<attr name="newnode" type="object" status="required"></attr>
		<attr name="refnode" type="object" status="optional"></attr>
	</tag>
	<tag name="DOMNode->isSameNode" type="function" returnType="bool" version="">
		<attr name="node" type="object" status="required"></attr>
	</tag>
	<tag name="DOMNode->isSupported" type="function" returnType="bool" version="">
		<attr name="feature" type="string" status="required"></attr>
		<attr name="version" type="string" status="required"></attr>
	</tag>
	<tag name="DOMNode->lookupNamespaceURI" type="function" returnType="string" version="">
		<attr name="prefix" type="string" status="required"></attr>
	</tag>
	<tag name="DOMNode->lookupPrefix" type="function" returnType="string" version="">
		<attr name="namespaceURI" type="string" status="required"></attr>
	</tag>
	<tag name="DOMNode->normalize" type="function" returnType="void" version="">
	</tag>
	<tag name="DOMNode->removeChild" type="function" returnType="object" version="">
		<attr name="oldchild" type="object" status="required"></attr>
	</tag>
	<tag name="DOMNode->replaceChild" type="function" returnType="object" version="">
		<attr name="newnode" type="object" status="required"></attr>
		<attr name="oldnode" type="object" status="required"></attr>
	</tag>
	<tag name="DOMNodelist->item" type="function" returnType="object" version="">
		<attr name="index" type="int" status="required"></attr>
	</tag>
	<tag name="DOMText->isWhitespaceInElementContent" type="function" returnType="bool" version="">
	</tag>
	<tag name="DOMText->splitText" type="function" returnType="object" version="">
		<attr name="offset" type="int" status="required"></attr>
	</tag>
	<tag name="DOMXPath->query" type="function" returnType="object" version="">
		<attr name="expression" type="string" status="required"></attr>
		<attr name="contextnode" type="object" status="optional"></attr>
	</tag>
	<tag name="DOMXPath->registerNamespace" type="function" returnType="bool" version="">
		<attr name="prefix" type="string" status="required"></attr>
		<attr name="namespaceURI" type="string" status="required"></attr>
	</tag>
	<tag name="dom_import_simplexml" type="function" returnType="object" version="PHP 5">
		<attr name="node" type="object" status="required"></attr>
	</tag>
</tags>