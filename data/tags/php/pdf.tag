<!DOCTYPE tags>
<tags>
	<tag name="pdf_add_bookmark" type="function" returnType="int">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="text" type="string" status="optional"></attr>
		<attr name="parent" type="int" status="optional"></attr>
		<attr name="open" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_add_launchlink" type="function" returnType="int">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="llx" type="float" status="optional"></attr>
		<attr name="lly" type="float" status="optional"></attr>
		<attr name="urx" type="float" status="optional"></attr>
		<attr name="ury" type="float" status="optional"></attr>
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="pdf_add_locallink" type="function" returnType="int">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="llx" type="float" status="optional"></attr>
		<attr name="lly" type="float" status="optional"></attr>
		<attr name="urx" type="float" status="optional"></attr>
		<attr name="ury" type="float" status="optional"></attr>
		<attr name="page" type="int" status="optional"></attr>
		<attr name="dest" type="string" status="optional"></attr>
	</tag>
	<tag name="pdf_add_note" type="function" returnType="int">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="llx" type="float" status="optional"></attr>
		<attr name="lly" type="float" status="optional"></attr>
		<attr name="urx" type="float" status="optional"></attr>
		<attr name="ury" type="float" status="optional"></attr>
		<attr name="contents" type="string" status="optional"></attr>
		<attr name="title" type="string" status="optional"></attr>
		<attr name="icon" type="string" status="optional"></attr>
		<attr name="open" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_add_pdflink" type="function" returnType="int">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="llx" type="float" status="optional"></attr>
		<attr name="lly" type="float" status="optional"></attr>
		<attr name="urx" type="float" status="optional"></attr>
		<attr name="ury" type="float" status="optional"></attr>
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="page" type="int" status="optional"></attr>
		<attr name="dest" type="string" status="optional"></attr>
	</tag>
	<tag name="pdf_add_thumbnail" type="function" returnType="int">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="image" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_add_weblink" type="function" returnType="int">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="llx" type="float" status="optional"></attr>
		<attr name="lly" type="float" status="optional"></attr>
		<attr name="urx" type="float" status="optional"></attr>
		<attr name="ury" type="float" status="optional"></attr>
		<attr name="url" type="string" status="optional"></attr>
	</tag>
	<tag name="pdf_arc" type="function" returnType="void">
		<attr name="pdf object" type="resource" status="optional"></attr>
		<attr name="x" type="float" status="optional"></attr>
		<attr name="y" type="float" status="optional"></attr>
		<attr name="r" type="float" status="optional"></attr>
		<attr name="alpha" type="float" status="optional"></attr>
		<attr name="beta" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_arc" type="function" returnType="void">
		<attr name="pdf object" type="resource" status="optional"></attr>
		<attr name="x" type="float" status="optional"></attr>
		<attr name="y" type="float" status="optional"></attr>
		<attr name="r" type="float" status="optional"></attr>
		<attr name="alpha" type="float" status="optional"></attr>
		<attr name="beta" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_attach_file" type="function" returnType="int">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="llx" type="float" status="optional"></attr>
		<attr name="lly" type="float" status="optional"></attr>
		<attr name="urx" type="float" status="optional"></attr>
		<attr name="ury" type="float" status="optional"></attr>
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="description" type="string" status="optional"></attr>
		<attr name="author" type="string" status="optional"></attr>
		<attr name="mimetype" type="string" status="optional"></attr>
		<attr name="icon" type="string" status="optional"></attr>
	</tag>
	<tag name="pdf_begin_page" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="width" type="float" status="optional"></attr>
		<attr name="height" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_begin_pattern" type="function" returnType="int">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="width" type="float" status="optional"></attr>
		<attr name="height" type="float" status="optional"></attr>
		<attr name="xstep" type="float" status="optional"></attr>
		<attr name="ystep" type="float" status="optional"></attr>
		<attr name="painttype" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_begin_template" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="width" type="float" status="optional"></attr>
		<attr name="height" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_circle" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="x" type="float" status="optional"></attr>
		<attr name="y" type="float" status="optional"></attr>
		<attr name="r" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_clip" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_close_image" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="image" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_close_pdi_page" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="pagehandle" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_close_pdi" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="dochandle" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_close" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_closepath_fill_stroke" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_closepath_stroke" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_closepath" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_concat" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="a" type="float" status="optional"></attr>
		<attr name="b" type="float" status="optional"></attr>
		<attr name="c" type="float" status="optional"></attr>
		<attr name="d" type="float" status="optional"></attr>
		<attr name="e" type="float" status="optional"></attr>
		<attr name="f" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_continue_text" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="text" type="string" status="optional"></attr>
	</tag>
	<tag name="pdf_curveto" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="x1" type="float" status="optional"></attr>
		<attr name="y1" type="float" status="optional"></attr>
		<attr name="x2" type="float" status="optional"></attr>
		<attr name="y2" type="float" status="optional"></attr>
		<attr name="x3" type="float" status="optional"></attr>
		<attr name="y3" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_delete" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_end_page" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_end_pattern" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_end_template" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_fill_stroke" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_fill_stroke" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_findfont" type="function" returnType="int">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="fontname" type="string" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
		<attr name="embed" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_get_buffer" type="function" returnType="string">
		<attr name="pdf object" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_get_image_height" type="function" returnType="string">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="image" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_get_image_width" type="function" returnType="string">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="image" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_get_majorversion" type="function" returnType="int">
	</tag>
	<tag name="pdf_get_majorversion" type="function" returnType="int">
	</tag>
	<tag name="pdf_get_parameter" type="function" returnType="string">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="key" type="string" status="optional"></attr>
		<attr name="modifier" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_get_pdi_parameter" type="function" returnType="string">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="key" type="string" status="optional"></attr>
		<attr name="doc" type="int" status="optional"></attr>
		<attr name="page" type="int" status="optional"></attr>
		<attr name="index" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_get_pdi_value" type="function" returnType="string">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="key" type="string" status="optional"></attr>
		<attr name="doc" type="int" status="optional"></attr>
		<attr name="page" type="int" status="optional"></attr>
		<attr name="index" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_get_value" type="function" returnType="float">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="key" type="string" status="optional"></attr>
		<attr name="modifier" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_initgraphics" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_lineto" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="x" type="float" status="optional"></attr>
		<attr name="y" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_makespotcolor" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="spotname" type="string" status="optional"></attr>
	</tag>
	<tag name="pdf_moveto" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="x" type="float" status="optional"></attr>
		<attr name="y" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_new" type="function" returnType="int">
	</tag>
	<tag name="pdf_open_CCITT" type="function" returnType="int">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="width" type="int" status="optional"></attr>
		<attr name="height" type="int" status="optional"></attr>
		<attr name="BitReverse" type="int" status="optional"></attr>
		<attr name="k" type="int" status="optional"></attr>
		<attr name="Blackls1" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_open_file" type="function" returnType="int">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="pdf_open_image_file" type="function" returnType="int">
		<attr name="PDF-document" type="int" status="optional"></attr>
		<attr name="imagetype" type="string" status="optional"></attr>
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="stringparam" type="string" status="optional"></attr>
		<attr name="intparam" type="string" status="optional"></attr>
	</tag>
	<tag name="pdf_open_image" type="function" returnType="int">
		<attr name="PDF-document" type="int" status="optional"></attr>
		<attr name="imagetype" type="string" status="optional"></attr>
		<attr name="source" type="string" status="optional"></attr>
		<attr name="data" type="string" status="optional"></attr>
		<attr name="length" type="long" status="optional"></attr>
		<attr name="width" type="int" status="optional"></attr>
		<attr name="height" type="int" status="optional"></attr>
		<attr name="components" type="int" status="optional"></attr>
		<attr name="bpc" type="int" status="optional"></attr>
		<attr name="params" type="string" status="optional"></attr>
	</tag>
	<tag name="pdf_open_memory_image" type="function" returnType="int">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="image" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_open_pdi_page" type="function" returnType="int">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="dochandle" type="int" status="optional"></attr>
		<attr name="pagenumber" type="int" status="optional"></attr>
		<attr name="pagelabel" type="string" status="optional"></attr>
	</tag>
	<tag name="pdf_open_pdi" type="function" returnType="int">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="stringparam" type="string" status="optional"></attr>
		<attr name="intparam" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_open_tiff" type="function" returnType="int">
		<attr name="PDF-document" type="int" status="optional"></attr>
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="pdf_place_image" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="image" type="int" status="optional"></attr>
		<attr name="x" type="float" status="optional"></attr>
		<attr name="y" type="float" status="optional"></attr>
		<attr name="scale" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_place_pdi_page" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="page" type="int" status="optional"></attr>
		<attr name="x" type="float" status="optional"></attr>
		<attr name="y" type="float" status="optional"></attr>
		<attr name="sx" type="float" status="optional"></attr>
		<attr name="sy" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_rect" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="x" type="float" status="optional"></attr>
		<attr name="y" type="float" status="optional"></attr>
		<attr name="width" type="float" status="optional"></attr>
		<attr name="height" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_restore" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_rotate" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="phi" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_save" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_scale" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="x-scale" type="float" status="optional"></attr>
		<attr name="y-scale" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_set_border_color" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="red" type="float" status="optional"></attr>
		<attr name="green" type="float" status="optional"></attr>
		<attr name="blue" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_set_border_dash" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="black" type="float" status="optional"></attr>
		<attr name="white" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_set_border_style" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="style" type="string" status="optional"></attr>
		<attr name="width" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_set_horiz_scaling" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="scale" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_set_info_author" type="function" returnType="bool">
		<attr name="pdfdoc" type="int" status="optional"></attr>
		<attr name="author" type="string" status="optional"></attr>
	</tag>
	<tag name="pdf_set_info_creator" type="function" returnType="bool">
		<attr name="pdfdoc" type="int" status="optional"></attr>
		<attr name="creator" type="string" status="optional"></attr>
	</tag>
	<tag name="pdf_set_info_keywords" type="function" returnType="bool">
		<attr name="pdfdoc" type="int" status="optional"></attr>
		<attr name="keywords" type="string" status="optional"></attr>
	</tag>
	<tag name="pdf_set_info_subject" type="function" returnType="bool">
		<attr name="pdfdoc" type="int" status="optional"></attr>
		<attr name="subject" type="string" status="optional"></attr>
	</tag>
	<tag name="pdf_set_info_title" type="function" returnType="bool">
		<attr name="pdfdoc" type="int" status="optional"></attr>
		<attr name="title" type="string" status="optional"></attr>
	</tag>
	<tag name="pdf_set_info" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="key" type="string" status="optional"></attr>
		<attr name="value" type="string" status="optional"></attr>
	</tag>
	<tag name="pdf_set_parameter" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="key" type="string" status="optional"></attr>
		<attr name="value" type="string" status="optional"></attr>
	</tag>
	<tag name="pdf_set_text_pos" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="x" type="float" status="optional"></attr>
		<attr name="y" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_set_value" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="key" type="string" status="optional"></attr>
		<attr name="value" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_setcolor" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="type" type="string" status="optional"></attr>
		<attr name="colorspace" type="string" status="optional"></attr>
		<attr name="c1" type="float" status="optional"></attr>
		<attr name="c2" type="float" status="optional"></attr>
		<attr name="c3" type="float" status="optional"></attr>
		<attr name="c4" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_setdash" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="b" type="float" status="optional"></attr>
		<attr name="w" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_setflat" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="flatness" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_setfont" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="font" type="int" status="optional"></attr>
		<attr name="size" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_setgray_fill" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="gray" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_setgray_stroke" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="gray" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_setgray" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="gray" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_setlinecap" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="linecap" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_setlinejoin" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="linejoin" type="long" status="optional"></attr>
	</tag>
	<tag name="pdf_setlinewidth" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="width" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_setmatrix" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="a" type="float" status="optional"></attr>
		<attr name="b" type="float" status="optional"></attr>
		<attr name="c" type="float" status="optional"></attr>
		<attr name="d" type="float" status="optional"></attr>
		<attr name="e" type="float" status="optional"></attr>
		<attr name="f" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_setmiterlimit" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="miter" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_setpolydash" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="* dasharray" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_setrgbcolor_fill" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="red value" type="float" status="optional"></attr>
		<attr name="green value" type="float" status="optional"></attr>
		<attr name="blue value" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_setrgbcolor_stroke" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="red value" type="float" status="optional"></attr>
		<attr name="green value" type="float" status="optional"></attr>
		<attr name="blue value" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_setrgbcolor" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="red value" type="float" status="optional"></attr>
		<attr name="green value" type="float" status="optional"></attr>
		<attr name="blue value" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_show_boxed" type="function" returnType="int">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="text" type="string" status="optional"></attr>
		<attr name="left" type="float" status="optional"></attr>
		<attr name="top" type="float" status="optional"></attr>
		<attr name="width" type="float" status="optional"></attr>
		<attr name="height" type="float" status="optional"></attr>
		<attr name="hmode" type="string" status="optional"></attr>
		<attr name="feature" type="string" status="optional"></attr>
	</tag>
	<tag name="pdf_show_xy" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="text" type="string" status="optional"></attr>
		<attr name="x" type="float" status="optional"></attr>
		<attr name="y" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_show" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="text" type="string" status="optional"></attr>
	</tag>
	<tag name="pdf_skew" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="alpha" type="float" status="optional"></attr>
		<attr name="beta" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_stringwidth" type="function" returnType="float">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="text" type="string" status="optional"></attr>
		<attr name="font" type="int" status="optional"></attr>
		<attr name="size" type="float" status="optional"></attr>
	</tag>
	<tag name="pdf_stroke" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
	</tag>
	<tag name="pdf_translate" type="function" returnType="void">
		<attr name="pdf object" type="int" status="optional"></attr>
		<attr name="tx" type="float" status="optional"></attr>
		<attr name="ty" type="float" status="optional"></attr>
	</tag>
</tags>
