<!DOCTYPE tags>
<tags>	<tag name="cpdf_add_annotation" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="llx" type="float" status="optional"></attr>
		<attr name="lly" type="float" status="optional"></attr>
		<attr name="urx" type="float" status="optional"></attr>
		<attr name="ury" type="float" status="optional"></attr>
		<attr name="title" type="string" status="optional"></attr>
		<attr name="content" type="string" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_add_outline" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="text" type="string" status="optional"></attr>
	</tag>
	<tag name="cpdf_arc" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="x-coor" type="float" status="optional"></attr>
		<attr name="y-coor" type="float" status="optional"></attr>
		<attr name="radius" type="float" status="optional"></attr>
		<attr name="start" type="float" status="optional"></attr>
		<attr name="end" type="float" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_begin_text" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_circle" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="x-coor" type="float" status="optional"></attr>
		<attr name="y-coor" type="float" status="optional"></attr>
		<attr name="radius" type="float" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_clip" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_close" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_closepath_fill_stroke" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_closepath_stroke" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_closepath" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_continue_text" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="text" type="string" status="optional"></attr>
	</tag>
	<tag name="cpdf_curveto" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="x1" type="float" status="optional"></attr>
		<attr name="y1" type="float" status="optional"></attr>
		<attr name="x2" type="float" status="optional"></attr>
		<attr name="y2" type="float" status="optional"></attr>
		<attr name="x3" type="float" status="optional"></attr>
		<attr name="y3" type="float" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_end_text" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_fill_stroke" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_fill" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_finalize_page" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="page number" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_finalize" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_global_set_document_limits" type="function" returnType="void">
		<attr name="maxpages" type="int" status="optional"></attr>
		<attr name="maxfonts" type="int" status="optional"></attr>
		<attr name="maximages" type="int" status="optional"></attr>
		<attr name="maxannotations" type="int" status="optional"></attr>
		<attr name="maxobjects" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_import_jpeg" type="function" returnType="int">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="file name" type="string" status="optional"></attr>
		<attr name="x-coor" type="float" status="optional"></attr>
		<attr name="y-coor" type="float" status="optional"></attr>
		<attr name="angle" type="float" status="optional"></attr>
		<attr name="width" type="float" status="optional"></attr>
		<attr name="height" type="float" status="optional"></attr>
		<attr name="x-scale" type="float" status="optional"></attr>
		<attr name="y-scale" type="float" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_lineto" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="x-coor" type="float" status="optional"></attr>
		<attr name="y-coor" type="float" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_moveto" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="x-coor" type="float" status="optional"></attr>
		<attr name="y-coor" type="float" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_newpath" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_open" type="function" returnType="int">
		<attr name="compression" type="int" status="optional"></attr>
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="cpdf_output_buffer" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_page_init" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="page number" type="int" status="optional"></attr>
		<attr name="orientation" type="int" status="optional"></attr>
		<attr name="height" type="float" status="optional"></attr>
		<attr name="width" type="float" status="optional"></attr>
		<attr name="unit" type="float" status="optional"></attr>
	</tag>
	<tag name="cpdf_place_inline_image" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="image" type="int" status="optional"></attr>
		<attr name="x-coor" type="float" status="optional"></attr>
		<attr name="y-coor" type="float" status="optional"></attr>
		<attr name="angle" type="float" status="optional"></attr>
		<attr name="width" type="float" status="optional"></attr>
		<attr name="height" type="float" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_rect" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="x-coor" type="float" status="optional"></attr>
		<attr name="y-coor" type="float" status="optional"></attr>
		<attr name="width" type="float" status="optional"></attr>
		<attr name="height" type="float" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_restore" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_rlineto" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="x-coor" type="float" status="optional"></attr>
		<attr name="y-coor" type="float" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_rmoveto" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="x-coor" type="float" status="optional"></attr>
		<attr name="y-coor" type="float" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_rotate_text" type="function" returnType="void">
		<attr name="pdfdoc" type="int" status="optional"></attr>
		<attr name="angle" type="float" status="optional"></attr>
	</tag>
	<tag name="cpdf_rotate" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="angle" type="float" status="optional"></attr>
	</tag>
	<tag name="cpdf_save_to_file" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="cpdf_save" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_scale" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="x-scale" type="float" status="optional"></attr>
		<attr name="y-scale" type="float" status="optional"></attr>
	</tag>
	<tag name="cpdf_set_action_url" type="function" returnType="void">
		<attr name="pdfdoc" type="int" status="optional"></attr>
		<attr name="xll" type="float" status="optional"></attr>
		<attr name="yll" type="float" status="optional"></attr>
		<attr name="xur" type="float" status="optional"></attr>
		<attr name="xur" type="float" status="optional"></attr>
		<attr name="url" type="string" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_set_char_spacing" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="space" type="float" status="optional"></attr>
	</tag>
	<tag name="cpdf_set_creator" type="function" returnType="void">
		<attr name="creator" type="string" status="optional"></attr>
	</tag>
	<tag name="cpdf_set_current_page" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="page number" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_set_font_directories" type="function" returnType="void">
		<attr name="pdfdoc" type="int" status="optional"></attr>
		<attr name="pfmdir" type="string" status="optional"></attr>
		<attr name="pfbdir" type="string" status="optional"></attr>
	</tag>
	<tag name="cpdf_set_font_map_file" type="function" returnType="void">
		<attr name="pdfdoc" type="int" status="optional"></attr>
		<attr name="filename" type="string" status="optional"></attr>
	</tag>
	<tag name="cpdf_set_font" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="font name" type="string" status="optional"></attr>
		<attr name="size" type="float" status="optional"></attr>
		<attr name="encoding" type="string" status="optional"></attr>
	</tag>
	<tag name="cpdf_set_horiz_scaling" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="scale" type="float" status="optional"></attr>
	</tag>
	<tag name="cpdf_set_keywords" type="function" returnType="void">
		<attr name="keywords" type="string" status="optional"></attr>
	</tag>
	<tag name="cpdf_set" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="distance" type="float" status="optional"></attr>
	</tag>
	<tag name="cpdf_set_page_animation" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="transition" type="int" status="optional"></attr>
		<attr name="duration" type="float" status="optional"></attr>
	</tag>
	<tag name="cpdf_set_subject" type="function" returnType="void">
		<attr name="subject" type="string" status="optional"></attr>
	</tag>
	<tag name="cpdf_set_text_matrix" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="matrix" type="array" status="optional"></attr>
	</tag>
	<tag name="cpdf_set_text_pos" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="x-coor" type="float" status="optional"></attr>
		<attr name="y-coor" type="float" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_set_text_rendering" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_set_text_rise" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="value" type="float" status="optional"></attr>
	</tag>
	<tag name="cpdf_set_title" type="function" returnType="void">
		<attr name="title" type="string" status="optional"></attr>
	</tag>
	<tag name="cpdf_set_viewer_preferences" type="function" returnType="void">
		<attr name="pdfdoc" type="int" status="optional"></attr>
		<attr name="preferences" type="array" status="optional"></attr>
	</tag>
	<tag name="cpdf_set_word_spacing" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="space" type="float" status="optional"></attr>
	</tag>
	<tag name="cpdf_setdash" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="white" type="float" status="optional"></attr>
		<attr name="black" type="float" status="optional"></attr>
	</tag>
	<tag name="cpdf_setflat" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="value" type="float" status="optional"></attr>
	</tag>
	<tag name="cpdf_setgray_fill" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="value" type="float" status="optional"></attr>
	</tag>
	<tag name="cpdf_setgray_stroke" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="gray value" type="float" status="optional"></attr>
	</tag>
	<tag name="cpdf_setgray" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="gray value" type="float" status="optional"></attr>
	</tag>
	<tag name="cpdf_setlinecap" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="value" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_setlinejoin" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="value" type="long" status="optional"></attr>
	</tag>
	<tag name="cpdf_setlinewidth" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="width" type="float" status="optional"></attr>
	</tag>
	<tag name="cpdf_setmiterlimit" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="value" type="float" status="optional"></attr>
	</tag>
	<tag name="cpdf_setrgbcolor_fill" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="red value" type="float" status="optional"></attr>
		<attr name="green value" type="float" status="optional"></attr>
		<attr name="blue value" type="float" status="optional"></attr>
	</tag>
	<tag name="cpdf_setrgbcolor_stroke" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="red value" type="float" status="optional"></attr>
		<attr name="green value" type="float" status="optional"></attr>
		<attr name="blue value" type="float" status="optional"></attr>
	</tag>
	<tag name="cpdf_setrgbcolor" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="red value" type="float" status="optional"></attr>
		<attr name="green value" type="float" status="optional"></attr>
		<attr name="blue value" type="float" status="optional"></attr>
	</tag>
	<tag name="cpdf_show_xy" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="text" type="string" status="optional"></attr>
		<attr name="x-coor" type="float" status="optional"></attr>
		<attr name="y-coor" type="float" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_show" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="text" type="string" status="optional"></attr>
	</tag>
	<tag name="cpdf_stringwidth" type="function" returnType="float">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="text" type="string" status="optional"></attr>
	</tag>
	<tag name="cpdf_stroke" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_text" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="text" type="string" status="optional"></attr>
		<attr name="x-coor" type="float" status="optional"></attr>
		<attr name="y-coor" type="float" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
		<attr name="orientation" type="float" status="optional"></attr>
		<attr name="alignmode" type="int" status="optional"></attr>
	</tag>
	<tag name="cpdf_translate" type="function" returnType="void">
		<attr name="pdf document" type="int" status="optional"></attr>
		<attr name="x-coor" type="float" status="optional"></attr>
		<attr name="y-coor" type="float" status="optional"></attr>
		<attr name="mode" type="int" status="optional"></attr>
	</tag>
</tags>
