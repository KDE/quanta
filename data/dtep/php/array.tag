<!DOCTYPE tags>
<tags>
	<tag name="array_change_key_case" type="function" returnType="array" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="input" type="array" status="required"></attr>
		<attr name="case" type="int" status="optional"></attr>
	</tag>
	<tag name="array_chunk" type="function" returnType="array" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="input" type="array" status="required"></attr>
		<attr name="size" type="int" status="required"></attr>
		<attr name="preserve_keys" type="bool" status="optional"></attr>
	</tag>
	<tag name="array_combine" type="function" returnType="array" version="PHP 5">
		<attr name="keys" type="array" status="required"></attr>
		<attr name="values" type="array" status="required"></attr>
	</tag>
	<tag name="array_count_values" type="function" returnType="array" version="PHP 4 , PHP 5">
		<attr name="input" type="array" status="required"></attr>
	</tag>
	<tag name="array_diff_assoc" type="function" returnType="array" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="array1" type="array" status="required"></attr>
		<attr name="array2" type="array" status="required"></attr>
		<attr name=" ..." type="array" status="optional"></attr>
	</tag>
	<tag name="array_diff_key" type="function" returnType="array" version="">
		<attr name="array1" type="array" status="required"></attr>
		<attr name="array2" type="array" status="required"></attr>
		<attr name=" ..." type="array" status="optional"></attr>
	</tag>
	<tag name="array_diff_uassoc" type="function" returnType="array" version="PHP 5">
		<attr name="array1" type="array" status="required"></attr>
		<attr name="array2" type="array" status="required"></attr>
		<attr name="..." type="array" status="optional"></attr>
		<attr name="key_compare_func" type="callback" status="required"></attr>
	</tag>
	<tag name="array_diff_ukey" type="function" returnType="array" version="">
		<attr name="array1" type="array" status="required"></attr>
		<attr name="array2" type="array" status="required"></attr>
		<attr name=" ..." type="array" status="optional"></attr>
		<attr name="key_compare_func" type="callback" status="required"></attr>
	</tag>
	<tag name="array_diff" type="function" returnType="array" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="array1" type="array" status="required"></attr>
		<attr name="array2" type="array" status="required"></attr>
		<attr name=" ..." type="array" status="optional"></attr>
	</tag>
	<tag name="array_fill" type="function" returnType="array" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="start_index" type="int" status="required"></attr>
		<attr name="num" type="int" status="required"></attr>
		<attr name="value" type="mixed" status="required"></attr>
	</tag>
	<tag name="array_filter" type="function" returnType="array" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="input" type="array" status="required"></attr>
		<attr name="callback" type="callback" status="optional"></attr>
	</tag>
	<tag name="array_flip" type="function" returnType="array" version="PHP 4 , PHP 5">
		<attr name="trans" type="array" status="required"></attr>
	</tag>
	<tag name="array_intersect_assoc" type="function" returnType="array" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="array1" type="array" status="required"></attr>
		<attr name="array2" type="array" status="required"></attr>
		<attr name=" ..." type="array" status="optional"></attr>
	</tag>
	<tag name="array_intersect_key" type="function" returnType="array" version="">
		<attr name="array1" type="array" status="required"></attr>
		<attr name="array2" type="array" status="required"></attr>
		<attr name=" ..." type="array" status="optional"></attr>
	</tag>
	<tag name="array_intersect_uassoc" type="function" returnType="array" version="PHP 5">
		<attr name="array1" type="array" status="required"></attr>
		<attr name="array2" type="array" status="required"></attr>
		<attr name=" ..." type="array" status="optional"></attr>
		<attr name="key_compare_func" type="callback" status="required"></attr>
	</tag>
	<tag name="array_intersect_ukey" type="function" returnType="array" version="">
		<attr name="array1" type="array" status="required"></attr>
		<attr name="array2" type="array" status="required"></attr>
		<attr name="..." type="array" status="optional"></attr>
		<attr name="key_compare_func" type="callback" status="required"></attr>
	</tag>
	<tag name="array_intersect" type="function" returnType="array" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="array1" type="array" status="required"></attr>
		<attr name="array2" type="array" status="required"></attr>
		<attr name=" ..." type="array" status="optional"></attr>
	</tag>
	<tag name="array_key_exists" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="key" type="mixed" status="required"></attr>
		<attr name="search" type="array" status="required"></attr>
	</tag>
	<tag name="array_keys" type="function" returnType="array" version="PHP 4 , PHP 5">
		<attr name="input" type="array" status="required"></attr>
		<attr name="search_value" type="mixed" status="optional"></attr>
		<attr name="strict" type="bool" status="optional"></attr>
	</tag>
	<tag name="array_map" type="function" returnType="array" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="callback" type="callback" status="required"></attr>
		<attr name="arr1" type="array" status="required"></attr>
		<attr name="..." type="array" status="optional"></attr>
	</tag>
	<tag name="array_merge_recursive" type="function" returnType="array" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="array1" type="array" status="required"></attr>
		<attr name="array2" type="array" status="required"></attr>
		<attr name="..." type="array" status="optional"></attr>
	</tag>
	<tag name="array_merge" type="function" returnType="array" version="PHP 4 , PHP 5">
		<attr name="array1" type="array" status="required"></attr>
		<attr name="array2" type="array" status="optional"></attr>
		<attr name="..." type="array" status="optional"></attr>
	</tag>
	<tag name="array_multisort" type="function" returnType="bool" version="PHP 4 , PHP 5">
		<attr name="ar1" type="array" status="required"></attr>
		<attr name="arg" type="mixed" status="optional"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
		<attr name="..." type="array" status="optional"></attr>
	</tag>
	<tag name="array_pad" type="function" returnType="array" version="PHP 4 , PHP 5">
		<attr name="input" type="array" status="required"></attr>
		<attr name="pad_size" type="int" status="required"></attr>
		<attr name="pad_value" type="mixed" status="required"></attr>
	</tag>
	<tag name="array_pop" type="function" returnType="mixed" version="PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
	</tag>
	<tag name="array_push" type="function" returnType="int" version="PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
		<attr name="var" type="mixed" status="required"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="array_rand" type="function" returnType="mixed" version="PHP 4 , PHP 5">
		<attr name="input" type="array" status="required"></attr>
		<attr name="num_req" type="int" status="optional"></attr>
	</tag>
	<tag name="array_reduce" type="function" returnType="mixed" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="input" type="array" status="required"></attr>
		<attr name="function" type="callback" status="required"></attr>
		<attr name="initial" type="int" status="optional"></attr>
	</tag>
	<tag name="array_reverse" type="function" returnType="array" version="PHP 4 , PHP 5">
		<attr name="array" type="array" status="required"></attr>
		<attr name="preserve_keys" type="bool" status="optional"></attr>
	</tag>
	<tag name="array_search" type="function" returnType="mixed" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="needle" type="mixed" status="required"></attr>
		<attr name="haystack" type="array" status="required"></attr>
		<attr name="strict" type="bool" status="optional"></attr>
	</tag>
	<tag name="array_shift" type="function" returnType="mixed" version="PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
	</tag>
	<tag name="array_slice" type="function" returnType="array" version="PHP 4 , PHP 5">
		<attr name="array" type="array" status="required"></attr>
		<attr name="offset" type="int" status="required"></attr>
		<attr name="length" type="int" status="optional"></attr>
		<attr name="preserve_keys" type="bool" status="optional"></attr>
	</tag>
	<tag name="array_splice" type="function" returnType="array" version="PHP 4 , PHP 5">
		<attr name="&amp;input" type="array" status="required"></attr>
		<attr name="offset" type="int" status="required"></attr>
		<attr name="length" type="int" status="optional"></attr>
		<attr name="replacement" type="array" status="optional"></attr>
	</tag>
	<tag name="array_sum" type="function" returnType="number" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="array" type="array" status="required"></attr>
	</tag>
	<tag name="array_udiff_assoc" type="function" returnType="array" version="PHP 5">
		<attr name="array1" type="array" status="required"></attr>
		<attr name="array2" type="array" status="required"></attr>
		<attr name=" ..." type="array" status="optional"></attr>
		<attr name="data_compare_func" type="callback" status="required"></attr>
	</tag>
	<tag name="array_udiff_uassoc" type="function" returnType="array" version="PHP 5">
		<attr name="array1" type="array" status="required"></attr>
		<attr name="array2" type="array" status="required"></attr>
		<attr name=" ..." type="array" status="optional"></attr>
		<attr name="data_compare_func" type="callback" status="required"></attr>
		<attr name="key_compare_func" type="callback" status="required"></attr>
	</tag>
	<tag name="array_udiff" type="function" returnType="array" version="PHP 5">
		<attr name="array1" type="array" status="required"></attr>
		<attr name="array2" type="array" status="required"></attr>
		<attr name=" ..." type="array" status="optional"></attr>
		<attr name="data_compare_func" type="callback" status="required"></attr>
	</tag>
	<tag name="array_uintersect_assoc" type="function" returnType="array" version="PHP 5">
		<attr name="array1" type="array" status="required"></attr>
		<attr name="array2" type="array" status="required"></attr>
		<attr name=" ..." type="array" status="optional"></attr>
		<attr name="data_compare_func" type="callback" status="required"></attr>
	</tag>
	<tag name="array_uintersect_uassoc" type="function" returnType="array" version="PHP 5">
		<attr name="array1" type="array" status="required"></attr>
		<attr name="array2" type="array" status="required"></attr>
		<attr name=" ..." type="array" status="optional"></attr>
		<attr name="data_compare_func" type="callback" status="required"></attr>
		<attr name="key_compare_func" type="callback" status="required"></attr>
	</tag>
	<tag name="array_uintersect" type="function" returnType="array" version="PHP 5">
		<attr name="array1" type="array" status="required"></attr>
		<attr name="array2" type="array" status="required"></attr>
		<attr name=" ..." type="array" status="optional"></attr>
		<attr name="data_compare_func" type="callback" status="required"></attr>
	</tag>
	<tag name="array_unique" type="function" returnType="array" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="array" type="array" status="required"></attr>
	</tag>
	<tag name="array_unshift" type="function" returnType="int" version="PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
		<attr name="var" type="mixed" status="required"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="array_values" type="function" returnType="array" version="PHP 4 , PHP 5">
		<attr name="input" type="array" status="required"></attr>
	</tag>
	<tag name="array_walk_recursive" type="function" returnType="bool" version="PHP 5">
		<attr name="&amp;input" type="array" status="required"></attr>
		<attr name="funcname" type="callback" status="required"></attr>
		<attr name="userdata" type="mixed" status="optional"></attr>
	</tag>
	<tag name="array_walk" type="function" returnType="bool" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
		<attr name="funcname" type="callback" status="required"></attr>
		<attr name="userdata" type="mixed" status="optional"></attr>
	</tag>
	<tag name="array" type="function" returnType="array" version="">
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="arsort" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
		<attr name="sort_flags" type="int" status="optional"></attr>
	</tag>
	<tag name="asort" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
		<attr name="sort_flags" type="int" status="optional"></attr>
	</tag>
	<tag name="compact" type="function" returnType="array" version="PHP 4 , PHP 5">
		<attr name="varname" type="mixed" status="required"></attr>
		<attr name="..." type="mixed" status="optional"></attr>
	</tag>
	<tag name="count" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="var" type="mixed" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="current" type="function" returnType="mixed" version="PHP 3, PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
	</tag>
	<tag name="each" type="function" returnType="array" version="PHP 3, PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
	</tag>
	<tag name="end" type="function" returnType="mixed" version="PHP 3, PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
	</tag>
	<tag name="extract" type="function" returnType="int" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="var_array" type="array" status="required"></attr>
		<attr name="extract_type" type="int" status="optional"></attr>
		<attr name="prefix" type="string" status="optional"></attr>
	</tag>
	<tag name="in_array" type="function" returnType="bool" version="PHP 4 , PHP 5">
		<attr name="needle" type="mixed" status="required"></attr>
		<attr name="haystack" type="array" status="required"></attr>
		<attr name="strict" type="bool" status="optional"></attr>
	</tag>
	<tag name="key" type="function" returnType="mixed" version="PHP 3, PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
	</tag>
	<tag name="krsort" type="function" returnType="bool" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
		<attr name="sort_flags" type="int" status="optional"></attr>
	</tag>
	<tag name="ksort" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
		<attr name="sort_flags" type="int" status="optional"></attr>
	</tag>
	<tag name="list" type="function" returnType="void" version="">
		<attr name="varname" type="mixed" status="required"></attr>
		<attr name="..." type="mixed" status="required"></attr>
	</tag>
	<tag name="natcasesort" type="function" returnType="void" version="PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
	</tag>
	<tag name="natsort" type="function" returnType="void" version="PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
	</tag>
	<tag name="next" type="function" returnType="mixed" version="PHP 3, PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
	</tag>
	<tag name="prev" type="function" returnType="mixed" version="PHP 3, PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
	</tag>
	<tag name="range" type="function" returnType="array" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="low" type="number" status="required"></attr>
		<attr name="high" type="number" status="required"></attr>
		<attr name="step" type="number" status="optional"></attr>
	</tag>
	<tag name="reset" type="function" returnType="mixed" version="PHP 3, PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
	</tag>
	<tag name="rsort" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
		<attr name="sort_flags" type="int" status="optional"></attr>
	</tag>
	<tag name="shuffle" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
	</tag>
	<tag name="sort" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
		<attr name="sort_flags" type="int" status="optional"></attr>
	</tag>
	<tag name="uasort" type="function" returnType="bool" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
		<attr name="cmp_function" type="callback" status="required"></attr>
	</tag>
	<tag name="uksort" type="function" returnType="bool" version="PHP 3&gt;= 3.0.4, PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
		<attr name="cmp_function" type="callback" status="required"></attr>
	</tag>
	<tag name="usort" type="function" returnType="bool" version="PHP 3&gt;= 3.0.3, PHP 4 , PHP 5">
		<attr name="&amp;array" type="array" status="required"></attr>
		<attr name="cmp_function" type="callback" status="required"></attr>
	</tag>
</tags>