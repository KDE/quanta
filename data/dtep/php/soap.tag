<!DOCTYPE tags>
<tags>
	<tag name="SoapClient::SoapClient" type="function" returnType="object" version="">
		<attr name="wsdl" type="mixed" status="required"></attr>
		<attr name="options" type="array" status="optional"></attr>
	</tag>
	<tag name="SoapClient::__call" type="function" returnType="mixed" version="">
		<attr name="function_name" type="string" status="required"></attr>
		<attr name="arguments" type="array" status="optional"></attr>
		<attr name="options" type="array" status="optional"></attr>
		<attr name="input_headers" type="array" status="optional"></attr>
		<attr name="output_headers" type="array" status="optional"></attr>
	</tag>
	<tag name="SoapClient::__getFunctions" type="function" returnType="array" version="">
	</tag>
	<tag name="SoapClient::__getLastRequest" type="function" returnType="string" version="">
	</tag>
	<tag name="SoapClient::__getLastResponse" type="function" returnType="object" version="">
	</tag>
	<tag name="SoapClient::__getTypes" type="function" returnType="array" version="">
	</tag>
	<tag name="SoapFault::SoapFault" type="function" returnType="object" version="">
		<attr name="faultcode" type="string" status="required"></attr>
		<attr name="faultstring" type="string" status="required"></attr>
		<attr name="faultactor" type="string" status="optional"></attr>
		<attr name="detail" type="mixed" status="optional"></attr>
		<attr name="faultname" type="string" status="optional"></attr>
		<attr name="headerfault" type="mixed" status="optional"></attr>
	</tag>
	<tag name="SoapHeader::SoapHeader" type="function" returnType="object" version="">
		<attr name="namespace" type="string" status="required"></attr>
		<attr name="name" type="string" status="required"></attr>
		<attr name="data" type="mixed" status="optional"></attr>
		<attr name="mustUnderstand" type="bool" status="optional"></attr>
		<attr name="actor" type="mixed" status="optional"></attr>
	</tag>
	<tag name="SoapParam::SoapParam" type="function" returnType="object" version="">
		<attr name="data" type="mixed" status="required"></attr>
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="SoapServer::SoapServer" type="function" returnType="object" version="">
		<attr name="wsdl" type="mixed" status="required"></attr>
		<attr name="options" type="array" status="optional"></attr>
	</tag>
	<tag name="SoapServer::addFunction" type="function" returnType="void" version="">
		<attr name="functions" type="mixed" status="required"></attr>
	</tag>
	<tag name="SoapServer::getFunctions" type="function" returnType="array" version="">
	</tag>
	<tag name="SoapServer::handle" type="function" returnType="void" version="">
		<attr name="soap_request" type="string" status="optional"></attr>
	</tag>
	<tag name="SoapServer::setClass" type="function" returnType="void" version="">
		<attr name="class_name" type="string" status="required"></attr>
		<attr name="args" type="mixed" status="optional"></attr>
	</tag>
	<tag name="SoapServer::setPersistence" type="function" returnType="void" version="">
		<attr name="mode" type="int" status="required"></attr>
	</tag>
	<tag name="SoapVar::SoapVar" type="function" returnType="object" version="">
		<attr name="data" type="mixed" status="required"></attr>
		<attr name="encoding" type="int" status="required"></attr>
		<attr name="type_name" type="string" status="optional"></attr>
		<attr name="type_namespace" type="string" status="optional"></attr>
		<attr name="node_name" type="string" status="optional"></attr>
		<attr name="node_namespace" type="string" status="optional"></attr>
	</tag>
	<tag name="is_soap_fault" type="function" returnType="bool" version="PHP 5">
		<attr name="obj" type="mixed" status="required"></attr>
	</tag>
</tags>