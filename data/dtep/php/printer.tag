<!DOCTYPE tags>
<tags>
	<tag name="printer_abort" type="function" returnType="void" version="">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="printer_close" type="function" returnType="void" version="">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="printer_create_brush" type="function" returnType="mixed" version="">
		<attr name="style" type="int" status="required"></attr>
		<attr name="color" type="string" status="required"></attr>
	</tag>
	<tag name="printer_create_dc" type="function" returnType="void" version="">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="printer_create_font" type="function" returnType="mixed" version="">
		<attr name="face" type="string" status="required"></attr>
		<attr name="height" type="int" status="required"></attr>
		<attr name="width" type="int" status="required"></attr>
		<attr name="font_weight" type="int" status="required"></attr>
		<attr name="italic" type="bool" status="required"></attr>
		<attr name="underline" type="bool" status="required"></attr>
		<attr name="strikeout" type="bool" status="required"></attr>
		<attr name="orientation" type="int" status="required"></attr>
	</tag>
	<tag name="printer_create_pen" type="function" returnType="mixed" version="">
		<attr name="style" type="int" status="required"></attr>
		<attr name="width" type="int" status="required"></attr>
		<attr name="color" type="string" status="required"></attr>
	</tag>
	<tag name="printer_delete_brush" type="function" returnType="bool" version="">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="printer_delete_dc" type="function" returnType="bool" version="">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="printer_delete_font" type="function" returnType="bool" version="">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="printer_delete_pen" type="function" returnType="bool" version="">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="printer_draw_bmp" type="function" returnType="void" version="">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="filename" type="string" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
		<attr name="y" type="int" status="required"></attr>
		<attr name="width" type="int" status="optional"></attr>
		<attr name="height" type="int" status="required"></attr>
	</tag>
	<tag name="printer_draw_chord" type="function" returnType="void" version="">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="rec_x" type="int" status="required"></attr>
		<attr name="rec_y" type="int" status="required"></attr>
		<attr name="rec_x1" type="int" status="required"></attr>
		<attr name="rec_y1" type="int" status="required"></attr>
		<attr name="rad_x" type="int" status="required"></attr>
		<attr name="rad_y" type="int" status="required"></attr>
		<attr name="rad_x1" type="int" status="required"></attr>
		<attr name="rad_y1" type="int" status="required"></attr>
	</tag>
	<tag name="printer_draw_elipse" type="function" returnType="void" version="">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="ul_x" type="int" status="required"></attr>
		<attr name="ul_y" type="int" status="required"></attr>
		<attr name="lr_x" type="int" status="required"></attr>
		<attr name="lr_y" type="int" status="required"></attr>
	</tag>
	<tag name="printer_draw_line" type="function" returnType="void" version="">
		<attr name="printer_handle" type="resource" status="required"></attr>
		<attr name="from_x" type="int" status="required"></attr>
		<attr name="from_y" type="int" status="required"></attr>
		<attr name="to_x" type="int" status="required"></attr>
		<attr name="to_y" type="int" status="required"></attr>
	</tag>
	<tag name="printer_draw_pie" type="function" returnType="void" version="">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="rec_x" type="int" status="required"></attr>
		<attr name="rec_y" type="int" status="required"></attr>
		<attr name="rec_x1" type="int" status="required"></attr>
		<attr name="rec_y1" type="int" status="required"></attr>
		<attr name="rad1_x" type="int" status="required"></attr>
		<attr name="rad1_y" type="int" status="required"></attr>
		<attr name="rad2_x" type="int" status="required"></attr>
		<attr name="rad2_y" type="int" status="required"></attr>
	</tag>
	<tag name="printer_draw_rectangle" type="function" returnType="void" version="">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="ul_x" type="int" status="required"></attr>
		<attr name="ul_y" type="int" status="required"></attr>
		<attr name="lr_x" type="int" status="required"></attr>
		<attr name="lr_y" type="int" status="required"></attr>
	</tag>
	<tag name="printer_draw_roundrect" type="function" returnType="void" version="">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="ul_x" type="int" status="required"></attr>
		<attr name="ul_y" type="int" status="required"></attr>
		<attr name="lr_x" type="int" status="required"></attr>
		<attr name="lr_y" type="int" status="required"></attr>
		<attr name="width" type="int" status="required"></attr>
		<attr name="height" type="int" status="required"></attr>
	</tag>
	<tag name="printer_draw_text" type="function" returnType="void" version="">
		<attr name="printer_handle" type="resource" status="required"></attr>
		<attr name="text" type="string" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
		<attr name="y" type="int" status="required"></attr>
	</tag>
	<tag name="printer_end_doc" type="function" returnType="bool" version="">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="printer_end_page" type="function" returnType="bool" version="">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="printer_get_option" type="function" returnType="mixed" version="">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="option" type="string" status="required"></attr>
	</tag>
	<tag name="printer_list" type="function" returnType="array" version="">
		<attr name="enumtype" type="int" status="required"></attr>
		<attr name="name" type="string" status="optional"></attr>
		<attr name="level" type="int" status="optional"></attr>
	</tag>
	<tag name="printer_logical_fontheight" type="function" returnType="int" version="">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="height" type="int" status="required"></attr>
	</tag>
	<tag name="printer_open" type="function" returnType="mixed" version="">
		<attr name="devicename" type="string" status="optional"></attr>
	</tag>
	<tag name="printer_select_brush" type="function" returnType="void" version="">
		<attr name="printer_handle" type="resource" status="required"></attr>
		<attr name="brush_handle" type="resource" status="required"></attr>
	</tag>
	<tag name="printer_select_font" type="function" returnType="void" version="">
		<attr name="printer_handle" type="resource" status="required"></attr>
		<attr name="font_handle" type="resource" status="required"></attr>
	</tag>
	<tag name="printer_select_pen" type="function" returnType="void" version="">
		<attr name="printer_handle" type="resource" status="required"></attr>
		<attr name="pen_handle" type="resource" status="required"></attr>
	</tag>
	<tag name="printer_set_option" type="function" returnType="bool" version="">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="option" type="int" status="required"></attr>
		<attr name="value" type="mixed" status="required"></attr>
	</tag>
	<tag name="printer_start_doc" type="function" returnType="bool" version="">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="document" type="string" status="optional"></attr>
	</tag>
	<tag name="printer_start_page" type="function" returnType="bool" version="">
		<attr name="handle" type="resource" status="required"></attr>
	</tag>
	<tag name="printer_write" type="function" returnType="bool" version="">
		<attr name="handle" type="resource" status="required"></attr>
		<attr name="content" type="string" status="required"></attr>
	</tag>
</tags>