<!DOCTYPE tags>
<tags>
	<tag name="id3_get_genre_id" type="function" returnType="int">
		<attr name="genre" type="string"></attr>
	</tag>
	<tag name="id3_get_genre_list" type="function" returnType="array">
	</tag>
	<tag name="id3_get_genre_name" type="function" returnType="string">
		<attr name="genre_id" type="int"></attr>
	</tag>
	<tag name="id3_get_tag" type="function" returnType="array">
		<attr name="filename" type="string"></attr>
		<attr name="version" type="int" status="optional"></attr>
	</tag>
	<tag name="id3_get_version" type="function" returnType="int">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="id3_remove_tag" type="function" returnType="bool">
		<attr name="filename" type="string"></attr>
		<attr name="version" type="int" status="optional"></attr>
	</tag>
	<tag name="id3_set_tag" type="function" returnType="bool">
		<attr name="filename" type="string"></attr>
		<attr name="tag" type="array"></attr>
		<attr name="version" type="int" status="optional"></attr>
	</tag>
</tags>