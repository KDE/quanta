<!DOCTYPE tags>
<tags>
	<tag name="mcrypt_cbc" type="function" returnType="string">
		<attr name="cipher" type="int"></attr>
		<attr name="key" type="string"></attr>
		<attr name="data" type="string"></attr>
		<attr name="mode" type="int"></attr>
		<attr name="iv" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_cfb" type="function" returnType="string">
		<attr name="cipher" type="int"></attr>
		<attr name="key" type="string"></attr>
		<attr name="data" type="string"></attr>
		<attr name="mode" type="int"></attr>
		<attr name="iv" type="string"></attr>
	</tag>
	<tag name="mcrypt_create_iv" type="function" returnType="string">
		<attr name="size" type="int"></attr>
		<attr name="source" type="int" status="optional"></attr>
	</tag>
	<tag name="mcrypt_decrypt" type="function" returnType="string">
		<attr name="cipher" type="string"></attr>
		<attr name="key" type="string"></attr>
		<attr name="data" type="string"></attr>
		<attr name="mode" type="string"></attr>
		<attr name="iv" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_ecb" type="function" returnType="string">
		<attr name="cipher" type="int"></attr>
		<attr name="key" type="string"></attr>
		<attr name="data" type="string"></attr>
		<attr name="mode" type="int"></attr>
	</tag>
	<tag name="mcrypt_enc_get_algorithms_name" type="function" returnType="string">
		<attr name="td" type="resource"></attr>
	</tag>
	<tag name="mcrypt_enc_get_block_size" type="function" returnType="int">
		<attr name="td" type="resource"></attr>
	</tag>
	<tag name="mcrypt_enc_get_iv_size" type="function" returnType="int">
		<attr name="td" type="resource"></attr>
	</tag>
	<tag name="mcrypt_enc_get_key_size" type="function" returnType="int">
		<attr name="td" type="resource"></attr>
	</tag>
	<tag name="mcrypt_enc_get_modes_name" type="function" returnType="string">
		<attr name="td" type="resource"></attr>
	</tag>
	<tag name="mcrypt_enc_get_supported_key_sizes" type="function" returnType="array">
		<attr name="td" type="resource"></attr>
	</tag>
	<tag name="mcrypt_enc_is_block_algorithm_mode" type="function" returnType="bool">
		<attr name="td" type="resource"></attr>
	</tag>
	<tag name="mcrypt_enc_is_block_algorithm" type="function" returnType="bool">
		<attr name="td" type="resource"></attr>
	</tag>
	<tag name="mcrypt_enc_is_block_mode" type="function" returnType="bool">
		<attr name="td" type="resource"></attr>
	</tag>
	<tag name="mcrypt_enc_self_test" type="function" returnType="bool">
		<attr name="td" type="resource"></attr>
	</tag>
	<tag name="mcrypt_encrypt" type="function" returnType="string">
		<attr name="cipher" type="string"></attr>
		<attr name="key" type="string"></attr>
		<attr name="data" type="string"></attr>
		<attr name="mode" type="string"></attr>
		<attr name="iv" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_generic_deinit" type="function" returnType="bool">
		<attr name="td" type="resource"></attr>
	</tag>
	<tag name="mcrypt_generic_end" type="function" returnType="bool">
		<attr name="td" type="resource"></attr>
	</tag>
	<tag name="mcrypt_generic_init" type="function" returnType="int">
		<attr name="td" type="resource"></attr>
		<attr name="key" type="string"></attr>
		<attr name="iv" type="string"></attr>
	</tag>
	<tag name="mcrypt_generic" type="function" returnType="string">
		<attr name="td" type="resource"></attr>
		<attr name="data" type="string"></attr>
	</tag>
	<tag name="mcrypt_get_block_size" type="function" returnType="int">
		<attr name="cipher" type="int"></attr>
	</tag>
	<tag name="mcrypt_get_cipher_name" type="function" returnType="string">
		<attr name="cipher" type="int"></attr>
	</tag>
	<tag name="mcrypt_get_iv_size" type="function" returnType="int">
		<attr name="cipher" type="string"></attr>
		<attr name="mode" type="string"></attr>
	</tag>
	<tag name="mcrypt_get_key_size" type="function" returnType="int">
		<attr name="cipher" type="int"></attr>
	</tag>
	<tag name="mcrypt_list_algorithms" type="function" returnType="array">
		<attr name="lib_dir" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_list_modes" type="function" returnType="array">
		<attr name="lib_dir" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_module_close" type="function" returnType="bool">
		<attr name="td" type="resource"></attr>
	</tag>
	<tag name="mcrypt_module_get_algo_block_size" type="function" returnType="int">
		<attr name="algorithm" type="string"></attr>
		<attr name="lib_dir" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_module_get_algo_key_size" type="function" returnType="int">
		<attr name="algorithm" type="string"></attr>
		<attr name="lib_dir" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_module_get_supported_key_sizes" type="function" returnType="array">
		<attr name="algorithm" type="string"></attr>
		<attr name="lib_dir" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_module_is_block_algorithm_mode" type="function" returnType="bool">
		<attr name="mode" type="string"></attr>
		<attr name="lib_dir" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_module_is_block_algorithm" type="function" returnType="bool">
		<attr name="algorithm" type="string"></attr>
		<attr name="lib_dir" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_module_is_block_mode" type="function" returnType="bool">
		<attr name="mode" type="string"></attr>
		<attr name="lib_dir" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_module_open" type="function" returnType="resource">
		<attr name="algorithm" type="string"></attr>
		<attr name="algorithm_directory" type="string"></attr>
		<attr name="mode" type="string"></attr>
		<attr name="mode_directory" type="string"></attr>
	</tag>
	<tag name="mcrypt_module_self_test" type="function" returnType="bool">
		<attr name="algorithm" type="string"></attr>
		<attr name="lib_dir" type="string" status="optional"></attr>
	</tag>
	<tag name="mcrypt_ofb" type="function" returnType="string">
		<attr name="cipher" type="int"></attr>
		<attr name="key" type="string"></attr>
		<attr name="data" type="string"></attr>
		<attr name="mode" type="int"></attr>
		<attr name="iv" type="string"></attr>
	</tag>
	<tag name="mdecrypt_generic" type="function" returnType="string">
		<attr name="td" type="resource"></attr>
		<attr name="data" type="string"></attr>
	</tag>
</tags>