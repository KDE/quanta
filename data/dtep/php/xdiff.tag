<!DOCTYPE tags>
<tags>
	<tag name="xdiff_file_diff_binary" type="function" returnType="bool" version="">
		<attr name="file1" type="string" status="required"></attr>
		<attr name="file2" type="string" status="required"></attr>
		<attr name="dest" type="string" status="required"></attr>
	</tag>
	<tag name="xdiff_file_diff" type="function" returnType="bool" version="">
		<attr name="file1" type="string" status="required"></attr>
		<attr name="file2" type="string" status="required"></attr>
		<attr name="dest" type="string" status="required"></attr>
		<attr name="context" type="int" status="optional"></attr>
		<attr name="minimal" type="bool" status="optional"></attr>
	</tag>
	<tag name="xdiff_file_merge3" type="function" returnType="mixed" version="">
		<attr name="file1" type="string" status="required"></attr>
		<attr name="file2" type="string" status="required"></attr>
		<attr name="file3" type="string" status="required"></attr>
		<attr name="dest" type="string" status="required"></attr>
	</tag>
	<tag name="xdiff_file_patch_binary" type="function" returnType="bool" version="">
		<attr name="file" type="string" status="required"></attr>
		<attr name="patch" type="string" status="required"></attr>
		<attr name="dest" type="string" status="required"></attr>
	</tag>
	<tag name="xdiff_file_patch" type="function" returnType="mixed" version="">
		<attr name="file" type="string" status="required"></attr>
		<attr name="patch" type="string" status="required"></attr>
		<attr name="dest" type="string" status="required"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="xdiff_string_diff_binary" type="function" returnType="mixed" version="">
		<attr name="str1" type="string" status="required"></attr>
		<attr name="str2" type="string" status="required"></attr>
	</tag>
	<tag name="xdiff_string_diff" type="function" returnType="mixed" version="">
		<attr name="str1" type="string" status="required"></attr>
		<attr name="str2" type="string" status="required"></attr>
		<attr name="context" type="int" status="optional"></attr>
		<attr name="minimal" type="bool" status="optional"></attr>
	</tag>
	<tag name="xdiff_string_merge3" type="function" returnType="string" version="">
		<attr name="str1" type="string" status="required"></attr>
		<attr name="str2" type="string" status="required"></attr>
		<attr name="str3" type="string" status="required"></attr>
		<attr name="&amp;error" type="string" status="optional"></attr>
	</tag>
	<tag name="xdiff_string_patch_binary" type="function" returnType="string" version="">
		<attr name="str" type="string" status="required"></attr>
		<attr name="patch" type="string" status="required"></attr>
	</tag>
	<tag name="xdiff_string_patch" type="function" returnType="string" version="">
		<attr name="str" type="string" status="required"></attr>
		<attr name="patch" type="string" status="required"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="&amp;error" type="string" status="optional"></attr>
	</tag>
</tags>