<!DOCTYPE tags>
<tags>
	<tag name="pdf_add_bookmark" type="function" returnType="int" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="text" type="string" status="required"></attr>
		<attr name="parent" type="int" status="required"></attr>
		<attr name="open" type="int" status="required"></attr>
	</tag>
	<tag name="pdf_add_launchlink" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="llx" type="float" status="required"></attr>
		<attr name="lly" type="float" status="required"></attr>
		<attr name="urx" type="float" status="required"></attr>
		<attr name="ury" type="float" status="required"></attr>
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="pdf_add_locallink" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="lowerleftx" type="float" status="required"></attr>
		<attr name="lowerlefty" type="float" status="required"></attr>
		<attr name="upperrightx" type="float" status="required"></attr>
		<attr name="upperrighty" type="float" status="required"></attr>
		<attr name="page" type="int" status="required"></attr>
		<attr name="dest" type="string" status="required"></attr>
	</tag>
	<tag name="pdf_add_note" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="llx" type="float" status="required"></attr>
		<attr name="lly" type="float" status="required"></attr>
		<attr name="urx" type="float" status="required"></attr>
		<attr name="ury" type="float" status="required"></attr>
		<attr name="contents" type="string" status="required"></attr>
		<attr name="title" type="string" status="required"></attr>
		<attr name="icon" type="string" status="required"></attr>
		<attr name="open" type="int" status="required"></attr>
	</tag>
	<tag name="pdf_add_pdflink" type="function" returnType="bool" version="PHP 3&gt;= 3.0.12, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="bottom_left_x" type="float" status="required"></attr>
		<attr name="bottom_left_y" type="float" status="required"></attr>
		<attr name="up_right_x" type="float" status="required"></attr>
		<attr name="up_right_y" type="float" status="required"></attr>
		<attr name="filename" type="string" status="required"></attr>
		<attr name="page" type="int" status="required"></attr>
		<attr name="dest" type="string" status="required"></attr>
	</tag>
	<tag name="pdf_add_thumbnail" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="image" type="int" status="required"></attr>
	</tag>
	<tag name="pdf_add_weblink" type="function" returnType="bool" version="PHP 3&gt;= 3.0.12, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="lowerleftx" type="float" status="required"></attr>
		<attr name="lowerlefty" type="float" status="required"></attr>
		<attr name="upperrightx" type="float" status="required"></attr>
		<attr name="upperrighty" type="float" status="required"></attr>
		<attr name="url" type="string" status="required"></attr>
	</tag>
	<tag name="pdf_arc" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="x" type="float" status="required"></attr>
		<attr name="y" type="float" status="required"></attr>
		<attr name="r" type="float" status="required"></attr>
		<attr name="alpha" type="float" status="required"></attr>
		<attr name="beta" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_arcn" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="x" type="float" status="required"></attr>
		<attr name="y" type="float" status="required"></attr>
		<attr name="r" type="float" status="required"></attr>
		<attr name="alpha" type="float" status="required"></attr>
		<attr name="beta" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_attach_file" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="llx" type="float" status="required"></attr>
		<attr name="lly" type="float" status="required"></attr>
		<attr name="urx" type="float" status="required"></attr>
		<attr name="ury" type="float" status="required"></attr>
		<attr name="filename" type="string" status="required"></attr>
		<attr name="description" type="string" status="required"></attr>
		<attr name="author" type="string" status="required"></attr>
		<attr name="mimetype" type="string" status="required"></attr>
		<attr name="icon" type="string" status="required"></attr>
	</tag>
	<tag name="pdf_begin_page" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="width" type="float" status="required"></attr>
		<attr name="height" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_begin_pattern" type="function" returnType="int" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="width" type="float" status="required"></attr>
		<attr name="height" type="float" status="required"></attr>
		<attr name="xstep" type="float" status="required"></attr>
		<attr name="ystep" type="float" status="required"></attr>
		<attr name="painttype" type="int" status="required"></attr>
	</tag>
	<tag name="pdf_begin_template" type="function" returnType="int" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="width" type="float" status="required"></attr>
		<attr name="height" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_circle" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="x" type="float" status="required"></attr>
		<attr name="y" type="float" status="required"></attr>
		<attr name="r" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_clip" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
	</tag>
	<tag name="pdf_close_image" type="function" returnType="void" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="image" type="int" status="required"></attr>
	</tag>
	<tag name="pdf_close_pdi_page" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="pagehandle" type="int" status="required"></attr>
	</tag>
	<tag name="pdf_close_pdi" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="dochandle" type="int" status="required"></attr>
	</tag>
	<tag name="pdf_close" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
	</tag>
	<tag name="pdf_closepath_fill_stroke" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
	</tag>
	<tag name="pdf_closepath_stroke" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
	</tag>
	<tag name="pdf_closepath" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
	</tag>
	<tag name="pdf_concat" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="a" type="float" status="required"></attr>
		<attr name="b" type="float" status="required"></attr>
		<attr name="c" type="float" status="required"></attr>
		<attr name="d" type="float" status="required"></attr>
		<attr name="e" type="float" status="required"></attr>
		<attr name="f" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_continue_text" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="text" type="string" status="required"></attr>
	</tag>
	<tag name="pdf_curveto" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="x1" type="float" status="required"></attr>
		<attr name="y1" type="float" status="required"></attr>
		<attr name="x2" type="float" status="required"></attr>
		<attr name="y2" type="float" status="required"></attr>
		<attr name="x3" type="float" status="required"></attr>
		<attr name="y3" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_delete" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
	</tag>
	<tag name="pdf_end_page" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
	</tag>
	<tag name="pdf_end_pattern" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
	</tag>
	<tag name="pdf_end_template" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
	</tag>
	<tag name="pdf_fill_stroke" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
	</tag>
	<tag name="pdf_fill" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
	</tag>
	<tag name="pdf_findfont" type="function" returnType="int" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="fontname" type="string" status="required"></attr>
		<attr name="encoding" type="string" status="required"></attr>
		<attr name="embed" type="int" status="required"></attr>
	</tag>
	<tag name="pdf_get_buffer" type="function" returnType="string" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
	</tag>
	<tag name="pdf_get_majorversion" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
	</tag>
	<tag name="pdf_get_minorversion" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
	</tag>
	<tag name="pdf_get_parameter" type="function" returnType="string" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
		<attr name="modifier" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_get_pdi_parameter" type="function" returnType="string" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
		<attr name="document" type="int" status="required"></attr>
		<attr name="page" type="int" status="required"></attr>
		<attr name="index" type="int" status="required"></attr>
	</tag>
	<tag name="pdf_get_pdi_value" type="function" returnType="string" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
		<attr name="doc" type="int" status="required"></attr>
		<attr name="page" type="int" status="required"></attr>
		<attr name="index" type="int" status="required"></attr>
	</tag>
	<tag name="pdf_get_value" type="function" returnType="float" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
		<attr name="modifier" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_initgraphics" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
	</tag>
	<tag name="pdf_lineto" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="x" type="float" status="required"></attr>
		<attr name="y" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_makespotcolor" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="spotname" type="string" status="required"></attr>
	</tag>
	<tag name="pdf_moveto" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="x" type="float" status="required"></attr>
		<attr name="y" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_new" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="&lt;/methodparam&gt;" type="" status="required"></attr>
	</tag>
	<tag name="pdf_open_ccitt" type="function" returnType="int" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="filename" type="string" status="required"></attr>
		<attr name="width" type="int" status="required"></attr>
		<attr name="height" type="int" status="required"></attr>
		<attr name="BitReverse" type="int" status="required"></attr>
		<attr name="k" type="int" status="required"></attr>
		<attr name="Blackls1" type="int" status="required"></attr>
	</tag>
	<tag name="pdf_open_file" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="pdf_open_image_file" type="function" returnType="int" version="PHP 3 CVS only, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="imagetype" type="string" status="required"></attr>
		<attr name="filename" type="string" status="required"></attr>
		<attr name="stringparam" type="string" status="required"></attr>
		<attr name="intparam" type="int" status="required"></attr>
	</tag>
	<tag name="pdf_open_image" type="function" returnType="int" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="PDF-document" type="resource" status="required"></attr>
		<attr name="imagetype" type="string" status="required"></attr>
		<attr name="source" type="string" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
		<attr name="length" type="int" status="required"></attr>
		<attr name="width" type="int" status="required"></attr>
		<attr name="height" type="int" status="required"></attr>
		<attr name="components" type="int" status="required"></attr>
		<attr name="bpc" type="int" status="required"></attr>
		<attr name="params" type="string" status="required"></attr>
	</tag>
	<tag name="pdf_open_memory_image" type="function" returnType="int" version="PHP 3&gt;= 3.0.10, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="image" type="resource" status="required"></attr>
	</tag>
	<tag name="pdf_open_pdi_page" type="function" returnType="int" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="dochandle" type="int" status="required"></attr>
		<attr name="pagenumber" type="int" status="required"></attr>
		<attr name="pagelabel" type="string" status="required"></attr>
	</tag>
	<tag name="pdf_open_pdi" type="function" returnType="int" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="filename" type="string" status="required"></attr>
		<attr name="stringparam" type="string" status="required"></attr>
		<attr name="intparam" type="int" status="required"></attr>
	</tag>
	<tag name="pdf_place_image" type="function" returnType="bool" version="PHP 3&gt;= 3.0.7, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="image" type="int" status="required"></attr>
		<attr name="x" type="float" status="required"></attr>
		<attr name="y" type="float" status="required"></attr>
		<attr name="scale" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_place_pdi_page" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="page" type="int" status="required"></attr>
		<attr name="x" type="float" status="required"></attr>
		<attr name="y" type="float" status="required"></attr>
		<attr name="sx" type="float" status="required"></attr>
		<attr name="sy" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_rect" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="x" type="float" status="required"></attr>
		<attr name="y" type="float" status="required"></attr>
		<attr name="width" type="float" status="required"></attr>
		<attr name="height" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_restore" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
	</tag>
	<tag name="pdf_rotate" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="phi" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_save" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
	</tag>
	<tag name="pdf_scale" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="x-scale" type="float" status="required"></attr>
		<attr name="y-scale" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_set_border_color" type="function" returnType="bool" version="PHP 3&gt;= 3.0.12, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="red" type="float" status="required"></attr>
		<attr name="green" type="float" status="required"></attr>
		<attr name="blue" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_set_border_dash" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="black" type="float" status="required"></attr>
		<attr name="white" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_set_border_style" type="function" returnType="bool" version="PHP 3&gt;= 3.0.12, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="style" type="string" status="required"></attr>
		<attr name="width" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_set_info" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
		<attr name="value" type="string" status="required"></attr>
	</tag>
	<tag name="pdf_set_parameter" type="function" returnType="bool" version="PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
		<attr name="value" type="string" status="required"></attr>
	</tag>
	<tag name="pdf_set_text_pos" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="x" type="float" status="required"></attr>
		<attr name="y" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_set_value" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.1, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
		<attr name="value" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_setcolor" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="type" type="string" status="required"></attr>
		<attr name="colorspace" type="string" status="required"></attr>
		<attr name="c1" type="float" status="required"></attr>
		<attr name="c2" type="float" status="required"></attr>
		<attr name="c3" type="float" status="required"></attr>
		<attr name="c4" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_setdash" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="b" type="float" status="required"></attr>
		<attr name="w" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_setflat" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="flatness" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_setfont" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="font" type="int" status="required"></attr>
		<attr name="size" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_setgray_fill" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="gray" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_setgray_stroke" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="gray" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_setgray" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="gray" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_setlinecap" type="function" returnType="void" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="linecap" type="int" status="required"></attr>
	</tag>
	<tag name="pdf_setlinejoin" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="value" type="int" status="required"></attr>
	</tag>
	<tag name="pdf_setlinewidth" type="function" returnType="void" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="width" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_setmatrix" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="a" type="float" status="required"></attr>
		<attr name="b" type="float" status="required"></attr>
		<attr name="c" type="float" status="required"></attr>
		<attr name="d" type="float" status="required"></attr>
		<attr name="e" type="float" status="required"></attr>
		<attr name="f" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_setmiterlimit" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="miter" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_setrgbcolor_fill" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="red_value" type="float" status="required"></attr>
		<attr name="green_value" type="float" status="required"></attr>
		<attr name="blue_value" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_setrgbcolor_stroke" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="red_value" type="float" status="required"></attr>
		<attr name="green_value" type="float" status="required"></attr>
		<attr name="blue_value" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_setrgbcolor" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="red_value" type="float" status="required"></attr>
		<attr name="green_value" type="float" status="required"></attr>
		<attr name="blue_value" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_show_boxed" type="function" returnType="int" version="PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="text" type="string" status="required"></attr>
		<attr name="left" type="float" status="required"></attr>
		<attr name="top" type="float" status="required"></attr>
		<attr name="width" type="float" status="required"></attr>
		<attr name="height" type="float" status="required"></attr>
		<attr name="mode" type="string" status="required"></attr>
		<attr name="feature" type="string" status="required"></attr>
	</tag>
	<tag name="pdf_show_xy" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="text" type="string" status="required"></attr>
		<attr name="x" type="float" status="required"></attr>
		<attr name="y" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_show" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="text" type="string" status="required"></attr>
	</tag>
	<tag name="pdf_skew" type="function" returnType="bool" version="PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="alpha" type="float" status="required"></attr>
		<attr name="beta" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_stringwidth" type="function" returnType="float" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="text" type="string" status="required"></attr>
		<attr name="font" type="int" status="required"></attr>
		<attr name="size" type="float" status="required"></attr>
	</tag>
	<tag name="pdf_stroke" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
	</tag>
	<tag name="pdf_translate" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="resource" status="required"></attr>
		<attr name="tx" type="float" status="required"></attr>
		<attr name="ty" type="float" status="required"></attr>
	</tag>
</tags>