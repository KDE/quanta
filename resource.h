/***************************************************************************
                          resource.h  -  description
                             -------------------
    begin                : Втр Май  9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    email                : pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef RESOURCE_H
#define RESOURCE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

///////////////////////////////////////////////////////////////////
// resource.h  -- contains macros used for commands


///////////////////////////////////////////////////////////////////
// COMMAND VALUES FOR MENUBAR AND TOOLBAR ENTRIES


///////////////////////////////////////////////////////////////////
// File-menu entries
#define ID_FILE_NEW_WINDOW          10010
#define ID_FILE_NEW                 10020
#define ID_FILE_OPEN                10030
#define ID_FILE_OPEN_RECENT         10040
#define ID_FILE_CLOSE               10050
#define ID_FILE_CLOSE_ALL           10055

#define ID_FILE_SAVE                10060
#define ID_FILE_SAVE_AS             10070
#define ID_FILE_SAVE_ALL            10075

#define ID_FILE_PRINT               10080

#define ID_FILE_QUIT                10090

#define ID_FILE_LINT                10100

#define ID_FILE_NEXT                10091
#define ID_FILE_PREV                10092



///////////////////////////////////////////////////////////////////
// Edit-menu entries
#define ID_EDIT_COPY                11010
#define ID_EDIT_CUT                 11020
#define ID_EDIT_PASTE               11030
#define ID_EDIT_REDO                11040
#define ID_EDIT_UNDO                11050
#define ID_EDIT_SEARCH							11060
#define ID_EDIT_SEARCH_AGAIN				11070
#define ID_EDIT_REPLACE							11080
#define ID_EDIT_FIND_IN_FILES							11081
#define ID_EDIT_GOTO_LINE           11090
#define ID_EDIT_CURRENT_TAG         11100
#define ID_EDIT_SPELL			          11110
#define ID_EDIT_UREDO_HISTORY       11120
#define ID_EDIT_SELECT_ALL          11130
#define ID_EDIT_DESELECT_ALL        11140
#define ID_EDIT_INVERT_SELECT       11150
#define ID_EDIT_INDENT              11160
#define ID_EDIT_UNINDENT            11170
#define ID_EDIT_CLEAN_INDENT        11180
#define ID_EDIT_VERTICAL_SELECT     11190


///////////////////////////////////////////////////////////////////
// View-menu entries                    
#define ID_VIEW_TOOLBAR             12010
#define ID_VIEW_STATUSBAR           12020

#define ID_VIEW_REPAINT							12030
#define ID_VIEW_PREVIEW							12040
#define ID_VIEW_BACK								12050
#define ID_VIEW_FORWARD							12060
#define ID_VIEW_TREE								12070
#define ID_VIEW_MES 								12080
#define ID_VIEW_MIN                 12090
#define ID_VIEW_MAX                 12100
#define ID_VIEW_IN_KFM              12110
#define ID_VIEW_IN_NETSCAPE         12120
#define ID_VIEW_IN_KFM2             12130
#define ID_VIEW_FTP								12140
#define ID_VIEW_PREV_FILE						12150
#define ID_VIEW_NEXT_FILE						12160



///////////////////////////////////////////////////////////////////
// tree-menu entries
#define ID_TREE_ADDTOTOP            13010
#define ID_TREE_OPEN                13020
#define ID_TREE_CLOSE               13030
#define ID_TREE_REMOVE              13040
#define ID_TREE_RELOAD              13050
#define ID_TREE_FILE_TAG            13060
#define ID_TREE_FILE_REMOVE         13070
#define ID_TREE_FILE_PROPERTIES     13080
#define ID_TREE_FILE_OPEN           13090
#define ID_TREE_INSERT_IN_PROJECT   13100
#define ID_TREE_UPLOAD_FILE         13110
#define ID_TREE_SHOW_ALL            13120


///////////////////////////////////////////////////////////////////
// options-menu entries
#define ID_OPTIONS                  14000
#define ID_OPTIONS_EDITOR           14010
#define ID_OPTIONS_HIGHLIGHT        14040
#define ID_OPTIONS_TOOLBARS         14050
#define ID_OPTIONS_KEYS		          14060


///////////////////////////////////////////////////////////////////
// tags-menu entries
#define ID_MENU_STANDARD            15010
#define ID_MENU_FONTS               15020
#define ID_MENU_TABLES              15030
#define ID_MENU_FORMS               15040
#define ID_MENU_LISTS               15050
#define ID_MENU_OTHER               15060

///////////////////////////////////////////////////////////////////
// bookmarks-menu entries
#define ID_BOOKMARKS_SET            16010
#define ID_BOOKMARKS_ADD            16020
#define ID_BOOKMARKS_CLEAR          16030

///////////////////////////////////////////////////////////////////
// bookmarks-menu entries
#define ID_PROJECT_NEW              17010
#define ID_PROJECT_OPEN             17020
#define ID_PROJECT_CLOSE            17030
#define ID_PROJECT_ADD_FILE         17040
#define ID_PROJECT_ADD_DIRECTORY		17045
#define ID_PROJECT_FILE_PROPERTIES  17050
#define ID_PROJECT_UPLOAD_FILE      17060
#define ID_PROJECT_UPLOAD_PROJECT   17070
#define ID_PROJECT_PROPERTIES       17080
#define ID_PROJECT_OPEN_RECENT      17090
#define ID_PROJECT_RESCAN_FILES     17100

///////////////////////////////////////////////////////////////////
// tools menu
#define ID_TOOL_SYNTAX_CHECK        18010

///////////////////////////////////////////////////////////////////
// other-id entries
#define ID_CONTEXT_HELP             30010
#define ID_ATTRIB_POPUP             30020

///////////////////////////////////////////////////////////////////
// Values for Tags toolbar
#define ID_TAG_BOLD									20001
#define ID_TAG_QUICKSTART						20002
#define ID_TAG_H1										20003
#define ID_TAG_H2                   20004
#define ID_TAG_H3                   20005
#define ID_TAG_I										20006
#define ID_TAG_U										20007
#define ID_TAG_BR										20008
#define ID_TAG_P										20009
#define ID_TAG_NBSP									20010
#define ID_TAG_DIV_LEFT   					20011
#define ID_TAG_DIV_RIGHT            20012
#define ID_TAG_DIV_CENTER           20013
#define ID_TAG_DIV_JUSTIFY          20014
#define ID_TAG_H4										20015
#define ID_TAG_H5                   20016
#define ID_TAG_FONT_INC							20017
#define ID_TAG_FONT_DEC							20018
#define ID_TAG_HR										20019
#define ID_TAG_COMM									20020
#define ID_TAG_PRE									20021
#define ID_TAG_SUB									20022
#define ID_TAG_SUP									20023
#define ID_TAG_TABLE								20024
#define ID_TAG_TH     		          20025
#define ID_TAG_TR    								20026
#define ID_TAG_TD           				20027
#define ID_TAG_CAPTION       				20028
#define ID_TAG_UL           				20029
#define ID_TAG_OL           				20030
#define ID_TAG_LI           				20031
#define ID_TAG_DL           				20032
#define ID_TAG_DT           				20033
#define ID_TAG_DD           				20034
#define ID_TAG_MENU         				20035
#define ID_TAG_MAIL         				20036
#define ID_TAG_IMAGE								20037
#define ID_TAG_A    								20038
#define ID_TAG_FONT  								20039
#define ID_TAG_FONT_BASE						20040
#define ID_TAG_QUICK_LIST						20041
#define ID_TAG_QUICK_TABLE					20042
#define ID_TAG_TABLE_ROW						20043
#define ID_TAG_TABLE_HEAD						20044
#define ID_TAG_TABLE_DATA						20045
#define ID_TAG_COLOR								20046

#define ID_TAG_TEXTAREA							20047
#define ID_TAG_RADIO								20048
#define ID_TAG_CHECK								20049
#define ID_TAG_SELECT								20050
#define ID_TAG_DATE 								20060
#define ID_TAG_FORM 								20070
#define ID_TAG_LINEEDIT							20080
#define ID_TAG_LINEPAS							20090
#define ID_TAG_SUBMIT               20100
#define ID_TAG_RESET                20110
#define ID_TAG_CSS									20120

///////////////////////////////////////////////////////////////////
// Help-menu entries
#define ID_HELP_CONTENTS            1002

///////////////////////////////////////////////////////////////////
// General application values
#define ID_STATUS_MSG               1001
#define ID_STATUS_CLM               1002

#define IDS_STATUS_DEFAULT          "Ready."


class QStrList;

extern QStrList *tagsList; // list of known tags
extern QStrList *tagsCore; // list of tags, allowed core attributes ( id, class, id, title )
extern QStrList *tagsI18n; // list of tags, allowed i18 attribs.
extern QStrList *tagsScript; // list of tags, allowed script attribs ( onClicl, omMouseOver... )
extern QStrList *quotedAttribs; // list of attribs, that have quoted values ( alt, src ... )
extern QStrList *lCore;          // list of core attributes ( id, class, style ... )
extern QStrList *lI18n;
extern QStrList *lScript;
extern QStrList *singleTags; // tags without end  part </ >
extern QStrList *optionalTags; // tags with optional end part

extern bool tagsCapital; // use capital for tags
extern bool attrCapital; // use capital letters for attributes of tag
extern bool useCloseTag; // use close tag if optional

extern QString fileMaskHtml;
extern QString fileMaskPhp;
extern QString fileMaskJava;
extern QString fileMaskText;
extern QString fileMaskImage;

#endif // RESOURCE_H
