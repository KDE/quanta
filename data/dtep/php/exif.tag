<!DOCTYPE tags>
<tags>
	<tag name="exif_imagetype" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="exif_read_data" type="function" returnType="array" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="sections" type="string" status="optional"></attr>
		<attr name="arrays" type="bool" status="optional"></attr>
		<attr name="thumbnail" type="bool" status="optional"></attr>
	</tag>
	<tag name="exif_tagname" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="index" type="string" status="required"></attr>
	</tag>
	<tag name="exif_thumbnail" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="width" type="int" status="optional"></attr>
		<attr name="height" type="int" status="optional"></attr>
		<attr name="imagetype" type="int" status="optional"></attr>
	</tag>
</tags>