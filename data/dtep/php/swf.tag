<!DOCTYPE tags>
<tags>
	<tag name="swf_actiongeturl" type="function" returnType="void">
		<attr name="url" type="string"></attr>
		<attr name="target" type="string"></attr>
	</tag>
	<tag name="swf_actiongotoframe" type="function" returnType="void">
		<attr name="framenumber" type="int"></attr>
	</tag>
	<tag name="swf_actiongotolabel" type="function" returnType="void">
		<attr name="label" type="string"></attr>
	</tag>
	<tag name="swf_actionnextframe" type="function" returnType="void">
	</tag>
	<tag name="swf_actionplay" type="function" returnType="void">
	</tag>
	<tag name="swf_actionprevframe" type="function" returnType="void">
	</tag>
	<tag name="swf_actionsettarget" type="function" returnType="void">
		<attr name="target" type="string"></attr>
	</tag>
	<tag name="swf_actionstop" type="function" returnType="void">
	</tag>
	<tag name="swf_actiontogglequality" type="function" returnType="void">
	</tag>
	<tag name="swf_actionwaitforframe" type="function" returnType="void">
		<attr name="framenumber" type="int"></attr>
		<attr name="skipcount" type="int"></attr>
	</tag>
	<tag name="swf_addbuttonrecord" type="function" returnType="void">
		<attr name="states" type="int"></attr>
		<attr name="shapeid" type="int"></attr>
		<attr name="depth" type="int"></attr>
	</tag>
	<tag name="swf_addcolor" type="function" returnType="void">
		<attr name="r" type="float"></attr>
		<attr name="g" type="float"></attr>
		<attr name="b" type="float"></attr>
		<attr name="a" type="float"></attr>
	</tag>
	<tag name="swf_closefile" type="function" returnType="void">
		<attr name="return_file" type="int" status="optional"></attr>
	</tag>
	<tag name="swf_definebitmap" type="function" returnType="void">
		<attr name="objid" type="int"></attr>
		<attr name="image_name" type="string"></attr>
	</tag>
	<tag name="swf_definefont" type="function" returnType="void">
		<attr name="fontid" type="int"></attr>
		<attr name="fontname" type="string"></attr>
	</tag>
	<tag name="swf_defineline" type="function" returnType="void">
		<attr name="objid" type="int"></attr>
		<attr name="x1" type="float"></attr>
		<attr name="y1" type="float"></attr>
		<attr name="x2" type="float"></attr>
		<attr name="y2" type="float"></attr>
		<attr name="width" type="float"></attr>
	</tag>
	<tag name="swf_definepoly" type="function" returnType="void">
		<attr name="objid" type="int"></attr>
		<attr name="coords" type="array"></attr>
		<attr name="npoints" type="int"></attr>
		<attr name="width" type="float"></attr>
	</tag>
	<tag name="swf_definerect" type="function" returnType="void">
		<attr name="objid" type="int"></attr>
		<attr name="x1" type="float"></attr>
		<attr name="y1" type="float"></attr>
		<attr name="x2" type="float"></attr>
		<attr name="y2" type="float"></attr>
		<attr name="width" type="float"></attr>
	</tag>
	<tag name="swf_definetext" type="function" returnType="void">
		<attr name="objid" type="int"></attr>
		<attr name="str" type="string"></attr>
		<attr name="docenter" type="int"></attr>
	</tag>
	<tag name="swf_endbutton" type="function" returnType="void">
	</tag>
	<tag name="swf_enddoaction" type="function" returnType="void">
	</tag>
	<tag name="swf_endshape" type="function" returnType="void">
	</tag>
	<tag name="swf_endsymbol" type="function" returnType="void">
	</tag>
	<tag name="swf_fontsize" type="function" returnType="void">
		<attr name="size" type="float"></attr>
	</tag>
	<tag name="swf_fontslant" type="function" returnType="void">
		<attr name="slant" type="float"></attr>
	</tag>
	<tag name="swf_fonttracking" type="function" returnType="void">
		<attr name="tracking" type="float"></attr>
	</tag>
	<tag name="swf_getbitmapinfo" type="function" returnType="array">
		<attr name="bitmapid" type="int"></attr>
	</tag>
	<tag name="swf_getfontinfo" type="function" returnType="array">
	</tag>
	<tag name="swf_getframe" type="function" returnType="int">
	</tag>
	<tag name="swf_labelframe" type="function" returnType="void">
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="swf_lookat" type="function" returnType="void">
		<attr name="view_x" type="float"></attr>
		<attr name="view_y" type="float"></attr>
		<attr name="view_z" type="float"></attr>
		<attr name="reference_x" type="float"></attr>
		<attr name="reference_y" type="float"></attr>
		<attr name="reference_z" type="float"></attr>
		<attr name="twist" type="float"></attr>
	</tag>
	<tag name="swf_modifyobject" type="function" returnType="void">
		<attr name="depth" type="int"></attr>
		<attr name="how" type="int"></attr>
	</tag>
	<tag name="swf_mulcolor" type="function" returnType="void">
		<attr name="r" type="float"></attr>
		<attr name="g" type="float"></attr>
		<attr name="b" type="float"></attr>
		<attr name="a" type="float"></attr>
	</tag>
	<tag name="swf_nextid" type="function" returnType="int">
	</tag>
	<tag name="swf_oncondition" type="function" returnType="void">
		<attr name="transition" type="int"></attr>
	</tag>
	<tag name="swf_openfile" type="function" returnType="void">
		<attr name="filename" type="string"></attr>
		<attr name="width" type="float"></attr>
		<attr name="height" type="float"></attr>
		<attr name="framerate" type="float"></attr>
		<attr name="r" type="float"></attr>
		<attr name="g" type="float"></attr>
		<attr name="b" type="float"></attr>
	</tag>
	<tag name="swf_ortho" type="function" returnType="void">
		<attr name="xmin" type="float"></attr>
		<attr name="xmax" type="float"></attr>
		<attr name="ymin" type="float"></attr>
		<attr name="ymax" type="float"></attr>
		<attr name="zmin" type="float"></attr>
		<attr name="zmax" type="float"></attr>
	</tag>
	<tag name="swf_ortho2" type="function" returnType="void">
		<attr name="xmin" type="float"></attr>
		<attr name="xmax" type="float"></attr>
		<attr name="ymin" type="float"></attr>
		<attr name="ymax" type="float"></attr>
	</tag>
	<tag name="swf_perspective" type="function" returnType="void">
		<attr name="fovy" type="float"></attr>
		<attr name="aspect" type="float"></attr>
		<attr name="near" type="float"></attr>
		<attr name="far" type="float"></attr>
	</tag>
	<tag name="swf_placeobject" type="function" returnType="void">
		<attr name="objid" type="int"></attr>
		<attr name="depth" type="int"></attr>
	</tag>
	<tag name="swf_polarview" type="function" returnType="void">
		<attr name="dist" type="float"></attr>
		<attr name="azimuth" type="float"></attr>
		<attr name="incidence" type="float"></attr>
		<attr name="twist" type="float"></attr>
	</tag>
	<tag name="swf_popmatrix" type="function" returnType="void">
	</tag>
	<tag name="swf_posround" type="function" returnType="void">
		<attr name="round" type="int"></attr>
	</tag>
	<tag name="swf_pushmatrix" type="function" returnType="void">
	</tag>
	<tag name="swf_removeobject" type="function" returnType="void">
		<attr name="depth" type="int"></attr>
	</tag>
	<tag name="swf_rotate" type="function" returnType="void">
		<attr name="angle" type="float"></attr>
		<attr name="axis" type="string"></attr>
	</tag>
	<tag name="swf_scale" type="function" returnType="void">
		<attr name="x" type="float"></attr>
		<attr name="y" type="float"></attr>
		<attr name="z" type="float"></attr>
	</tag>
	<tag name="swf_setfont" type="function" returnType="void">
		<attr name="fontid" type="int"></attr>
	</tag>
	<tag name="swf_setframe" type="function" returnType="void">
		<attr name="framenumber" type="int"></attr>
	</tag>
	<tag name="swf_shapearc" type="function" returnType="void">
		<attr name="x" type="float"></attr>
		<attr name="y" type="float"></attr>
		<attr name="r" type="float"></attr>
		<attr name="ang1" type="float"></attr>
		<attr name="ang2" type="float"></attr>
	</tag>
	<tag name="swf_shapecurveto" type="function" returnType="void">
		<attr name="x1" type="float"></attr>
		<attr name="y1" type="float"></attr>
		<attr name="x2" type="float"></attr>
		<attr name="y2" type="float"></attr>
	</tag>
	<tag name="swf_shapecurveto3" type="function" returnType="void">
		<attr name="x1" type="float"></attr>
		<attr name="y1" type="float"></attr>
		<attr name="x2" type="float"></attr>
		<attr name="y2" type="float"></attr>
		<attr name="x3" type="float"></attr>
		<attr name="y3" type="float"></attr>
	</tag>
	<tag name="swf_shapefillbitmapclip" type="function" returnType="void">
		<attr name="bitmapid" type="int"></attr>
	</tag>
	<tag name="swf_shapefillbitmaptile" type="function" returnType="void">
		<attr name="bitmapid" type="int"></attr>
	</tag>
	<tag name="swf_shapefilloff" type="function" returnType="void">
	</tag>
	<tag name="swf_shapefillsolid" type="function" returnType="void">
		<attr name="r" type="float"></attr>
		<attr name="g" type="float"></attr>
		<attr name="b" type="float"></attr>
		<attr name="a" type="float"></attr>
	</tag>
	<tag name="swf_shapelinesolid" type="function" returnType="void">
		<attr name="r" type="float"></attr>
		<attr name="g" type="float"></attr>
		<attr name="b" type="float"></attr>
		<attr name="a" type="float"></attr>
		<attr name="width" type="float"></attr>
	</tag>
	<tag name="swf_shapelineto" type="function" returnType="void">
		<attr name="x" type="float"></attr>
		<attr name="y" type="float"></attr>
	</tag>
	<tag name="swf_shapemoveto" type="function" returnType="void">
		<attr name="x" type="float"></attr>
		<attr name="y" type="float"></attr>
	</tag>
	<tag name="swf_showframe" type="function" returnType="void">
	</tag>
	<tag name="swf_startbutton" type="function" returnType="void">
		<attr name="objid" type="int"></attr>
		<attr name="type" type="int"></attr>
	</tag>
	<tag name="swf_startdoaction" type="function" returnType="void">
	</tag>
	<tag name="swf_startshape" type="function" returnType="void">
		<attr name="objid" type="int"></attr>
	</tag>
	<tag name="swf_startsymbol" type="function" returnType="void">
		<attr name="objid" type="int"></attr>
	</tag>
	<tag name="swf_textwidth" type="function" returnType="float">
		<attr name="str" type="string"></attr>
	</tag>
	<tag name="swf_translate" type="function" returnType="void">
		<attr name="x" type="float"></attr>
		<attr name="y" type="float"></attr>
		<attr name="z" type="float"></attr>
	</tag>
	<tag name="swf_viewport" type="function" returnType="void">
		<attr name="xmin" type="float"></attr>
		<attr name="xmax" type="float"></attr>
		<attr name="ymin" type="float"></attr>
		<attr name="ymax" type="float"></attr>
	</tag>
</tags>