<!DOCTYPE tags>
<tags>
	<tag name="openssl_csr_export_to_file" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="csr" type="resource" status="required"></attr>
		<attr name="outfilename" type="string" status="required"></attr>
		<attr name="notext" type="bool" status="optional"></attr>
	</tag>
	<tag name="openssl_csr_export" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="csr" type="resource" status="required"></attr>
		<attr name="&amp;out" type="string" status="required"></attr>
		<attr name="notext" type="bool" status="optional"></attr>
	</tag>
	<tag name="openssl_csr_new" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="dn" type="array" status="required"></attr>
		<attr name="&amp;privkey" type="resource" status="required"></attr>
		<attr name="configargs" type="array" status="optional"></attr>
		<attr name="extraattribs" type="array" status="optional"></attr>
	</tag>
	<tag name="openssl_csr_sign" type="function" returnType="resource" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="csr" type="mixed" status="required"></attr>
		<attr name="cacert" type="mixed" status="required"></attr>
		<attr name="priv_key" type="mixed" status="required"></attr>
		<attr name="days" type="int" status="required"></attr>
		<attr name="configargs" type="array" status="optional"></attr>
		<attr name="serial" type="int" status="optional"></attr>
	</tag>
	<tag name="openssl_error_string" type="function" returnType="mixed" version="PHP 4 &gt;= 4.0.6, PHP 5">
	</tag>
	<tag name="openssl_free_key" type="function" returnType="void" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="key_identifier" type="resource" status="required"></attr>
	</tag>
	<tag name="openssl_get_privatekey" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="key" type="mixed" status="required"></attr>
		<attr name="passphrase" type="string" status="optional"></attr>
	</tag>
	<tag name="openssl_get_publickey" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="certificate" type="mixed" status="required"></attr>
	</tag>
	<tag name="openssl_open" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="sealed_data" type="string" status="required"></attr>
		<attr name="&amp;open_data" type="string" status="required"></attr>
		<attr name="env_key" type="string" status="required"></attr>
		<attr name="priv_key_id" type="mixed" status="required"></attr>
	</tag>
	<tag name="openssl_pkcs7_decrypt" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="infilename" type="string" status="required"></attr>
		<attr name="outfilename" type="string" status="required"></attr>
		<attr name="recipcert" type="mixed" status="required"></attr>
		<attr name="recipkey" type="mixed" status="optional"></attr>
	</tag>
	<tag name="openssl_pkcs7_encrypt" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="infile" type="string" status="required"></attr>
		<attr name="outfile" type="string" status="required"></attr>
		<attr name="recipcerts" type="mixed" status="required"></attr>
		<attr name="headers" type="array" status="required"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="cipherid" type="int" status="optional"></attr>
	</tag>
	<tag name="openssl_pkcs7_sign" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="infilename" type="string" status="required"></attr>
		<attr name="outfilename" type="string" status="required"></attr>
		<attr name="signcert" type="mixed" status="required"></attr>
		<attr name="privkey" type="mixed" status="required"></attr>
		<attr name="headers" type="array" status="required"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="extracerts" type="string" status="optional"></attr>
	</tag>
	<tag name="openssl_pkcs7_verify" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="flags" type="int" status="required"></attr>
		<attr name="outfilename" type="string" status="optional"></attr>
		<attr name="cainfo" type="array" status="optional"></attr>
		<attr name="extracerts" type="string" status="optional"></attr>
	</tag>
	<tag name="openssl_pkey_export_to_file" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="key" type="mixed" status="required"></attr>
		<attr name="outfilename" type="string" status="required"></attr>
		<attr name="passphrase" type="string" status="optional"></attr>
		<attr name="configargs" type="array" status="optional"></attr>
	</tag>
	<tag name="openssl_pkey_export" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="key" type="mixed" status="required"></attr>
		<attr name="&amp;out" type="string" status="required"></attr>
		<attr name="passphrase" type="string" status="optional"></attr>
		<attr name="configargs" type="array" status="optional"></attr>
	</tag>
	<tag name="openssl_pkey_get_private" type="function" returnType="resource" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="key" type="mixed" status="required"></attr>
		<attr name="passphrase" type="string" status="optional"></attr>
	</tag>
	<tag name="openssl_pkey_get_public" type="function" returnType="resource" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="certificate" type="mixed" status="required"></attr>
	</tag>
	<tag name="openssl_pkey_new" type="function" returnType="resource" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="configargs" type="array" status="optional"></attr>
	</tag>
	<tag name="openssl_private_decrypt" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="data" type="string" status="required"></attr>
		<attr name="&amp;decrypted" type="string" status="required"></attr>
		<attr name="key" type="mixed" status="required"></attr>
		<attr name="padding" type="int" status="optional"></attr>
	</tag>
	<tag name="openssl_private_encrypt" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="data" type="string" status="required"></attr>
		<attr name="&amp;crypted" type="string" status="required"></attr>
		<attr name="key" type="mixed" status="required"></attr>
		<attr name="padding" type="int" status="optional"></attr>
	</tag>
	<tag name="openssl_public_decrypt" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="data" type="string" status="required"></attr>
		<attr name="&amp;decrypted" type="string" status="required"></attr>
		<attr name="key" type="mixed" status="required"></attr>
		<attr name="padding" type="int" status="optional"></attr>
	</tag>
	<tag name="openssl_public_encrypt" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="data" type="string" status="required"></attr>
		<attr name="&amp;crypted" type="string" status="required"></attr>
		<attr name="key" type="mixed" status="required"></attr>
		<attr name="padding" type="int" status="optional"></attr>
	</tag>
	<tag name="openssl_seal" type="function" returnType="int" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="data" type="string" status="required"></attr>
		<attr name="&amp;sealed_data" type="string" status="required"></attr>
		<attr name="&amp;env_keys" type="array" status="required"></attr>
		<attr name="pub_key_ids" type="array" status="required"></attr>
	</tag>
	<tag name="openssl_sign" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="data" type="string" status="required"></attr>
		<attr name="&amp;signature" type="string" status="required"></attr>
		<attr name="priv_key_id" type="mixed" status="required"></attr>
		<attr name="signature_alg" type="int" status="optional"></attr>
	</tag>
	<tag name="openssl_verify" type="function" returnType="int" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="data" type="string" status="required"></attr>
		<attr name="signature" type="string" status="required"></attr>
		<attr name="pub_key_id" type="mixed" status="required"></attr>
	</tag>
	<tag name="openssl_x509_check_private_key" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="cert" type="mixed" status="required"></attr>
		<attr name="key" type="mixed" status="required"></attr>
	</tag>
	<tag name="openssl_x509_checkpurpose" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="x509cert" type="mixed" status="required"></attr>
		<attr name="purpose" type="int" status="required"></attr>
		<attr name="cainfo" type="array" status="optional"></attr>
		<attr name="untrustedfile" type="string" status="optional"></attr>
	</tag>
	<tag name="openssl_x509_export_to_file" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="x509" type="mixed" status="required"></attr>
		<attr name="outfilename" type="string" status="required"></attr>
		<attr name="notext" type="bool" status="optional"></attr>
	</tag>
	<tag name="openssl_x509_export" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="x509" type="mixed" status="required"></attr>
		<attr name="&amp;output" type="string" status="required"></attr>
		<attr name="notext" type="bool" status="optional"></attr>
	</tag>
	<tag name="openssl_x509_free" type="function" returnType="void" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="x509cert" type="resource" status="required"></attr>
	</tag>
	<tag name="openssl_x509_parse" type="function" returnType="array" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="x509cert" type="mixed" status="required"></attr>
		<attr name="shortnames" type="bool" status="optional"></attr>
	</tag>
	<tag name="openssl_x509_read" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="x509certdata" type="mixed" status="required"></attr>
	</tag>
</tags>