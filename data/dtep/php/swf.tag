<!DOCTYPE tags>
<tags>
	<tag name="swf_actiongeturl" type="function" returnType="void" version="PHP 4 ">
		<attr name="url" type="string" status="required"></attr>
		<attr name="target" type="string" status="required"></attr>
	</tag>
	<tag name="swf_actiongotoframe" type="function" returnType="void" version="PHP 4 ">
		<attr name="framenumber" type="int" status="required"></attr>
	</tag>
	<tag name="swf_actiongotolabel" type="function" returnType="void" version="PHP 4 ">
		<attr name="label" type="string" status="required"></attr>
	</tag>
	<tag name="swf_actionnextframe" type="function" returnType="void" version="PHP 4 ">
	</tag>
	<tag name="swf_actionplay" type="function" returnType="void" version="PHP 4 ">
	</tag>
	<tag name="swf_actionprevframe" type="function" returnType="void" version="PHP 4 ">
	</tag>
	<tag name="swf_actionsettarget" type="function" returnType="void" version="PHP 4 ">
		<attr name="target" type="string" status="required"></attr>
	</tag>
	<tag name="swf_actionstop" type="function" returnType="void" version="PHP 4 ">
	</tag>
	<tag name="swf_actiontogglequality" type="function" returnType="void" version="PHP 4 ">
	</tag>
	<tag name="swf_actionwaitforframe" type="function" returnType="void" version="PHP 4 ">
		<attr name="framenumber" type="int" status="required"></attr>
		<attr name="skipcount" type="int" status="required"></attr>
	</tag>
	<tag name="swf_addbuttonrecord" type="function" returnType="void" version="PHP 4 ">
		<attr name="states" type="int" status="required"></attr>
		<attr name="shapeid" type="int" status="required"></attr>
		<attr name="depth" type="int" status="required"></attr>
	</tag>
	<tag name="swf_addcolor" type="function" returnType="void" version="PHP 4 ">
		<attr name="r" type="float" status="required"></attr>
		<attr name="g" type="float" status="required"></attr>
		<attr name="b" type="float" status="required"></attr>
		<attr name="a" type="float" status="required"></attr>
	</tag>
	<tag name="swf_closefile" type="function" returnType="void" version="PHP 4 ">
		<attr name="return_file" type="int" status="optional"></attr>
	</tag>
	<tag name="swf_definebitmap" type="function" returnType="void" version="PHP 4 ">
		<attr name="objid" type="int" status="required"></attr>
		<attr name="image_name" type="string" status="required"></attr>
	</tag>
	<tag name="swf_definefont" type="function" returnType="void" version="PHP 4 ">
		<attr name="fontid" type="int" status="required"></attr>
		<attr name="fontname" type="string" status="required"></attr>
	</tag>
	<tag name="swf_defineline" type="function" returnType="void" version="PHP 4 ">
		<attr name="objid" type="int" status="required"></attr>
		<attr name="x1" type="float" status="required"></attr>
		<attr name="y1" type="float" status="required"></attr>
		<attr name="x2" type="float" status="required"></attr>
		<attr name="y2" type="float" status="required"></attr>
		<attr name="width" type="float" status="required"></attr>
	</tag>
	<tag name="swf_definepoly" type="function" returnType="void" version="PHP 4 ">
		<attr name="objid" type="int" status="required"></attr>
		<attr name="coords" type="array" status="required"></attr>
		<attr name="npoints" type="int" status="required"></attr>
		<attr name="width" type="float" status="required"></attr>
	</tag>
	<tag name="swf_definerect" type="function" returnType="void" version="PHP 4 ">
		<attr name="objid" type="int" status="required"></attr>
		<attr name="x1" type="float" status="required"></attr>
		<attr name="y1" type="float" status="required"></attr>
		<attr name="x2" type="float" status="required"></attr>
		<attr name="y2" type="float" status="required"></attr>
		<attr name="width" type="float" status="required"></attr>
	</tag>
	<tag name="swf_definetext" type="function" returnType="void" version="PHP 4 ">
		<attr name="objid" type="int" status="required"></attr>
		<attr name="str" type="string" status="required"></attr>
		<attr name="docenter" type="int" status="required"></attr>
	</tag>
	<tag name="swf_endbutton" type="function" returnType="void" version="PHP 4 ">
	</tag>
	<tag name="swf_enddoaction" type="function" returnType="void" version="PHP 4 ">
	</tag>
	<tag name="swf_endshape" type="function" returnType="void" version="PHP 4 ">
	</tag>
	<tag name="swf_endsymbol" type="function" returnType="void" version="PHP 4 ">
	</tag>
	<tag name="swf_fontsize" type="function" returnType="void" version="PHP 4 ">
		<attr name="size" type="float" status="required"></attr>
	</tag>
	<tag name="swf_fontslant" type="function" returnType="void" version="PHP 4 ">
		<attr name="slant" type="float" status="required"></attr>
	</tag>
	<tag name="swf_fonttracking" type="function" returnType="void" version="PHP 4 ">
		<attr name="tracking" type="float" status="required"></attr>
	</tag>
	<tag name="swf_getbitmapinfo" type="function" returnType="array" version="PHP 4 ">
		<attr name="bitmapid" type="int" status="required"></attr>
	</tag>
	<tag name="swf_getfontinfo" type="function" returnType="array" version="PHP 4 ">
	</tag>
	<tag name="swf_getframe" type="function" returnType="int" version="PHP 4 ">
	</tag>
	<tag name="swf_labelframe" type="function" returnType="void" version="PHP 4 ">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="swf_lookat" type="function" returnType="void" version="PHP 4 ">
		<attr name="view_x" type="float" status="required"></attr>
		<attr name="view_y" type="float" status="required"></attr>
		<attr name="view_z" type="float" status="required"></attr>
		<attr name="reference_x" type="float" status="required"></attr>
		<attr name="reference_y" type="float" status="required"></attr>
		<attr name="reference_z" type="float" status="required"></attr>
		<attr name="twist" type="float" status="required"></attr>
	</tag>
	<tag name="swf_modifyobject" type="function" returnType="void" version="PHP 4 ">
		<attr name="depth" type="int" status="required"></attr>
		<attr name="how" type="int" status="required"></attr>
	</tag>
	<tag name="swf_mulcolor" type="function" returnType="void" version="PHP 4 ">
		<attr name="r" type="float" status="required"></attr>
		<attr name="g" type="float" status="required"></attr>
		<attr name="b" type="float" status="required"></attr>
		<attr name="a" type="float" status="required"></attr>
	</tag>
	<tag name="swf_nextid" type="function" returnType="int" version="PHP 4 ">
	</tag>
	<tag name="swf_oncondition" type="function" returnType="void" version="PHP 4 ">
		<attr name="transition" type="int" status="required"></attr>
	</tag>
	<tag name="swf_openfile" type="function" returnType="void" version="PHP 4 ">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="width" type="float" status="required"></attr>
		<attr name="height" type="float" status="required"></attr>
		<attr name="framerate" type="float" status="required"></attr>
		<attr name="r" type="float" status="required"></attr>
		<attr name="g" type="float" status="required"></attr>
		<attr name="b" type="float" status="required"></attr>
	</tag>
	<tag name="swf_ortho" type="function" returnType="void" version="PHP 4 &gt;= 4.0.1">
		<attr name="xmin" type="float" status="required"></attr>
		<attr name="xmax" type="float" status="required"></attr>
		<attr name="ymin" type="float" status="required"></attr>
		<attr name="ymax" type="float" status="required"></attr>
		<attr name="zmin" type="float" status="required"></attr>
		<attr name="zmax" type="float" status="required"></attr>
	</tag>
	<tag name="swf_ortho2" type="function" returnType="void" version="PHP 4 ">
		<attr name="xmin" type="float" status="required"></attr>
		<attr name="xmax" type="float" status="required"></attr>
		<attr name="ymin" type="float" status="required"></attr>
		<attr name="ymax" type="float" status="required"></attr>
	</tag>
	<tag name="swf_perspective" type="function" returnType="void" version="PHP 4 ">
		<attr name="fovy" type="float" status="required"></attr>
		<attr name="aspect" type="float" status="required"></attr>
		<attr name="near" type="float" status="required"></attr>
		<attr name="far" type="float" status="required"></attr>
	</tag>
	<tag name="swf_placeobject" type="function" returnType="void" version="PHP 4 ">
		<attr name="objid" type="int" status="required"></attr>
		<attr name="depth" type="int" status="required"></attr>
	</tag>
	<tag name="swf_polarview" type="function" returnType="void" version="PHP 4 ">
		<attr name="dist" type="float" status="required"></attr>
		<attr name="azimuth" type="float" status="required"></attr>
		<attr name="incidence" type="float" status="required"></attr>
		<attr name="twist" type="float" status="required"></attr>
	</tag>
	<tag name="swf_popmatrix" type="function" returnType="void" version="PHP 4 ">
	</tag>
	<tag name="swf_posround" type="function" returnType="void" version="PHP 4 ">
		<attr name="round" type="int" status="required"></attr>
	</tag>
	<tag name="swf_pushmatrix" type="function" returnType="void" version="PHP 4 ">
	</tag>
	<tag name="swf_removeobject" type="function" returnType="void" version="PHP 4 ">
		<attr name="depth" type="int" status="required"></attr>
	</tag>
	<tag name="swf_rotate" type="function" returnType="void" version="PHP 4 ">
		<attr name="angle" type="float" status="required"></attr>
		<attr name="axis" type="string" status="required"></attr>
	</tag>
	<tag name="swf_scale" type="function" returnType="void" version="PHP 4 ">
		<attr name="x" type="float" status="required"></attr>
		<attr name="y" type="float" status="required"></attr>
		<attr name="z" type="float" status="required"></attr>
	</tag>
	<tag name="swf_setfont" type="function" returnType="void" version="PHP 4 ">
		<attr name="fontid" type="int" status="required"></attr>
	</tag>
	<tag name="swf_setframe" type="function" returnType="void" version="PHP 4 ">
		<attr name="framenumber" type="int" status="required"></attr>
	</tag>
	<tag name="swf_shapearc" type="function" returnType="void" version="PHP 4 ">
		<attr name="x" type="float" status="required"></attr>
		<attr name="y" type="float" status="required"></attr>
		<attr name="r" type="float" status="required"></attr>
		<attr name="ang1" type="float" status="required"></attr>
		<attr name="ang2" type="float" status="required"></attr>
	</tag>
	<tag name="swf_shapecurveto" type="function" returnType="void" version="PHP 4 ">
		<attr name="x1" type="float" status="required"></attr>
		<attr name="y1" type="float" status="required"></attr>
		<attr name="x2" type="float" status="required"></attr>
		<attr name="y2" type="float" status="required"></attr>
	</tag>
	<tag name="swf_shapecurveto3" type="function" returnType="void" version="PHP 4 ">
		<attr name="x1" type="float" status="required"></attr>
		<attr name="y1" type="float" status="required"></attr>
		<attr name="x2" type="float" status="required"></attr>
		<attr name="y2" type="float" status="required"></attr>
		<attr name="x3" type="float" status="required"></attr>
		<attr name="y3" type="float" status="required"></attr>
	</tag>
	<tag name="swf_shapefillbitmapclip" type="function" returnType="void" version="PHP 4 ">
		<attr name="bitmapid" type="int" status="required"></attr>
	</tag>
	<tag name="swf_shapefillbitmaptile" type="function" returnType="void" version="PHP 4 ">
		<attr name="bitmapid" type="int" status="required"></attr>
	</tag>
	<tag name="swf_shapefilloff" type="function" returnType="void" version="PHP 4 ">
	</tag>
	<tag name="swf_shapefillsolid" type="function" returnType="void" version="PHP 4 ">
		<attr name="r" type="float" status="required"></attr>
		<attr name="g" type="float" status="required"></attr>
		<attr name="b" type="float" status="required"></attr>
		<attr name="a" type="float" status="required"></attr>
	</tag>
	<tag name="swf_shapelinesolid" type="function" returnType="void" version="PHP 4 ">
		<attr name="r" type="float" status="required"></attr>
		<attr name="g" type="float" status="required"></attr>
		<attr name="b" type="float" status="required"></attr>
		<attr name="a" type="float" status="required"></attr>
		<attr name="width" type="float" status="required"></attr>
	</tag>
	<tag name="swf_shapelineto" type="function" returnType="void" version="PHP 4 ">
		<attr name="x" type="float" status="required"></attr>
		<attr name="y" type="float" status="required"></attr>
	</tag>
	<tag name="swf_shapemoveto" type="function" returnType="void" version="PHP 4 ">
		<attr name="x" type="float" status="required"></attr>
		<attr name="y" type="float" status="required"></attr>
	</tag>
	<tag name="swf_showframe" type="function" returnType="void" version="PHP 4 ">
	</tag>
	<tag name="swf_startbutton" type="function" returnType="void" version="PHP 4 ">
		<attr name="objid" type="int" status="required"></attr>
		<attr name="type" type="int" status="required"></attr>
	</tag>
	<tag name="swf_startdoaction" type="function" returnType="void" version="PHP 4 ">
	</tag>
	<tag name="swf_startshape" type="function" returnType="void" version="PHP 4 ">
		<attr name="objid" type="int" status="required"></attr>
	</tag>
	<tag name="swf_startsymbol" type="function" returnType="void" version="PHP 4 ">
		<attr name="objid" type="int" status="required"></attr>
	</tag>
	<tag name="swf_textwidth" type="function" returnType="float" version="PHP 4 ">
		<attr name="str" type="string" status="required"></attr>
	</tag>
	<tag name="swf_translate" type="function" returnType="void" version="PHP 4 ">
		<attr name="x" type="float" status="required"></attr>
		<attr name="y" type="float" status="required"></attr>
		<attr name="z" type="float" status="required"></attr>
	</tag>
	<tag name="swf_viewport" type="function" returnType="void" version="PHP 4 ">
		<attr name="xmin" type="float" status="required"></attr>
		<attr name="xmax" type="float" status="required"></attr>
		<attr name="ymin" type="float" status="required"></attr>
		<attr name="ymax" type="float" status="required"></attr>
	</tag>
</tags>