<!DOCTYPE tags>
<tags>
	<tag name="cpdf_add_annotation" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="llx" type="float"></attr>
		<attr name="lly" type="float"></attr>
		<attr name="urx" type="float"></attr>
		<attr name="ury" type="float"></attr>
		<attr name="title" type="string"></attr>
		<attr name="content" type="string"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_add_outline" type="function" returnType="int">
		<attr name="pdf_document" type="int"></attr>
		<attr name="lastoutline" type="int"></attr>
		<attr name="sublevel" type="int"></attr>
		<attr name="open" type="int"></attr>
		<attr name="pagenr" type="int"></attr>
		<attr name="text" type="string"></attr>
	</tag>
	<tag name="cpdf_arc" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="x-coor" type="float"></attr>
		<attr name="y-coor" type="float"></attr>
		<attr name="radius" type="float"></attr>
		<attr name="start" type="float"></attr>
		<attr name="end" type="float"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_begin_text" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
	</tag>
	<tag name="cpdf_circle" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="x-coor" type="float"></attr>
		<attr name="y-coor" type="float"></attr>
		<attr name="radius" type="float"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_clip" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
	</tag>
	<tag name="cpdf_close" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
	</tag>
	<tag name="cpdf_closepath_fill_stroke" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
	</tag>
	<tag name="cpdf_closepath_stroke" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
	</tag>
	<tag name="cpdf_closepath" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
	</tag>
	<tag name="cpdf_continue_text" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="text" type="string"></attr>
	</tag>
	<tag name="cpdf_curveto" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="x1" type="float"></attr>
		<attr name="y1" type="float"></attr>
		<attr name="x2" type="float"></attr>
		<attr name="y2" type="float"></attr>
		<attr name="x3" type="float"></attr>
		<attr name="y3" type="float"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_end_text" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
	</tag>
	<tag name="cpdf_fill_stroke" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
	</tag>
	<tag name="cpdf_fill" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
	</tag>
	<tag name="cpdf_finalize_page" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="page_number" type="int"></attr>
	</tag>
	<tag name="cpdf_finalize" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
	</tag>
	<tag name="cpdf_global_set_document_limits" type="function" returnType="bool">
		<attr name="maxpages" type="int"></attr>
		<attr name="maxfonts" type="int"></attr>
		<attr name="maximages" type="int"></attr>
		<attr name="maxannotations" type="int"></attr>
		<attr name="maxobjects" type="int"></attr>
	</tag>
	<tag name="cpdf_import_jpeg" type="function" returnType="int">
		<attr name="pdf_document" type="int"></attr>
		<attr name="file_name" type="string"></attr>
		<attr name="x-coor" type="float"></attr>
		<attr name="y-coor" type="float"></attr>
		<attr name="angle" type="float"></attr>
		<attr name="width" type="float"></attr>
		<attr name="height" type="float"></attr>
		<attr name="x-scale" type="float"></attr>
		<attr name="y-scale" type="float"></attr>
		<attr name="gsave" type="int"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_lineto" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="x-coor" type="float"></attr>
		<attr name="y-coor" type="float"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_moveto" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="x-coor" type="float"></attr>
		<attr name="y-coor" type="float"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_newpath" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
	</tag>
	<tag name="cpdf_open" type="function" returnType="int">
		<attr name="compression" type="int"></attr>
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="doc_limits" type="array" status="optional"></attr>
	</tag>
	<tag name="cpdf_output_buffer" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
	</tag>
	<tag name="cpdf_page_init" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="page_number" type="int"></attr>
		<attr name="orientation" type="int"></attr>
		<attr name="height" type="float"></attr>
		<attr name="width" type="float"></attr>
		<attr name="unit" type="float" status="optional"></attr>
	</tag>
	<tag name="cpdf_place_inline_image" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="image" type="int"></attr>
		<attr name="x-coor" type="float"></attr>
		<attr name="y-coor" type="float"></attr>
		<attr name="angle" type="float"></attr>
		<attr name="width" type="float"></attr>
		<attr name="height" type="float"></attr>
		<attr name="gsave" type="int"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_rect" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="x-coor" type="float"></attr>
		<attr name="y-coor" type="float"></attr>
		<attr name="width" type="float"></attr>
		<attr name="height" type="float"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_restore" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
	</tag>
	<tag name="cpdf_rlineto" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="x-coor" type="float"></attr>
		<attr name="y-coor" type="float"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_rmoveto" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="x-coor" type="float"></attr>
		<attr name="y-coor" type="float"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_rotate_text" type="function" returnType="bool">
		<attr name="pdfdoc" type="int"></attr>
		<attr name="angle" type="float"></attr>
	</tag>
	<tag name="cpdf_rotate" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="angle" type="float"></attr>
	</tag>
	<tag name="cpdf_save_to_file" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="cpdf_save" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
	</tag>
	<tag name="cpdf_scale" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="x-scale" type="float"></attr>
		<attr name="y-scale" type="float"></attr>
	</tag>
	<tag name="cpdf_set_action_url" type="function" returnType="bool">
		<attr name="pdfdoc" type="int"></attr>
		<attr name="xll" type="float"></attr>
		<attr name="yll" type="float"></attr>
		<attr name="xur" type="float"></attr>
		<attr name="xur" type="float"></attr>
		<attr name="url" type="string"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_set_char_spacing" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="space" type="float"></attr>
	</tag>
	<tag name="cpdf_set_creator" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="creator" type="string"></attr>
	</tag>
	<tag name="cpdf_set_current_page" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="page_number" type="int"></attr>
	</tag>
	<tag name="cpdf_set_font_directories" type="function" returnType="bool">
		<attr name="pdfdoc" type="int"></attr>
		<attr name="pfmdir" type="string"></attr>
		<attr name="pfbdir" type="string"></attr>
	</tag>
	<tag name="cpdf_set_font_map_file" type="function" returnType="bool">
		<attr name="pdfdoc" type="int"></attr>
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="cpdf_set_font" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="font_name" type="string"></attr>
		<attr name="size" type="float"></attr>
		<attr name="encoding" type="string"></attr>
	</tag>
	<tag name="cpdf_set_horiz_scaling" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="scale" type="float"></attr>
	</tag>
	<tag name="cpdf_set_keywords" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="keywords" type="string"></attr>
	</tag>
	<tag name="cpdf_set_leading" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="distance" type="float"></attr>
	</tag>
	<tag name="cpdf_set_page_animation" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="transition" type="int"></attr>
		<attr name="duration" type="float"></attr>
		<attr name="direction" type="float"></attr>
		<attr name="orientation" type="int"></attr>
		<attr name="inout" type="int"></attr>
	</tag>
	<tag name="cpdf_set_subject" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="subject" type="string"></attr>
	</tag>
	<tag name="cpdf_set_text_matrix" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="matrix" type="array"></attr>
	</tag>
	<tag name="cpdf_set_text_pos" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="x-coor" type="float"></attr>
		<attr name="y-coor" type="float"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_set_text_rendering" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="rendermode" type="int"></attr>
	</tag>
	<tag name="cpdf_set_text_rise" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="value" type="float"></attr>
	</tag>
	<tag name="cpdf_set_title" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="title" type="string"></attr>
	</tag>
	<tag name="cpdf_set_viewer_preferences" type="function" returnType="bool">
		<attr name="pdfdoc" type="int"></attr>
		<attr name="preferences" type="array"></attr>
	</tag>
	<tag name="cpdf_set_word_spacing" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="space" type="float"></attr>
	</tag>
	<tag name="cpdf_setdash" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="white" type="float"></attr>
		<attr name="black" type="float"></attr>
	</tag>
	<tag name="cpdf_setflat" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="value" type="float"></attr>
	</tag>
	<tag name="cpdf_setgray_fill" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="value" type="float"></attr>
	</tag>
	<tag name="cpdf_setgray_stroke" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="gray_value" type="float"></attr>
	</tag>
	<tag name="cpdf_setgray" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="gray_value" type="float"></attr>
	</tag>
	<tag name="cpdf_setlinecap" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="value" type="int"></attr>
	</tag>
	<tag name="cpdf_setlinejoin" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="value" type="int"></attr>
	</tag>
	<tag name="cpdf_setlinewidth" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="width" type="float"></attr>
	</tag>
	<tag name="cpdf_setmiterlimit" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="value" type="float"></attr>
	</tag>
	<tag name="cpdf_setrgbcolor_fill" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="red_value" type="float"></attr>
		<attr name="green_value" type="float"></attr>
		<attr name="blue_value" type="float"></attr>
	</tag>
	<tag name="cpdf_setrgbcolor_stroke" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="red_value" type="float"></attr>
		<attr name="green_value" type="float"></attr>
		<attr name="blue_value" type="float"></attr>
	</tag>
	<tag name="cpdf_setrgbcolor" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="red_value" type="float"></attr>
		<attr name="green_value" type="float"></attr>
		<attr name="blue_value" type="float"></attr>
	</tag>
	<tag name="cpdf_show_xy" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="text" type="string"></attr>
		<attr name="x-coor" type="float"></attr>
		<attr name="y-coor" type="float"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_show" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="text" type="string"></attr>
	</tag>
	<tag name="cpdf_stringwidth" type="function" returnType="float">
		<attr name="pdf_document" type="int"></attr>
		<attr name="text" type="string"></attr>
	</tag>
	<tag name="cpdf_stroke" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
	</tag>
	<tag name="cpdf_text" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="text" type="string"></attr>
		<attr name="x-coor" type="float" status="optional"></attr>
		<attr name="y-coor" type="float" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="orientation" type="float" status="optional"></attr>
		<attr name="alignmode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_translate" type="function" returnType="bool">
		<attr name="pdf_document" type="int"></attr>
		<attr name="x-coor" type="float"></attr>
		<attr name="y-coor" type="float"></attr>
	</tag>
</tags>