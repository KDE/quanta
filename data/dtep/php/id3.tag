<!DOCTYPE tags>
<tags>
	<tag name="id3_get_genre_id" type="function" returnType="int" langVersion="">
		<attr name="genre" type="string" status="required"></attr>
	</tag>
	<tag name="id3_get_genre_list" type="function" returnType="array" langVersion="">
	</tag>
	<tag name="id3_get_genre_name" type="function" returnType="string" langVersion="">
		<attr name="genre_id" type="int" status="required"></attr>
	</tag>
	<tag name="id3_get_tag" type="function" returnType="array" langVersion="">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="version" type="int" status="optional"></attr>
	</tag>
	<tag name="id3_get_version" type="function" returnType="int" langVersion="">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="id3_remove_tag" type="function" returnType="bool" langVersion="">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="version" type="int" status="optional"></attr>
	</tag>
	<tag name="id3_set_tag" type="function" returnType="bool" langVersion="">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="tag" type="array" status="required"></attr>
		<attr name="version" type="int" status="optional"></attr>
	</tag>
</tags>