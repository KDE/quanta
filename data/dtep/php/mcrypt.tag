<!DOCTYPE tags>
<tags>
	<tag name="mcrypt_cbc" type="function" returnType="string" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="cipher" type="int" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
		<attr name="mode" type="int" status="required"></attr>
		<attr name="iv" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_cfb" type="function" returnType="string" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="cipher" type="int" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
		<attr name="mode" type="int" status="required"></attr>
		<attr name="iv" type="string" status="required"></attr>
	</tag>
	<tag name="mcrypt_create_iv" type="function" returnType="string" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="size" type="int" status="required"></attr>
		<attr name="source" type="int" status="optional"></attr>
	</tag>
	<tag name="mcrypt_decrypt" type="function" returnType="string" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="cipher" type="string" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
		<attr name="mode" type="string" status="required"></attr>
		<attr name="iv" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_ecb" type="function" returnType="string" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="cipher" type="int" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
		<attr name="mode" type="int" status="required"></attr>
	</tag>
	<tag name="mcrypt_enc_get_algorithms_name" type="function" returnType="string" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="td" type="resource" status="required"></attr>
	</tag>
	<tag name="mcrypt_enc_get_block_size" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="td" type="resource" status="required"></attr>
	</tag>
	<tag name="mcrypt_enc_get_iv_size" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="td" type="resource" status="required"></attr>
	</tag>
	<tag name="mcrypt_enc_get_key_size" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="td" type="resource" status="required"></attr>
	</tag>
	<tag name="mcrypt_enc_get_modes_name" type="function" returnType="string" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="td" type="resource" status="required"></attr>
	</tag>
	<tag name="mcrypt_enc_get_supported_key_sizes" type="function" returnType="array" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="td" type="resource" status="required"></attr>
	</tag>
	<tag name="mcrypt_enc_is_block_algorithm_mode" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="td" type="resource" status="required"></attr>
	</tag>
	<tag name="mcrypt_enc_is_block_algorithm" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="td" type="resource" status="required"></attr>
	</tag>
	<tag name="mcrypt_enc_is_block_mode" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="td" type="resource" status="required"></attr>
	</tag>
	<tag name="mcrypt_enc_self_test" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="td" type="resource" status="required"></attr>
	</tag>
	<tag name="mcrypt_encrypt" type="function" returnType="string" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="cipher" type="string" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
		<attr name="mode" type="string" status="required"></attr>
		<attr name="iv" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_generic_deinit" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.1, PHP 5">
		<attr name="td" type="resource" status="required"></attr>
	</tag>
	<tag name="mcrypt_generic_end" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="td" type="resource" status="required"></attr>
	</tag>
	<tag name="mcrypt_generic_init" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="td" type="resource" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
		<attr name="iv" type="string" status="required"></attr>
	</tag>
	<tag name="mcrypt_generic" type="function" returnType="string" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="td" type="resource" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
	</tag>
	<tag name="mcrypt_get_block_size" type="function" returnType="int" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="cipher" type="int" status="required"></attr>
	</tag>
	<tag name="mcrypt_get_cipher_name" type="function" returnType="string" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="cipher" type="int" status="required"></attr>
	</tag>
	<tag name="mcrypt_get_iv_size" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="cipher" type="string" status="required"></attr>
		<attr name="mode" type="string" status="required"></attr>
	</tag>
	<tag name="mcrypt_get_key_size" type="function" returnType="int" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="cipher" type="int" status="required"></attr>
	</tag>
	<tag name="mcrypt_list_algorithms" type="function" returnType="array" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="lib_dir" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_list_modes" type="function" returnType="array" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="lib_dir" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_module_close" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="td" type="resource" status="required"></attr>
	</tag>
	<tag name="mcrypt_module_get_algo_block_size" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="algorithm" type="string" status="required"></attr>
		<attr name="lib_dir" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_module_get_algo_key_size" type="function" returnType="int" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="algorithm" type="string" status="required"></attr>
		<attr name="lib_dir" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_module_get_supported_key_sizes" type="function" returnType="array" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="algorithm" type="string" status="required"></attr>
		<attr name="lib_dir" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_module_is_block_algorithm_mode" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="mode" type="string" status="required"></attr>
		<attr name="lib_dir" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_module_is_block_algorithm" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="algorithm" type="string" status="required"></attr>
		<attr name="lib_dir" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_module_is_block_mode" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="mode" type="string" status="required"></attr>
		<attr name="lib_dir" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_module_open" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="algorithm" type="string" status="required"></attr>
		<attr name="algorithm_directory" type="string" status="required"></attr>
		<attr name="mode" type="string" status="required"></attr>
		<attr name="mode_directory" type="string" status="required"></attr>
	</tag>
	<tag name="mcrypt_module_self_test" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="algorithm" type="string" status="required"></attr>
		<attr name="lib_dir" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_ofb" type="function" returnType="string" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="cipher" type="int" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
		<attr name="mode" type="int" status="required"></attr>
		<attr name="iv" type="string" status="required"></attr>
	</tag>
	<tag name="mdecrypt_generic" type="function" returnType="string" version="PHP 4 &gt;= 4.0.2, PHP 5">
		<attr name="td" type="resource" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
	</tag>
</tags>