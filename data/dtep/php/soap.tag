<!DOCTYPE tags>
<tags>
	<tag name="SoapClient::SoapClient" type="function" returnType="object">
		<attr name="wsdl" type="mixed"></attr>
		<attr name="options" type="array" status="optional"></attr>
	</tag>
	<tag name="SoapClient::__call" type="function" returnType="mixed">
		<attr name="function_name" type="string"></attr>
		<attr name="arguments" type="array" status="optional"></attr>
		<attr name="options" type="array" status="optional"></attr>
		<attr name="input_headers" type="array" status="optional"></attr>
		<attr name="output_headers" type="array" status="optional"></attr>
	</tag>
	<tag name="SoapClient::__getFunctions" type="function" returnType="array">
	</tag>
	<tag name="SoapClient::__getLastRequest" type="function" returnType="string">
	</tag>
	<tag name="SoapClient::__getLastResponse" type="function" returnType="object">
	</tag>
	<tag name="SoapClient::__getTypes" type="function" returnType="array">
	</tag>
	<tag name="SoapFault::SoapFault" type="function" returnType="object">
		<attr name="faultcode" type="string"></attr>
		<attr name="faultstring" type="string"></attr>
		<attr name="faultactor" type="string" status="optional"></attr>
		<attr name="detail" type="mixed" status="optional"></attr>
		<attr name="faultname" type="string" status="optional"></attr>
		<attr name="headerfault" type="mixed" status="optional"></attr>
	</tag>
	<tag name="SoapHeader::SoapHeader" type="function" returnType="object">
		<attr name="namespace" type="string"></attr>
		<attr name="name" type="string"></attr>
		<attr name="data" type="mixed" status="optional"></attr>
		<attr name="mustUnderstand" type="bool" status="optional"></attr>
		<attr name="actor" type="mixed" status="optional"></attr>
	</tag>
	<tag name="SoapParam::SoapParam" type="function" returnType="object">
		<attr name="data" type="mixed"></attr>
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="SoapServer::SoapServer" type="function" returnType="object">
		<attr name="wsdl" type="mixed"></attr>
		<attr name="options" type="array" status="optional"></attr>
	</tag>
	<tag name="SoapServer::addFunction" type="function" returnType="void">
		<attr name="functions" type="mixed"></attr>
	</tag>
	<tag name="SoapServer::getFunctions" type="function" returnType="array">
	</tag>
	<tag name="SoapServer::handle" type="function" returnType="void">
		<attr name="soap_request" type="string" status="optional"></attr>
	</tag>
	<tag name="SoapServer::setClass" type="function" returnType="void">
		<attr name="class_name" type="string"></attr>
		<attr name="args" type="mixed" status="optional"></attr>
	</tag>
	<tag name="SoapServer::setPersistence" type="function" returnType="void">
		<attr name="mode" type="int"></attr>
	</tag>
	<tag name="SoapVar::SoapVar" type="function" returnType="object">
		<attr name="data" type="mixed"></attr>
		<attr name="encoding" type="int"></attr>
		<attr name="type_name" type="string" status="optional"></attr>
		<attr name="type_namespace" type="string" status="optional"></attr>
		<attr name="node_name" type="string" status="optional"></attr>
		<attr name="node_namespace" type="string" status="optional"></attr>
	</tag>
	<tag name="is_soap_fault" type="function" returnType="bool">
		<attr name="obj" type="mixed"></attr>
	</tag>
</tags>