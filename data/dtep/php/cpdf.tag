<!DOCTYPE tags>
<tags>
	<tag name="cpdf_add_annotation" type="function" returnType="bool" version="PHP 3&gt;= 3.0.12, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="llx" type="float" status="required"></attr>
		<attr name="lly" type="float" status="required"></attr>
		<attr name="urx" type="float" status="required"></attr>
		<attr name="ury" type="float" status="required"></attr>
		<attr name="title" type="string" status="required"></attr>
		<attr name="content" type="string" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_add_outline" type="function" returnType="int" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="lastoutline" type="int" status="required"></attr>
		<attr name="sublevel" type="int" status="required"></attr>
		<attr name="open" type="int" status="required"></attr>
		<attr name="pagenr" type="int" status="required"></attr>
		<attr name="text" type="string" status="required"></attr>
	</tag>
	<tag name="cpdf_arc" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="x-coor" type="float" status="required"></attr>
		<attr name="y-coor" type="float" status="required"></attr>
		<attr name="radius" type="float" status="required"></attr>
		<attr name="start" type="float" status="required"></attr>
		<attr name="end" type="float" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_begin_text" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
	</tag>
	<tag name="cpdf_circle" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="x-coor" type="float" status="required"></attr>
		<attr name="y-coor" type="float" status="required"></attr>
		<attr name="radius" type="float" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_clip" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
	</tag>
	<tag name="cpdf_close" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
	</tag>
	<tag name="cpdf_closepath_fill_stroke" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
	</tag>
	<tag name="cpdf_closepath_stroke" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
	</tag>
	<tag name="cpdf_closepath" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
	</tag>
	<tag name="cpdf_continue_text" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="text" type="string" status="required"></attr>
	</tag>
	<tag name="cpdf_curveto" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="x1" type="float" status="required"></attr>
		<attr name="y1" type="float" status="required"></attr>
		<attr name="x2" type="float" status="required"></attr>
		<attr name="y2" type="float" status="required"></attr>
		<attr name="x3" type="float" status="required"></attr>
		<attr name="y3" type="float" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_end_text" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
	</tag>
	<tag name="cpdf_fill_stroke" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
	</tag>
	<tag name="cpdf_fill" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
	</tag>
	<tag name="cpdf_finalize_page" type="function" returnType="bool" version="PHP 3&gt;= 3.0.10, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="page_number" type="int" status="required"></attr>
	</tag>
	<tag name="cpdf_finalize" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
	</tag>
	<tag name="cpdf_global_set_document_limits" type="function" returnType="bool" version="PHP 4 , PHP 5">
		<attr name="maxpages" type="int" status="required"></attr>
		<attr name="maxfonts" type="int" status="required"></attr>
		<attr name="maximages" type="int" status="required"></attr>
		<attr name="maxannotations" type="int" status="required"></attr>
		<attr name="maxobjects" type="int" status="required"></attr>
	</tag>
	<tag name="cpdf_import_jpeg" type="function" returnType="int" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="file_name" type="string" status="required"></attr>
		<attr name="x-coor" type="float" status="required"></attr>
		<attr name="y-coor" type="float" status="required"></attr>
		<attr name="angle" type="float" status="required"></attr>
		<attr name="width" type="float" status="required"></attr>
		<attr name="height" type="float" status="required"></attr>
		<attr name="x-scale" type="float" status="required"></attr>
		<attr name="y-scale" type="float" status="required"></attr>
		<attr name="gsave" type="int" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_lineto" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="x-coor" type="float" status="required"></attr>
		<attr name="y-coor" type="float" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_moveto" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="x-coor" type="float" status="required"></attr>
		<attr name="y-coor" type="float" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_newpath" type="function" returnType="bool" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
	</tag>
	<tag name="cpdf_open" type="function" returnType="int" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="compression" type="int" status="required"></attr>
		<attr name="filename" type="string" status="optional"></attr>
		<attr name="doc_limits" type="array" status="optional"></attr>
	</tag>
	<tag name="cpdf_output_buffer" type="function" returnType="bool" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
	</tag>
	<tag name="cpdf_page_init" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="page_number" type="int" status="required"></attr>
		<attr name="orientation" type="int" status="required"></attr>
		<attr name="height" type="float" status="required"></attr>
		<attr name="width" type="float" status="required"></attr>
		<attr name="unit" type="float" status="optional"></attr>
	</tag>
	<tag name="cpdf_place_inline_image" type="function" returnType="bool" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="image" type="int" status="required"></attr>
		<attr name="x-coor" type="float" status="required"></attr>
		<attr name="y-coor" type="float" status="required"></attr>
		<attr name="angle" type="float" status="required"></attr>
		<attr name="width" type="float" status="required"></attr>
		<attr name="height" type="float" status="required"></attr>
		<attr name="gsave" type="int" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_rect" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="x-coor" type="float" status="required"></attr>
		<attr name="y-coor" type="float" status="required"></attr>
		<attr name="width" type="float" status="required"></attr>
		<attr name="height" type="float" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_restore" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
	</tag>
	<tag name="cpdf_rlineto" type="function" returnType="bool" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="x-coor" type="float" status="required"></attr>
		<attr name="y-coor" type="float" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_rmoveto" type="function" returnType="bool" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="x-coor" type="float" status="required"></attr>
		<attr name="y-coor" type="float" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_rotate_text" type="function" returnType="bool" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="int" status="required"></attr>
		<attr name="angle" type="float" status="required"></attr>
	</tag>
	<tag name="cpdf_rotate" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="angle" type="float" status="required"></attr>
	</tag>
	<tag name="cpdf_save_to_file" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="cpdf_save" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
	</tag>
	<tag name="cpdf_scale" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="x-scale" type="float" status="required"></attr>
		<attr name="y-scale" type="float" status="required"></attr>
	</tag>
	<tag name="cpdf_set_action_url" type="function" returnType="bool" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="int" status="required"></attr>
		<attr name="xll" type="float" status="required"></attr>
		<attr name="yll" type="float" status="required"></attr>
		<attr name="xur" type="float" status="required"></attr>
		<attr name="xur" type="float" status="required"></attr>
		<attr name="url" type="string" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_set_char_spacing" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="space" type="float" status="required"></attr>
	</tag>
	<tag name="cpdf_set_creator" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="creator" type="string" status="required"></attr>
	</tag>
	<tag name="cpdf_set_current_page" type="function" returnType="bool" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="page_number" type="int" status="required"></attr>
	</tag>
	<tag name="cpdf_set_font_directories" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="pdfdoc" type="int" status="required"></attr>
		<attr name="pfmdir" type="string" status="required"></attr>
		<attr name="pfbdir" type="string" status="required"></attr>
	</tag>
	<tag name="cpdf_set_font_map_file" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.6, PHP 5">
		<attr name="pdfdoc" type="int" status="required"></attr>
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="cpdf_set_font" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="font_name" type="string" status="required"></attr>
		<attr name="size" type="float" status="required"></attr>
		<attr name="encoding" type="string" status="required"></attr>
	</tag>
	<tag name="cpdf_set_horiz_scaling" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="scale" type="float" status="required"></attr>
	</tag>
	<tag name="cpdf_set_keywords" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="keywords" type="string" status="required"></attr>
	</tag>
	<tag name="cpdf_set_leading" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="distance" type="float" status="required"></attr>
	</tag>
	<tag name="cpdf_set_page_animation" type="function" returnType="bool" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="transition" type="int" status="required"></attr>
		<attr name="duration" type="float" status="required"></attr>
		<attr name="direction" type="float" status="required"></attr>
		<attr name="orientation" type="int" status="required"></attr>
		<attr name="inout" type="int" status="required"></attr>
	</tag>
	<tag name="cpdf_set_subject" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="subject" type="string" status="required"></attr>
	</tag>
	<tag name="cpdf_set_text_matrix" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="matrix" type="array" status="required"></attr>
	</tag>
	<tag name="cpdf_set_text_pos" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="x-coor" type="float" status="required"></attr>
		<attr name="y-coor" type="float" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_set_text_rendering" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="rendermode" type="int" status="required"></attr>
	</tag>
	<tag name="cpdf_set_text_rise" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="value" type="float" status="required"></attr>
	</tag>
	<tag name="cpdf_set_title" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="title" type="string" status="required"></attr>
	</tag>
	<tag name="cpdf_set_viewer_preferences" type="function" returnType="bool" version="PHP 3&gt;= 3.0.9, PHP 4 , PHP 5">
		<attr name="pdfdoc" type="int" status="required"></attr>
		<attr name="preferences" type="array" status="required"></attr>
	</tag>
	<tag name="cpdf_set_word_spacing" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="space" type="float" status="required"></attr>
	</tag>
	<tag name="cpdf_setdash" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="white" type="float" status="required"></attr>
		<attr name="black" type="float" status="required"></attr>
	</tag>
	<tag name="cpdf_setflat" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="value" type="float" status="required"></attr>
	</tag>
	<tag name="cpdf_setgray_fill" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="value" type="float" status="required"></attr>
	</tag>
	<tag name="cpdf_setgray_stroke" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="gray_value" type="float" status="required"></attr>
	</tag>
	<tag name="cpdf_setgray" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="gray_value" type="float" status="required"></attr>
	</tag>
	<tag name="cpdf_setlinecap" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="value" type="int" status="required"></attr>
	</tag>
	<tag name="cpdf_setlinejoin" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="value" type="int" status="required"></attr>
	</tag>
	<tag name="cpdf_setlinewidth" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="width" type="float" status="required"></attr>
	</tag>
	<tag name="cpdf_setmiterlimit" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="value" type="float" status="required"></attr>
	</tag>
	<tag name="cpdf_setrgbcolor_fill" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="red_value" type="float" status="required"></attr>
		<attr name="green_value" type="float" status="required"></attr>
		<attr name="blue_value" type="float" status="required"></attr>
	</tag>
	<tag name="cpdf_setrgbcolor_stroke" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="red_value" type="float" status="required"></attr>
		<attr name="green_value" type="float" status="required"></attr>
		<attr name="blue_value" type="float" status="required"></attr>
	</tag>
	<tag name="cpdf_setrgbcolor" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="red_value" type="float" status="required"></attr>
		<attr name="green_value" type="float" status="required"></attr>
		<attr name="blue_value" type="float" status="required"></attr>
	</tag>
	<tag name="cpdf_show_xy" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="text" type="string" status="required"></attr>
		<attr name="x-coor" type="float" status="required"></attr>
		<attr name="y-coor" type="float" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_show" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="text" type="string" status="required"></attr>
	</tag>
	<tag name="cpdf_stringwidth" type="function" returnType="float" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="text" type="string" status="required"></attr>
	</tag>
	<tag name="cpdf_stroke" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
	</tag>
	<tag name="cpdf_text" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="text" type="string" status="required"></attr>
		<attr name="x-coor" type="float" status="optional"></attr>
		<attr name="y-coor" type="float" status="required"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="orientation" type="float" status="optional"></attr>
		<attr name="alignmode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_translate" type="function" returnType="bool" version="PHP 3&gt;= 3.0.8, PHP 4 , PHP 5">
		<attr name="pdf_document" type="int" status="required"></attr>
		<attr name="x-coor" type="float" status="required"></attr>
		<attr name="y-coor" type="float" status="required"></attr>
	</tag>
</tags>