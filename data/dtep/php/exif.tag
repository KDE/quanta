<!DOCTYPE tags>
<tags>
	<tag name="exif_imagetype" type="function" returnType="int">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="exif_read_data" type="function" returnType="array">
		<attr name="filename" type="string"></attr>
		<attr name="sections" type="string" status="optional"></attr>
		<attr name="arrays" type="bool" status="optional"></attr>
		<attr name="thumbnail" type="bool" status="optional"></attr>
	</tag>
	<tag name="exif_tagname" type="function" returnType="string">
		<attr name="index" type="string"></attr>
	</tag>
	<tag name="exif_thumbnail" type="function" returnType="string">
		<attr name="filename" type="string"></attr>
		<attr name="" type="int" status="optional"></attr>
		<attr name="" type="int" status="optional"></attr>
		<attr name="" type="int" status="optional"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
</tags>