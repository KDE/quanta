<!DOCTYPE tags>
<tags>
	<tag name="ncurses_addch" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="ch" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_addchnstr" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="s" type="string" status="required"></attr>
		<attr name="n" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_addchstr" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="s" type="string" status="required"></attr>
	</tag>
	<tag name="ncurses_addnstr" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="s" type="string" status="required"></attr>
		<attr name="n" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_addstr" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="text" type="string" status="required"></attr>
	</tag>
	<tag name="ncurses_assume_default_colors" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="fg" type="int" status="required"></attr>
		<attr name="bg" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_attroff" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="attributes" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_attron" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="attributes" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_attrset" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="attributes" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_baudrate" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_beep" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_bkgd" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="attrchar" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_bkgdset" type="function" returnType="void" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="attrchar" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_border" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="left" type="int" status="required"></attr>
		<attr name="right" type="int" status="required"></attr>
		<attr name="top" type="int" status="required"></attr>
		<attr name="bottom" type="int" status="required"></attr>
		<attr name="tl_corner" type="int" status="required"></attr>
		<attr name="tr_corner" type="int" status="required"></attr>
		<attr name="bl_corner" type="int" status="required"></attr>
		<attr name="br_corner" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_bottom_panel" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="panel" type="resource" status="required"></attr>
	</tag>
	<tag name="ncurses_can_change_color" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_cbreak" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_clear" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_clrtobot" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_clrtoeol" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_color_content" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="color" type="int" status="required"></attr>
		<attr name="&amp;r" type="int" status="required"></attr>
		<attr name="&amp;g" type="int" status="required"></attr>
		<attr name="&amp;b" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_color_set" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="pair" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_curs_set" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="visibility" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_def_prog_mode" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_def_shell_mode" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_define_key" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="definition" type="string" status="required"></attr>
		<attr name="keycode" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_del_panel" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="panel" type="resource" status="required"></attr>
	</tag>
	<tag name="ncurses_delay_output" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="milliseconds" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_delch" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_deleteln" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_delwin" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
	</tag>
	<tag name="ncurses_doupdate" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_echo" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_echochar" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="character" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_end" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_erase" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_erasechar" type="function" returnType="string" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_filter" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_flash" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_flushinp" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_getch" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_getmaxyx" type="function" returnType="void" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
		<attr name="&amp;y" type="int" status="required"></attr>
		<attr name="&amp;x" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_getmouse" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="&amp;mevent" type="array" status="required"></attr>
	</tag>
	<tag name="ncurses_getyx" type="function" returnType="void" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
		<attr name="&amp;y" type="int" status="required"></attr>
		<attr name="&amp;x" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_halfdelay" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="tenth" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_has_colors" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_has_ic" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_has_il" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_has_key" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="keycode" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_hide_panel" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="panel" type="resource" status="required"></attr>
	</tag>
	<tag name="ncurses_hline" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="charattr" type="int" status="required"></attr>
		<attr name="n" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_inch" type="function" returnType="string" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_init_color" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="color" type="int" status="required"></attr>
		<attr name="r" type="int" status="required"></attr>
		<attr name="g" type="int" status="required"></attr>
		<attr name="b" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_init_pair" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="pair" type="int" status="required"></attr>
		<attr name="fg" type="int" status="required"></attr>
		<attr name="bg" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_init" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_insch" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="character" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_insdelln" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="count" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_insertln" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_insstr" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="text" type="string" status="required"></attr>
	</tag>
	<tag name="ncurses_instr" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="&amp;buffer" type="string" status="required"></attr>
	</tag>
	<tag name="ncurses_isendwin" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_keyok" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="keycode" type="int" status="required"></attr>
		<attr name="enable" type="bool" status="required"></attr>
	</tag>
	<tag name="ncurses_keypad" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
		<attr name="bf" type="bool" status="required"></attr>
	</tag>
	<tag name="ncurses_killchar" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_longname" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
	</tag>
	<tag name="ncurses_meta" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
		<attr name="8bit" type="bool" status="required"></attr>
	</tag>
	<tag name="ncurses_mouse_trafo" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="&amp;y" type="int" status="required"></attr>
		<attr name="&amp;x" type="int" status="required"></attr>
		<attr name="toscreen" type="bool" status="required"></attr>
	</tag>
	<tag name="ncurses_mouseinterval" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="milliseconds" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_mousemask" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="newmask" type="int" status="required"></attr>
		<attr name="&amp;oldmask" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_move_panel" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="panel" type="resource" status="required"></attr>
		<attr name="startx" type="int" status="required"></attr>
		<attr name="starty" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_move" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="y" type="int" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_mvaddch" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="y" type="int" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
		<attr name="c" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_mvaddchnstr" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="y" type="int" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
		<attr name="s" type="string" status="required"></attr>
		<attr name="n" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_mvaddchstr" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="y" type="int" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
		<attr name="s" type="string" status="required"></attr>
	</tag>
	<tag name="ncurses_mvaddnstr" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="y" type="int" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
		<attr name="s" type="string" status="required"></attr>
		<attr name="n" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_mvaddstr" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="y" type="int" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
		<attr name="s" type="string" status="required"></attr>
	</tag>
	<tag name="ncurses_mvcur" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="old_y" type="int" status="required"></attr>
		<attr name="old_x" type="int" status="required"></attr>
		<attr name="new_y" type="int" status="required"></attr>
		<attr name="new_x" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_mvdelch" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="y" type="int" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_mvgetch" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="y" type="int" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_mvhline" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="y" type="int" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
		<attr name="attrchar" type="int" status="required"></attr>
		<attr name="n" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_mvinch" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="y" type="int" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_mvvline" type="function" returnType="int" version="">
		<attr name="y" type="int" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
		<attr name="attrchar" type="int" status="required"></attr>
		<attr name="n" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_mvwaddstr" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
		<attr name="y" type="int" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
		<attr name="text" type="string" status="required"></attr>
	</tag>
	<tag name="ncurses_napms" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="milliseconds" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_new_panel" type="function" returnType="resource" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
	</tag>
	<tag name="ncurses_newpad" type="function" returnType="resource" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="rows" type="int" status="required"></attr>
		<attr name="cols" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_newwin" type="function" returnType="resource" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="rows" type="int" status="required"></attr>
		<attr name="cols" type="int" status="required"></attr>
		<attr name="y" type="int" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_nl" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_nocbreak" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_noecho" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_nonl" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_noqiflush" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_noraw" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_pair_content" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="pair" type="int" status="required"></attr>
		<attr name="&amp;f" type="int" status="required"></attr>
		<attr name="&amp;b" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_panel_above" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="panel" type="resource" status="required"></attr>
	</tag>
	<tag name="ncurses_panel_below" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="panel" type="resource" status="required"></attr>
	</tag>
	<tag name="ncurses_panel_window" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="panel" type="resource" status="required"></attr>
	</tag>
	<tag name="ncurses_pnoutrefresh" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="pad" type="resource" status="required"></attr>
		<attr name="pminrow" type="int" status="required"></attr>
		<attr name="pmincol" type="int" status="required"></attr>
		<attr name="sminrow" type="int" status="required"></attr>
		<attr name="smincol" type="int" status="required"></attr>
		<attr name="smaxrow" type="int" status="required"></attr>
		<attr name="smaxcol" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_prefresh" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="pad" type="resource" status="required"></attr>
		<attr name="pminrow" type="int" status="required"></attr>
		<attr name="pmincol" type="int" status="required"></attr>
		<attr name="sminrow" type="int" status="required"></attr>
		<attr name="smincol" type="int" status="required"></attr>
		<attr name="smaxrow" type="int" status="required"></attr>
		<attr name="smaxcol" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_putp" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="text" type="string" status="required"></attr>
	</tag>
	<tag name="ncurses_qiflush" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_raw" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_refresh" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="ch" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_replace_panel" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="panel" type="resource" status="required"></attr>
		<attr name="window" type="resource" status="required"></attr>
	</tag>
	<tag name="ncurses_reset_prog_mode" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
	</tag>
	<tag name="ncurses_reset_shell_mode" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
	</tag>
	<tag name="ncurses_resetty" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_savetty" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_scr_dump" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="ncurses_scr_init" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="ncurses_scr_restore" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="ncurses_scr_set" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="ncurses_scrl" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="count" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_show_panel" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="panel" type="resource" status="required"></attr>
	</tag>
	<tag name="ncurses_slk_attr" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_slk_attroff" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="intarg" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_slk_attron" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="intarg" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_slk_attrset" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="intarg" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_slk_clear" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_slk_color" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="intarg" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_slk_init" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="format" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_slk_noutrefresh" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_slk_refresh" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_slk_restore" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_slk_set" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="labelnr" type="int" status="required"></attr>
		<attr name="label" type="string" status="required"></attr>
		<attr name="format" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_slk_touch" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_standend" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_standout" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_start_color" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_termattrs" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_termname" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
	</tag>
	<tag name="ncurses_timeout" type="function" returnType="void" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="millisec" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_top_panel" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="panel" type="resource" status="required"></attr>
	</tag>
	<tag name="ncurses_typeahead" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="fd" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_ungetch" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="keycode" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_ungetmouse" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="mevent" type="array" status="required"></attr>
	</tag>
	<tag name="ncurses_update_panels" type="function" returnType="void" version="PHP 4 &gt;= 4.3.0, PHP 5">
	</tag>
	<tag name="ncurses_use_default_colors" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
	</tag>
	<tag name="ncurses_use_env" type="function" returnType="void" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="flag" type="bool" status="required"></attr>
	</tag>
	<tag name="ncurses_use_extended_names" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="flag" type="bool" status="required"></attr>
	</tag>
	<tag name="ncurses_vidattr" type="function" returnType="int" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="intarg" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_vline" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="charattr" type="int" status="required"></attr>
		<attr name="n" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_waddch" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
		<attr name="ch" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_waddstr" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
		<attr name="str" type="string" status="required"></attr>
		<attr name="n" type="int" status="optional"></attr>
	</tag>
	<tag name="ncurses_wattroff" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
		<attr name="attrs" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_wattron" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
		<attr name="attrs" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_wattrset" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
		<attr name="attrs" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_wborder" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
		<attr name="left" type="int" status="required"></attr>
		<attr name="right" type="int" status="required"></attr>
		<attr name="top" type="int" status="required"></attr>
		<attr name="bottom" type="int" status="required"></attr>
		<attr name="tl_corner" type="int" status="required"></attr>
		<attr name="tr_corner" type="int" status="required"></attr>
		<attr name="bl_corner" type="int" status="required"></attr>
		<attr name="br_corner" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_wclear" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
	</tag>
	<tag name="ncurses_wcolor_set" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
		<attr name="color_pair" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_werase" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
	</tag>
	<tag name="ncurses_wgetch" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
	</tag>
	<tag name="ncurses_whline" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
		<attr name="charattr" type="int" status="required"></attr>
		<attr name="n" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_wmouse_trafo" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
		<attr name="&amp;y" type="int" status="required"></attr>
		<attr name="&amp;x" type="int" status="required"></attr>
		<attr name="toscreen" type="bool" status="required"></attr>
	</tag>
	<tag name="ncurses_wmove" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
		<attr name="y" type="int" status="required"></attr>
		<attr name="x" type="int" status="required"></attr>
	</tag>
	<tag name="ncurses_wnoutrefresh" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
	</tag>
	<tag name="ncurses_wrefresh" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
	</tag>
	<tag name="ncurses_wstandend" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
	</tag>
	<tag name="ncurses_wstandout" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
	</tag>
	<tag name="ncurses_wvline" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="window" type="resource" status="required"></attr>
		<attr name="charattr" type="int" status="required"></attr>
		<attr name="n" type="int" status="required"></attr>
	</tag>
</tags>