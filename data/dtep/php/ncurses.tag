<!DOCTYPE tags>
<tags>
	<tag name="ncurses_addch" type="function" returnType="int">
		<attr name="ch" type="int"></attr>
	</tag>
	<tag name="ncurses_addchnstr" type="function" returnType="int">
		<attr name="s" type="string"></attr>
		<attr name="n" type="int"></attr>
	</tag>
	<tag name="ncurses_addchstr" type="function" returnType="int">
		<attr name="s" type="string"></attr>
	</tag>
	<tag name="ncurses_addnstr" type="function" returnType="int">
		<attr name="s" type="string"></attr>
		<attr name="n" type="int"></attr>
	</tag>
	<tag name="ncurses_addstr" type="function" returnType="int">
		<attr name="text" type="string"></attr>
	</tag>
	<tag name="ncurses_assume_default_colors" type="function" returnType="int">
		<attr name="fg" type="int"></attr>
		<attr name="bg" type="int"></attr>
	</tag>
	<tag name="ncurses_attroff" type="function" returnType="int">
		<attr name="attributes" type="int"></attr>
	</tag>
	<tag name="ncurses_attron" type="function" returnType="int">
		<attr name="attributes" type="int"></attr>
	</tag>
	<tag name="ncurses_attrset" type="function" returnType="int">
		<attr name="attributes" type="int"></attr>
	</tag>
	<tag name="ncurses_baudrate" type="function" returnType="int">
	</tag>
	<tag name="ncurses_beep" type="function" returnType="int">
	</tag>
	<tag name="ncurses_bkgd" type="function" returnType="int">
		<attr name="attrchar" type="int"></attr>
	</tag>
	<tag name="ncurses_bkgdset" type="function" returnType="void">
		<attr name="attrchar" type="int"></attr>
	</tag>
	<tag name="ncurses_border" type="function" returnType="int">
		<attr name="left" type="int"></attr>
		<attr name="right" type="int"></attr>
		<attr name="top" type="int"></attr>
		<attr name="bottom" type="int"></attr>
		<attr name="tl_corner" type="int"></attr>
		<attr name="tr_corner" type="int"></attr>
		<attr name="bl_corner" type="int"></attr>
		<attr name="br_corner" type="int"></attr>
	</tag>
	<tag name="ncurses_bottom_panel" type="function" returnType="int">
		<attr name="panel" type="resource"></attr>
	</tag>
	<tag name="ncurses_can_change_color" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_cbreak" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_clear" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_clrtobot" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_clrtoeol" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_color_content" type="function" returnType="int">
		<attr name="color" type="int"></attr>
		<attr name="&amp;r" type="int"></attr>
		<attr name="&amp;g" type="int"></attr>
		<attr name="&amp;b" type="int"></attr>
	</tag>
	<tag name="ncurses_color_set" type="function" returnType="int">
		<attr name="pair" type="int"></attr>
	</tag>
	<tag name="ncurses_curs_set" type="function" returnType="int">
		<attr name="visibility" type="int"></attr>
	</tag>
	<tag name="ncurses_def_prog_mode" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_def_shell_mode" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_define_key" type="function" returnType="int">
		<attr name="definition" type="string"></attr>
		<attr name="keycode" type="int"></attr>
	</tag>
	<tag name="ncurses_del_panel" type="function" returnType="int">
		<attr name="panel" type="resource"></attr>
	</tag>
	<tag name="ncurses_delay_output" type="function" returnType="int">
		<attr name="milliseconds" type="int"></attr>
	</tag>
	<tag name="ncurses_delch" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_deleteln" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_delwin" type="function" returnType="int">
		<attr name="window" type="resource"></attr>
	</tag>
	<tag name="ncurses_doupdate" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_echo" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_echochar" type="function" returnType="int">
		<attr name="character" type="int"></attr>
	</tag>
	<tag name="ncurses_end" type="function" returnType="int">
	</tag>
	<tag name="ncurses_erase" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_erasechar" type="function" returnType="string">
	</tag>
	<tag name="ncurses_filter" type="function" returnType="int">
	</tag>
	<tag name="ncurses_flash" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_flushinp" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_getch" type="function" returnType="int">
	</tag>
	<tag name="ncurses_getmaxyx" type="function" returnType="void">
		<attr name="window" type="resource"></attr>
		<attr name="&amp;y" type="int"></attr>
		<attr name="&amp;x" type="int"></attr>
	</tag>
	<tag name="ncurses_getmouse" type="function" returnType="bool">
		<attr name="&amp;mevent" type="array"></attr>
	</tag>
	<tag name="ncurses_getyx" type="function" returnType="void">
		<attr name="window" type="resource"></attr>
		<attr name="&amp;y" type="int"></attr>
		<attr name="&amp;x" type="int"></attr>
	</tag>
	<tag name="ncurses_halfdelay" type="function" returnType="int">
		<attr name="tenth" type="int"></attr>
	</tag>
	<tag name="ncurses_has_colors" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_has_ic" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_has_il" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_has_key" type="function" returnType="int">
		<attr name="keycode" type="int"></attr>
	</tag>
	<tag name="ncurses_hide_panel" type="function" returnType="int">
		<attr name="panel" type="resource"></attr>
	</tag>
	<tag name="ncurses_hline" type="function" returnType="int">
		<attr name="charattr" type="int"></attr>
		<attr name="n" type="int"></attr>
	</tag>
	<tag name="ncurses_inch" type="function" returnType="string">
	</tag>
	<tag name="ncurses_init_color" type="function" returnType="int">
		<attr name="color" type="int"></attr>
		<attr name="r" type="int"></attr>
		<attr name="g" type="int"></attr>
		<attr name="b" type="int"></attr>
	</tag>
	<tag name="ncurses_init_pair" type="function" returnType="int">
		<attr name="pair" type="int"></attr>
		<attr name="fg" type="int"></attr>
		<attr name="bg" type="int"></attr>
	</tag>
	<tag name="ncurses_init" type="function" returnType="int">
	</tag>
	<tag name="ncurses_insch" type="function" returnType="int">
		<attr name="character" type="int"></attr>
	</tag>
	<tag name="ncurses_insdelln" type="function" returnType="int">
		<attr name="count" type="int"></attr>
	</tag>
	<tag name="ncurses_insertln" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_insstr" type="function" returnType="int">
		<attr name="text" type="string"></attr>
	</tag>
	<tag name="ncurses_instr" type="function" returnType="int">
		<attr name="&amp;buffer" type="string"></attr>
	</tag>
	<tag name="ncurses_isendwin" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_keyok" type="function" returnType="int">
		<attr name="keycode" type="int"></attr>
		<attr name="enable" type="bool"></attr>
	</tag>
	<tag name="ncurses_keypad" type="function" returnType="int">
		<attr name="window" type="resource"></attr>
		<attr name="bf" type="bool"></attr>
	</tag>
	<tag name="ncurses_killchar" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_longname" type="function" returnType="string">
	</tag>
	<tag name="ncurses_meta" type="function" returnType="int">
		<attr name="window" type="resource"></attr>
		<attr name="8bit" type="bool"></attr>
	</tag>
	<tag name="ncurses_mouse_trafo" type="function" returnType="bool">
		<attr name="&amp;y" type="int"></attr>
		<attr name="&amp;x" type="int"></attr>
		<attr name="toscreen" type="bool"></attr>
	</tag>
	<tag name="ncurses_mouseinterval" type="function" returnType="int">
		<attr name="milliseconds" type="int"></attr>
	</tag>
	<tag name="ncurses_mousemask" type="function" returnType="int">
		<attr name="newmask" type="int"></attr>
		<attr name="&amp;oldmask" type="int"></attr>
	</tag>
	<tag name="ncurses_move_panel" type="function" returnType="int">
		<attr name="panel" type="resource"></attr>
		<attr name="startx" type="int"></attr>
		<attr name="starty" type="int"></attr>
	</tag>
	<tag name="ncurses_move" type="function" returnType="int">
		<attr name="y" type="int"></attr>
		<attr name="x" type="int"></attr>
	</tag>
	<tag name="ncurses_mvaddch" type="function" returnType="int">
		<attr name="y" type="int"></attr>
		<attr name="x" type="int"></attr>
		<attr name="c" type="int"></attr>
	</tag>
	<tag name="ncurses_mvaddchnstr" type="function" returnType="int">
		<attr name="y" type="int"></attr>
		<attr name="x" type="int"></attr>
		<attr name="s" type="string"></attr>
		<attr name="n" type="int"></attr>
	</tag>
	<tag name="ncurses_mvaddchstr" type="function" returnType="int">
		<attr name="y" type="int"></attr>
		<attr name="x" type="int"></attr>
		<attr name="s" type="string"></attr>
	</tag>
	<tag name="ncurses_mvaddnstr" type="function" returnType="int">
		<attr name="y" type="int"></attr>
		<attr name="x" type="int"></attr>
		<attr name="s" type="string"></attr>
		<attr name="n" type="int"></attr>
	</tag>
	<tag name="ncurses_mvaddstr" type="function" returnType="int">
		<attr name="y" type="int"></attr>
		<attr name="x" type="int"></attr>
		<attr name="s" type="string"></attr>
	</tag>
	<tag name="ncurses_mvcur" type="function" returnType="int">
		<attr name="old_y" type="int"></attr>
		<attr name="old_x" type="int"></attr>
		<attr name="new_y" type="int"></attr>
		<attr name="new_x" type="int"></attr>
	</tag>
	<tag name="ncurses_mvdelch" type="function" returnType="int">
		<attr name="y" type="int"></attr>
		<attr name="x" type="int"></attr>
	</tag>
	<tag name="ncurses_mvgetch" type="function" returnType="int">
		<attr name="y" type="int"></attr>
		<attr name="x" type="int"></attr>
	</tag>
	<tag name="ncurses_mvhline" type="function" returnType="int">
		<attr name="y" type="int"></attr>
		<attr name="x" type="int"></attr>
		<attr name="attrchar" type="int"></attr>
		<attr name="n" type="int"></attr>
	</tag>
	<tag name="ncurses_mvinch" type="function" returnType="int">
		<attr name="y" type="int"></attr>
		<attr name="x" type="int"></attr>
	</tag>
	<tag name="ncurses_mvvline" type="function" returnType="int">
		<attr name="y" type="int"></attr>
		<attr name="x" type="int"></attr>
		<attr name="attrchar" type="int"></attr>
		<attr name="n" type="int"></attr>
	</tag>
	<tag name="ncurses_mvwaddstr" type="function" returnType="int">
		<attr name="window" type="resource"></attr>
		<attr name="y" type="int"></attr>
		<attr name="x" type="int"></attr>
		<attr name="text" type="string"></attr>
	</tag>
	<tag name="ncurses_napms" type="function" returnType="int">
		<attr name="milliseconds" type="int"></attr>
	</tag>
	<tag name="ncurses_new_panel" type="function" returnType="resource">
		<attr name="window" type="resource"></attr>
	</tag>
	<tag name="ncurses_newpad" type="function" returnType="resource">
		<attr name="rows" type="int"></attr>
		<attr name="cols" type="int"></attr>
	</tag>
	<tag name="ncurses_newwin" type="function" returnType="resource">
		<attr name="rows" type="int"></attr>
		<attr name="cols" type="int"></attr>
		<attr name="y" type="int"></attr>
		<attr name="x" type="int"></attr>
	</tag>
	<tag name="ncurses_nl" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_nocbreak" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_noecho" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_nonl" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_noqiflush" type="function" returnType="int">
	</tag>
	<tag name="ncurses_noraw" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_pair_content" type="function" returnType="int">
		<attr name="pair" type="int"></attr>
		<attr name="&amp;f" type="int"></attr>
		<attr name="&amp;b" type="int"></attr>
	</tag>
	<tag name="ncurses_panel_above" type="function" returnType="int">
		<attr name="panel" type="resource"></attr>
	</tag>
	<tag name="ncurses_panel_below" type="function" returnType="int">
		<attr name="panel" type="resource"></attr>
	</tag>
	<tag name="ncurses_panel_window" type="function" returnType="int">
		<attr name="panel" type="resource"></attr>
	</tag>
	<tag name="ncurses_pnoutrefresh" type="function" returnType="int">
		<attr name="pad" type="resource"></attr>
		<attr name="pminrow" type="int"></attr>
		<attr name="pmincol" type="int"></attr>
		<attr name="sminrow" type="int"></attr>
		<attr name="smincol" type="int"></attr>
		<attr name="smaxrow" type="int"></attr>
		<attr name="smaxcol" type="int"></attr>
	</tag>
	<tag name="ncurses_prefresh" type="function" returnType="int">
		<attr name="pad" type="resource"></attr>
		<attr name="pminrow" type="int"></attr>
		<attr name="pmincol" type="int"></attr>
		<attr name="sminrow" type="int"></attr>
		<attr name="smincol" type="int"></attr>
		<attr name="smaxrow" type="int"></attr>
		<attr name="smaxcol" type="int"></attr>
	</tag>
	<tag name="ncurses_putp" type="function" returnType="int">
		<attr name="text" type="string"></attr>
	</tag>
	<tag name="ncurses_qiflush" type="function" returnType="int">
	</tag>
	<tag name="ncurses_raw" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_refresh" type="function" returnType="int">
		<attr name="ch" type="int"></attr>
	</tag>
	<tag name="ncurses_replace_panel" type="function" returnType="int">
		<attr name="panel" type="resource"></attr>
		<attr name="window" type="resource"></attr>
	</tag>
	<tag name="ncurses_reset_prog_mode" type="function" returnType="int">
	</tag>
	<tag name="ncurses_reset_shell_mode" type="function" returnType="int">
	</tag>
	<tag name="ncurses_resetty" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_savetty" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_scr_dump" type="function" returnType="int">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="ncurses_scr_init" type="function" returnType="int">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="ncurses_scr_restore" type="function" returnType="int">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="ncurses_scr_set" type="function" returnType="int">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="ncurses_scrl" type="function" returnType="int">
		<attr name="count" type="int"></attr>
	</tag>
	<tag name="ncurses_show_panel" type="function" returnType="int">
		<attr name="panel" type="resource"></attr>
	</tag>
	<tag name="ncurses_slk_attr" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_slk_attroff" type="function" returnType="int">
		<attr name="intarg" type="int"></attr>
	</tag>
	<tag name="ncurses_slk_attron" type="function" returnType="int">
		<attr name="intarg" type="int"></attr>
	</tag>
	<tag name="ncurses_slk_attrset" type="function" returnType="int">
		<attr name="intarg" type="int"></attr>
	</tag>
	<tag name="ncurses_slk_clear" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_slk_color" type="function" returnType="int">
		<attr name="intarg" type="int"></attr>
	</tag>
	<tag name="ncurses_slk_init" type="function" returnType="bool">
		<attr name="format" type="int"></attr>
	</tag>
	<tag name="ncurses_slk_noutrefresh" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_slk_refresh" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_slk_restore" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_slk_set" type="function" returnType="bool">
		<attr name="labelnr" type="int"></attr>
		<attr name="label" type="string"></attr>
		<attr name="format" type="int"></attr>
	</tag>
	<tag name="ncurses_slk_touch" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_standend" type="function" returnType="int">
	</tag>
	<tag name="ncurses_standout" type="function" returnType="int">
	</tag>
	<tag name="ncurses_start_color" type="function" returnType="int">
	</tag>
	<tag name="ncurses_termattrs" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_termname" type="function" returnType="string">
	</tag>
	<tag name="ncurses_timeout" type="function" returnType="void">
		<attr name="millisec" type="int"></attr>
	</tag>
	<tag name="ncurses_top_panel" type="function" returnType="int">
		<attr name="panel" type="resource"></attr>
	</tag>
	<tag name="ncurses_typeahead" type="function" returnType="int">
		<attr name="fd" type="int"></attr>
	</tag>
	<tag name="ncurses_ungetch" type="function" returnType="int">
		<attr name="keycode" type="int"></attr>
	</tag>
	<tag name="ncurses_ungetmouse" type="function" returnType="bool">
		<attr name="mevent" type="array"></attr>
	</tag>
	<tag name="ncurses_update_panels" type="function" returnType="void">
	</tag>
	<tag name="ncurses_use_default_colors" type="function" returnType="bool">
	</tag>
	<tag name="ncurses_use_env" type="function" returnType="void">
		<attr name="flag" type="bool"></attr>
	</tag>
	<tag name="ncurses_use_extended_names" type="function" returnType="int">
		<attr name="flag" type="bool"></attr>
	</tag>
	<tag name="ncurses_vidattr" type="function" returnType="int">
		<attr name="intarg" type="int"></attr>
	</tag>
	<tag name="ncurses_vline" type="function" returnType="int">
		<attr name="charattr" type="int"></attr>
		<attr name="n" type="int"></attr>
	</tag>
	<tag name="ncurses_waddch" type="function" returnType="int">
		<attr name="window" type="resource"></attr>
		<attr name="ch" type="int"></attr>
	</tag>
	<tag name="ncurses_waddstr" type="function" returnType="int">
		<attr name="window" type="resource"></attr>
		<attr name="str" type="string"></attr>
		<attr name="n" type="int" status="optional"></attr>
	</tag>
	<tag name="ncurses_wattroff" type="function" returnType="int">
		<attr name="window" type="resource"></attr>
		<attr name="attrs" type="int"></attr>
	</tag>
	<tag name="ncurses_wattron" type="function" returnType="int">
		<attr name="window" type="resource"></attr>
		<attr name="attrs" type="int"></attr>
	</tag>
	<tag name="ncurses_wattrset" type="function" returnType="int">
		<attr name="window" type="resource"></attr>
		<attr name="attrs" type="int"></attr>
	</tag>
	<tag name="ncurses_wborder" type="function" returnType="int">
		<attr name="window" type="resource"></attr>
		<attr name="left" type="int"></attr>
		<attr name="right" type="int"></attr>
		<attr name="top" type="int"></attr>
		<attr name="bottom" type="int"></attr>
		<attr name="tl_corner" type="int"></attr>
		<attr name="tr_corner" type="int"></attr>
		<attr name="bl_corner" type="int"></attr>
		<attr name="br_corner" type="int"></attr>
	</tag>
	<tag name="ncurses_wclear" type="function" returnType="int">
		<attr name="window" type="resource"></attr>
	</tag>
	<tag name="ncurses_wcolor_set" type="function" returnType="int">
		<attr name="window" type="resource"></attr>
		<attr name="color_pair" type="int"></attr>
	</tag>
	<tag name="ncurses_werase" type="function" returnType="int">
		<attr name="window" type="resource"></attr>
	</tag>
	<tag name="ncurses_wgetch" type="function" returnType="int">
		<attr name="window" type="resource"></attr>
	</tag>
	<tag name="ncurses_whline" type="function" returnType="int">
		<attr name="window" type="resource"></attr>
		<attr name="charattr" type="int"></attr>
		<attr name="n" type="int"></attr>
	</tag>
	<tag name="ncurses_wmouse_trafo" type="function" returnType="bool">
		<attr name="window" type="resource"></attr>
		<attr name="&amp;y" type="int"></attr>
		<attr name="&amp;x" type="int"></attr>
		<attr name="toscreen" type="bool"></attr>
	</tag>
	<tag name="ncurses_wmove" type="function" returnType="int">
		<attr name="window" type="resource"></attr>
		<attr name="y" type="int"></attr>
		<attr name="x" type="int"></attr>
	</tag>
	<tag name="ncurses_wnoutrefresh" type="function" returnType="int">
		<attr name="window" type="resource"></attr>
	</tag>
	<tag name="ncurses_wrefresh" type="function" returnType="int">
		<attr name="window" type="resource"></attr>
	</tag>
	<tag name="ncurses_wstandend" type="function" returnType="int">
		<attr name="window" type="resource"></attr>
	</tag>
	<tag name="ncurses_wstandout" type="function" returnType="int">
		<attr name="window" type="resource"></attr>
	</tag>
	<tag name="ncurses_wvline" type="function" returnType="int">
		<attr name="window" type="resource"></attr>
		<attr name="charattr" type="int"></attr>
		<attr name="n" type="int"></attr>
	</tag>
</tags>