<!DOCTYPE tags>
<tags>
	<tag name="openssl_csr_export_to_file" type="function" returnType="bool">
		<attr name="csr" type="resource"></attr>
		<attr name="outfilename" type="string"></attr>
		<attr name="notext" type="bool" status="optional"></attr>
	</tag>
	<tag name="openssl_csr_export" type="function" returnType="bool">
		<attr name="csr" type="resource"></attr>
		<attr name="&amp;out" type="string"></attr>
		<attr name="notext" type="bool" status="optional"></attr>
	</tag>
	<tag name="openssl_csr_new" type="function" returnType="bool">
		<attr name="dn" type="array"></attr>
		<attr name="&amp;privkey" type="resource"></attr>
		<attr name="configargs" type="array" status="optional"></attr>
		<attr name="extraattribs" type="array" status="optional"></attr>
	</tag>
	<tag name="openssl_csr_sign" type="function" returnType="resource">
		<attr name="csr" type="mixed"></attr>
		<attr name="cacert" type="mixed"></attr>
		<attr name="priv_key" type="mixed"></attr>
		<attr name="days" type="int"></attr>
		<attr name="configargs" type="array" status="optional"></attr>
		<attr name="serial" type="int" status="optional"></attr>
	</tag>
	<tag name="openssl_error_string" type="function" returnType="mixed">
	</tag>
	<tag name="openssl_free_key" type="function" returnType="void">
		<attr name="key_identifier" type="resource"></attr>
	</tag>
	<tag name="openssl_get_privatekey" type="function" returnType="resource">
		<attr name="key" type="mixed"></attr>
		<attr name="passphrase" type="string" status="optional"></attr>
	</tag>
	<tag name="openssl_get_publickey" type="function" returnType="resource">
		<attr name="certificate" type="mixed"></attr>
	</tag>
	<tag name="openssl_open" type="function" returnType="bool">
		<attr name="sealed_data" type="string"></attr>
		<attr name="&amp;open_data" type="string"></attr>
		<attr name="env_key" type="string"></attr>
		<attr name="priv_key_id" type="mixed"></attr>
	</tag>
	<tag name="openssl_pkcs7_decrypt" type="function" returnType="bool">
		<attr name="infilename" type="string"></attr>
		<attr name="outfilename" type="string"></attr>
		<attr name="recipcert" type="mixed"></attr>
		<attr name="recipkey" type="mixed" status="optional"></attr>
	</tag>
	<tag name="openssl_pkcs7_encrypt" type="function" returnType="bool">
		<attr name="infile" type="string"></attr>
		<attr name="outfile" type="string"></attr>
		<attr name="recipcerts" type="mixed"></attr>
		<attr name="headers" type="array"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="cipherid" type="int" status="optional"></attr>
	</tag>
	<tag name="openssl_pkcs7_sign" type="function" returnType="bool">
		<attr name="infilename" type="string"></attr>
		<attr name="outfilename" type="string"></attr>
		<attr name="signcert" type="mixed"></attr>
		<attr name="privkey" type="mixed"></attr>
		<attr name="headers" type="array"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="extracerts" type="string" status="optional"></attr>
	</tag>
	<tag name="openssl_pkcs7_verify" type="function" returnType="bool">
		<attr name="filename" type="string"></attr>
		<attr name="flags" type="int"></attr>
		<attr name="outfilename" type="string" status="optional"></attr>
		<attr name="cainfo" type="array" status="optional"></attr>
		<attr name="extracerts" type="string" status="optional"></attr>
	</tag>
	<tag name="openssl_pkey_export_to_file" type="function" returnType="bool">
		<attr name="key" type="mixed"></attr>
		<attr name="outfilename" type="string"></attr>
		<attr name="passphrase" type="string" status="optional"></attr>
		<attr name="configargs" type="array" status="optional"></attr>
	</tag>
	<tag name="openssl_pkey_export" type="function" returnType="bool">
		<attr name="key" type="mixed"></attr>
		<attr name="&amp;out" type="string"></attr>
		<attr name="passphrase" type="string" status="optional"></attr>
		<attr name="configargs" type="array" status="optional"></attr>
	</tag>
	<tag name="openssl_pkey_get_private" type="function" returnType="resource">
		<attr name="key" type="mixed"></attr>
		<attr name="passphrase" type="string" status="optional"></attr>
	</tag>
	<tag name="openssl_pkey_get_public" type="function" returnType="resource">
		<attr name="certificate" type="mixed"></attr>
	</tag>
	<tag name="openssl_pkey_new" type="function" returnType="resource">
		<attr name="configargs" type="array" status="optional"></attr>
	</tag>
	<tag name="openssl_private_decrypt" type="function" returnType="bool">
		<attr name="data" type="string"></attr>
		<attr name="&amp;decrypted" type="string"></attr>
		<attr name="key" type="mixed"></attr>
		<attr name="padding" type="int" status="optional"></attr>
	</tag>
	<tag name="openssl_private_encrypt" type="function" returnType="bool">
		<attr name="data" type="string"></attr>
		<attr name="&amp;crypted" type="string"></attr>
		<attr name="key" type="mixed"></attr>
		<attr name="padding" type="int" status="optional"></attr>
	</tag>
	<tag name="openssl_public_decrypt" type="function" returnType="bool">
		<attr name="data" type="string"></attr>
		<attr name="&amp;decrypted" type="string"></attr>
		<attr name="key" type="mixed"></attr>
		<attr name="padding" type="int" status="optional"></attr>
	</tag>
	<tag name="openssl_public_encrypt" type="function" returnType="bool">
		<attr name="data" type="string"></attr>
		<attr name="&amp;crypted" type="string"></attr>
		<attr name="key" type="mixed"></attr>
		<attr name="padding" type="int" status="optional"></attr>
	</tag>
	<tag name="openssl_seal" type="function" returnType="int">
		<attr name="data" type="string"></attr>
		<attr name="&amp;sealed_data" type="string"></attr>
		<attr name="&amp;env_keys" type="array"></attr>
		<attr name="pub_key_ids" type="array"></attr>
	</tag>
	<tag name="openssl_sign" type="function" returnType="bool">
		<attr name="data" type="string"></attr>
		<attr name="&amp;signature" type="string"></attr>
		<attr name="priv_key_id" type="mixed"></attr>
		<attr name="signature_alg" type="int" status="optional"></attr>
	</tag>
	<tag name="openssl_verify" type="function" returnType="int">
		<attr name="data" type="string"></attr>
		<attr name="signature" type="string"></attr>
		<attr name="pub_key_id" type="mixed"></attr>
	</tag>
	<tag name="openssl_x509_check_private_key" type="function" returnType="bool">
		<attr name="cert" type="mixed"></attr>
		<attr name="key" type="mixed"></attr>
	</tag>
	<tag name="openssl_x509_checkpurpose" type="function" returnType="bool">
		<attr name="x509cert" type="mixed"></attr>
		<attr name="purpose" type="int"></attr>
		<attr name="cainfo" type="array" status="optional"></attr>
		<attr name="untrustedfile" type="string" status="optional"></attr>
	</tag>
	<tag name="openssl_x509_export_to_file" type="function" returnType="bool">
		<attr name="x509" type="mixed"></attr>
		<attr name="outfilename" type="string"></attr>
		<attr name="notext" type="bool" status="optional"></attr>
	</tag>
	<tag name="openssl_x509_export" type="function" returnType="bool">
		<attr name="x509" type="mixed"></attr>
		<attr name="&amp;output" type="string"></attr>
		<attr name="notext" type="bool" status="optional"></attr>
	</tag>
	<tag name="openssl_x509_free" type="function" returnType="void">
		<attr name="x509cert" type="resource"></attr>
	</tag>
	<tag name="openssl_x509_parse" type="function" returnType="array">
		<attr name="x509cert" type="mixed"></attr>
		<attr name="shortnames" type="bool" status="optional"></attr>
	</tag>
	<tag name="openssl_x509_read" type="function" returnType="resource">
		<attr name="x509certdata" type="mixed"></attr>
	</tag>
</tags>