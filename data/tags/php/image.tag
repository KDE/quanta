<!DOCTYPE tags>
<tags>	<tag name="exif_imagetype" type="function" returnType="int|false">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="exif_read_data" type="function" returnType="array">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="sections" type="string" status="optional"></attr>
		<attr name="arrays" type="bool" status="optional"></attr>
		<attr name="thumbnail" type="bool" status="optional"></attr>
	</tag>
	<tag name="exif_thumbnail" type="function" returnType="string">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="&width" type="int" status="optional"></attr>
		<attr name="&height" type="int" status="optional"></attr>
		<attr name="&imagetype" type="int" status="optional"></attr>
	</tag>
	<tag name="getimagesize" type="function" returnType="array">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="imageinfo" type="array" status="optional"></attr>
	</tag>
	<tag name="image2wbmp" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="threshold" type="int" status="optional"></attr>
	</tag>
	<tag name="imagealphablending" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="blendmode" type="bool" status="optional"></attr>
	</tag>
	<tag name="imagearc" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="cx" type="int" status="optional"></attr>
		<attr name="cy" type="int" status="optional"></attr>
		<attr name="w" type="int" status="optional"></attr>
		<attr name="h" type="int" status="optional"></attr>
		<attr name="s" type="int" status="optional"></attr>
		<attr name="e" type="int" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
	</tag>
	<tag name="imagechar" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="font" type="int" status="optional"></attr>
		<attr name="x" type="int" status="optional"></attr>
		<attr name="y" type="int" status="optional"></attr>
		<attr name="c" type="string" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
	</tag>
	<tag name="imagecharup" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="font" type="int" status="optional"></attr>
		<attr name="x" type="int" status="optional"></attr>
		<attr name="y" type="int" status="optional"></attr>
		<attr name="c" type="string" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
	</tag>
	<tag name="imagecolorallocate" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="red" type="int" status="optional"></attr>
		<attr name="green" type="int" status="optional"></attr>
		<attr name="blue" type="int" status="optional"></attr>
	</tag>
	<tag name="imagecolorat" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="x" type="int" status="optional"></attr>
		<attr name="y" type="int" status="optional"></attr>
	</tag>
	<tag name="imagecolorclosest" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="red" type="int" status="optional"></attr>
		<attr name="green" type="int" status="optional"></attr>
		<attr name="blue" type="int" status="optional"></attr>
	</tag>
	<tag name="imagecolorclosestalpha" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="red" type="int" status="optional"></attr>
		<attr name="green" type="int" status="optional"></attr>
		<attr name="blue" type="int" status="optional"></attr>
		<attr name="alpha" type="int" status="optional"></attr>
	</tag>
	<tag name="imagecolorclosesthwb" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="red" type="int" status="optional"></attr>
		<attr name="green" type="int" status="optional"></attr>
		<attr name="blue" type="int" status="optional"></attr>
	</tag>
	<tag name="imagecolordeallocate" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="color" type="int" status="optional"></attr>
	</tag>
	<tag name="imagecolorexact" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="red" type="int" status="optional"></attr>
		<attr name="green" type="int" status="optional"></attr>
		<attr name="blue" type="int" status="optional"></attr>
	</tag>
	<tag name="imagecolorexactalpha" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="red" type="int" status="optional"></attr>
		<attr name="green" type="int" status="optional"></attr>
		<attr name="blue" type="int" status="optional"></attr>
		<attr name="alpha" type="int" status="optional"></attr>
	</tag>
	<tag name="imagecolorresolve" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="red" type="int" status="optional"></attr>
		<attr name="green" type="int" status="optional"></attr>
		<attr name="blue" type="int" status="optional"></attr>
	</tag>
	<tag name="imagecolorresolvealpha" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="red" type="int" status="optional"></attr>
		<attr name="green" type="int" status="optional"></attr>
		<attr name="blue" type="int" status="optional"></attr>
		<attr name="alpha" type="int" status="optional"></attr>
	</tag>
	<tag name="imagecolorset" type="function" returnType="bool">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="index" type="int" status="optional"></attr>
		<attr name="red" type="int" status="optional"></attr>
		<attr name="green" type="int" status="optional"></attr>
		<attr name="blue" type="int" status="optional"></attr>
	</tag>
	<tag name="imagecolorsforindex" type="function" returnType="array">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="index" type="int" status="optional"></attr>
	</tag>
	<tag name="imagecolorstotal" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
	</tag>
	<tag name="imagecolortransparent" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="color" type="int" status="optional"></attr>
	</tag>
	<tag name="imagecopy" type="function" returnType="int">
		<attr name="dst_im" type="resource" status="optional"></attr>
		<attr name="src_im" type="resource" status="optional"></attr>
		<attr name="dst_x" type="int" status="optional"></attr>
		<attr name="dst_y" type="int" status="optional"></attr>
		<attr name="src_x" type="int" status="optional"></attr>
		<attr name="src_y" type="int" status="optional"></attr>
		<attr name="src_w" type="int" status="optional"></attr>
		<attr name="src_h" type="int" status="optional"></attr>
	</tag>
	<tag name="imagecopymerge" type="function" returnType="int">
		<attr name="dst_im" type="resource" status="optional"></attr>
		<attr name="src_im" type="resource" status="optional"></attr>
		<attr name="dst_x" type="int" status="optional"></attr>
		<attr name="dst_y" type="int" status="optional"></attr>
		<attr name="src_x" type="int" status="optional"></attr>
		<attr name="src_y" type="int" status="optional"></attr>
		<attr name="src_w" type="int" status="optional"></attr>
		<attr name="src_h" type="int" status="optional"></attr>
		<attr name="pct" type="int" status="optional"></attr>
	</tag>
	<tag name="imagecopymergegray" type="function" returnType="int">
		<attr name="dst_im" type="resource" status="optional"></attr>
		<attr name="src_im" type="resource" status="optional"></attr>
		<attr name="dst_x" type="int" status="optional"></attr>
		<attr name="dst_y" type="int" status="optional"></attr>
		<attr name="src_x" type="int" status="optional"></attr>
		<attr name="src_y" type="int" status="optional"></attr>
		<attr name="src_w" type="int" status="optional"></attr>
		<attr name="src_h" type="int" status="optional"></attr>
		<attr name="pct" type="int" status="optional"></attr>
	</tag>
	<tag name="imagecopyresampled" type="function" returnType="int">
		<attr name="dst_im" type="resource" status="optional"></attr>
		<attr name="src_im" type="resource" status="optional"></attr>
		<attr name="dstX" type="int" status="optional"></attr>
		<attr name="dstY" type="int" status="optional"></attr>
		<attr name="srcX" type="int" status="optional"></attr>
		<attr name="srcY" type="int" status="optional"></attr>
		<attr name="dstW" type="int" status="optional"></attr>
		<attr name="dstH" type="int" status="optional"></attr>
		<attr name="srcW" type="int" status="optional"></attr>
		<attr name="srcH" type="int" status="optional"></attr>
	</tag>
	<tag name="imagecopyresized" type="function" returnType="int">
		<attr name="dst_im" type="resource" status="optional"></attr>
		<attr name="src_im" type="resource" status="optional"></attr>
		<attr name="dstX" type="int" status="optional"></attr>
		<attr name="dstY" type="int" status="optional"></attr>
		<attr name="srcX" type="int" status="optional"></attr>
		<attr name="srcY" type="int" status="optional"></attr>
		<attr name="dstW" type="int" status="optional"></attr>
		<attr name="dstH" type="int" status="optional"></attr>
		<attr name="srcW" type="int" status="optional"></attr>
		<attr name="srcH" type="int" status="optional"></attr>
	</tag>
	<tag name="imagecreate" type="function" returnType="resource">
		<attr name="x_size" type="int" status="optional"></attr>
		<attr name="y_size" type="int" status="optional"></attr>
	</tag>
	<tag name="imagecreatefromgd2" type="function" returnType="resource">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="imagecreatefromgd2part" type="function" returnType="resource">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="srcX" type="int" status="optional"></attr>
		<attr name="srcY" type="int" status="optional"></attr>
		<attr name="width" type="int" status="optional"></attr>
		<attr name="height" type="int" status="optional"></attr>
	</tag>
	<tag name="imagecreatefromgd" type="function" returnType="resource">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="imagecreatefromgif" type="function" returnType="resource">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="imagecreatefromjpeg" type="function" returnType="resource">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="imagecreatefrompng" type="function" returnType="resource">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="imagecreatefromstring" type="function" returnType="resource">
		<attr name="image" type="string" status="optional"></attr>
	</tag>
	<tag name="imagecreatefromwbmp" type="function" returnType="resource">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="imagecreatefromxbm" type="function" returnType="resource">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="imagecreatefromxpm" type="function" returnType="resource">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="imagecreatetruecolor" type="function" returnType="resource">
		<attr name="x_size" type="int" status="optional"></attr>
		<attr name="y_size" type="int" status="optional"></attr>
	</tag>
	<tag name="imagedashedline" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="x1" type="int" status="optional"></attr>
		<attr name="y1" type="int" status="optional"></attr>
		<attr name="x2" type="int" status="optional"></attr>
		<attr name="y2" type="int" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
	</tag>
	<tag name="imagedestroy" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
	</tag>
	<tag name="imageellipse" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="cx" type="int" status="optional"></attr>
		<attr name="cy" type="int" status="optional"></attr>
		<attr name="w" type="int" status="optional"></attr>
		<attr name="h" type="int" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
	</tag>
	<tag name="imagefill" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="x" type="int" status="optional"></attr>
		<attr name="y" type="int" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
	</tag>
	<tag name="imagefilledarc" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="cx" type="int" status="optional"></attr>
		<attr name="cy" type="int" status="optional"></attr>
		<attr name="w" type="int" status="optional"></attr>
		<attr name="h" type="int" status="optional"></attr>
		<attr name="s" type="int" status="optional"></attr>
		<attr name="e" type="int" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
		<attr name="style" type="int" status="optional"></attr>
	</tag>
	<tag name="imagefilledellipse" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="cx" type="int" status="optional"></attr>
		<attr name="cy" type="int" status="optional"></attr>
		<attr name="w" type="int" status="optional"></attr>
		<attr name="h" type="int" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
	</tag>
	<tag name="imagefilledpolygon" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="points" type="array" status="optional"></attr>
		<attr name="num_points" type="int" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
	</tag>
	<tag name="imagefilledrectangle" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="x1" type="int" status="optional"></attr>
		<attr name="y1" type="int" status="optional"></attr>
		<attr name="x2" type="int" status="optional"></attr>
		<attr name="y2" type="int" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
	</tag>
	<tag name="imagefilltoborder" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="x" type="int" status="optional"></attr>
		<attr name="y" type="int" status="optional"></attr>
		<attr name="border" type="int" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
	</tag>
	<tag name="imagefontheight" type="function" returnType="int">
		<attr name="font" type="int" status="optional"></attr>
	</tag>
	<tag name="imagefontwidth" type="function" returnType="int">
		<attr name="font" type="int" status="optional"></attr>
	</tag>
	<tag name="imageftbbox" type="function" returnType="array">
		<attr name="size" type="int" status="optional"></attr>
		<attr name="angle" type="int" status="optional"></attr>
		<attr name="font_file" type="string" status="optional"></attr>
		<attr name="text" type="string" status="optional"></attr>
		<attr name="extrainfo" type="array" status="optional"></attr>
	</tag>
	<tag name="imagefttext" type="function" returnType="array">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="size" type="int" status="optional"></attr>
		<attr name="angle" type="int" status="optional"></attr>
		<attr name="x" type="int" status="optional"></attr>
		<attr name="y" type="int" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
		<attr name="font_file" type="string" status="optional"></attr>
		<attr name="text" type="string" status="optional"></attr>
		<attr name="extrainfo" type="array" status="optional"></attr>
	</tag>
	<tag name="imagegammacorrect" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="inputgamma" type="float" status="optional"></attr>
		<attr name="outputgamma" type="float" status="optional"></attr>
	</tag>
	<tag name="imagegd2" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="imagegd" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="imagegif" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="imageinterlace" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="interlace" type="int" status="optional"></attr>
	</tag>
	<tag name="imagejpeg" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="quality" type="int" status="optional"></attr>
	</tag>
	<tag name="imageline" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="x1" type="int" status="optional"></attr>
		<attr name="y1" type="int" status="optional"></attr>
		<attr name="x2" type="int" status="optional"></attr>
		<attr name="y2" type="int" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
	</tag>
	<tag name="imageloadfont" type="function" returnType="int">
		<attr name="file" type="string" status="optional"></attr>
	</tag>
	<tag name="imagepalettecopy" type="function" returnType="int">
		<attr name="destination" type="resource" status="optional"></attr>
		<attr name="source" type="resource" status="optional"></attr>
	</tag>
	<tag name="imagepng" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="imagepolygon" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="points" type="array" status="optional"></attr>
		<attr name="num_points" type="int" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
	</tag>
	<tag name="imagepsbbox" type="function" returnType="array">
		<attr name="text" type="string" status="optional"></attr>
		<attr name="font" type="int" status="optional"></attr>
		<attr name="size" type="int" status="optional"></attr>
		<attr name="space" type="int" status="optional"></attr>
		<attr name="tightness" type="int" status="optional"></attr>
		<attr name="angle" type="float" status="optional"></attr>
	</tag>
	<tag name="imagepscopyfont" type="function" returnType="int">
		<attr name="fontindex" type="int" status="optional"></attr>
	</tag>
	<tag name="imagepsencodefont" type="function" returnType="int">
		<attr name="font_index" type="int" status="optional"></attr>
		<attr name="encodingfile" type="string" status="optional"></attr>
	</tag>
	<tag name="imagepsextendfont" type="function" returnType="bool">
		<attr name="font_index" type="int" status="optional"></attr>
		<attr name="extend" type="float" status="optional"></attr>
	</tag>
	<tag name="imagepsfreefont" type="function" returnType="void">
		<attr name="fontindex" type="int" status="optional"></attr>
	</tag>
	<tag name="imagepsloadfont" type="function" returnType="int">
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="imagepsslantfont" type="function" returnType="bool">
		<attr name="font_index" type="int" status="optional"></attr>
		<attr name="slant" type="float" status="optional"></attr>
	</tag>
	<tag name="imagepstext" type="function" returnType="array">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="text" type="string" status="optional"></attr>
		<attr name="font" type="int" status="optional"></attr>
		<attr name="size" type="int" status="optional"></attr>
		<attr name="foreground" type="int" status="optional"></attr>
		<attr name="background" type="int" status="optional"></attr>
		<attr name="x" type="int" status="optional"></attr>
		<attr name="y" type="int" status="optional"></attr>
		<attr name="space" type="int" status="optional"></attr>
		<attr name="tightness" type="int" status="optional"></attr>
		<attr name="angle" type="float" status="optional"></attr>
		<attr name="antialias_steps" type="int" status="optional"></attr>
	</tag>
	<tag name="imagerectangle" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="x1" type="int" status="optional"></attr>
		<attr name="y1" type="int" status="optional"></attr>
		<attr name="x2" type="int" status="optional"></attr>
		<attr name="y2" type="int" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
	</tag>
	<tag name="imagesetbrush" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="brush" type="resource" status="optional"></attr>
	</tag>
	<tag name="imagesetpixel" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="x" type="int" status="optional"></attr>
		<attr name="y" type="int" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
	</tag>
	<tag name="imagesetstyle" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="style" type="array" status="optional"></attr>
	</tag>
	<tag name="imagesetthickness" type="function" returnType="void">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="thickness" type="int" status="optional"></attr>
	</tag>
	<tag name="imagesettile" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="tile" type="resource" status="optional"></attr>
	</tag>
	<tag name="imagestring" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="font" type="int" status="optional"></attr>
		<attr name="x" type="int" status="optional"></attr>
		<attr name="y" type="int" status="optional"></attr>
		<attr name="s" type="string" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
	</tag>
	<tag name="imagestringup" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="font" type="int" status="optional"></attr>
		<attr name="x" type="int" status="optional"></attr>
		<attr name="y" type="int" status="optional"></attr>
		<attr name="s" type="string" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
	</tag>
	<tag name="imagesx" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
	</tag>
	<tag name="imagesy" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
	</tag>
	<tag name="imagetruecolortopalette" type="function" returnType="void">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="dither" type="bool" status="optional"></attr>
		<attr name="ncolors" type="int" status="optional"></attr>
	</tag>
	<tag name="imagettfbbox" type="function" returnType="array">
		<attr name="size" type="int" status="optional"></attr>
		<attr name="angle" type="int" status="optional"></attr>
		<attr name="fontfile" type="string" status="optional"></attr>
		<attr name="text" type="string" status="optional"></attr>
	</tag>
	<tag name="imagettftext" type="function" returnType="array">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="size" type="int" status="optional"></attr>
		<attr name="angle" type="int" status="optional"></attr>
		<attr name="x" type="int" status="optional"></attr>
		<attr name="y" type="int" status="optional"></attr>
		<attr name="col" type="int" status="optional"></attr>
		<attr name="fontfile" type="string" status="optional"></attr>
		<attr name="text" type="string" status="optional"></attr>
	</tag>
	<tag name="imagetypes" type="function" returnType="int">
	</tag>
	<tag name="imagewbmp" type="function" returnType="int">
		<attr name="image" type="resource" status="optional"></attr>
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="foreground" type="int" status="optional"></attr>
	</tag>
	<tag name="iptcembed" type="function" returnType="array">
		<attr name="iptcdata" type="string" status="optional"></attr>
		<attr name="jpeg_file_name" type="string" status="optional"></attr>
		<attr name="spool" type="int" status="optional"></attr>
	</tag>
	<tag name="iptcparse" type="function" returnType="array">
		<attr name="iptcblock" type="string" status="optional"></attr>
	</tag>
	<tag name="jpeg2wbmp" type="function" returnType="int">
		<attr name="jpegname" type="string" status="optional"></attr>
		<attr name="wbmpname" type="string" status="optional"></attr>
		<attr name="d_height" type="int" status="optional"></attr>
		<attr name="d_width" type="int" status="optional"></attr>
		<attr name="threshold" type="int" status="optional"></attr>
	</tag>
	<tag name="png2wbmp" type="function" returnType="int">
		<attr name="pngname" type="string" status="optional"></attr>
		<attr name="wbmpname" type="string" status="optional"></attr>
		<attr name="d_height" type="int" status="optional"></attr>
		<attr name="d_width" type="int" status="optional"></attr>
		<attr name="threshold" type="int" status="optional"></attr>
	</tag>
	<tag name="exif_read_data" type="function" returnType="array">
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="sections" type="string" status="optional"></attr>
		<attr name="arrays" type="bool" status="optional"></attr>
		<attr name="thumbnail" type="bool" status="optional"></attr>
	</tag>
</tags>
