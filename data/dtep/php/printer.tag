<!DOCTYPE tags>
<tags>
	<tag name="printer_abort" type="function" returnType="void">
		<attr name="handle" type="resource"></attr>
	</tag>
	<tag name="printer_close" type="function" returnType="void">
		<attr name="handle" type="resource"></attr>
	</tag>
	<tag name="printer_create_brush" type="function" returnType="mixed">
		<attr name="style" type="int"></attr>
		<attr name="color" type="string"></attr>
	</tag>
	<tag name="printer_create_dc" type="function" returnType="void">
		<attr name="handle" type="resource"></attr>
	</tag>
	<tag name="printer_create_font" type="function" returnType="mixed">
		<attr name="face" type="string"></attr>
		<attr name="height" type="int"></attr>
		<attr name="width" type="int"></attr>
		<attr name="font_weight" type="int"></attr>
		<attr name="italic" type="bool"></attr>
		<attr name="underline" type="bool"></attr>
		<attr name="strikeout" type="bool"></attr>
		<attr name="orientation" type="int"></attr>
	</tag>
	<tag name="printer_create_pen" type="function" returnType="mixed">
		<attr name="style" type="int"></attr>
		<attr name="width" type="int"></attr>
		<attr name="color" type="string"></attr>
	</tag>
	<tag name="printer_delete_brush" type="function" returnType="bool">
		<attr name="handle" type="resource"></attr>
	</tag>
	<tag name="printer_delete_dc" type="function" returnType="bool">
		<attr name="handle" type="resource"></attr>
	</tag>
	<tag name="printer_delete_font" type="function" returnType="bool">
		<attr name="handle" type="resource"></attr>
	</tag>
	<tag name="printer_delete_pen" type="function" returnType="bool">
		<attr name="handle" type="resource"></attr>
	</tag>
	<tag name="printer_draw_bmp" type="function" returnType="void">
		<attr name="handle" type="resource"></attr>
		<attr name="filename" type="string"></attr>
		<attr name="x" type="int"></attr>
		<attr name="y" type="int"></attr>
		<attr name="width" type="int" status="optional"></attr>
		<attr name="height" type="int" status="optional"></attr>
	</tag>
	<tag name="printer_draw_chord" type="function" returnType="void">
		<attr name="handle" type="resource"></attr>
		<attr name="rec_x" type="int"></attr>
		<attr name="rec_y" type="int"></attr>
		<attr name="rec_x1" type="int"></attr>
		<attr name="rec_y1" type="int"></attr>
		<attr name="rad_x" type="int"></attr>
		<attr name="rad_y" type="int"></attr>
		<attr name="rad_x1" type="int"></attr>
		<attr name="rad_y1" type="int"></attr>
	</tag>
	<tag name="printer_draw_elipse" type="function" returnType="void">
		<attr name="handle" type="resource"></attr>
		<attr name="ul_x" type="int"></attr>
		<attr name="ul_y" type="int"></attr>
		<attr name="lr_x" type="int"></attr>
		<attr name="lr_y" type="int"></attr>
	</tag>
	<tag name="printer_draw_line" type="function" returnType="void">
		<attr name="printer_handle" type="resource"></attr>
		<attr name="from_x" type="int"></attr>
		<attr name="from_y" type="int"></attr>
		<attr name="to_x" type="int"></attr>
		<attr name="to_y" type="int"></attr>
	</tag>
	<tag name="printer_draw_pie" type="function" returnType="void">
		<attr name="handle" type="resource"></attr>
		<attr name="rec_x" type="int"></attr>
		<attr name="rec_y" type="int"></attr>
		<attr name="rec_x1" type="int"></attr>
		<attr name="rec_y1" type="int"></attr>
		<attr name="rad1_x" type="int"></attr>
		<attr name="rad1_y" type="int"></attr>
		<attr name="rad2_x" type="int"></attr>
		<attr name="rad2_y" type="int"></attr>
	</tag>
	<tag name="printer_draw_rectangle" type="function" returnType="void">
		<attr name="handle" type="resource"></attr>
		<attr name="ul_x" type="int"></attr>
		<attr name="ul_y" type="int"></attr>
		<attr name="lr_x" type="int"></attr>
		<attr name="lr_y" type="int"></attr>
	</tag>
	<tag name="printer_draw_roundrect" type="function" returnType="void">
		<attr name="handle" type="resource"></attr>
		<attr name="ul_x" type="int"></attr>
		<attr name="ul_y" type="int"></attr>
		<attr name="lr_x" type="int"></attr>
		<attr name="lr_y" type="int"></attr>
		<attr name="width" type="int"></attr>
		<attr name="height" type="int"></attr>
	</tag>
	<tag name="printer_draw_text" type="function" returnType="void">
		<attr name="printer_handle" type="resource"></attr>
		<attr name="text" type="string"></attr>
		<attr name="x" type="int"></attr>
		<attr name="y" type="int"></attr>
	</tag>
	<tag name="printer_end_doc" type="function" returnType="bool">
		<attr name="handle" type="resource"></attr>
	</tag>
	<tag name="printer_end_page" type="function" returnType="bool">
		<attr name="handle" type="resource"></attr>
	</tag>
	<tag name="printer_get_option" type="function" returnType="mixed">
		<attr name="handle" type="resource"></attr>
		<attr name="option" type="string"></attr>
	</tag>
	<tag name="printer_list" type="function" returnType="array">
		<attr name="enumtype" type="int"></attr>
		<attr name="name" type="string" status="optional"></attr>
		<attr name="level" type="int" status="optional"></attr>
	</tag>
	<tag name="printer_logical_fontheight" type="function" returnType="int">
		<attr name="handle" type="resource"></attr>
		<attr name="height" type="int"></attr>
	</tag>
	<tag name="printer_open" type="function" returnType="mixed">
		<attr name="devicename" type="string" status="optional"></attr>
	</tag>
	<tag name="printer_select_brush" type="function" returnType="void">
		<attr name="printer_handle" type="resource"></attr>
		<attr name="brush_handle" type="resource"></attr>
	</tag>
	<tag name="printer_select_font" type="function" returnType="void">
		<attr name="printer_handle" type="resource"></attr>
		<attr name="font_handle" type="resource"></attr>
	</tag>
	<tag name="printer_select_pen" type="function" returnType="void">
		<attr name="printer_handle" type="resource"></attr>
		<attr name="pen_handle" type="resource"></attr>
	</tag>
	<tag name="printer_set_option" type="function" returnType="bool">
		<attr name="handle" type="resource"></attr>
		<attr name="option" type="int"></attr>
		<attr name="value" type="mixed"></attr>
	</tag>
	<tag name="printer_start_doc" type="function" returnType="bool">
		<attr name="handle" type="resource"></attr>
		<attr name="document" type="string" status="optional"></attr>
	</tag>
	<tag name="printer_start_page" type="function" returnType="bool">
		<attr name="handle" type="resource"></attr>
	</tag>
	<tag name="printer_write" type="function" returnType="bool">
		<attr name="handle" type="resource"></attr>
		<attr name="content" type="string"></attr>
	</tag>
</tags>