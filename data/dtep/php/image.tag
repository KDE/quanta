<!DOCTYPE tags>
<tags>
	<tag name="gd_info" type="function" returnType="array" version="PHP 4 &gt;= 4.3.0, PHP 5">
	</tag>
	<tag name="getimagesize" type="function" returnType="array" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="&amp;imageinfo" type="array" status="optional"></attr>
	</tag>
	<tag name="image_type_to_extension" type="function" returnType="string" version="">
		<attr name="imagetype" type="int" status="required"></attr>
		<attr name="include_dot" type="bool" status="optional"></attr>
	</tag>
	<tag name="image_type_to_mime_type" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="imagetype" type="int" status="required"></attr>
	</tag>
	<tag name="image2wbmp" type="function" returnType="int" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="threshold" type="int" status="optional"></attr>
	</tag>
	<tag name="imagealphablending" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="blendmode" type="bool" status="required"></attr>
	</tag>
	<tag name="imageantialias" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.2, PHP 5">
		<attr name="im" type="resource" status="required"></attr>
		<attr name="on" type="bool" status="required"></attr>
	</tag>
	<tag name="imagearc" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="cx" type="int" status="required"></attr>
		<attr name="cy" type="int" status="required"></attr>
		<attr name="w" type="int" status="required"></attr>
		<attr name="h" type="int" status="required"></attr>
		<attr name="s" type="int" status="required"></attr>
		<attr name="e" type="int" status="required"></attr>
		<attr name="color" type="int" status="required"></attr>
	</tag>
	<tag name="imagechar" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="font" type="int" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
		<attr name="y" type="int" status="required"></attr>
		<attr name="c" type="string" status="required"></attr>
		<attr name="color" type="int" status="required"></attr>
	</tag>
	<tag name="imagecharup" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="font" type="int" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
		<attr name="y" type="int" status="required"></attr>
		<attr name="c" type="string" status="required"></attr>
		<attr name="color" type="int" status="required"></attr>
	</tag>
	<tag name="imagecolorallocate" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="red" type="int" status="required"></attr>
		<attr name="green" type="int" status="required"></attr>
		<attr name="blue" type="int" status="required"></attr>
	</tag>
	<tag name="imagecolorallocatealpha" type="function" returnType="int" version="PHP 4 &gt;= 4.3.2, PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="red" type="int" status="required"></attr>
		<attr name="green" type="int" status="required"></attr>
		<attr name="blue" type="int" status="required"></attr>
		<attr name="alpha" type="int" status="required"></attr>
	</tag>
	<tag name="imagecolorat" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
		<attr name="y" type="int" status="required"></attr>
	</tag>
	<tag name="imagecolorclosest" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="red" type="int" status="required"></attr>
		<attr name="green" type="int" status="required"></attr>
		<attr name="blue" type="int" status="required"></attr>
	</tag>
	<tag name="imagecolorclosestalpha" type="function" returnType="int" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="red" type="int" status="required"></attr>
		<attr name="green" type="int" status="required"></attr>
		<attr name="blue" type="int" status="required"></attr>
		<attr name="alpha" type="int" status="required"></attr>
	</tag>
	<tag name="imagecolorclosesthwb" type="function" returnType="int" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="red" type="int" status="required"></attr>
		<attr name="green" type="int" status="required"></attr>
		<attr name="blue" type="int" status="required"></attr>
	</tag>
	<tag name="imagecolordeallocate" type="function" returnType="int" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="color" type="int" status="required"></attr>
	</tag>
	<tag name="imagecolorexact" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="red" type="int" status="required"></attr>
		<attr name="green" type="int" status="required"></attr>
		<attr name="blue" type="int" status="required"></attr>
	</tag>
	<tag name="imagecolorexactalpha" type="function" returnType="int" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="red" type="int" status="required"></attr>
		<attr name="green" type="int" status="required"></attr>
		<attr name="blue" type="int" status="required"></attr>
		<attr name="alpha" type="int" status="required"></attr>
	</tag>
	<tag name="imagecolormatch" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="image1" type="resource" status="required"></attr>
		<attr name="image2" type="resource" status="required"></attr>
	</tag>
	<tag name="imagecolorresolve" type="function" returnType="int" version="PHP 3&gt;= 3.0.2, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="red" type="int" status="required"></attr>
		<attr name="green" type="int" status="required"></attr>
		<attr name="blue" type="int" status="required"></attr>
	</tag>
	<tag name="imagecolorresolvealpha" type="function" returnType="int" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="red" type="int" status="required"></attr>
		<attr name="green" type="int" status="required"></attr>
		<attr name="blue" type="int" status="required"></attr>
		<attr name="alpha" type="int" status="required"></attr>
	</tag>
	<tag name="imagecolorset" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="index" type="int" status="required"></attr>
		<attr name="red" type="int" status="required"></attr>
		<attr name="green" type="int" status="required"></attr>
		<attr name="blue" type="int" status="required"></attr>
	</tag>
	<tag name="imagecolorsforindex" type="function" returnType="array" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="index" type="int" status="required"></attr>
	</tag>
	<tag name="imagecolorstotal" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
	</tag>
	<tag name="imagecolortransparent" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="color" type="int" status="optional"></attr>
	</tag>
	<tag name="imagecopy" type="function" returnType="int" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="dst_im" type="resource" status="required"></attr>
		<attr name="src_im" type="resource" status="required"></attr>
		<attr name="dst_x" type="int" status="required"></attr>
		<attr name="dst_y" type="int" status="required"></attr>
		<attr name="src_x" type="int" status="required"></attr>
		<attr name="src_y" type="int" status="required"></attr>
		<attr name="src_w" type="int" status="required"></attr>
		<attr name="src_h" type="int" status="required"></attr>
	</tag>
	<tag name="imagecopymerge" type="function" returnType="int" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="dst_im" type="resource" status="required"></attr>
		<attr name="src_im" type="resource" status="required"></attr>
		<attr name="dst_x" type="int" status="required"></attr>
		<attr name="dst_y" type="int" status="required"></attr>
		<attr name="src_x" type="int" status="required"></attr>
		<attr name="src_y" type="int" status="required"></attr>
		<attr name="src_w" type="int" status="required"></attr>
		<attr name="src_h" type="int" status="required"></attr>
		<attr name="pct" type="int" status="required"></attr>
	</tag>
	<tag name="imagecopymergegray" type="function" returnType="int" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="dst_im" type="resource" status="required"></attr>
		<attr name="src_im" type="resource" status="required"></attr>
		<attr name="dst_x" type="int" status="required"></attr>
		<attr name="dst_y" type="int" status="required"></attr>
		<attr name="src_x" type="int" status="required"></attr>
		<attr name="src_y" type="int" status="required"></attr>
		<attr name="src_w" type="int" status="required"></attr>
		<attr name="src_h" type="int" status="required"></attr>
		<attr name="pct" type="int" status="required"></attr>
	</tag>
	<tag name="imagecopyresampled" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="dst_image" type="resource" status="required"></attr>
		<attr name="src_image" type="resource" status="required"></attr>
		<attr name="dst_x" type="int" status="required"></attr>
		<attr name="dst_y" type="int" status="required"></attr>
		<attr name="src_x" type="int" status="required"></attr>
		<attr name="src_y" type="int" status="required"></attr>
		<attr name="dst_w" type="int" status="required"></attr>
		<attr name="dst_h" type="int" status="required"></attr>
		<attr name="src_w" type="int" status="required"></attr>
		<attr name="src_h" type="int" status="required"></attr>
	</tag>
	<tag name="imagecopyresized" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="dst_image" type="resource" status="required"></attr>
		<attr name="src_image" type="resource" status="required"></attr>
		<attr name="dst_x" type="int" status="required"></attr>
		<attr name="dst_y" type="int" status="required"></attr>
		<attr name="src_x" type="int" status="required"></attr>
		<attr name="src_y" type="int" status="required"></attr>
		<attr name="dst_w" type="int" status="required"></attr>
		<attr name="dst_h" type="int" status="required"></attr>
		<attr name="src_w" type="int" status="required"></attr>
		<attr name="src_h" type="int" status="required"></attr>
	</tag>
	<tag name="imagecreate" type="function" returnType="resource" version="PHP 3, PHP 4 , PHP 5">
		<attr name="x_size" type="int" status="required"></attr>
		<attr name="y_size" type="int" status="required"></attr>
	</tag>
	<tag name="imagecreatefromgd" type="function" returnType="resource" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="imagecreatefromgd2" type="function" returnType="resource" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="imagecreatefromgd2part" type="function" returnType="resource" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="srcX" type="int" status="required"></attr>
		<attr name="srcY" type="int" status="required"></attr>
		<attr name="width" type="int" status="required"></attr>
		<attr name="height" type="int" status="required"></attr>
	</tag>
	<tag name="imagecreatefromgif" type="function" returnType="resource" version="PHP 3, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="imagecreatefromjpeg" type="function" returnType="resource" version="PHP 3&gt;= 3.0.16, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="imagecreatefrompng" type="function" returnType="resource" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="imagecreatefromstring" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="image" type="string" status="required"></attr>
	</tag>
	<tag name="imagecreatefromwbmp" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="imagecreatefromxbm" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="imagecreatefromxpm" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="imagecreatetruecolor" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="x_size" type="int" status="required"></attr>
		<attr name="y_size" type="int" status="required"></attr>
	</tag>
	<tag name="imagedashedline" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="x1" type="int" status="required"></attr>
		<attr name="y1" type="int" status="required"></attr>
		<attr name="x2" type="int" status="required"></attr>
		<attr name="y2" type="int" status="required"></attr>
		<attr name="color" type="int" status="required"></attr>
	</tag>
	<tag name="imagedestroy" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
	</tag>
	<tag name="imageellipse" type="function" returnType="int" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="cx" type="int" status="required"></attr>
		<attr name="cy" type="int" status="required"></attr>
		<attr name="w" type="int" status="required"></attr>
		<attr name="h" type="int" status="required"></attr>
		<attr name="color" type="int" status="required"></attr>
	</tag>
	<tag name="imagefill" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
		<attr name="y" type="int" status="required"></attr>
		<attr name="color" type="int" status="required"></attr>
	</tag>
	<tag name="imagefilledarc" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="cx" type="int" status="required"></attr>
		<attr name="cy" type="int" status="required"></attr>
		<attr name="w" type="int" status="required"></attr>
		<attr name="h" type="int" status="required"></attr>
		<attr name="s" type="int" status="required"></attr>
		<attr name="e" type="int" status="required"></attr>
		<attr name="color" type="int" status="required"></attr>
		<attr name="style" type="int" status="required"></attr>
	</tag>
	<tag name="imagefilledellipse" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="cx" type="int" status="required"></attr>
		<attr name="cy" type="int" status="required"></attr>
		<attr name="w" type="int" status="required"></attr>
		<attr name="h" type="int" status="required"></attr>
		<attr name="color" type="int" status="required"></attr>
	</tag>
	<tag name="imagefilledpolygon" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="points" type="array" status="required"></attr>
		<attr name="num_points" type="int" status="required"></attr>
		<attr name="color" type="int" status="required"></attr>
	</tag>
	<tag name="imagefilledrectangle" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="x1" type="int" status="required"></attr>
		<attr name="y1" type="int" status="required"></attr>
		<attr name="x2" type="int" status="required"></attr>
		<attr name="y2" type="int" status="required"></attr>
		<attr name="color" type="int" status="required"></attr>
	</tag>
	<tag name="imagefilltoborder" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
		<attr name="y" type="int" status="required"></attr>
		<attr name="border" type="int" status="required"></attr>
		<attr name="color" type="int" status="required"></attr>
	</tag>
	<tag name="imagefilter" type="function" returnType="bool" version="PHP 5">
		<attr name="src_im" type="resource" status="required"></attr>
		<attr name="filtertype" type="int" status="required"></attr>
		<attr name="arg1" type="int" status="optional"></attr>
		<attr name="arg2" type="int" status="optional"></attr>
		<attr name="arg3" type="int" status="optional"></attr>
	</tag>
	<tag name="imagefontheight" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="font" type="int" status="required"></attr>
	</tag>
	<tag name="imagefontwidth" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="font" type="int" status="required"></attr>
	</tag>
	<tag name="imageftbbox" type="function" returnType="array" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="size" type="float" status="required"></attr>
		<attr name="angle" type="float" status="required"></attr>
		<attr name="font_file" type="string" status="required"></attr>
		<attr name="text" type="string" status="required"></attr>
		<attr name="extrainfo" type="array" status="optional"></attr>
	</tag>
	<tag name="imagefttext" type="function" returnType="array" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="size" type="float" status="required"></attr>
		<attr name="angle" type="float" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
		<attr name="y" type="int" status="required"></attr>
		<attr name="col" type="int" status="required"></attr>
		<attr name="font_file" type="string" status="required"></attr>
		<attr name="text" type="string" status="required"></attr>
		<attr name="extrainfo" type="array" status="optional"></attr>
	</tag>
	<tag name="imagegammacorrect" type="function" returnType="int" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="inputgamma" type="float" status="required"></attr>
		<attr name="outputgamma" type="float" status="required"></attr>
	</tag>
	<tag name="imagegd" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="imagegd2" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="chunk_size" type="int" status="optional"></attr>
		<attr name="type" type="int" status="optional"></attr>
	</tag>
	<tag name="imagegif" type="function" returnType="bool" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="imageinterlace" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="interlace" type="int" status="optional"></attr>
	</tag>
	<tag name="imageistruecolor" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.2, PHP 5">
		<attr name="image" type="resource" status="required"></attr>
	</tag>
	<tag name="imagejpeg" type="function" returnType="bool" version="PHP 3&gt;= 3.0.16, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="quality" type="int" status="optional"></attr>
	</tag>
	<tag name="imagelayereffect" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="effect" type="int" status="required"></attr>
	</tag>
	<tag name="imageline" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="x1" type="int" status="required"></attr>
		<attr name="y1" type="int" status="required"></attr>
		<attr name="x2" type="int" status="required"></attr>
		<attr name="y2" type="int" status="required"></attr>
		<attr name="color" type="int" status="required"></attr>
	</tag>
	<tag name="imageloadfont" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="file" type="string" status="required"></attr>
	</tag>
	<tag name="imagepalettecopy" type="function" returnType="int" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="destination" type="resource" status="required"></attr>
		<attr name="source" type="resource" status="required"></attr>
	</tag>
	<tag name="imagepng" type="function" returnType="bool" version="PHP 3&gt;= 3.0.13, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="imagepolygon" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="points" type="array" status="required"></attr>
		<attr name="num_points" type="int" status="required"></attr>
		<attr name="color" type="int" status="required"></attr>
	</tag>
	<tag name="imagepsbbox" type="function" returnType="array" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="text" type="string" status="required"></attr>
		<attr name="font" type="int" status="required"></attr>
		<attr name="size" type="int" status="required"></attr>
		<attr name="space" type="int" status="optional"></attr>
		<attr name="tightness" type="int" status="required"></attr>
		<attr name="angle" type="float" status="required"></attr>
	</tag>
	<tag name="imagepscopyfont" type="function" returnType="int" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="fontindex" type="int" status="required"></attr>
	</tag>
	<tag name="imagepsencodefont" type="function" returnType="int" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="font_index" type="int" status="required"></attr>
		<attr name="encodingfile" type="string" status="required"></attr>
	</tag>
	<tag name="imagepsextendfont" type="function" returnType="bool" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="font_index" type="int" status="required"></attr>
		<attr name="extend" type="float" status="required"></attr>
	</tag>
	<tag name="imagepsfreefont" type="function" returnType="void" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="fontindex" type="int" status="required"></attr>
	</tag>
	<tag name="imagepsloadfont" type="function" returnType="int" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="imagepsslantfont" type="function" returnType="bool" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="font_index" type="int" status="required"></attr>
		<attr name="slant" type="float" status="required"></attr>
	</tag>
	<tag name="imagepstext" type="function" returnType="array" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="text" type="string" status="required"></attr>
		<attr name="font" type="int" status="required"></attr>
		<attr name="size" type="int" status="required"></attr>
		<attr name="foreground" type="int" status="required"></attr>
		<attr name="background" type="int" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
		<attr name="y" type="int" status="required"></attr>
		<attr name="space" type="int" status="optional"></attr>
		<attr name="tightness" type="int" status="required"></attr>
		<attr name="angle" type="float" status="required"></attr>
		<attr name="antialias_steps" type="int" status="required"></attr>
	</tag>
	<tag name="imagerectangle" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="x1" type="int" status="required"></attr>
		<attr name="y1" type="int" status="required"></attr>
		<attr name="x2" type="int" status="required"></attr>
		<attr name="y2" type="int" status="required"></attr>
		<attr name="col" type="int" status="required"></attr>
	</tag>
	<tag name="imagerotate" type="function" returnType="resource" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="src_im" type="resource" status="required"></attr>
		<attr name="angle" type="float" status="required"></attr>
		<attr name="bgd_color" type="int" status="required"></attr>
	</tag>
	<tag name="imagesavealpha" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.2, PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="saveflag" type="bool" status="required"></attr>
	</tag>
	<tag name="imagesetbrush" type="function" returnType="int" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="brush" type="resource" status="required"></attr>
	</tag>
	<tag name="imagesetpixel" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
		<attr name="y" type="int" status="required"></attr>
		<attr name="color" type="int" status="required"></attr>
	</tag>
	<tag name="imagesetstyle" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="style" type="array" status="required"></attr>
	</tag>
	<tag name="imagesetthickness" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="thickness" type="int" status="required"></attr>
	</tag>
	<tag name="imagesettile" type="function" returnType="int" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="tile" type="resource" status="required"></attr>
	</tag>
	<tag name="imagestring" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="font" type="int" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
		<attr name="y" type="int" status="required"></attr>
		<attr name="s" type="string" status="required"></attr>
		<attr name="col" type="int" status="required"></attr>
	</tag>
	<tag name="imagestringup" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="font" type="int" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
		<attr name="y" type="int" status="required"></attr>
		<attr name="s" type="string" status="required"></attr>
		<attr name="col" type="int" status="required"></attr>
	</tag>
	<tag name="imagesx" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
	</tag>
	<tag name="imagesy" type="function" returnType="int" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
	</tag>
	<tag name="imagetruecolortopalette" type="function" returnType="void" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="dither" type="bool" status="required"></attr>
		<attr name="ncolors" type="int" status="required"></attr>
	</tag>
	<tag name="imagettfbbox" type="function" returnType="array" version="PHP 3&gt;= 3.0.1, PHP 4 , PHP 5">
		<attr name="size" type="float" status="required"></attr>
		<attr name="angle" type="float" status="required"></attr>
		<attr name="fontfile" type="string" status="required"></attr>
		<attr name="text" type="string" status="required"></attr>
	</tag>
	<tag name="imagettftext" type="function" returnType="array" version="PHP 3, PHP 4 , PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="size" type="float" status="required"></attr>
		<attr name="angle" type="float" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
		<attr name="y" type="int" status="required"></attr>
		<attr name="color" type="int" status="required"></attr>
		<attr name="fontfile" type="string" status="required"></attr>
		<attr name="text" type="string" status="required"></attr>
	</tag>
	<tag name="imagetypes" type="function" returnType="int" version="PHP 3 CVS only, PHP 4 &gt;= 4.0.2, PHP 5">
	</tag>
	<tag name="imagewbmp" type="function" returnType="bool" version="PHP 3&gt;= 3.0.15, PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="foreground" type="int" status="optional"></attr>
	</tag>
	<tag name="imagexbm" type="function" returnType="bool" version="PHP 5">
		<attr name="image" type="resource" status="required"></attr>
		<attr name="filename" type="string" status="required"></attr>
		<attr name="foreground" type="int" status="optional"></attr>
	</tag>
	<tag name="iptcembed" type="function" returnType="array" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="iptcdata" type="string" status="required"></attr>
		<attr name="jpeg_file_name" type="string" status="required"></attr>
		<attr name="spool" type="int" status="optional"></attr>
	</tag>
	<tag name="iptcparse" type="function" returnType="array" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="iptcblock" type="string" status="required"></attr>
	</tag>
	<tag name="jpeg2wbmp" type="function" returnType="int" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="jpegname" type="string" status="required"></attr>
		<attr name="wbmpname" type="string" status="required"></attr>
		<attr name="d_height" type="int" status="required"></attr>
		<attr name="d_width" type="int" status="required"></attr>
		<attr name="threshold" type="int" status="required"></attr>
	</tag>
	<tag name="png2wbmp" type="function" returnType="int" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pngname" type="string" status="required"></attr>
		<attr name="wbmpname" type="string" status="required"></attr>
		<attr name="d_height" type="int" status="required"></attr>
		<attr name="d_width" type="int" status="required"></attr>
		<attr name="threshold" type="int" status="required"></attr>
	</tag>
</tags>