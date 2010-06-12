<!DOCTYPE tags>
<tags>	<tag name="array_change_key_case" type="function" returnType="array">
		<attr name="input" type="array" status="optional"></attr>
		<attr name="case" type="int" status="optional"></attr>
	</tag>
	<tag name="array_chunk" type="function" returnType="array">
		<attr name="input" type="array" status="optional"></attr>
		<attr name="size" type="int" status="optional"></attr>
		<attr name="preserve_keys" type="bool" status="optional"></attr>
	</tag>
	<tag name="array_count_values" type="function" returnType="array">
		<attr name="input" type="array" status="optional"></attr>
	</tag>
	<tag name="array_diff" type="function" returnType="array">
		<attr name="array1" type="array" status="optional"></attr>
		<attr name="array2" type="array" status="optional"></attr>
		<attr name="arrayN" type="array" status="optional"></attr>
	</tag>
	<tag name="array_fill" type="function" returnType="array">
		<attr name="start_index" type="int" status="optional"></attr>
		<attr name="num" type="int" status="optional"></attr>
		<attr name="value" type="mixed" status="optional"></attr>
	</tag>
	<tag name="array_filter" type="function" returnType="array">
		<attr name="input" type="array" status="optional"></attr>
		<attr name="callback" type="mixed" status="optional"></attr>
	</tag>
	<tag name="array_flip" type="function" returnType="array">
		<attr name="trans" type="array" status="optional"></attr>
	</tag>
	<tag name="array_intersect" type="function" returnType="array">
		<attr name="array1" type="array" status="optional"></attr>
		<attr name="array2" type="array" status="optional"></attr>
		<attr name="arrayN" type="array" status="optional"></attr>
	</tag>
	<tag name="array_key_exists" type="function" returnType="bool">
		<attr name="key" type="mixed" status="optional"></attr>
		<attr name="search" type="array" status="optional"></attr>
	</tag>
	<tag name="array_keys" type="function" returnType="array">
		<attr name="input" type="array" status="optional"></attr>
		<attr name="search_value" type="mixed" status="optional"></attr>
	</tag>
	<tag name="array_map" type="function" returnType="array">
		<attr name="callback" type="mixed" status="optional"></attr>
		<attr name="arr1" type="array" status="optional"></attr>
		<attr name="arrayN" type="array" status="optional"></attr>
	</tag>
	<tag name="array_merge_recursive" type="function" returnType="array">
		<attr name="array1" type="array" status="optional"></attr>
		<attr name="array2" type="array" status="optional"></attr>
		<attr name="arrayN" type="array" status="optional"></attr>
	</tag>
	<tag name="array_merge" type="function" returnType="array">
		<attr name="array1" type="array" status="optional"></attr>
		<attr name="array2" type="array" status="optional"></attr>
		<attr name="arrayN" type="array" status="optional"></attr>
	</tag>
	<tag name="array_multisort" type="function" returnType="bool">
		<attr name="ar1" type="array" status="optional"></attr>
		<attr name="arg" type="mixed" status="optional"></attr>
		<attr name="mixedN" type="mixed" status="optional"></attr>
		<attr name="arrayN" type="array" status="optional"></attr>
	</tag>
	<tag name="array_pad" type="function" returnType="array">
		<attr name="input" type="array" status="optional"></attr>
		<attr name="pad_size" type="int" status="optional"></attr>
		<attr name="pad_value" type="mixed" status="optional"></attr>
	</tag>
	<tag name="array_pop" type="function" returnType="mixed">
		<attr name="array" type="array" status="optional"></attr>
	</tag>
	<tag name="array_push" type="function" returnType="int">
		<attr name="array" type="array" status="optional"></attr>
		<attr name="var" type="mixed" status="optional"></attr>
		<attr name="mixedN" type="mixed" status="optional"></attr>
	</tag>
	<tag name="array_rand" type="function" returnType="mixed">
		<attr name="input" type="array" status="optional"></attr>
		<attr name="num_req" type="int" status="optional"></attr>
	</tag>
	<tag name="array_reduce" type="function" returnType="mixed">
		<attr name="input" type="array" status="optional"></attr>
		<attr name="callback" type="mixed" status="optional"></attr>
		<attr name="initial" type="int" status="optional"></attr>
	</tag>
	<tag name="array_reverse" type="function" returnType="array">
		<attr name="array" type="array" status="optional"></attr>
		<attr name="preserve_keys" type="bool" status="optional"></attr>
	</tag>
	<tag name="array_search" type="function" returnType="mixed">
		<attr name="needle" type="mixed" status="optional"></attr>
		<attr name="haystack" type="array" status="optional"></attr>
		<attr name="strict" type="bool" status="optional"></attr>
	</tag>
	<tag name="array_shift" type="function" returnType="mixed">
		<attr name="array" type="array" status="optional"></attr>
	</tag>
	<tag name="array_slice" type="function" returnType="array">
		<attr name="array" type="array" status="optional"></attr>
		<attr name="offset" type="int" status="optional"></attr>
		<attr name="length" type="int" status="optional"></attr>
	</tag>
	<tag name="array_splice" type="function" returnType="array">
		<attr name="input" type="array" status="optional"></attr>
		<attr name="offset" type="int" status="optional"></attr>
		<attr name="length" type="int" status="optional"></attr>
		<attr name="replacement" type="array" status="optional"></attr>
	</tag>
	<tag name="array_sum" type="function" returnType="mixed">
		<attr name="array" type="array" status="optional"></attr>
	</tag>
	<tag name="array_unique" type="function" returnType="array">
		<attr name="array" type="array" status="optional"></attr>
	</tag>
	<tag name="array_unshift" type="function" returnType="int">
		<attr name="array" type="array" status="optional"></attr>
		<attr name="var" type="mixed" status="optional"></attr>
		<attr name="mixedN" type="mixed" status="optional"></attr>
	</tag>
	<tag name="array_values" type="function" returnType="array">
		<attr name="input" type="array" status="optional"></attr>
	</tag>
	<tag name="array_walk" type="function" returnType="int">
		<attr name="array" type="array" status="optional"></attr>
		<attr name="func" type="string" status="optional"></attr>
		<attr name="userdata" type="mixed" status="optional"></attr>
	</tag>
	<tag name="array" type="function" returnType="array">
		<attr name="mixedN" type="mixed" status="optional"></attr>
	</tag>
	<tag name="arsort" type="function" returnType="void">
		<attr name="array" type="array" status="optional"></attr>
		<attr name="sort_flags" type="int" status="optional"></attr>
	</tag>
	<tag name="asort" type="function" returnType="void">
		<attr name="array" type="array" status="optional"></attr>
		<attr name="sort_flags" type="int" status="optional"></attr>
	</tag>
	<tag name="compact" type="function" returnType="array">
		<attr name="varname" type="mixed" status="optional"></attr>
		<attr name="mixedN" type="mixed" status="optional"></attr>
	</tag>
	<tag name="count" type="function" returnType="int">
		<attr name="var" type="mixed" status="optional"></attr>
	</tag>
	<tag name="current" type="function" returnType="mixed">
		<attr name="array" type="array" status="optional"></attr>
	</tag>
	<tag name="each" type="function" returnType="array">
		<attr name="array" type="array" status="optional"></attr>
	</tag>
	<tag name="end" type="function" returnType="mixed">
		<attr name="array" type="array" status="optional"></attr>
	</tag>
	<tag name="extract" type="function" returnType="int">
		<attr name="var_array" type="array" status="optional"></attr>
		<attr name="extract_type" type="int" status="optional"></attr>
		<attr name="prefix" type="string" status="optional"></attr>
	</tag>
	<tag name="in_array" type="function" returnType="bool">
		<attr name="needle" type="mixed" status="optional"></attr>
		<attr name="haystack" type="array" status="optional"></attr>
		<attr name="strict" type="bool" status="optional"></attr>
	</tag>
	<tag name="key" type="function" returnType="mixed">
		<attr name="array" type="array" status="optional"></attr>
	</tag>
	<tag name="krsort" type="function" returnType="int">
		<attr name="array" type="array" status="optional"></attr>
		<attr name="sort_flags" type="int" status="optional"></attr>
	</tag>
	<tag name="ksort" type="function" returnType="int">
		<attr name="array" type="array" status="optional"></attr>
		<attr name="sort_flags" type="int" status="optional"></attr>
	</tag>
	<tag name="list" type="function" returnType="void">
		<attr name="mixedN" type="mixed" status="optional"></attr>
	</tag>
	<tag name="natcasesort" type="function" returnType="void">
		<attr name="array" type="array" status="optional"></attr>
	</tag>
	<tag name="natsort" type="function" returnType="void">
		<attr name="array" type="array" status="optional"></attr>
	</tag>
	<tag name="next" type="function" returnType="mixed">
		<attr name="array" type="array" status="optional"></attr>
	</tag>
	<tag name="pos" type="function" returnType="mixed">
		<attr name="array" type="array" status="optional"></attr>
	</tag>
	<tag name="prev" type="function" returnType="mixed">
		<attr name="array" type="array" status="optional"></attr>
	</tag>
	<tag name="range" type="function" returnType="array">
		<attr name="low" type="mixed" status="optional"></attr>
		<attr name="high" type="mixed" status="optional"></attr>
	</tag>
	<tag name="reset" type="function" returnType="mixed">
		<attr name="array" type="array" status="optional"></attr>
	</tag>
	<tag name="rsort" type="function" returnType="void">
		<attr name="array" type="array" status="optional"></attr>
		<attr name="sort_flags" type="int" status="optional"></attr>
	</tag>
	<tag name="shuffle" type="function" returnType="void">
		<attr name="array" type="array" status="optional"></attr>
	</tag>
	<tag name="sizeof" type="function" returnType="int">
		<attr name="var" type="mixed" status="optional"></attr>
	</tag>
	<tag name="sort" type="function" returnType="void">
		<attr name="array" type="array" status="optional"></attr>
		<attr name="sort_flags" type="int" status="optional"></attr>
	</tag>
	<tag name="uasort" type="function" returnType="void">
		<attr name="array" type="array" status="optional"></attr>
		<attr name="cmp_function" type="function" status="optional"></attr>
	</tag>
	<tag name="uksort" type="function" returnType="void">
		<attr name="array" type="array" status="optional"></attr>
		<attr name="cmp_function" type="function" status="optional"></attr>
	</tag>
	<tag name="usort" type="function" returnType="void">
		<attr name="array" type="array" status="optional"></attr>
		<attr name="cmp_function" type="string" status="optional"></attr>
	</tag>
</tags>
