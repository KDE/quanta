<!DOCTYPE tags>
<tags>
	<tag name="xdiff_file_diff_binary" type="function" returnType="bool">
		<attr name="file1" type="string"></attr>
		<attr name="file2" type="string"></attr>
		<attr name="dest" type="string"></attr>
	</tag>
	<tag name="xdiff_file_diff" type="function" returnType="bool">
		<attr name="file1" type="string"></attr>
		<attr name="file2" type="string"></attr>
		<attr name="dest" type="string"></attr>
		<attr name="context" type="int" status="optional"></attr>
		<attr name="minimal" type="bool" status="optional"></attr>
	</tag>
	<tag name="xdiff_file_merge3" type="function" returnType="mixed">
		<attr name="file1" type="string"></attr>
		<attr name="file2" type="string"></attr>
		<attr name="file3" type="string"></attr>
		<attr name="dest" type="string"></attr>
	</tag>
	<tag name="xdiff_file_patch_binary" type="function" returnType="bool">
		<attr name="file" type="string"></attr>
		<attr name="patch" type="string"></attr>
		<attr name="dest" type="string"></attr>
	</tag>
	<tag name="xdiff_file_patch" type="function" returnType="mixed">
		<attr name="file" type="string"></attr>
		<attr name="patch" type="string"></attr>
		<attr name="dest" type="string"></attr>
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
	<tag name="xdiff_string_diff_binary" type="function" returnType="mixed">
		<attr name="str1" type="string"></attr>
		<attr name="str2" type="string"></attr>
	</tag>
	<tag name="xdiff_string_diff" type="function" returnType="mixed">
		<attr name="str1" type="string"></attr>
		<attr name="str2" type="string"></attr>
		<attr name="context" type="int" status="optional"></attr>
		<attr name="minimal" type="bool" status="optional"></attr>
	</tag>
	<tag name="xdiff_string_merge3" type="function" returnType="string">
		<attr name="str1" type="string"></attr>
		<attr name="str2" type="string"></attr>
		<attr name="str3" type="string"></attr>
		<attr name="&amp;error" type="string" status="optional"></attr>
	</tag>
	<tag name="xdiff_string_patch_binary" type="function" returnType="string">
		<attr name="str" type="string"></attr>
		<attr name="patch" type="string"></attr>
	</tag>
	<tag name="xdiff_string_patch" type="function" returnType="string">
		<attr name="str" type="string"></attr>
		<attr name="patch" type="string"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="&amp;error" type="string" status="optional"></attr>
	</tag>
</tags>