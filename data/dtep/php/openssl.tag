<!DOCTYPE tags>
<tags>	<tag name="openssl_csr_export_to_file" type="function" returnType="bool">
		<attr name="csr" type="resource" status="optional"></attr>
		<attr name="outfilename" type="string" status="optional"></attr>
		<attr name="notext" type="bool" status="optional"></attr>
	</tag>
	<tag name="openssl_csr_export" type="function" returnType="bool">
		<attr name="csr" type="resource" status="optional"></attr>
		<attr name="out" type="string" status="optional"></attr>
		<attr name="notext" type="bool" status="optional"></attr>
	</tag>
	<tag name="openssl_csr_new" type="function" returnType="bool">
		<attr name="dn" type="array" status="optional"></attr>
		<attr name="privkey" type="resource" status="optional"></attr>
		<attr name="extraattribs" type="array" status="optional"></attr>
		<attr name="configargs" type="array" status="optional"></attr>
	</tag>
	<tag name="openssl_csr_sign" type="function" returnType="resource">
		<attr name="csr" type="mixed" status="optional"></attr>
		<attr name="x509" type="mixed" status="optional"></attr>
		<attr name="priv_key" type="mixed" status="optional"></attr>
		<attr name="days" type="long" status="optional"></attr>
	</tag>
	<tag name="openssl_error_string" type="function" returnType="mixed">
	</tag>
	<tag name="openssl_free_key" type="function" returnType="void">
		<attr name="key_identifier" type="resource" status="optional"></attr>
	</tag>
	<tag name="openssl_get_privatekey" type="function" returnType="resource">
		<attr name="key" type="mixed" status="optional"></attr>
		<attr name="passphrase" type="string" status="optional"></attr>
	</tag>
	<tag name="openssl_get_publickey" type="function" returnType="resource">
		<attr name="certificate" type="mixed" status="optional"></attr>
	</tag>
	<tag name="openssl_open" type="function" returnType="bool">
		<attr name="sealed_data" type="string" status="optional"></attr>
		<attr name="open_data" type="string" status="optional"></attr>
		<attr name="env_key" type="string" status="optional"></attr>
		<attr name="priv_key_id" type="mixed" status="optional"></attr>
	</tag>
	<tag name="openssl_pkcs7_decrypt" type="function" returnType="bool">
		<attr name="infilename" type="string" status="optional"></attr>
		<attr name="outfilename" type="string" status="optional"></attr>
		<attr name="recipcert" type="mixed" status="optional"></attr>
		<attr name="recipkey" type="mixed" status="optional"></attr>
	</tag>
	<tag name="openssl_pkcs7_encrypt" type="function" returnType="bool">
		<attr name="infile" type="string" status="optional"></attr>
		<attr name="outfile" type="string" status="optional"></attr>
		<attr name="recipcerts" type="mixed" status="optional"></attr>
		<attr name="headers" type="array" status="optional"></attr>
		<attr name="flags" type="long" status="optional"></attr>
	</tag>
	<tag name="openssl_pkcs7_sign" type="function" returnType="bool">
		<attr name="infilename" type="string" status="optional"></attr>
		<attr name="outfilename" type="string" status="optional"></attr>
		<attr name="signcert" type="mixed" status="optional"></attr>
		<attr name="privkey" type="mixed" status="optional"></attr>
		<attr name="headers" type="array" status="optional"></attr>
		<attr name="flags" type="long" status="optional"></attr>
		<attr name="extracertsfilename" type="string" status="optional"></attr>
	</tag>
	<tag name="openssl_pkcs7_verify" type="function" returnType="bool">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="outfilename" type="string" status="optional"></attr>
		<attr name="cainfo" type="array" status="optional"></attr>
		<attr name="extracerts" type="string" status="optional"></attr>
	</tag>
	<tag name="openssl_pkey_export_to_file" type="function" returnType="bool">
		<attr name="key" type="mixed" status="optional"></attr>
		<attr name="outfilename" type="string" status="optional"></attr>
		<attr name="passphrase" type="string" status="optional"></attr>
		<attr name="config_args" type="array" status="optional"></attr>
	</tag>
	<tag name="openssl_pkey_export" type="function" returnType="bool">
		<attr name="key" type="mixed" status="optional"></attr>
		<attr name="out" type="mixed" status="optional"></attr>
		<attr name="passphrase" type="string" status="optional"></attr>
		<attr name="config_args" type="array" status="optional"></attr>
	</tag>
	<tag name="openssl_pkey_new" type="function" returnType="resource">
		<attr name="configargs" type="array" status="optional"></attr>
	</tag>
	<tag name="openssl_private_decrypt" type="function" returnType="bool">
		<attr name="data" type="string" status="optional"></attr>
		<attr name="crypted" type="string" status="optional"></attr>
		<attr name="key" type="mixed" status="optional"></attr>
		<attr name="padding" type="int" status="optional"></attr>
	</tag>
	<tag name="openssl_private_encrypt" type="function" returnType="bool">
		<attr name="data" type="string" status="optional"></attr>
		<attr name="crypted" type="string" status="optional"></attr>
		<attr name="key" type="mixed" status="optional"></attr>
		<attr name="padding" type="int" status="optional"></attr>
	</tag>
	<tag name="openssl_public_decrypt" type="function" returnType="bool">
		<attr name="data" type="string" status="optional"></attr>
		<attr name="crypted" type="string" status="optional"></attr>
		<attr name="key" type="resource" status="optional"></attr>
		<attr name="padding" type="int" status="optional"></attr>
	</tag>
	<tag name="openssl_public_encrypt" type="function" returnType="bool">
		<attr name="data" type="string" status="optional"></attr>
		<attr name="crypted" type="string" status="optional"></attr>
		<attr name="key" type="mixed" status="optional"></attr>
		<attr name="padding" type="int" status="optional"></attr>
	</tag>
	<tag name="openssl_seal" type="function" returnType="int">
		<attr name="data" type="string" status="optional"></attr>
		<attr name="sealed_data" type="string" status="optional"></attr>
		<attr name="env_keys" type="array" status="optional"></attr>
		<attr name="pub_key_ids" type="array" status="optional"></attr>
	</tag>
	<tag name="openssl_sign" type="function" returnType="bool">
		<attr name="data" type="string" status="optional"></attr>
		<attr name="signature" type="string" status="optional"></attr>
		<attr name="priv_key_id" type="mixed" status="optional"></attr>
	</tag>
	<tag name="openssl_verify" type="function" returnType="int">
		<attr name="data" type="string" status="optional"></attr>
		<attr name="signature" type="string" status="optional"></attr>
		<attr name="pub_key_id" type="mixed" status="optional"></attr>
	</tag>
	<tag name="openssl_x509_check_private_key" type="function" returnType="bool">
		<attr name="cert" type="mixed" status="optional"></attr>
		<attr name="key" type="mixed" status="optional"></attr>
	</tag>
	<tag name="openssl_x509_checkpurpose" type="function" returnType="bool">
		<attr name="x509cert" type="mixed" status="optional"></attr>
		<attr name="purpose" type="int" status="optional"></attr>
		<attr name="cainfo" type="array" status="optional"></attr>
		<attr name="untrustedfile" type="string" status="optional"></attr>
	</tag>
	<tag name="openssl_x509_export_to_file" type="function" returnType="bool">
		<attr name="x509" type="mixed" status="optional"></attr>
		<attr name="outfilename" type="string" status="optional"></attr>
		<attr name="notext" type="bool" status="optional"></attr>
	</tag>
	<tag name="openssl_x509_export" type="function" returnType="bool">
		<attr name="x509" type="mixed" status="optional"></attr>
		<attr name="outfilename" type="string" status="optional"></attr>
		<attr name="notext" type="bool" status="optional"></attr>
	</tag>
	<tag name="openssl_x509_free" type="function" returnType="void">
		<attr name="x509cert" type="resource" status="optional"></attr>
	</tag>
	<tag name="openssl_x509_parse" type="function" returnType="array">
		<attr name="x509cert" type="mixed" status="optional"></attr>
		<attr name="shortnames" type="bool" status="optional"></attr>
	</tag>
	<tag name="openssl_x509_read" type="function" returnType="resource">
		<attr name="x509certdata" type="mixed" status="optional"></attr>
	</tag>
</tags>
