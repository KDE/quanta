<!DOCTYPE tags>
<tags>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="pdf_add_bookmark" type="function" returnType="int">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="text" type="string"></attr>
		<attr name="parent" type="int"></attr>
		<attr name="open" type="int"></attr>
	</tag>
	<tag name="pdf_add_launchlink" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="llx" type="float"></attr>
		<attr name="lly" type="float"></attr>
		<attr name="urx" type="float"></attr>
		<attr name="ury" type="float"></attr>
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="pdf_add_locallink" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="lowerleftx" type="float"></attr>
		<attr name="lowerlefty" type="float"></attr>
		<attr name="upperrightx" type="float"></attr>
		<attr name="upperrighty" type="float"></attr>
		<attr name="page" type="int"></attr>
		<attr name="dest" type="string"></attr>
	</tag>
	<tag name="pdf_add_note" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="llx" type="float"></attr>
		<attr name="lly" type="float"></attr>
		<attr name="urx" type="float"></attr>
		<attr name="ury" type="float"></attr>
		<attr name="contents" type="string"></attr>
		<attr name="title" type="string"></attr>
		<attr name="icon" type="string"></attr>
		<attr name="open" type="int"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="pdf_add_pdflink" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="bottom_left_x" type="float"></attr>
		<attr name="bottom_left_y" type="float"></attr>
		<attr name="up_right_x" type="float"></attr>
		<attr name="up_right_y" type="float"></attr>
		<attr name="filename" type="string"></attr>
		<attr name="page" type="int"></attr>
		<attr name="dest" type="string"></attr>
	</tag>
	<tag name="pdf_add_thumbnail" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="image" type="int"></attr>
	</tag>
	<tag name="pdf_add_weblink" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="lowerleftx" type="float"></attr>
		<attr name="lowerlefty" type="float"></attr>
		<attr name="upperrightx" type="float"></attr>
		<attr name="upperrighty" type="float"></attr>
		<attr name="url" type="string"></attr>
	</tag>
	<tag name="pdf_arc" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="x" type="float"></attr>
		<attr name="y" type="float"></attr>
		<attr name="r" type="float"></attr>
		<attr name="alpha" type="float"></attr>
		<attr name="beta" type="float"></attr>
	</tag>
	<tag name="pdf_arcn" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="x" type="float"></attr>
		<attr name="y" type="float"></attr>
		<attr name="r" type="float"></attr>
		<attr name="alpha" type="float"></attr>
		<attr name="beta" type="float"></attr>
	</tag>
	<tag name="pdf_attach_file" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="llx" type="float"></attr>
		<attr name="lly" type="float"></attr>
		<attr name="urx" type="float"></attr>
		<attr name="ury" type="float"></attr>
		<attr name="filename" type="string"></attr>
		<attr name="description" type="string"></attr>
		<attr name="author" type="string"></attr>
		<attr name="mimetype" type="string"></attr>
		<attr name="icon" type="string"></attr>
	</tag>
	<tag name="pdf_begin_page" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="width" type="float"></attr>
		<attr name="height" type="float"></attr>
	</tag>
	<tag name="pdf_begin_pattern" type="function" returnType="int">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="width" type="float"></attr>
		<attr name="height" type="float"></attr>
		<attr name="xstep" type="float"></attr>
		<attr name="ystep" type="float"></attr>
		<attr name="painttype" type="int"></attr>
	</tag>
	<tag name="pdf_begin_template" type="function" returnType="int">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="width" type="float"></attr>
		<attr name="height" type="float"></attr>
	</tag>
	<tag name="pdf_circle" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="x" type="float"></attr>
		<attr name="y" type="float"></attr>
		<attr name="r" type="float"></attr>
	</tag>
	<tag name="pdf_clip" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
	</tag>
	<tag name="pdf_close_image" type="function" returnType="void">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="image" type="int"></attr>
	</tag>
	<tag name="pdf_close_pdi_page" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="pagehandle" type="int"></attr>
	</tag>
	<tag name="pdf_close_pdi" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="dochandle" type="int"></attr>
	</tag>
	<tag name="pdf_close" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
	</tag>
	<tag name="pdf_closepath_fill_stroke" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
	</tag>
	<tag name="pdf_closepath_stroke" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
	</tag>
	<tag name="pdf_closepath" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
	</tag>
	<tag name="pdf_concat" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="a" type="float"></attr>
		<attr name="b" type="float"></attr>
		<attr name="c" type="float"></attr>
		<attr name="d" type="float"></attr>
		<attr name="e" type="float"></attr>
		<attr name="f" type="float"></attr>
	</tag>
	<tag name="pdf_continue_text" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="text" type="string"></attr>
	</tag>
	<tag name="pdf_curveto" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="x1" type="float"></attr>
		<attr name="y1" type="float"></attr>
		<attr name="x2" type="float"></attr>
		<attr name="y2" type="float"></attr>
		<attr name="x3" type="float"></attr>
		<attr name="y3" type="float"></attr>
	</tag>
	<tag name="pdf_delete" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
	</tag>
	<tag name="pdf_end_page" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
	</tag>
	<tag name="pdf_end_pattern" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
	</tag>
	<tag name="pdf_end_template" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="pdf_fill_stroke" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
	</tag>
	<tag name="pdf_fill" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
	</tag>
	<tag name="pdf_findfont" type="function" returnType="int">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="fontname" type="string"></attr>
		<attr name="encoding" type="string"></attr>
		<attr name="embed" type="int"></attr>
	</tag>
	<tag name="pdf_get_buffer" type="function" returnType="string">
		<attr name="pdfdoc" type="resource"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="pdf_get_majorversion" type="function" returnType="int">
	</tag>
	<tag name="pdf_get_minorversion" type="function" returnType="int">
	</tag>
	<tag name="pdf_get_parameter" type="function" returnType="string">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="key" type="string"></attr>
		<attr name="modifier" type="float"></attr>
	</tag>
	<tag name="pdf_get_pdi_parameter" type="function" returnType="string">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="key" type="string"></attr>
		<attr name="document" type="int"></attr>
		<attr name="page" type="int"></attr>
		<attr name="index" type="int"></attr>
	</tag>
	<tag name="pdf_get_pdi_value" type="function" returnType="string">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="key" type="string"></attr>
		<attr name="doc" type="int"></attr>
		<attr name="page" type="int"></attr>
		<attr name="index" type="int"></attr>
	</tag>
	<tag name="pdf_get_value" type="function" returnType="float">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="key" type="string"></attr>
		<attr name="modifier" type="float"></attr>
	</tag>
	<tag name="pdf_initgraphics" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
	</tag>
	<tag name="pdf_lineto" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="x" type="float"></attr>
		<attr name="y" type="float"></attr>
	</tag>
	<tag name="pdf_makespotcolor" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="spotname" type="string"></attr>
	</tag>
	<tag name="pdf_moveto" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="x" type="float"></attr>
		<attr name="y" type="float"></attr>
	</tag>
	<tag name="pdf_new" type="function" returnType="resource">
		<attr name="" type=""></attr>
	</tag>
	<tag name="pdf_open_ccitt" type="function" returnType="int">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="filename" type="string"></attr>
		<attr name="width" type="int"></attr>
		<attr name="height" type="int"></attr>
		<attr name="BitReverse" type="int"></attr>
		<attr name="k" type="int"></attr>
		<attr name="Blackls1" type="int"></attr>
	</tag>
	<tag name="pdf_open_file" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="pdf_open_image_file" type="function" returnType="int">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="imagetype" type="string"></attr>
		<attr name="filename" type="string"></attr>
		<attr name="stringparam" type="string"></attr>
		<attr name="intparam" type="int"></attr>
	</tag>
	<tag name="pdf_open_image" type="function" returnType="int">
		<attr name="PDF-document" type="resource"></attr>
		<attr name="imagetype" type="string"></attr>
		<attr name="source" type="string"></attr>
		<attr name="data" type="string"></attr>
		<attr name="length" type="int"></attr>
		<attr name="width" type="int"></attr>
		<attr name="height" type="int"></attr>
		<attr name="components" type="int"></attr>
		<attr name="bpc" type="int"></attr>
		<attr name="params" type="string"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="pdf_open_memory_image" type="function" returnType="int">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="image" type="resource"></attr>
	</tag>
	<tag name="pdf_open_pdi_page" type="function" returnType="int">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="dochandle" type="int"></attr>
		<attr name="pagenumber" type="int"></attr>
		<attr name="pagelabel" type="string"></attr>
	</tag>
	<tag name="pdf_open_pdi" type="function" returnType="int">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="filename" type="string"></attr>
		<attr name="stringparam" type="string"></attr>
		<attr name="intparam" type="int"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="pdf_place_image" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="image" type="int"></attr>
		<attr name="x" type="float"></attr>
		<attr name="y" type="float"></attr>
		<attr name="scale" type="float"></attr>
	</tag>
	<tag name="pdf_place_pdi_page" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="page" type="int"></attr>
		<attr name="x" type="float"></attr>
		<attr name="y" type="float"></attr>
		<attr name="sx" type="float"></attr>
		<attr name="sy" type="float"></attr>
	</tag>
	<tag name="pdf_rect" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="x" type="float"></attr>
		<attr name="y" type="float"></attr>
		<attr name="width" type="float"></attr>
		<attr name="height" type="float"></attr>
	</tag>
	<tag name="pdf_restore" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
	</tag>
	<tag name="pdf_rotate" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="phi" type="float"></attr>
	</tag>
	<tag name="pdf_save" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
	</tag>
	<tag name="pdf_scale" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="x-scale" type="float"></attr>
		<attr name="y-scale" type="float"></attr>
	</tag>
	<tag name="pdf_set_border_color" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="red" type="float"></attr>
		<attr name="green" type="float"></attr>
		<attr name="blue" type="float"></attr>
	</tag>
	<tag name="pdf_set_border_dash" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="black" type="float"></attr>
		<attr name="white" type="float"></attr>
	</tag>
	<tag name="pdf_set_border_style" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="style" type="string"></attr>
		<attr name="width" type="float"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="pdf_set_info" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="key" type="string"></attr>
		<attr name="value" type="string"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="pdf_set_parameter" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="key" type="string"></attr>
		<attr name="value" type="string"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="pdf_set_text_pos" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="x" type="float"></attr>
		<attr name="y" type="float"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="pdf_set_value" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="key" type="string"></attr>
		<attr name="value" type="float"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="pdf_setcolor" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="type" type="string"></attr>
		<attr name="colorspace" type="string"></attr>
		<attr name="c1" type="float"></attr>
		<attr name="c2" type="float"></attr>
		<attr name="c3" type="float"></attr>
		<attr name="c4" type="float"></attr>
	</tag>
	<tag name="pdf_setdash" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="b" type="float"></attr>
		<attr name="w" type="float"></attr>
	</tag>
	<tag name="pdf_setflat" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="flatness" type="float"></attr>
	</tag>
	<tag name="pdf_setfont" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="font" type="int"></attr>
		<attr name="size" type="float"></attr>
	</tag>
	<tag name="pdf_setgray_fill" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="gray" type="float"></attr>
	</tag>
	<tag name="pdf_setgray_stroke" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="gray" type="float"></attr>
	</tag>
	<tag name="pdf_setgray" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="gray" type="float"></attr>
	</tag>
	<tag name="pdf_setlinecap" type="function" returnType="void">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="linecap" type="int"></attr>
	</tag>
	<tag name="pdf_setlinejoin" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="value" type="int"></attr>
	</tag>
	<tag name="pdf_setlinewidth" type="function" returnType="void">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="width" type="float"></attr>
	</tag>
	<tag name="pdf_setmatrix" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="a" type="float"></attr>
		<attr name="b" type="float"></attr>
		<attr name="c" type="float"></attr>
		<attr name="d" type="float"></attr>
		<attr name="e" type="float"></attr>
		<attr name="f" type="float"></attr>
	</tag>
	<tag name="pdf_setmiterlimit" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="miter" type="float"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="pdf_setrgbcolor_fill" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="red_value" type="float"></attr>
		<attr name="green_value" type="float"></attr>
		<attr name="blue_value" type="float"></attr>
	</tag>
	<tag name="pdf_setrgbcolor_stroke" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="red_value" type="float"></attr>
		<attr name="green_value" type="float"></attr>
		<attr name="blue_value" type="float"></attr>
	</tag>
	<tag name="pdf_setrgbcolor" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="red_value" type="float"></attr>
		<attr name="green_value" type="float"></attr>
		<attr name="blue_value" type="float"></attr>
	</tag>
	<tag name="pdf_show_boxed" type="function" returnType="int">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="text" type="string"></attr>
		<attr name="left" type="float"></attr>
		<attr name="top" type="float"></attr>
		<attr name="width" type="float"></attr>
		<attr name="height" type="float"></attr>
		<attr name="mode" type="string"></attr>
		<attr name="feature" type="string"></attr>
	</tag>
	<tag name="pdf_show_xy" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="text" type="string"></attr>
		<attr name="x" type="float"></attr>
		<attr name="y" type="float"></attr>
	</tag>
	<tag name="pdf_show" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="text" type="string"></attr>
	</tag>
	<tag name="pdf_skew" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="alpha" type="float"></attr>
		<attr name="beta" type="float"></attr>
	</tag>
	<tag name="pdf_stringwidth" type="function" returnType="float">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="text" type="string"></attr>
		<attr name="font" type="int"></attr>
		<attr name="size" type="float"></attr>
	</tag>
	<tag name="pdf_stroke" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
	</tag>
	<tag name="pdf_translate" type="function" returnType="bool">
		<attr name="pdfdoc" type="resource"></attr>
		<attr name="tx" type="float"></attr>
		<attr name="ty" type="float"></attr>
	</tag>
</tags>