/*
  $Id$

   Copyright (C) 1998, 1999 Jochen Wilhelmy
                            digisnap@cs.tu-berlin.de

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include <string.h>

#include <qcheckbox.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qtextstream.h>
#include <qregexp.h>
#include <qfile.h>
#include <qlabel.h>

#include <qlayout.h>
#include <qgrid.h>
#include <qhbox.h>
#include <qvgroupbox.h>

#include <kapp.h>
#include <kconfig.h>
#include <kglobal.h>
#include <kfontdialog.h>
#include <kcharsets.h>
#include <kmimemagic.h>
#include <klocale.h>
#include <kregexp.h>
#include <kglobalsettings.h>

#include "highlight.h"
#include "../kwdoc.h"
#include <kstddirs.h>

// general rule for keywords: if one keyword contains another at the beginning,
// the long one has to come first (eg. "const_cast", "const")

// ISO/IEC 9899:1990 (aka ANSI C)
// "interrupt" isn�t an ANSI keyword, but an extension of some C compilers
const char *cKeywords[] = {
  "break", "case", "continue", "default", "do", "else", "enum", "extern",
  "for", "goto", "if", /*"interrupt",*/  "return", "sizeof", "struct",
  "switch", "typedef", "union", "while", 0L};

const char *cTypes[] = {
  "auto", "char", "const", "double", "float", "int", "long", "register",
  "short", "signed", "static", "unsigned", "void", "volatile", 0L};

// ISO/IEC 14882:1998 . Sec. 2.11.1 (aka ANSI C++)
// keyword "const" (apart from a type spec.) is also a keyword, so it is named inside this array
// what about typeof?
const char *cppKeywords[] = {
  "asm", "catch", "class", "const_cast", "const", "delete", "dynamic_cast",
  "explicit", "export", "false", "friend", "inline", "namespace", "new",
  "operator", "private", "protected", "public", "reinterpret_cast",
  "static_cast", "template", "this", "throw", "true", "try", "typeid",
  "typename", "using", "virtual",
  // alternative representations  (these words are reserved and shall not be used otherwise)
  //  ISO/IEC 14882:1998 . Sec. 2.11.2
  "and_eq", "and", "bitand", "bitor", "compl", "not_eq", "not", "or_eq", "or",
  "xor_eq", "xor", 0L};

const char *cppTypes[] = {
  "bool", "wchar_t", "mutable", 0L};

const char *objcKeywords[] = {
  "@class", "@defs", "@encode", "@end", "@implementation", "@interface", "@private", "@protected",
  "@protocol", "@public","@selector", "self", "super", 0L};

const char *objcTypes[] = {
  "auto", "char", "const", "double", "float", "int", "long", "register",
  "short", "signed", "static",
  "unsigned", "void", "volatile",
  0L};

const char *idlKeywords[] = {
  "module", "interface", "struct", "case", "enum", "typedef","signal", "slot",
  "attribute", "readonly", "context", "oneway", "union", "in", "out", "inout",
  0L};

const char *idlTypes[] = {
  "long", "short", "unsigned", "double", "octet", "sequence", "char", "wchar",
  "string", "wstring", "any", "fixed", "Object", "void", "boolean", 0L};

const char *javaKeywords[] = {
  "abstract", "break", "case", "cast", "catch", "class", "continue",
  "default", "do", "else", "extends", "false", "finally", "for", "future",
  "generic", "goto", "if", "implements", "import", "inner", "instanceof",
  "interface", "native", "new", "null", "operator", "outer", "package",
  "private", "protected", "public", "rest", "return", "super", "switch",
  "synchronized", "this", "throws", "throw", "transient", "true", "try",
  "var", "volatile", "while", 0L};

const char *javaTypes[] = {
  "boolean", "byte", "char", "const", "double", "final", "float", "int",
  "long", "short", "static", "void", 0L};

const char *bashKeywords[] = {
  "break","case","done","do","elif","else","esac","exit","export","fi","for",
  "function","if","in","return","select","then","until","while",".",0L};

const char *modulaKeywords[] = {
  "BEGIN","CONST","DEFINITION","DIV","DO","ELSE","ELSIF","END","FOR","FROM",
  "IF","IMPLEMENTATION","IMPORT","MODULE","MOD","PROCEDURE","RECORD","REPEAT",
  "RETURN","THEN","TYPE","VAR","WHILE","WITH","|",0L};

const char *adaKeywords[] = {
  "abort","abs","accept","access","all","and","array","at","begin","body",
  "case","constant","declare","delay","delta","digits","do","else","elsif",
  "end","entry","exception","exit","for", "function","generic","goto","if",
  "in","is","limited","loop","mod","new", "not","null","of","or","others",
  "out","package","pragma","private","procedure", "raise","range","rem",
  "record","renames","return","reverse","select","separate","subtype", "task",
  "terminate","then","type","use","when","while","with","xor",0L};

const char *pythonKeywords[] = {
  "and","assert","break","class","continue","def","del","elif","else",
  "except","exec"," finally","for","from","global","if","import","in","is",
  "lambda","not","or","pass","print","raise","return","try","while",0L};

const char *perlKeywords[] = {
  "and","&&", "bless","caller","cmp","continue","dbmclose","dbmopen","do",
  "die", "dump", "each", "else", "elsif","eq","eval", "exit", "foreach",
  "for", "ge", "goto", "gt", "if", "import", "last", "le", "local", "lt",
  "my", "next", "ne", "not", "no", "!", "or", "||", "package", "ref",
  "redo", "require","return","sub", "tied", "tie", "unless",
  "until", "untie", "use", "wantarray", "while", "xor", 0L};

const char *satherKeywords[] = {
  "and","assert","attr","break!","case","class","const","else","elsif",
  "end","exception","external","false","if","include","initial","is","ITER",
  "loop","new","or","post","pre","private","protect","quit","raise",
  "readonly","result","return","ROUT","SAME","self","shared","then","true",
  "typecase","type","until!","value","void","when","while!","yield",
// new in 1.1 and pSather:
  "abstract","any","bind","fork","guard","immutable","inout","in","lock",
  "once","out","parloop","partial","par","spread","stub", 0L};

// are those Sather keywords, too?
//     "nil","do@", "do"

const char *satherSpecClassNames[] = {
  "$OB","ARRAY","AREF","AVAL","BOOL","CHAR","EXT_OB","FLTDX","FLTD","FLTX",
  "FLTI","FLT","INTI","INT","$REHASH","STR","SYS",0L};

const char *satherSpecFeatureNames[] = {
// real special features
  "create","invariant","main",
// sugar feature names
  "aget","aset","div","is_eq","is_geq","is_gt","is_leq","is_lt","is_neq",
  "minus","mod","negate","not","plus","pow","times", 0L};
  
// *******************************************************************
// quanta types
// *******************************************************************

const char *phpKeywords[] = {
  "break", "case", "endfor", "default", "exit", "else", "array",
  "for", "if", "endif",
  "return", "or","and",
  "switch","xor","elseif","endwhile","do","continue",
  "while", "class", "new", "this","extends","function", "var","int","integer",
  "real","float","double","string", "object", "include", "require", 0L};

const char *phpTypes[] = {
  "printf","list","each","sprintf","echo","print", "apache_lookup_uri",
  "apache_note", "getallheaders", "virtual", "aspell_new", "aspell_check",
  "aspell_check-raw", "aspell_suggest", "bcadd", "bccomp", "bcdiv", "bcmod",
  "bcmul", "bcpow", "bcscale", "bcsqrt", "bcsub", "JDToGregorian",
  "GregorianToJD", "JDToJulian", "JulianToJD", "JDToJewish", "JewishToJD",
  "JDToFrench", "FrenchToJD", "JDMonthName", "JDDayOfWeek", "checkdate",
  "date", "strftime", "getdate", "gettimeofday", "gmdate", "mktime",
  "gmmktime", "time", "microtime", "dbase_create", "dbase_open",
  "dbase_close", "dbase_pack", "dbase_add_record", "dbase_delete_record",
  "dbase_get_record", "dbase_numfields", "dbase_numrecords", "dbmopen",
  "dbmclose", "dbmexists", "dbmfetch", "dbminsert", "dbmreplace",
  "dbmdelete", "dbmfirstkey", "dbmnextkey", "dblist", "chdir", "dir",
  "closedir", "opendir", "readdir", "rewinddir", "dl", "escapeshellcmd",
  "exec", "system", "passthru", "filepro", "filepro_fieldname",
  "filepro_fieldtype", "filepro_fieldwidth", "filepro_retrieve",
  "filepro_fieldcount", "filepro_rowcount", "basename", "chgrp",
  "chmod", "chown", "clearstatcache", "copy", "dirname", "diskfreespace",
  "fclose", "feof", "fgetc", "fgets", "fgetss", "file", "file_exists",
  "fileatime", "filectime", "filegroup", "fileinode", "filemtime",
  "fileowner", "fileperms", "filesize", "filetype", "flock", "fopen",
  "fpassthru", "fputs", "fread", "fseek", "ftell", "fwrite", "is_dir",
  "is_executable", "is_file", "is_link", "is_readable", "is_writeable",
  "link", "linkinfo", "mkdir", "pclose", "popen", "readfile", "readlink",
  "rename", "rewind", "rmdir", "stat", "lstat", "symlink", "tempnam",
  "touch", "umask", "unlink", "header", "setcookie", "hw_Changeobject",
  "hw_Children", "hw_ChildrenObj", "hw_Close", "hw_Connect", "hw_Cp",
  "hw_Deleteobject", "hw_DocByAnchor", "hw_DocByAnchorObj",
  "hw_DocumentAttributes", "hw_DocumentBodyTag", "hw_DocumentSize",
  "hw_ErrorMsg", "hw_EditText", "hw_Error", "hw_Free_Document",
  "hw_GetParents", "hw_GetParentsObj", "hw_GetChildColl", "hw_GetChildCollObj",
  "hw_GetSrcByDestObj", "hw_GetObject", "hw_GetAndLock", "hw_GetText",
  "hw_GetObjectByQuery", "hw_GetObjectByQueryObj", "hw_GetObjectByQueryColl",
  "hw_GetObjectByQueryCollObj", "hw_GetChildDocColl", "hw_GetChildDocCollObj",
  "hw_GetAnchors", "hw_GetAnchorsObj", "hw_Mv", "hw_Identify",
  "hw_InCollections", "hw_Info", "hw_InsColl", "hw_InsDoc",
  "hw_InsertDocument", "hw_InsertObject", "hw_New_Document",
  "hw_Objrec2Array", "hw_OutputDocument", "hw_pConnect", "hw_PipeDocument",
  "hw_Root", "hw_Unlock", "hw_Username", "GetImageSize", "ImageArc",
  "ImageChar", "ImageCharUp", "ImageColorAllocate", "ImageColorTransparent",
  "ImageCopyResized", "ImageCreate", "ImageCreateFromGif", "ImageDashedLine",
  "ImageDestroy", "ImageFill", "ImageFilledPolygon", "ImageFilledRectangle",
  "ImageFillToBorder", "ImageFontHeight", "ImageFontWidth", "ImageGif",
  "ImageInterlace", "ImageLine", "ImageLoadFont", "ImagePolygon",
  "ImageRectangle", "ImageSetPixel", "ImageString", "ImageStringUp",
  "ImageSX", "ImageSY", "ImageTTFBBox", "ImageTTFText", "ImageColorAt",
  "ImageColorClosest", "ImageColorExact", "ImageColorResolve",
  "ImageColorSet", "ImageColorsForIndex", "ImageColorsTotal",
  "imap_append", "imap_base64", "imap_body", "imap_check", "imap_close",
  "imap_createmailbox", "imap_delete", "imap_deletemailbox",
  "imap_expunge", "imap_fetchbody", "imap_fetchstructure", "imap_header",
  "imap_headers", "imap_listmailbox", "imap_listsubscribed",
  "imap_mail_copy", "imap_mail_move", "imap_num_msg", "imap_num_recent",
  "imap_open", "imap_ping", "imap_renamemailbox", "imap_reopen",
  "imap_subscribe", "imap_undelete", "imap_unsubscribe", "imap_qprint",
  "imap_8bit", "imap_binary", "imap_scanmailbox", "imap_mailboxmsginfo",
  "imap_rfc822_write_address", "imap_rfc822_parse_adrlist",
  "imap_setflag_full", "imap_clearflag_full", "imap_sort",
  "imap_fetchheader", "imap_uid", "error_log", "error_reporting",
  "getenv", "get_cfg_var", "get_current_user", "get_magic_quotes_gpc",
  "get_magic_quotes_runtime", "getlastmod", "getmyinode", "getmypid",
  "getmyuid", "getrusage", "phpinfo", "phpversion", "putenv",
  "set_magic_quotes_runtime", "set_time_limit", "ifx_connect",
  "ifx_pconnect", "ifx_close", "ifx_query", "ifx_prepare", "ifx_do",
  "ifx_error", "ifx_errormsg", "ifx_affected_rows", "ifx_fetch_row",
  "ifx_htmltbl_result", "ifx_fieldtypes", "ifx_fieldproperties",
  "ifx_num_fields", "ifx_num_rows", "ifx_free_result", "ifx_create_char",
  "ifx_free_char", "ifx_update_char", "ifx_get_char", "ifx_create_blob",
  "ifx_copy_blob", "ifx_free_blob", "ifx_get_blob", "ifx_update_blob",
  "ifx_blobinfile_mode", "ifx_textasvarchar", "ifx_byteasvarchar",
  "ifx_nullformat", "ifxus_create_slob", "ifx_free_slob", "ifxus_close_slob",
  "ifxus_open_slob", "ifxus_tell_slob", "ifxus_seek_slob", "ifxus_read_slob",
  "ifxus_write_slob", "ibase_connect", "ibase_pconnect", "ibase_close",
  "ibase_query", "ibase_fetch_row", "ibase_free_result", "ibase_prepare",
  "ibase_bind", "ibase_execute", "ibase_free_query", "ibase_timefmt",
  "ldap_add", "ldap_bind", "ldap_close", "ldap_connect",
  "ldap_count_entries", "ldap_delete", "ldap_dn2ufn", "ldap_explode_dn",
  "ldap_first_attribute", "ldap_first_entry", "ldap_free_result",
  "ldap_get_attributes", "ldap_get_dn", "ldap_get_entries",
  "ldap_get_values", "ldap_list", "ldap_modify", "ldap_next_attribute",
  "ldap_next_entry", "ldap_read", "ldap_search", "ldap_unbind",
  "mail", "Abs", "Acos", "Asin", "Atan", "Atan2", "base_convert",
  "BinDec", "Ceil", "Cos", "DecBin", "DecHex", "DecOct", "Exp",
  "Floor", "getrandmax", "HexDec", "Log", "Log10", "max", "min",
  "mt_rand", "mt_srand", "mt_getrandmax", "number_format", "OctDec",
  "pi", "pow", "rand", "round", "Sin", "Sqrt", "srand", "Tan",
  "connection_aborted", "connection_status", "connection_timeout",
  "eval", "extract", "die", "exit", "function_exists",
  "ignore_user_abort", "iptcparse", "leak", "pack",
  "register_shutdown_function", "serialize", "sleep", "unpack",
  "unserialize", "uniqid", "usleep", "msql", "msql_affected_rows",
  "msql_close", "msql_connect", "msql_create_db", "msql_createdb",
  "msql_data_seek", "msql_dbname", "msql_drop_db", "msql_dropdb",
  "msql_error", "msql_fetch_array", "msql_fetch_field",
  "msql_fetch_object", "msql_fetch_row", "msql_fieldname",
  "msql_field_seek", "msql_fieldtable", "msql_fieldtype",
  "msql_fieldflags", "msql_fieldlen", "msql_free_result",
  "msql_freeresult", "msql_list_fields", "msql_listfields",
  "msql_list_dbs", "msql_listdbs", "msql_list_tables", "msql_listtables",
  "msql_num_fields", "msql_num_rows", "msql_numfields",
  "msql_numrows", "msql_pconnect", "msql_query", "msql_regcase",
  "msql_result", "msql_select_db", "msql_selectdb", "msql_tablename",
  "mssql_affected_rows", "mssql_close", "mssql_connect", "mssql_data_seek",
  "mssql_fetch_array", "mssql_fetch_field", "mssql_fetch_object",
  "mssql_fetch_row", "mssql_field_seek", "mssql_free_result",
  "mssql_num_fields", "mssql_num_rows", "mssql_pconnect", "mssql_query",
  "mssql_result", "mssql_select_db", "mysql_affected_rows", "mysql_close",
  "mysql_connect", "mysql_create_db", "mysql_data_seek", "mysql_db_query",
  "mysql_drop_db", "mysql_errno", "mysql_error", "mysql_fetch_array",
  "mysql_fetch_field", "mysql_fetch_lengths", "mysql_fetch_object",
  "mysql_fetch_row", "mysql_field_name", "mysql_field_seek",
  "mysql_field_table", "mysql_field_type", "mysql_field_flags",
  "mysql_field_len", "mysql_free_result", "mysql_insert_id",
  "mysql_list_fields", "mysql_list_dbs", "mysql_list_tables",
  "mysql_num_fields", "mysql_num_rows", "mysql_pconnect", "mysql_query",
  "mysql_result", "mysql_select_db", "mysql_tablename",
  "sybase_affected_rows", "sybase_close", "sybase_connect",
  "sybase_data_seek", "sybase_fetch_array", "sybase_fetch_field",
  "sybase_fetch_object", "sybase_fetch_row", "sybase_field_seek",
  "sybase_free_result", "sybase_num_fields", "sybase_num_rows",
  "sybase_pconnect", "sybase_query", "sybase_result", "sybase_select_db",
  "fsockopen", "pfsockopen", "set_socket_blocking", "gethostbyaddr",
  "gethostbyname", "gethostbynamel", "checkdnsrr", "getmxrr", "openlog",
  "syslog", "closelog", "debugger_on", "debugger_off", "odbc_autocommit",
  "odbc_binmode", "odbc_close", "odbc_close_all", "odbc_commit",
  "odbc_connect", "odbc_cursor", "odbc_do", "odbc_exec", "odbc_execute",
  "odbc_fetch_into", "odbc_fetch_row", "odbc_field_name", "odbc_field_type",
  "odbc_field_len", "odbc_free_result", "odbc_longreadlen",
  "odbc_num_fields", "odbc_pconnect", "odbc_prepare", "odbc_num_rows",
  "odbc_result", "odbc_result_all", "odbc_rollback", "odbc_setoption",
  "OCIDefineByName", "OCIBindByName", "OCILogon", "OCILogOff", "OCIExecute",
  "OCICommit", "OCIRollback", "OCINumRows", "OCIResult", "OCIFetch",
  "OCIFetchInto", "OCIColumnIsNULL", "OCIColumnSize", "Ora_Bind", "Ora_Close",
  "Ora_ColumnName", "Ora_ColumnType", "Ora_Commit", "Ora_CommitOff",
  "Ora_CommitOn", "Ora_Error", "Ora_ErrorCode", "Ora_Exec", "Ora_Fetch",
  "Ora_GetColumn", "Ora_Logoff", "Ora_Logon", "Ora_Open", "Ora_Parse",
  "Ora_Rollback", "PDF_get_info", "PDF_set_info_creator",
  "PDF_set_info_title", "PDF_set_info_subject", "PDF_set_info_keywords",
  "PDF_set_info_author", "PDF_open", "PDF_close", "PDF_begin_page",
  "PDF_end_page", "PDF_show", "PDF_show_xy", "PDF_set_font",
  "PDF_set_leading", "PDF_set_text_rendering", "PDF_set_horiz_scaling",
  "PDF_set_text_rise", "PDF_set_text_matrix", "PDF_set_text_pos",
  "PDF_set_char_spacing", "PDF_set_word_spacing", "PDF_continue_text",
  "PDF_stringwidth", "PDF_save", "PDF_restore", "PDF_translate",
  "PDF_scale", "PDF_rotate", "PDF_setflat", "PDF_setlinejoin",
  "PDF_setlinecap", "PDF_setmiterlimit", "PDF_setlinewidth",
  "PDF_setdash", "PDF_moveto", "PDF_curveto", "PDF_lineto", "PDF_circle",
  "PDF_arc", "PDF_rect", "PDF_closepath", "PDF_stroke",
  "PDF_closepath_stroke", "PDF_fill", "PDF_fill_stroke",
  "PDF_closepath_fill_stroke", "PDF_endpath", "PDF_clip",
  "PDF_setgray_fill", "PDF_setgray_stroke", "PDF_setgray",
  "PDF_setrgbcolor_fill", "PDF_setrgbcolor_stroke", "PDF_setrgbcolor",
  "PDF_add_outline", "PDF_set_transition", "PDF_set_duration",
  "PDF_open_jpeg", "PDF_close_image", "PDF_place_image", "PDF_put_image",
  "PDF_execute_image", "pg_Close", "pg_cmdTuples",
  "pg_Connect", "pg_DBname", "pg_ErrorMessage", "pg_Exec", "pg_Fetch_Array",
  "pg_Fetch_Object", "pg_Fetch_Row", "pg_FieldIsNull", "pg_FieldName",
  "pg_FieldNum", "pg_FieldPrtLen", "pg_FieldSize", "pg_FieldType", "pg_FreeResult",
  "pg_GetLastOid", "pg_Host", "pg_loclose", "pg_locreate", "pg_loopen",
  "pg_loread", "pg_loreadall", "pg_lounlink", "pg_lowrite", "pg_NumFields",
  "pg_NumRows", "pg_Options", "pg_pConnect", "pg_Port", "pg_Result", "pg_tty",
  "ereg", "ereg_replace", "eregi", "eregi_replace", "split",
  "sql_regcase", "sem_get", "sem_acquire", "sem_release",
  "shm_attach", "shm_detach", "shm_remove", "shm_put_var",
  "shm_get_var", "shm_remove_var", "solid_close", "solid_connect",
  "solid_exec", "solid_fetchrow", "solid_fieldname", "solid_fieldnum",
  "solid_freeresult", "solid_numfields", "solid_numrows",
  "solid_result", "snmpget", "snmpwalk", "AddSlashes", "Chop",
  "Chr", "chunk_split", "convert_cyr_string", "crypt", "echo",
  "explode", "flush", "get_meta_tags", "htmlspecialchars",
  "htmlentities", "implode", "join", "ltrim", "md5", "nl2br",
  "Ord", "parse_str", "print", "printf", "quoted_printable_decode",
  "QuoteMeta", "rawurldecode", "rawurlencode", "setlocale", "similar_text",
  "soundex", "sprintf", "strchr", "strcmp", "strcspn", "StripSlashes",
  "strlen", "strrpos", "strpos", "strrchr", "strrev", "strspn", "strstr",
  "strtok", "strtolower", "strtoupper", "str_replace", "strtr", "substr",
  "trim", "ucfirst", "ucwords", "parse_url", "urldecode", "urlencode",
  "base64_encode", "base64_decode", "gettype", "intval", "doubleval",
  "empty", "is_array", "is_double", "is_float", "is_int", "is_integer",
  "is_long", "is_object", "is_real", "is_string", "isset", "settype",
  "strval", "unset", "vm_adduser", "vm_addalias", "vm_passwd",
  "vm_delalias", "vm_deluser", "wddx_serialize_value",
  "wddx_serialize_vars", "wddx_packet_start", "wddx_packet_end",
  "wddx_add_vars", "wddx_deserialize", "gzclose", "gzeof", "gzfile",
  "gzgetc", "gzgets", "gzgetss", "gzopen", "gzpassthru", "gzputs",
  "gzread", "gzrewind", "gzseek", "gztell", "readgzfile", "gzwrite",
  "xml_parser_create", "xml_set_element_handler",
  "xml_set_character_data_handler", "xml_set_processing_instruction_handler",
  "xml_set_default_handler", "xml_set_unparsed_entity_decl_handler",
  "xml_set_notation_decl_handler", "xml_set_external_entity_ref_handler",
  "xml_parse", "xml_get_error_code", "xml_error_string",
  "xml_get_current_line_number", "xml_get_current_column_number",
  "xml_get_current_byte_index", "xml_parser_free",
  "xml_parser_set_option", "xml_parser_get_option", "utf8_decode",
  "utf8_encode", "sizeof", "include", "session_start",
  "session_destroy", "session_name", "session_module_name",
  "session_save_path", "session_id", "session_register",
  "session_unregister", "session_is_registered", "session_decode",
  "session_encode", "array_merge", "key", 0L};

const char *jsKeywords[] = {
  "break", "case", "endfor", "default", "exit", "else", "array",
  "for", "if",  "return",
   "switch","do","continue","true","false",
   "while",  "new", "function", "var",  0L};

const char *jsTypes[] = {
  "alert", "document","eval", 0L};

const char *sqlKeywords[] = {
  "abort", "access", "action", "add", "after", "aggregate", "all", "alter", "analyze",
  "and", "any", "archive", "asc", "as", "backward", "before", "begin",
  "between", "binary", "both", "by", "cache", "case", "cascade", "cast", "check",
  "close", "cluster", "colalese", "collate", "column", "committed", "commit", "constraint", "copy",
  "createuser", "createdb", "create", "cross", "current_user", "current_timestamp",
  "current_time", "current_date", "current", "cursor", "cycle",
  "database", "day", "declare", "default", "delete", "delimiters", "desc",
  "distinct", "do", "drop", "each", "else", "end", "encoding", "exclusive", "except","execute",
  "exists", "explain", "extend", "extract", "false", "fetch", "forward", "foreign",
  "for", "from", "full", "function", "grant", "group", "handler", "having",
  "hour", "if", "inherits", "instead", "insert", "intersect", "inner", "index",
  "into", "in", "isnull", "isolation", "is", "join", "key", "lancompiler", "language",
  "leading", "left", "level", "like", "limit", "listen", "load", "local", "location", "lock",
  "loop", "match", "maxvalue", "max", "minute", "mode", "month", "minvalue", "min", "move", "names",
  "national", "natural", "nchar", "new", "next", "notnull", "notify", "nothing",
  "not", "none", "nocreateuser", "nocreatedb", "no", "nullif", "null", "oids", "offset",
  "old", "only", "on", "operator", "option", "order", "or", "outer", "partial",
  "password", "position", "precision", "primary", "privileges", "prior",
  "procedural", "procedure", "public", "read", "recipe", "references", "relative", "rename",
  "reset", "returns", "return", "revoke", "right", "rollback", "row", "rule",
  "scroll", "second", "select", "sequence", "serialized", "serial", "setof", "set", "share", "show",
  "statement", "start", "stdout", "stdin", "substring", "table", "temp", "to", "then",
  "transaction", "trailing", "trigger", "trim", "trusted", "true", "type",
  "unique", "union", "unlisten", "until", "update", "using", "user", "vacuum",
  "values", "valid", "varying", "version", "verbose", "view",
  "where", "when", "while", "with", "work", "year", "zone", 0L};

const char *sqlTypes[] = {
	"abstime", "aclitem", "boolean", "bool", "box", "bpchar", "bytea", "char", "cidr", "cid",
	"circle", "datetime", "date", "double", "filename", "float4", "float8", "float", "interval",
	"integer", "inet", "int28", "int4", "int8", "int2", "int", "line", "lseg", "macaddr", "money",
	"name", "numeric", "oid8", "oid", "path", "point", "polygon", "regproc", "real",
	"reltime", "smallint", "smgr", "text", "tid", "timestamp", "timespan", "time",
	"tinterval", "unknown", "varchar", "xid", 0L};

// *******************************************************************
// end of quanta types
// *******************************************************************



char fontSizes[] = {4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,22,24,26,28,32,48,64,0};

//default item style indexes
const int dsNormal = 0;
const int dsKeyword = 1;
const int dsDataType = 2;
const int dsDecVal = 3;
const int dsBaseN = 4;
const int dsFloat = 5;
const int dsChar = 6;
const int dsString = 7;
const int dsComment = 8;
const int dsOthers = 9;

// It is my understanding that Roman numerals are also
// considered Numbers in the Unicode scheme of things
// so isInWord should be modified to look for letters
// and digits instead of letters and numbers
// CHANGED ch.isNumber to ch.isDigit
bool isInWord(QChar ch) {
  return ch.isLetter() || ch.isDigit() || ch == '_';
/*  static unsigned char data[] = {0,0,0,0,0,0,255,3,254,255,255,135,254,255,255,7};
  if (c & 128) return false;
  return !(data[c >> 3] & (1 << (c & 7)));*/
}

bool ucmp(const QChar *u, const char *s, int len) {
  while (len > 0) {
    if (*u != *s) return false;
    u++;
    s++;
    len--;
  }
  return true;
}

bool ustrchr(const char *s, QChar c) {
  while (*s != '\0') {
    if (*s == c) return true;
    s++;
  }
  return false;
}

HlItem::HlItem(int attribute, int context)
  : attr(attribute), ctx(context) {
}

HlItemWw::HlItemWw(int attribute, int context)
  : HlItem(attribute,context) {
}


HlCharDetect::HlCharDetect(int attribute, int context, QChar c)
  : HlItem(attribute,context), sChar(c) {
}

const QChar *HlCharDetect::checkHgl(const QChar *str) {
  if (*str == sChar) return str + 1;
  return 0L;
}

Hl2CharDetect::Hl2CharDetect(int attribute, int context, QChar ch1, QChar ch2)
  : HlItem(attribute,context) {
  sChar1 = ch1;
  sChar2 = ch2;
}

const QChar *Hl2CharDetect::checkHgl(const QChar *str) {
  if (str[0] == sChar1 && str[1] == sChar2) return str + 2;
  return 0L;
}

HlStringDetect::HlStringDetect(int attribute, int context, const QString &s)
  : HlItem(attribute, context), str(s) {
}

HlStringDetect::~HlStringDetect() {
}

const QChar *HlStringDetect::checkHgl(const QChar *s) {
  if (memcmp(s, str.unicode(), str.length()*sizeof(QChar)) == 0) return s + str.length();
  return 0L;
}

HlRangeDetect::HlRangeDetect(int attribute, int context, QChar ch1, QChar ch2)
  : HlItem(attribute,context) {
  sChar1 = ch1;
  sChar2 = ch2;
}

const QChar *HlRangeDetect::checkHgl(const QChar *s) {
  if (*s == sChar1) {
    do {
      s++;
      if (*s == '\0') return 0L;
    } while (*s != sChar2);
    return s + 1;
  }
  return 0L;
}

/*
KeywordData::KeywordData(const char *str) {
  len = strlen(str);
  s = new char[len];
  memcpy(s,str,len);
}

KeywordData::~KeywordData() {
  delete s;
}
*/
HlKeyword::HlKeyword(int attribute, int context)
  : HlItemWw(attribute,context) {
//  words.setAutoDelete(true);
}

HlKeyword::~HlKeyword() {
}


void HlKeyword::addWord(const QString &word) {
//  KeywordData *word;
//  word = new KeywordData(s);
  words.append(word);
}

void HlKeyword::addList(const char **list) {

  while (*list) {
    words.append(*list);
//    addWord(*list);
    list++;
  }
}

const QChar *HlKeyword::checkHgl(const QChar *s) {

  for (QStringList::Iterator it = words.begin(); it != words.end(); ++it) {
    if (memcmp(s, (*it).unicode(), (*it).length()*sizeof(QChar)) == 0) {
      return s + (*it).length();
    }
  }
  return 0L;
}


HlInt::HlInt(int attribute, int context)
  : HlItemWw(attribute,context) {
}

const QChar *HlInt::checkHgl(const QChar *str) {
  const QChar *s;

  s = str;
        while (s->isDigit()) s++;
  if (s > str) return s;
  return 0L;
}

HlFloat::HlFloat(int attribute, int context)
  : HlItemWw(attribute,context) {
}

const QChar *HlFloat::checkHgl(const QChar *s) {
  bool b, p;

  b = false;
  while (s->isDigit()){
    s++;
    b = true;
  }
  if (p = (*s == '.')) {
    s++;
    while (s->isDigit()) {
      s++;
      b = true;
    }
  }
  if (!b) return 0L;
  if (*s == 'E' || *s == 'e') s++; else return (p) ? s : 0L;
  if (*s == '-') s++;
  b = false;
  while (s->isDigit()) {
    s++;
    b = true;
  }
  if (b) return s; else return 0L;
}


HlCInt::HlCInt(int attribute, int context)
  : HlInt(attribute,context) {
}

const QChar *HlCInt::checkHgl(const QChar *s) {

//  if (*s == '0') s++; else s = HlInt::checkHgl(s);
  s = HlInt::checkHgl(s);
  if (s != 0L) {
    int l = 0;
    int u = 0;
    const QChar *str;

    do {
      str = s;
      if (*s == 'L' || *s == 'l') {
        l++;
        if (l > 2) return 0L;
        s++;
      }
      if (*s == 'U' || *s == 'u') {
        u++;
        if (u > 1) return 0L;
        s++;
      }
    } while (s != str);
  }
  return s;
}

HlCOct::HlCOct(int attribute, int context)
  : HlItemWw(attribute,context) {
}

const QChar *HlCOct::checkHgl(const QChar *str) {
  const QChar *s;

  if (*str == '0') {
    str++;
    s = str;
    while (*s >= '0' && *s <= '7') s++;
    if (s > str) {
      if (*s == 'L' || *s == 'l' || *s == 'U' || *s == 'u') s++;
      return s;
    }
  }
  return 0L;
}

HlCHex::HlCHex(int attribute, int context)
  : HlItemWw(attribute,context) {
}

const QChar *HlCHex::checkHgl(const QChar *str) {
  const QChar *s;

  if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
    str += 2;
    s = str;
    while (s->isDigit() || (*s >= 'A' && *s <= 'F') || (*s >= 'a' && *s <= 'f')) s++;
    if (s > str) {
      if (*s == 'L' || *s == 'l' || *s == 'U' || *s == 'u') s++;
      return s;
    }
  }
  return 0L;
}

HlCFloat::HlCFloat(int attribute, int context)
  : HlFloat(attribute,context) {
}

const QChar *HlCFloat::checkHgl(const QChar *s) {

  s = HlFloat::checkHgl(s);
  if (s && (*s == 'F' || *s == 'f')) s++;
  return s;
}

HlCSymbol::HlCSymbol(int attribute, int context)
  : HlItem(attribute, context) {
}

const QChar *HlCSymbol::checkHgl(const QChar *s) {
  if (ustrchr("!%&()*+,-./:;<=>?[]^{|}~", *s)) return s +1;
  return 0L;
}


HlLineContinue::HlLineContinue(int attribute, int context)
  : HlItem(attribute,context) {
}

const QChar *HlLineContinue::checkHgl(const QChar *s) {
  if (*s == '\\') return s + 1;
  return 0L;
}


HlCStringChar::HlCStringChar(int attribute, int context)
  : HlItem(attribute,context) {
}

//checks for hex and oct (for example \x1b or \033)
const QChar *checkCharHexOct(const QChar *str) {
  const QChar *s;
        s=str;
        int n;
  if (*s == 'x') {
    n = 0;
    do {
      s++;
      n *= 16;
      if (s->isDigit()) n += *s - '0';
      else if (*s >= 'A' && *s <= 'F') n += *s - 'A' + 10;
      else if (*s >= 'a' && *s <= 'f') n += *s - 'a' + 10;
      else break;
      if (n >= 256) return 0L;
    } while (true);
    if (s - str == 1) return 0L;
  } else {
    if (!(*s >= '0' && *s <= '7')) return 0L;
    n = *s - '0';
    do {
      s++;
      n *= 8;
      if (*s >= '0' && *s <= '7') n += *s - '0'; else break;
      if (n >= 256) return s;
    } while (s - str < 3);
  }
  return s;
}
#if 0
//checks for C escape chars like \n
const QChar *checkEscapedChar(const QChar *s) {

  if (s[0] == '\\' && s[1] != '\0' ){
    s++;
    if (strchr("abefnrtv\"\'\\",*s)) {
      s++;
    } else return checkCharHexOct(s); /*if (*s == 'x') {
      n = 0;
      do {
        s++;
        n *= 16;
        if (*s >= '0' && *s <= '9') n += *s - '0';
        else if (*s >= 'A' && *s <= 'F') n += *s - 'A' + 10;
        else if (*s >= 'a' && *s <= 'f') n += *s - 'a' + 10;
        else break;
        if (n >= 256) return 0L;
      } while (true);
      if (s - str == 2) return 0L;
    } else {
      if (!(*s >= '0' && *s <= '7')) return 0L;
      n = *s - '0';
      do {
        s++;
        n *= 8;
        if (*s >= '0' && *s <= '7') n += *s - '0'; else break;
        if (n >= 256) return s;
      } while (s - str < 4);
    }        */
    return s;
  }
  return 0L;
}
#else
// checks for C escaped chars \n and escaped hex/octal chars
// this one will eliminate 2 function calls
// a strchr and the checkCharHexOct function
// this is faster than the old one
// I bracketed the function with gettimeofday calls
// and got times from 3-9 mu secs
// while the orginal was 9-12 mu secs K6-2 500
// compiled with -O3
const QChar *checkEscapedChar(const QChar *s) {
  int i;
  if (s[0] == '\\' && s[1] != '\0' ) {
        s++;
        switch(*s){
                case  'a': // checks for control chars
                case  'b': // we want to fall through
                case  'e':
                case  'f':
                case  'n':
                case  'r':
                case  't':
                case  'v':
                case '\'':
                case '\"':
                case '?' : // added ? ANSI C classifies this as an escaped char
                case '\\': s++;
                           break;
                case 'x': // if it's like \xff
                        s++; // eat the x
                        // these for loops can probably be
                        // replaced with something else but
                        // for right now they work
                        // check for hexdigits
                        for(i=0;i<2&&(*s>='0' && *s<='9'|| *s >= 'a' && *s <='f'|| *s>='A' && *s<='F');i++,s++);
                        if(i==0) return 0L; // takes care of case '\x'
                        break;

                case '0': case '1': case '2': case '3' :
                case '4': case '5': case '6': case '7' :
                        for(i=0;i < 3 &&(*s >='0'&& *s<='7');i++,s++);
                        break;
                        default: return 0L;
        }
  return s;
  }
  return 0L;
}
#endif

const QChar *HlCStringChar::checkHgl(const QChar *str) {
  return checkEscapedChar(str);
}


HlCChar::HlCChar(int attribute, int context)
  : HlItemWw(attribute,context) {
}

const QChar *HlCChar::checkHgl(const QChar *str) {
  const QChar *s;

  if (str[0] == '\'' && str[1] != '\0' && str[1] != '\'') {
    s = checkEscapedChar(&str[1]); //try to match escaped char
    if (!s) s = &str[2];           //match single non-escaped char
    if (*s == '\'') return s + 1;
  }
  return 0L;
}

HlCPrep::HlCPrep(int attribute, int context)
  : HlItem(attribute,context) {
}

const QChar *HlCPrep::checkHgl(const QChar *s) {

  while (*s == ' ' || *s == '\t') s++;
  if (*s == '#') {
    s++;
    return s;
  }
  return 0L;
}

HlHtmlValue::HlHtmlValue(int attribute, int context)
  : HlItem(attribute,context) {
}

const QChar *HlHtmlValue::checkHgl(const QChar *s) {
  while (*s == ' ' || *s == '\t') s++;
  if (*s == '\"') {
    do {
      s++;
      if (*s == '\0') return 0L;
    } while (*s != '\"');
    s++;
  } else {
    while (*s != ' ' && *s != '\t' && *s != '>' && *s != '\0') s++;
  }
  return s;
}

HlShellComment::HlShellComment(int attribute, int context)
  : HlCharDetect(attribute,context,'#') {
}

HlMHex::HlMHex(int attribute, int context)
  : HlItemWw(attribute,context) {
}

const QChar *HlMHex::checkHgl(const QChar *s) {

  if (s->isDigit()) {
    s++;
    while ((s->isDigit()) || (*s >= 'A' && *s <= 'F')) s++;
    if (*s == 'H') return s + 1;
  }
  return 0L;
}

HlAdaDec::HlAdaDec(int attribute, int context)
  : HlItemWw(attribute,context) {
}

const QChar *HlAdaDec::checkHgl(const QChar *s) {
  const QChar *str;

  if (s->isDigit()) {
    s++;
    while ((s->isDigit()) || *s == '_') s++;
    if (*s != 'e' && *s != 'E') return s;
    s++;
    str = s;
    while ((s->isDigit()) || *s == '_') s++;
    if (s > str) return s;
  }
  return 0L;
}

HlAdaBaseN::HlAdaBaseN(int attribute, int context)
  : HlItemWw(attribute,context) {
}

const QChar *HlAdaBaseN::checkHgl(const QChar *s) {
  int base;
  QChar c1, c2, c3;
  const QChar *str;

  base = 0;
  while (s->isDigit()) {
    base *= 10;
    base += *s - '0';
    if (base > 16) return 0L;
    s++;
  }
  if (base >= 2 && *s == '#') {
    s++;
    c1 = '0' + ((base <= 10) ? base : 10);
    c2 = 'A' + base - 10;
    c3 = 'a' + base - 10;
    while ((*s >= '0' && *s < c1) || (*s >= 'A' && *s < c2)
      || (*s >= 'a' && *s < c3) || *s == '_') {
      s++;
    }
    if (*s == '#') {
      s++;
      if (*s != 'e' && *s != 'E') return s;
      s++;
      str = s;
      while ((s->isDigit()) || *s == '_') s++;
      if (s > str) return s;
    }
  }
  return 0L;
}

HlAdaFloat::HlAdaFloat(int attribute, int context)
  : HlItemWw(attribute,context) {
}

const QChar *HlAdaFloat::checkHgl(const QChar *s) {
  const QChar *str;

  str = s;
  while (s->isDigit()) s++;
  if (s > str && *s == '.') {
    s++;
    str = s;
    while (s->isDigit()) s++;
    if (s > str) {
      if (*s != 'e' && *s != 'E') return s;
      s++;
      if (*s == '-') s++;
      str = s;
      while ((s->isDigit()) || *s == '_') s++;
      if (s > str) return s;
    }
  }
  return 0L;
}

HlAdaChar::HlAdaChar(int attribute, int context)
  : HlItemWw(attribute,context) {
}

const QChar *HlAdaChar::checkHgl(const QChar *s) {
  if (s[0] == '\'' && s[1] && s[2] == '\'') return s + 3;
  return 0L;
}

HlSatherClassname::HlSatherClassname(int attribute, int context)
  : HlItemWw(attribute,context) {
}

const QChar *HlSatherClassname::checkHgl(const QChar *s) {
  if (*s == '$') s++;
  if (*s >= 'A' && *s <= 'Z') {
    s++;
    while ((*s >= 'A' && *s <= 'Z')
           || (s->isDigit())
           || *s == '_') s++;
    return s;
  }
  return 0L;
}

HlSatherIdent::HlSatherIdent(int attribute, int context)
  : HlItemWw(attribute,context) {
}

const QChar *HlSatherIdent::checkHgl(const QChar *s) {
  if (s->isLetter()) {
    s++;
                while(isInWord(*s)) s++;
    if (*s == '!') s++;
    return s;
  }
  return 0L;
}

HlSatherDec::HlSatherDec(int attribute, int context)
  : HlItemWw(attribute,context) {
}

const QChar *HlSatherDec::checkHgl(const QChar *s) {
  if (s->isDigit()) {
    s++;
    while ((s->isDigit()) || *s == '_') s++;
    if (*s == 'i') s++;
    return s;
  }
  return 0L;
}

HlSatherBaseN::HlSatherBaseN(int attribute, int context)
  : HlItemWw(attribute,context) {
}

const QChar *HlSatherBaseN::checkHgl(const QChar *s) {
  if (*s == '0') {
    s++;
    if (*s == 'x') {
      s++;
      while ((s->isDigit())
             || (*s >= 'a' && *s <= 'f')
             || (*s >= 'A' && *s <= 'F')
             || *s == '_') s++;
    } else if (*s == 'o') {
      s++;
      while ((*s >= '0' && *s <= '7') || *s == '_') s++;
    } else if (*s == 'b') {
      s++;
      while (*s == '0' || *s == '1' || *s == '_') s++;
    } else
      return 0L;
    if (*s == 'i') s++;
    return s;
  }
  return 0L;
}

HlSatherFloat::HlSatherFloat(int attribute, int context)
  : HlItemWw(attribute,context) {
}

const QChar *HlSatherFloat::checkHgl(const QChar *s) {
  if (s->isDigit()) {
    s++;
    while ((s->isDigit()) || *s == '_') s++;
    if (*s == '.') {
      s++;
      while (s->isDigit()) s++;
      if (*s == 'e' || *s == 'E') {
        s++;
        if (*s == '-') s++;
        if (s->isDigit()) {
          s++;
          while ((s->isDigit()) || *s == '_') s++;
        } else
          return 0L;
      }
      if (*s == 'i') return s+1;
      if (*s == 'd') s++;
      if (*s == 'x') s++;
                // "dx" is allowed too
      return s;
    }
  }
  return 0L;
}

HlSatherChar::HlSatherChar(int attribute, int context)
  : HlItemWw(attribute,context) {
}

const QChar *HlSatherChar::checkHgl(const QChar *s) {
  if (*s == '\'') {
    s++;
    if (*s == '\\') {
      s++;
      if (*s == 'a' || *s == 'b' || *s == 'f' || *s == 'n'
          || *s == 'r' || *s == 't' || *s == 'v' || *s == '\\'
          || *s == '\'' || *s == '\"') s++;
      else if (*s>='0' && *s<='7')
        while (*s>='0' && *s<='7') s++;
      else
        return 0L;
    } else if (*s != '\0') s++;
  }
  if (*s == '\'')
    return s+1;
  else
    return 0L;
}

HlSatherString::HlSatherString(int attribute, int context)
  : HlItemWw(attribute, context) {
}

const QChar *HlSatherString::checkHgl(const QChar *s) {
  if (*s == '\"') {
    s++;
    while (*s != '\"') {
      if (*s == '\\')
        s++;
      if (*s == '\n' || *s == '\0')
        return s;
      s++;
    }
    return s+1;
  }
  return 0L;
}


HlLatexTag::HlLatexTag(int attribute, int context)
  : HlItem(attribute, context) {
}

const QChar *HlLatexTag::checkHgl(const QChar *s) {
  const QChar *str;

  if (*s == '\\') {
    s++;
    if (*s == ' ' || *s == '/' || *s == '\\') return s +1;
    str = s;
    while ((*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <= 'Z')
      || (s->isDigit()) || *s == '@') {
      s++;
    }
    if (s != str) return s;
  } /*else if (*s == '$') return s +1*/;
  return 0L;
}

HlLatexChar::HlLatexChar(int attribute, int context)
  : HlItem(attribute, context) {
}

const QChar *HlLatexChar::checkHgl(const QChar *s) {
  if (*s == '\\') {
    s++;
    if (*s && strchr("{}$&#_%", *s)) return s +1;
  }/* else if (*s == '"') {
    s++;
    if (*s && (*s < '0' || *s > '9')) return s +1;
  } */
  return 0L;
}

HlLatexParam::HlLatexParam(int attribute, int context)
  : HlItem(attribute, context) {
}

const QChar *HlLatexParam::checkHgl(const QChar *s) {
  if (*s == '#') {
    s++;
    while (s->isDigit()) {
      s++;
    }
    return s;
  }
  return 0L;
}

//--------
ItemStyle::ItemStyle() : selCol(Qt::white), bold(false), italic(false) {
}

ItemStyle::ItemStyle(const QColor &col, const QColor &selCol,
  bool bold, bool italic)
  : col(col), selCol(selCol), bold(bold), italic(italic) {
}

ItemFont::ItemFont() : family("courier"), size(12), charset("") {
}

ItemData::ItemData(const char * name, int defStyleNum)
  : name(name), defStyleNum(defStyleNum), defStyle(true), defFont(true) {
}

ItemData::ItemData(const char * name, int defStyleNum,
  const QColor &col, const QColor &selCol, bool bold, bool italic)
  : ItemStyle(col,selCol,bold,italic), name(name), defStyleNum(defStyleNum),
  defStyle(false), defFont(true) {
}

HlData::HlData(const QString &wildcards, const QString &mimetypes)
  : wildcards(wildcards), mimetypes(mimetypes) {

  itemDataList.setAutoDelete(true);
}

Highlight::Highlight(const char * name) : iName(name), refCount(0) {
}

Highlight::~Highlight() {
}

KConfig *Highlight::getKConfig() {
  KConfig *config;

  config = kapp->config();
  config->setGroup(QString::fromUtf8(iName) + QString::fromUtf8(" Highlight"));
  return config;
}

QString Highlight::getWildcards() {
  KConfig *config;

  config = getKConfig();

  //if wildcards not yet in config, then use iWildCards as default
  return config->readEntry("Wildcards", iWildcards);
}


QString Highlight::getMimetypes() {
  KConfig *config;

  config = getKConfig();

  return config->readEntry("Mimetypes", iMimetypes);
}


HlData *Highlight::getData() {
  KConfig *config;
  HlData *hlData;

  config = getKConfig();

//  iWildcards = config->readEntry("Wildcards");
//  iMimetypes = config->readEntry("Mimetypes");
//  hlData = new HlData(iWildcards,iMimetypes);
  hlData = new HlData(
    config->readEntry("Wildcards", iWildcards),
    config->readEntry("Mimetypes", iMimetypes));
  getItemDataList(hlData->itemDataList, config);
  return hlData;
}

void Highlight::setData(HlData *hlData) {
  KConfig *config;

  config = getKConfig();

//  iWildcards = hlData->wildcards;
//  iMimetypes = hlData->mimetypes;

  config->writeEntry("Wildcards",hlData->wildcards);
  config->writeEntry("Mimetypes",hlData->mimetypes);

  setItemDataList(hlData->itemDataList,config);
}

void Highlight::getItemDataList(ItemDataList &list) {
  KConfig *config;

  config = getKConfig();
  getItemDataList(list, config);
}

void Highlight::getItemDataList(ItemDataList &list, KConfig *config) {
  ItemData *p;
  QString s;
  QRgb col, selCol;
  char family[96];
  char charset[48];

  list.clear();
  list.setAutoDelete(true);
  createItemData(list);

  for (p = list.first(); p != 0L; p = list.next()) {
    s = config->readEntry(p->name);
    if (!s.isEmpty()) {
      sscanf(s.ascii(),"%d,%X,%X,%d,%d,%d,%95[^,],%d,%47[^,]",
        &p->defStyle,&col,&selCol,&p->bold,&p->italic,
        &p->defFont,family,&p->size,charset);
      p->col.setRgb(col);
      p->selCol.setRgb(selCol);
      p->family = family;
      p->charset = charset;
    }
  }
}

void Highlight::setItemDataList(ItemDataList &list, KConfig *config) {
  ItemData *p;
  QString s;

  for (p = list.first(); p != 0L; p = list.next()) {
    s.sprintf("%d,%X,%X,%d,%d,%d,%1.95s,%d,%1.47s",
      p->defStyle,p->col.rgb(),p->selCol.rgb(),p->bold,p->italic,
      p->defFont,p->family.utf8().data(),p->size,p->charset.utf8().data());
    config->writeEntry(p->name,s);
  }
}

void Highlight::use() {
  if (refCount == 0) init();
  refCount++;
}

void Highlight::release() {
  refCount--;
  if (refCount == 0) done();
}

/*
bool Highlight::isInWord(char ch) {
  static char data[] = {0,0,0,0,0,0,255,3,254,255,255,135,254,255,255,7};
  if (ch & 128) return true;
  return data[ch >> 3] & (1 << (ch & 7));
}
*/
int Highlight::doHighlight(int, TextLine *textLine) {

  textLine->setAttribs(0,0,textLine->length());
  textLine->setAttr(0);
  return 0;
}

void Highlight::createItemData(ItemDataList &list) {

  list.append(new ItemData(I18N_NOOP("Normal Text"), dsNormal));
}


void Highlight::init() {
}

void Highlight::done() {
}


HlContext::HlContext(int attribute, int lineEndContext)
  : attr(attribute), ctx(lineEndContext) {
  items.setAutoDelete(true);
}


GenHighlight::GenHighlight(const char * name) : Highlight(name) {
}


int GenHighlight::doHighlight(int ctxNum, TextLine *textLine) {
  HlContext *context;
  const QChar *str, *s1, *s2;
  QChar lastChar;
  HlItem *item;

  context = contextList[ctxNum];
  str = textLine->getString();
  lastChar = '\0';

  s1 = str;
  while (*s1 != '\0') {
    for (item = context->items.first(); item != 0L; item = context->items.next()) {
      if (item->startEnable(lastChar)) {
        s2 = item->checkHgl(s1);
        if (s2 > s1) {
          if (item->endEnable(*s2)) {
            textLine->setAttribs(item->attr,s1 - str,s2 - str);
            ctxNum = item->ctx;
            context = contextList[ctxNum];
            s1 = s2 - 1;
            goto found;
          }
        }
      }
    }
    // nothing found: set attribute of one char
    textLine->setAttribs(context->attr,s1 - str,s1 - str + 1);

    found:
    lastChar = *s1;
    s1++;
  }
  //set "end of line"-properties
  textLine->setAttr(context->attr);
  //return new context
  return context->ctx;
}

void GenHighlight::init() {
  int z;

  for (z = 0; z < nContexts; z++) contextList[z] = 0L;
  makeContextList();
}

void GenHighlight::done() {
  int z;

  for (z = 0; z < nContexts; z++) delete contextList[z];
}


CHighlight::CHighlight(const char * name) : GenHighlight(name) {
  iWildcards = "*.c";
  iMimetypes = "text/x-c-src";
}

CHighlight::~CHighlight() {
}

void CHighlight::createItemData(ItemDataList &list) {

  list.append(new ItemData(I18N_NOOP("Normal Text" ),dsNormal));
  list.append(new ItemData(I18N_NOOP("Keyword"     ),dsKeyword));
  list.append(new ItemData(I18N_NOOP("Data Type"   ),dsDataType));
  list.append(new ItemData(I18N_NOOP("Decimal"     ),dsDecVal));
  list.append(new ItemData(I18N_NOOP("Octal"       ),dsBaseN));
  list.append(new ItemData(I18N_NOOP("Hex"         ),dsBaseN));
  list.append(new ItemData(I18N_NOOP("Float"       ),dsFloat));
  list.append(new ItemData(I18N_NOOP("Char"        ),dsChar));
  list.append(new ItemData(I18N_NOOP("String"      ),dsString));
  list.append(new ItemData(I18N_NOOP("String Char" ),dsChar));
  list.append(new ItemData(I18N_NOOP("Comment"     ),dsComment));
  list.append(new ItemData(I18N_NOOP("Symbol"      ),dsNormal));
  list.append(new ItemData(I18N_NOOP("Preprocessor"),dsOthers));
  list.append(new ItemData(I18N_NOOP("Prep. Lib"   ),dsOthers,Qt::darkYellow,Qt::yellow,false,false));

}

void CHighlight::makeContextList() {
  HlContext *c;
  HlKeyword *keyword, *dataType;

  //normal context
  contextList[0] = c = new HlContext(0,0);
  c->items.append(keyword = new HlKeyword(1,0));
  c->items.append(dataType = new HlKeyword(2,0));
  c->items.append(new HlCFloat(6,0));
  c->items.append(new HlCOct(4,0));
  c->items.append(new HlCHex(5,0));
  c->items.append(new HlCInt(3,0));
  c->items.append(new HlCChar(7,0));
  c->items.append(new HlCharDetect(8,1,'"'));
  c->items.append(new Hl2CharDetect(10,2, '/', '/'));
  c->items.append(new Hl2CharDetect(10,3, '/', '*'));
  c->items.append(new HlCSymbol(11,0));
  c->items.append(new HlCPrep(12,4));
  //string context
  contextList[1] = c = new HlContext(8,0);
  c->items.append(new HlLineContinue(8,6));
  c->items.append(new HlCStringChar(9,1));
  c->items.append(new HlCharDetect(8,0,'"'));
  //one line comment context
  contextList[2] = new HlContext(10,0);
  //multi line comment context
  contextList[3] = c = new HlContext(10,3);
  c->items.append(new Hl2CharDetect(10,0, '*', '/'));
  //preprocessor context
  contextList[4] = c = new HlContext(12,0);
  c->items.append(new HlLineContinue(12,7));
  c->items.append(new HlRangeDetect(13,4, '\"', '\"'));
  c->items.append(new HlRangeDetect(13,4, '<', '>'));
  c->items.append(new Hl2CharDetect(10,2, '/', '/'));
  c->items.append(new Hl2CharDetect(10,5, '/', '*'));
  //preprocessor multiline comment context
  contextList[5] = c = new HlContext(10,5);
  c->items.append(new Hl2CharDetect(10,4, '*', '/'));
  //string line continue
  contextList[6] = new HlContext(0,1);
  //preprocessor string line continue
  contextList[7] = new HlContext(0,4);

  setKeywords(keyword, dataType);
}

void CHighlight::setKeywords(HlKeyword *keyword, HlKeyword *dataType) {

  keyword->addList(cKeywords);
  dataType->addList(cTypes);
}


CppHighlight::CppHighlight(const char * name) : CHighlight(name) {

  iWildcards = "*.cpp;*.cc;*.C;*.h";
  iMimetypes = "text/x-c++-src;text/x-c++-hdr;text/x-c-hdr;text/x-c++-src";
}

CppHighlight::~CppHighlight() {
}

void CppHighlight::setKeywords(HlKeyword *keyword, HlKeyword *dataType) {

  keyword->addList(cKeywords);
  keyword->addList(cppKeywords);
  dataType->addList(cTypes);
  dataType->addList(cppTypes);
}

ObjcHighlight::ObjcHighlight(const char * name) : CHighlight(name) {
  iWildcards = "*.m;*.h";
  iMimetypes = "text/x-objc-src;text/x-c-hdr";
}

ObjcHighlight::~ObjcHighlight() {
}

void ObjcHighlight::makeContextList() {
  HlContext *c;

  CHighlight::makeContextList();
  c = contextList[0];
  c->items.append(new Hl2CharDetect(8,1,'@','"'));
}

void ObjcHighlight::setKeywords(HlKeyword *keyword, HlKeyword *dataType) {
  keyword->addList(cKeywords);
  keyword->addList(objcKeywords);
  dataType->addList(cTypes);
  dataType->addList(objcTypes);
}

IdlHighlight::IdlHighlight(const char * name) : CHighlight(name) {
  iWildcards = "*.idl";
  iMimetypes = "text/x-idl-src";
}

IdlHighlight::~IdlHighlight() {
}

void IdlHighlight::setKeywords(HlKeyword *keyword, HlKeyword *dataType) {
  keyword->addList(idlKeywords);
  dataType->addList(idlTypes);
}

JavaHighlight::JavaHighlight(const char * name) : CHighlight(name) {
  iWildcards = "*.java";
  iMimetypes = "text/x-java-src";
}

JavaHighlight::~JavaHighlight() {
}

void JavaHighlight::setKeywords(HlKeyword *keyword, HlKeyword *dataType) {

  keyword->addList(javaKeywords);
  dataType->addList(javaTypes);
}


HtmlHighlight::HtmlHighlight(const char * name) : GenHighlight(name) {
  iWildcards = "*.html;*.htm";
  iMimetypes = "text/html";
}

HtmlHighlight::~HtmlHighlight() {
}

void HtmlHighlight::createItemData(ItemDataList &list) {

  list.append(new ItemData(I18N_NOOP("Normal Text"),dsNormal));
  list.append(new ItemData(I18N_NOOP("Char"       ),dsChar,Qt::darkGreen,Qt::green,false,false));
  list.append(new ItemData(I18N_NOOP("Comment"    ),dsComment));
  list.append(new ItemData(I18N_NOOP("Tag Text"   ),dsOthers,Qt::black,Qt::white,true,false));
  list.append(new ItemData(I18N_NOOP("Tag"        ),dsKeyword,Qt::darkMagenta,Qt::magenta,true,false));
  list.append(new ItemData(I18N_NOOP("Tag Value"  ),dsDecVal,Qt::darkCyan,Qt::cyan,false,false));
}

void HtmlHighlight::makeContextList() {
  HlContext *c;

  contextList[0] = c = new HlContext(0,0);
    c->items.append(new HlRangeDetect(1,0, '&', ';'));
    c->items.append(new HlStringDetect(2,1,"<!--"));
    c->items.append(new HlStringDetect(2,2,"<COMMENT>"));
    c->items.append(new HlCharDetect(3,3,'<'));
  contextList[1] = c = new HlContext(2,1);
    c->items.append(new HlStringDetect(2,0,"-->"));
  contextList[2] = c = new HlContext(2,2);
    c->items.append(new HlStringDetect(2,0,"</COMMENT>"));
  contextList[3] = c = new HlContext(3,3);
    c->items.append(new HlHtmlTag(4,3));
    c->items.append(new HlHtmlValue(5,3));
    c->items.append(new HlCharDetect(3,0,'>'));
    
// ********************************************************
// quanta highlighting
// ********************************************************
    HlKeyword *keyword, *dataType;    

    c->items.append(new HlStringDetect(4,20,"script"));
    c->items.append(new HlStringDetect(4,20,"SCRIPT"));
    c->items.append(new HlHtmlValue(5,3));
    c->items.append(new HlHtmlTag(4,3));
    c->items.append(new HlHtmlTagUnknown(16,3));
    c->items.append(new HlHtmlAttrib(15,3));
    c->items.append(new HlHtmlAttribUnknown(17,3));
    c->items.append(new HlCharDetect(3,0,'>'));

  //JavaScript
  contextList[20] = c = new HlContext(3,20);
    c->items.append(new HlHtmlValue(5,20));
    c->items.append(new HlCharDetect(3,21,'>'));

  contextList[21] = c = new HlContext(0,21);
    c->items.append(keyword = new HlKeyword(6,21));
    c->items.append(dataType = new HlKeyword(7,21));
    c->items.append(new HlStringDetect(4,0,"</SCRIPT>"));
    c->items.append(new HlStringDetect(4,0,"</script>"));
    c->items.append(new HlStringDetect(2,21,"<!--"));
    c->items.append(new HlStringDetect(2,21,"-->"));
    c->items.append(new HlCFloat(11,21));
    c->items.append(new HlCOct(9,21));
    c->items.append(new HlCHex(10,21));
    c->items.append(new HlCInt(8,21));
    c->items.append(new HlCChar(12,21));
    c->items.append(new HlCharDetect(13,22,'"'));
    c->items.append(new Hl2CharDetect(2,23,'/','/'));
    c->items.append(new Hl2CharDetect(2,24,'/','*'));
//    c->items.append(new HlCPrep(11,25));
  contextList[22] = c = new HlContext(13,21);
    c->items.append(new HlLineContinue(13,27));
    c->items.append(new HlCStringChar(14,22));
    c->items.append(new HlCharDetect(13,21,'"'));
  contextList[23] = new HlContext(2,21);
  contextList[24] = c = new HlContext(2,24);
    c->items.append(new Hl2CharDetect(2,21,'*','/'));
  contextList[25] = c = new HlContext(11,21);
    c->items.append(new HlLineContinue(11,28));
    c->items.append(new HlRangeDetect(12,25,'"','"'));
    c->items.append(new HlRangeDetect(12,25,'<','>'));
    c->items.append(new Hl2CharDetect(2,23,'/','/'));
    c->items.append(new Hl2CharDetect(2,26,'/','*'));
  contextList[26] = c = new HlContext(2,26);
    c->items.append(new Hl2CharDetect(2,25,'*','/'));
  contextList[27] = new HlContext(0,22);
  contextList[28] = new HlContext(0,25);

  setKeywords(keyword, dataType);
}

void HtmlHighlight::setKeywords(HlKeyword *keyword, HlKeyword *dataType)
{
  keyword->addList(jsKeywords);
  dataType->addList(jsTypes);
}

// ********************************************************
// end of quanta highlighting
// ********************************************************


BashHighlight::BashHighlight(const char * name) : GenHighlight(name) {
//  iWildcards = "";
  iMimetypes = "text/x-shellscript";
}


BashHighlight::~BashHighlight() {
}

void BashHighlight::createItemData(ItemDataList &list) {

  list.append(new ItemData(I18N_NOOP("Normal Text" ),dsNormal));
  list.append(new ItemData(I18N_NOOP("Keyword"     ),dsKeyword));
  list.append(new ItemData(I18N_NOOP("Integer"     ),dsDecVal));
  list.append(new ItemData(I18N_NOOP("String"      ),dsString));
  list.append(new ItemData(I18N_NOOP("Substitution"),dsOthers));//darkCyan,cyan,false,false);
  list.append(new ItemData(I18N_NOOP("Comment"     ),dsComment));
}

void BashHighlight::makeContextList() {
  HlContext *c;
  HlKeyword *keyword;

  contextList[0] = c = new HlContext(0,0);
    c->items.append(keyword = new HlKeyword(1,0));
    c->items.append(new HlInt(2,0));
    c->items.append(new HlCharDetect(3,1,'"'));
    c->items.append(new HlCharDetect(4,2,'`'));
    c->items.append(new HlShellComment(5,3));
  contextList[1] = c = new HlContext(3,0);
    c->items.append(new HlCharDetect(3,0,'"'));
  contextList[2] = c = new HlContext(4,0);
    c->items.append(new HlCharDetect(4,0,'`'));
  contextList[3] = new HlContext(5,0);

  keyword->addList(bashKeywords);
}


ModulaHighlight::ModulaHighlight(const char * name) : GenHighlight(name) {
  iWildcards = "*.md;*.mi";
  iMimetypes = "text/x-modula-2-src";
}

ModulaHighlight::~ModulaHighlight() {
}

void ModulaHighlight::createItemData(ItemDataList &list) {

  list.append(new ItemData(I18N_NOOP("Normal Text"),dsNormal));
  list.append(new ItemData(I18N_NOOP("Keyword"    ),dsKeyword));
  list.append(new ItemData(I18N_NOOP("Decimal"    ),dsDecVal));
  list.append(new ItemData(I18N_NOOP("Hex"        ),dsBaseN));
  list.append(new ItemData(I18N_NOOP("Float"      ),dsFloat));
  list.append(new ItemData(I18N_NOOP("String"     ),dsString));
  list.append(new ItemData(I18N_NOOP("Comment"    ),dsComment));
}

void ModulaHighlight::makeContextList() {
  HlContext *c;
  HlKeyword *keyword;

  contextList[0] = c = new HlContext(0,0);
    c->items.append(keyword = new HlKeyword(1,0));
    c->items.append(new HlFloat(4,0));
    c->items.append(new HlMHex(3,0));
    c->items.append(new HlInt(2,0));
    c->items.append(new HlCharDetect(5,1,'"'));
    c->items.append(new Hl2CharDetect(6,2, '(', '*'));
  contextList[1] = c = new HlContext(5,0);
    c->items.append(new HlCharDetect(5,0,'"'));
  contextList[2] = c = new HlContext(6,2);
    c->items.append(new Hl2CharDetect(6,0, '*', ')'));

  keyword->addList(modulaKeywords);
}


AdaHighlight::AdaHighlight(const char * name) : GenHighlight(name) {
  iWildcards = "*.a";
  iMimetypes = "text/x-ada-src";
}

AdaHighlight::~AdaHighlight() {
}

void AdaHighlight::createItemData(ItemDataList &list) {

  list.append(new ItemData(I18N_NOOP("Normal Text"),dsNormal));
  list.append(new ItemData(I18N_NOOP("Keyword"    ),dsKeyword));
  list.append(new ItemData(I18N_NOOP("Decimal"    ),dsDecVal));
  list.append(new ItemData(I18N_NOOP("Base-N"     ),dsBaseN));
  list.append(new ItemData(I18N_NOOP("Float"      ),dsFloat));
  list.append(new ItemData(I18N_NOOP("Char"       ),dsChar));
  list.append(new ItemData(I18N_NOOP("String"     ),dsString));
  list.append(new ItemData(I18N_NOOP("Comment"    ),dsComment));
}

void AdaHighlight::makeContextList() {
  HlContext *c;
  HlKeyword *keyword;

  contextList[0] = c = new HlContext(0,0);
    c->items.append(keyword = new HlKeyword(1,0));
    c->items.append(new HlAdaBaseN(3,0));
    c->items.append(new HlAdaFloat(4,0));
    c->items.append(new HlAdaDec(2,0));
    c->items.append(new HlAdaChar(5,0));
    c->items.append(new HlCharDetect(6,1,'"'));
    c->items.append(new Hl2CharDetect(7,2, '-', '-'));
  contextList[1] = c = new HlContext(6,0);
    c->items.append(new HlCharDetect(6,0,'"'));
  contextList[2] = c = new HlContext(7,0);

  keyword->addList(adaKeywords);
}


PythonHighlight::PythonHighlight(const char * name) : GenHighlight(name) {
  iWildcards = "*.py";
  iMimetypes = "text/x-python-src";
}

PythonHighlight::~PythonHighlight() {
}

void PythonHighlight::createItemData(ItemDataList &list) {

  list.append(new ItemData(I18N_NOOP("Normal Text"),dsNormal));
  list.append(new ItemData(I18N_NOOP("Keyword"    ),dsKeyword));
  list.append(new ItemData(I18N_NOOP("Decimal"    ),dsDecVal));
  list.append(new ItemData(I18N_NOOP("Octal"      ),dsBaseN));
  list.append(new ItemData(I18N_NOOP("Hex"        ),dsBaseN));
  list.append(new ItemData(I18N_NOOP("Float"      ),dsFloat));
  list.append(new ItemData(I18N_NOOP("Char"       ),dsChar));
  list.append(new ItemData(I18N_NOOP("String"     ),dsString));
  list.append(new ItemData(I18N_NOOP("String Char"),dsChar));
  list.append(new ItemData(I18N_NOOP("Comment"    ),dsComment));
}

void PythonHighlight::makeContextList() {
  HlContext *c;
  HlKeyword *keyword;

  //note that a C octal has to be detected before an int and """ before "
  contextList[0] = c = new HlContext(0,0);
    c->items.append(keyword = new HlKeyword(1,0));
    c->items.append(new HlCOct(3,0));
    c->items.append(new HlInt(2,0));
    c->items.append(new HlCHex(4,0));
    c->items.append(new HlFloat(5,0));
    c->items.append(new HlCChar(6,0));
    c->items.append(new HlStringDetect(7,3,"\"\"\""));
    c->items.append(new HlStringDetect(7,4,"\'\'\'"));
    c->items.append(new HlCharDetect(7,1,'"'));
    c->items.append(new HlCharDetect(7,2,'\''));
    c->items.append(new HlCharDetect(9,5,'#'));
  contextList[1] = c = new HlContext(7,0);
    c->items.append(new HlLineContinue(7,6));
    c->items.append(new HlCStringChar(8,1));
    c->items.append(new HlCharDetect(7,0,'"'));
  contextList[2] = c = new HlContext(7,0);
    c->items.append(new HlLineContinue(7,7));
    c->items.append(new HlCStringChar(8,2));
    c->items.append(new HlCharDetect(7,0,'\''));
  contextList[3] = c = new HlContext(7,3);
    c->items.append(new HlStringDetect(7,0,"\"\"\""));
  contextList[4] = c = new HlContext(7,4);
    c->items.append(new HlStringDetect(7,0,"\'\'\'"));
  contextList[5] = new HlContext(9,0);
  contextList[6] = new HlContext(0,1);
  contextList[7] = new HlContext(0,2);

  keyword->addList(pythonKeywords);
}

PerlHighlight::PerlHighlight(const char * name) : Highlight(name) {
  iWildcards = "";
  iMimetypes = "application/x-perl";
}

void PerlHighlight::createItemData(ItemDataList &list) {

  list.append(new ItemData(I18N_NOOP("Normal Text"),dsNormal));
  list.append(new ItemData(I18N_NOOP("Keyword"    ),dsKeyword));
  list.append(new ItemData(I18N_NOOP("Variable"   ),dsDecVal));
  list.append(new ItemData(I18N_NOOP("Operator"   ),dsOthers));
  list.append(new ItemData(I18N_NOOP("String"     ),dsString));
  list.append(new ItemData(I18N_NOOP("String Char"),dsChar));
  list.append(new ItemData(I18N_NOOP("Comment"    ),dsComment));
  list.append(new ItemData(I18N_NOOP("Pod"        ),dsOthers, Qt::darkYellow, Qt::yellow, false, true));
}



/*
hardcoded perl highlight

Op Customary  Generic     Meaning    Interpolates         Modifiers
1     ''       q{}       Literal         no
2     ""      qq{}       Literal         yes
3     ``      qx{}       Command         yes (no for ')
4             qw{}      Word list        no
5     //       m{}    Pattern match      yes (no for ')   cgimosx
6              s{}{}   Substitution      yes (no for ')   egimosx
7             tr{}{}   Translation       no               cds
7              y{}{}   Translation       no               cds
*/
int PerlHighlight::doHighlight(int ctxNum, TextLine *textLine) {
  static const char *opList[] = {"q", "qq", "qx", "qw", "m", "s", "tr", "y"};
  static int opLenList[] = {1, 2, 2, 2, 1, 1, 2, 1}; // length of strings in opList
  QChar delimiter;
  bool division;
  int op, argCount;
  bool interpolating, brackets, pod;

  const QChar *str, *s, *s2;
  QChar lastChar;
  bool lastWw;
  int pos, z, l;

  //extract some states out of the context number
  delimiter = QChar(ctxNum >> 9);
  division = ctxNum & 256;
  op = (ctxNum >> 5) & 7;
  argCount = (ctxNum >> 3) & 3;
  interpolating = !(ctxNum & 4);
  brackets = ctxNum & 2;
  pod = ctxNum & 1;

  //current line to process
  str = textLine->getString();
  //last caracter and its whole word check status
  lastChar = ' ';
  lastWw = true;

  s = str;

  //match pod documentation tags
  if (*s == '=') {
    s++;
    pod = true;
    if (ucmp(s, "cut", 3)) {
      pod = false;
      s += 3;
      textLine->setAttribs(7, 0, 4);
    }
  }
  if (pod) {
    textLine->setAttribs(7, 0, textLine->length());
    textLine->setAttr(7);
    goto finished;
  }
  while (*s) {
    pos = s - str;
    if (op == 0 && lastWw) {
      //match keyword
      s2 = keyword->checkHgl(s);
      if (s2 && !isInWord(*s2)) {
        s = s2;
        textLine->setAttribs(1, pos, s - str);
        goto newContext;
      }
      //match perl operator
      if (lastChar != '-') {
        for (z = 0; z < 8; z++) {
          l = opLenList[z];
          if (ucmp(s, opList[z], l) && !isInWord(s[l])) {
            //operator found
            op = z;            // generate op number (1 to 7)
            if (op < 7) op++;
            argCount = (op >= 6) ? 2 : 1; // number of arguments
            s += l;
            textLine->setAttribs(3, pos, pos + l);
            goto newContext;
          }
        }
      }
      //match customary
      if (*s == '\'') {
        op = 1;
        interpolating = false;
      }
      if (*s == '"') {
        op = 2;
      }
      if (*s == '`') {
        op = 3;
      }
      if (!division && *s == '/') { // don't take it if / is division
        op = 5;
      }
      if (op != 0) {
        delimiter = *s;
        s++;
        argCount = 1;
        textLine->setAttribs(3, pos, pos + 1);
        goto newContext;
      }
    }
    if (delimiter == '\0') { //not in string
      // match comment
      if (lastWw && *s == '#') {
        textLine->setAttribs(6, pos, textLine->length());
        textLine->setAttr(6);
        goto finished;
      }
      // match delimiter
      if (op != 0 && !s->isSpace()) {
        delimiter = *s;
        if (delimiter == '(') {
          delimiter = ')';
          brackets = true;
        }
        if (delimiter == '<') {
          delimiter = '>';
          brackets = true;
        }
        if (delimiter == '[') {
          delimiter = ']';
          brackets = true;
        }
        if (delimiter == '{') {
          delimiter = '}';
          brackets = true;
        }
        s++;
        if (op == 1 || op == 4 || op == 7 || (delimiter == '\'' && op != 2))
          interpolating = false;
        textLine->setAttribs(3, pos, pos + 1);
        goto newContext;
      }
      // match bind operator or command end
      if (*s == '~' || *s == ';') {
        division = false; // pattern matches with / now allowed
      }
    }
    if (interpolating) {
      // match variable
      if (*s == '$' || *s == '@' || *s == '%') {
        s2 = s;
        do {
          s2++;
        } while ((isInWord(*s2) || *s2 == '#') && *s2 != delimiter);
        if (s2 - s > 1) {
          // variable found
          s = s2;
          textLine->setAttribs(2, pos, s2 - str);
          division = true; // division / or /= may follow
          goto newContext;
        }
      }
      // match special variables
      if (s[0] == '$' && s[1] != '\0' && s[1] != delimiter) {
        if (ustrchr("&`'+*./|,\\;#%=-~^:?!@$<>()[]", s[1])) {
          // special variable found
          s += 2;
          textLine->setAttribs(2, pos, pos + 2);
          division = true; // division / or /= may follow
          goto newContext;
        }
      }
    }
    if (delimiter != '\0') { //in string
      //match escaped char
      if (interpolating) {
        if (*s == '\\' && s[1] != '\0') {
          s++;
          s2 = checkCharHexOct(s);
          if (s2) s = s2; else s++;
          textLine->setAttribs(5, pos, s - str);
          goto newContext;
        }
      }
      //match string end
      if (delimiter == *s) {
        s++;
        argCount--;
        if (argCount < 1) {
          //match operator modifiers
          if (op == 5) while (*s && ustrchr("cgimosx", *s)) s++;
          if (op == 6) while (*s && ustrchr("egimosx", *s)) s++;
          if (op == 7) while (*s && ustrchr("cds", *s)) s++;
          op = 0;
        }
        textLine->setAttribs(3, pos, s - str);
        if (brackets || op == 0) {
          interpolating = true;
          delimiter = '\0'; //string end delimiter = '\0' means "not in string"
          brackets = false;
        }
      } else {
        //highlight a ordinary character in string
        s++;
        textLine->setAttribs(4, pos, pos + 1);
      }
      goto newContext;
    }
    s++;
    textLine->setAttribs(0, pos, pos + 1);
    newContext:
    lastChar = s[-1];
    lastWw = !isInWord(lastChar);
  }
  textLine->setAttr(0);
  finished:

  //compose new context number
  ctxNum = delimiter.unicode() << 9;
  if (division) ctxNum |= 256;
  ctxNum |= op << 5;
  ctxNum |= argCount << 3;
  if (!interpolating) ctxNum |= 4;
  if (brackets) ctxNum |= 2;
  if (pod) ctxNum |= 1;
  return ctxNum;
  //method will be called again if there are more lines to highlight
}

void PerlHighlight::init() {
  keyword = new HlKeyword(0,0);
  keyword->addList(perlKeywords);
}

void PerlHighlight::done() {
  delete keyword;
}

SatherHighlight::SatherHighlight(const char * name) : GenHighlight(name) {
  iWildcards = "*.sa";
  iMimetypes = "text/x-sather-src";
}

SatherHighlight::~SatherHighlight() {
}

void SatherHighlight::createItemData(ItemDataList &list) {

  list.append(new ItemData(I18N_NOOP("Normal Text"        ),dsNormal)); // 0
  list.append(new ItemData(I18N_NOOP("Keyword"            ),dsKeyword));// 1
  list.append(new ItemData(I18N_NOOP("Special Classname"  ), dsNormal));// 2
  list.append(new ItemData(I18N_NOOP("Classname"          ),dsNormal)); // 3
  list.append(new ItemData(I18N_NOOP("Special Featurename"),dsOthers)); // 4
  list.append(new ItemData(I18N_NOOP("Identifier"         ),dsOthers)); // 5
  list.append(new ItemData(I18N_NOOP("Decimal"            ),dsDecVal)); // 6
  list.append(new ItemData(I18N_NOOP("Base-N"             ),dsBaseN));  // 7
  list.append(new ItemData(I18N_NOOP("Float"              ),dsFloat));  // 8
  list.append(new ItemData(I18N_NOOP("Char"               ),dsChar));   // 9
  list.append(new ItemData(I18N_NOOP("String"             ),dsString)); // 10
  list.append(new ItemData(I18N_NOOP("Comment"            ),dsComment));// 11
}

void SatherHighlight::makeContextList() {
  HlContext *c;
  HlKeyword *keyword,*spec_class,*spec_feat;

  //Normal Context
  contextList[0] = c = new HlContext(0,0);
    c->items.append(keyword = new HlKeyword(1,0));
    c->items.append(spec_class = new HlKeyword(2,0));
    c->items.append(new HlSatherClassname(3,0));
    c->items.append(spec_feat = new HlKeyword(4,0));
    c->items.append(new HlSatherIdent(5,0));
    c->items.append(new HlSatherFloat(8,0)); // check float before int
    c->items.append(new HlSatherBaseN(7,0));
    c->items.append(new HlSatherDec(6,0));
    c->items.append(new HlSatherChar(9,0));
    c->items.append(new HlSatherString(10,0));
    c->items.append(new Hl2CharDetect(11,1, '-', '-'));
  //Comment Context
  contextList[1] = c = new HlContext(11,0);

  keyword->addList(satherKeywords);
  spec_class->addList(satherSpecClassNames);
  spec_feat->addList(satherSpecFeatureNames);
}

LatexHighlight::LatexHighlight(const char * name) : GenHighlight(name) {
  iWildcards = "*.tex;*.sty";
  iMimetypes = "text/x-tex";
}

LatexHighlight::~LatexHighlight() {
}

void LatexHighlight::createItemData(ItemDataList &list) {

  list.append(new ItemData(I18N_NOOP("Normal Text"), dsNormal));
  list.append(new ItemData(I18N_NOOP("Tag/Keyword"), dsKeyword));
 list.append(new ItemData(I18N_NOOP("Optional Argument"), dsKeyword));
 list.append(new ItemData(I18N_NOOP("Mandatory Argument"), dsKeyword));
 list.append(new ItemData(I18N_NOOP("Inline Maths"), dsDecVal));
  list.append(new ItemData(I18N_NOOP("Char"       ), dsChar));
  list.append(new ItemData(I18N_NOOP("Parameter"  ), dsDecVal));
  list.append(new ItemData(I18N_NOOP("Comment"    ), dsComment));
}

void LatexHighlight::makeContextList() {
  HlContext *c;
/*
  //normal context
  contextList[0] = c = new HlContext(0,0);
    c->items.append(new HlLatexTag(1,0));
    c->items.append(new HlLatexChar(2,0));
    c->items.append(new HlLatexParam(3,0));
    c->items.append(new HlCharDetect(4,1,'%'));
  //one line comment context
  contextList[1] = new HlContext(4,0);
*/

  //normal context
  contextList[0] = c = new HlContext(0,0);
    c->items.append(new HlLatexTag(1,4));
    c->items.append(new HlLatexChar(5,0));
    c->items.append(new HlCharDetect(4,2,'$'));
    c->items.append(new HlLatexParam(6,0));
    c->items.append(new HlCharDetect(7,1,'%'));
  //one line comment context
  contextList[1] = new HlContext(7,0);
  //multiline inline maths context
  contextList[2] = c = new HlContext(4,2);
    c->items.append(new HlLatexChar(4,2));
    c->items.append(new HlCharDetect(4,3,'%'));
    c->items.append(new HlCharDetect(4,0,'$'));
  //comments in math mode context
  contextList[3] = new HlContext(7,2);
  //arguments to functions context
  contextList[4] = c = new HlContext(0,0);
    c->items.append(new HlCharDetect(2,5, '['));
    c->items.append(new HlCharDetect(3,6, '{'));
    c->items.append(new HlLatexTag(1,4));
    c->items.append(new HlLatexChar(5,0));
    c->items.append(new HlCharDetect(4,2,'$'));
    c->items.append(new HlLatexParam(6,0));
    c->items.append(new HlCharDetect(7,1,'%'));
  //optional arguments to functions context
  //this is buggy because nested arguments can span lines
  //should be OK for 99% of the time
  contextList[5] = c = new HlContext(2,5);
    c->items.append(new HlRangeDetect(2,5, '[', ']'));
    c->items.append(new HlCharDetect(2,4, ']'));
  //mandatory arguments to functions context
  //this is buggy because nested arguments can span lines
  //should be OK for 99% of the time
  contextList[6] = c = new HlContext(3,6);
    c->items.append(new HlRangeDetect(3,6, '{', '}'));
    c->items.append(new HlCharDetect(3,4, '}'));
}

//--------

// *******************************************************************
// quanta highlightings
// *******************************************************************

QString currentTag;
extern QDict <QStrList> *tagsDict;

HlPHPLineContinue::HlPHPLineContinue(int attribute, int context)
  : HlItem(attribute,context) {
}

const QChar *HlPHPLineContinue::checkHgl(const QChar *s) {
  return s + 1;
}

HlHtmlTag::HlHtmlTag(int attribute, int context)
  : HlItem(attribute,context) {
}

const QChar *HlHtmlTag::checkHgl(const QChar *s)
{
  while (*s == ' ' || *s == '\t' || *s == '/') s++;

  currentTag = "";

  while (*s != ' ' && *s != '\t' && *s != '>' && *s != '\0')
  {
  	currentTag += *s;
  	s++;
  }

  if ( currentTag.isEmpty() )
    return s;

  if ( tagsDict->find( currentTag.upper() ) )
    return s;
  else
    return 0L;
}

HlHtmlTagUnknown::HlHtmlTagUnknown(int attribute, int context)
  : HlItem(attribute,context) {
}

const QChar *HlHtmlTagUnknown::checkHgl(const QChar *s) {

  while (*s == ' ' || *s == '\t' || *s == '/') s++;

  currentTag = "";

  while (*s != ' ' && *s != '\t' && *s != '>' && *s != '\0')
  {
  	currentTag += *s;
  	s++;
  }

  if ( currentTag.isEmpty() )
    return s;

  if ( tagsDict->find( currentTag.upper() ) )
    return s;
  else
    return 0L;
}


HlHtmlAttrib::HlHtmlAttrib(int attribute, int context)
  : HlItem(attribute,context) {
}

const QChar *HlHtmlAttrib::checkHgl(const QChar *s)
{
  while (*s == ' ' || *s == '\t') s++;
  QString attr = "";

  while (*s != ' ' && *s != '\t' && *s != '>' && *s != '\0' && *s != '=' )
  {
  	attr += *s;
  	s++;
  }

  if ( currentTag.isEmpty() )
    return 0L;

  QStrList *list = tagsDict->find( currentTag.upper() );

  if ( !list )
    return 0L;

  if ( list->find( attr.lower() ) != -1 )
    return s;
  else
    return 0L; 

  return s;
}

HlHtmlAttribUnknown::HlHtmlAttribUnknown(int attribute, int context)
  : HlItem(attribute,context) {
}

const QChar *HlHtmlAttribUnknown::checkHgl(const QChar *s)
{
  while (*s == ' ' || *s == '\t') s++;
  QString attr = "";

  while (*s != ' ' && *s != '\t' && *s != '>' && *s != '\0' && *s != '=' )
  {
  	attr += *s;
  	s++;
  }

  if ( currentTag.isEmpty() )
    return s;

  QStrList *list = tagsDict->find( currentTag.upper() );

  if ( !list )
    return s;

  if ( list->find( attr.lower() ) == -1 )
    return s;
  else
    return 0L;
  

  return s;
}

HlPhpVar::HlPhpVar(int attribute, int context)
  : HlItem(attribute,context) {
}

const QChar *HlPhpVar::checkHgl(const QChar *s) {
  while (*s == ' ' || *s == '\t') s++;
  if (*s == '\"') {
    do {
      s++;
      if (!*s) return 0L;
    } while (*s != '\"');
    s++;
  } else {
    while ((*s >= 'a' && *s <='z') || (*s>='A' && *s <= 'Z' ) ||
    				*s == '_' || *s == '-' || (*s>='0' && *s<='9')    ||
    			 (*s=='-' && *(s+1)=='>')|| (*(s-1)=='-' && *s=='>') ) s++;
   }
  return s;
}

// highlitings

PhpHighlight::PhpHighlight(const char *name) : GenHighlight(name) {
  iWildcards = "*.php;*.inc;*.php3;*.phps;*.phtml";
  iMimetypes = "text/x-php-src;text/-php3-src";
}

PhpHighlight::~PhpHighlight() {
}

void PhpHighlight::createItemData(ItemDataList &list) {

  list.append(new ItemData("Normal Text",dsNormal));  // 0
  list.append(new ItemData("Keyword",dsKeyword));
  list.append(new ItemData("Data Type",dsDataType));
  list.append(new ItemData("Decimal",dsDecVal));
  list.append(new ItemData("Octal",dsBaseN));
  list.append(new ItemData("Hex",dsBaseN));            // 5
  list.append(new ItemData("Float",dsFloat));
  list.append(new ItemData("Char",dsChar));
  list.append(new ItemData("String",dsString));
  list.append(new ItemData("String Char",dsChar));
  list.append(new ItemData("Comment",dsComment));      // 10
  list.append(new ItemData("Tag Text",dsOthers,Qt::black,Qt::white,true,false));
  list.append(new ItemData("Tag",dsKeyword,Qt::darkMagenta,Qt::magenta,true,false));
  list.append(new ItemData("Tag Value",dsDecVal,Qt::darkCyan,Qt::cyan,false,false));
  list.append(new ItemData("HTML Char",dsChar,Qt::darkGreen,Qt::green,false,false));
  list.append(new ItemData("Php Variable",dsOthers,Qt::darkMagenta,Qt::magenta,false,false)); // 15
// added by pdima Thu Apr 13 00:52:33 2000

  list.append(new ItemData("Tag Attribute",dsDecVal,Qt::darkBlue,Qt::green,false,false)); //16
  list.append(new ItemData("Unknown Tag",dsComment,Qt::red,Qt::red,false,false)); //17
  list.append(new ItemData("Unknown Tag Attribute",dsComment,Qt::gray,Qt::gray,false,false)); //18
  list.append(new ItemData("Php Variable in Strings",dsOthers,Qt::darkGreen,Qt::green,false,false)); // 19

// end (pdima)

}

void PhpHighlight::makeContextList() {
  HlContext *c;
  HlKeyword *keyword, *dataType;
  HlKeyword *jskeyword, *jsdataType;
  HlKeyword *jsphpkeyword, *jsphpdataType;
  HlKeyword *hphpkeyword, *hphpdataType;

  // HTML
  contextList[0] = c = new HlContext(0,0);
    c->items.append(new HlRangeDetect(14,0,'&',';'));
    c->items.append(new HlStringDetect(10,14,"<!--"));
    c->items.append(new HlStringDetect(10,12,"<COMMENT>"));
    c->items.append(new HlStringDetect(10,12,"<comment>"));
    c->items.append(new HlStringDetect(12,1,"<?php"));
    c->items.append(new Hl2CharDetect(12,1,'<','?'));
    c->items.append(new HlCharDetect(12,13,'<'));
  contextList[12] = c = new HlContext(10,12);
    c->items.append(new HlStringDetect(10,0,"</COMMENT>"));
    c->items.append(new HlStringDetect(10,0,"</comment>"));
  contextList[13] = c = new HlContext(11,13);
    c->items.append(new HlCharDetect(12,13,' '));
    c->items.append(new HlStringDetect(12,20,"script"));
    c->items.append(new HlStringDetect(12,20,"SCRIPT"));
    c->items.append(new HlStringDetect(12,31,"<?php"));
    c->items.append(new HlStringDetect(12,31,"<?"));
    c->items.append(new HlHtmlTag(12,13));
    c->items.append(new HlHtmlValue(13,13));
// added by pdima Thu Apr 13 00:52:33 2000
    c->items.append(new HlHtmlAttrib(16,13));
    c->items.append(new HlHtmlTagUnknown(17,13));
    c->items.append(new HlHtmlAttribUnknown(18,13));
// end (pdima)
    c->items.append(new HlCharDetect(12,0,'>'));
	contextList[14] = c = new HlContext(10,14);
    c->items.append(new HlStringDetect(10,0,"-->"));

  //JavaScript
  contextList[20] = c = new HlContext(11,20);
    c->items.append(new HlHtmlValue(13,20));
    c->items.append(new HlCharDetect(12,21,'>'));

  contextList[21] = c = new HlContext(0,21);
    c->items.append(new HlStringDetect(12,41,"<?php"));
    c->items.append(new HlStringDetect(12,41,"<?"));
    c->items.append(jskeyword = new HlKeyword(1,21));
    c->items.append(jsdataType = new HlKeyword(2,21));
    c->items.append(new HlStringDetect(12,0,"</SCRIPT>"));
    c->items.append(new HlStringDetect(12,0,"</script>"));
    c->items.append(new HlStringDetect(10,21,"<!--"));
    c->items.append(new HlStringDetect(10,21,"-->"));
    c->items.append(new HlCFloat(6,21));
    c->items.append(new HlCOct(4,21));
    c->items.append(new HlCHex(5,21));
    c->items.append(new HlCInt(3,21));
    c->items.append(new HlCChar(7,21));
    c->items.append(new HlCharDetect(8,22,'"'));
    c->items.append(new Hl2CharDetect(10,23,'/','/'));
    c->items.append(new Hl2CharDetect(10,24,'/','*'));
    c->items.append(new HlCPrep(11,25));
  contextList[22] = c = new HlContext(8,21);
    c->items.append(new HlLineContinue(8,27));
    c->items.append(new HlCStringChar(9,22));
    c->items.append(new HlCharDetect(8,21,'"'));
  contextList[23] = new HlContext(10,21);
  contextList[24] = c = new HlContext(10,24);
    c->items.append(new Hl2CharDetect(10,21,'*','/'));
  contextList[25] = c = new HlContext(11,21);
    c->items.append(new HlLineContinue(11,28));
    c->items.append(new HlRangeDetect(12,25,'"','"'));
    c->items.append(new HlRangeDetect(12,25,'<','>'));
    c->items.append(new Hl2CharDetect(10,23,'/','/'));
    c->items.append(new Hl2CharDetect(10,26,'/','*'));
  contextList[26] = c = new HlContext(10,26);
    c->items.append(new Hl2CharDetect(10,25,'*','/'));
  contextList[27] = new HlContext(0,22);
  contextList[28] = new HlContext(0,25);


  // PHP
  contextList[1] = c = new HlContext(0,1);
    c->items.append(keyword = new HlKeyword(1,1));
    c->items.append(dataType = new HlKeyword(2,1));
    c->items.append(new HlCFloat(6,1));
    c->items.append(new HlCOct(4,1));
    c->items.append(new HlCHex(5,1));
    c->items.append(new HlCInt(3,1));
    c->items.append(new HlCChar(7,1));
    c->items.append(new HlCharDetect(15,9,'$'));
    c->items.append(new HlCharDetect(8,2,'"'));
    c->items.append(new HlCharDetect(8,11,'\''));
    c->items.append(new Hl2CharDetect(10,3,'/','/'));
    c->items.append(new Hl2CharDetect(10,4,'/','*'));
    //c->items.append(new HlCSymbol(0,1));
    c->items.append(new HlCharDetect(10,3,'#'));
    c->items.append(new Hl2CharDetect(12,0,'?','>'));
  contextList[2] = c = new HlContext(8,2);
    c->items.append(new HlCStringChar(9,2));
    c->items.append(new HlCharDetect(19,10,'$'));
    c->items.append(new HlCharDetect(8,1,'"'));
    c->items.append(new HlPHPLineContinue(8,7));
  contextList[3] = new HlContext(10,1);
  contextList[4] = c = new HlContext(10,4);
    c->items.append(new Hl2CharDetect(10,1,'*','/'));
  contextList[5] = c = new HlContext(11,1);
    c->items.append(new HlLineContinue(11,8));
    c->items.append(new HlRangeDetect(12,5,'"','"'));
    c->items.append(new HlRangeDetect(12,5,'<','>'));
    c->items.append(new Hl2CharDetect(10,3,'/','/'));
    c->items.append(new Hl2CharDetect(10,6,'/','*'));
  contextList[6] = c = new HlContext(10,6);
    c->items.append(new Hl2CharDetect(10,5,'*','/'));
  contextList[7] = new HlContext(0,2);
  contextList[8] = new HlContext(0,5);
  contextList[9] = c = new HlContext(15,1);
    c->items.append(new HlPhpVar(15,1));
  contextList[10] = c = new HlContext(19,1);
    c->items.append(new HlPhpVar(19,2));
  contextList[11] = c = new HlContext(8,1);
    c->items.append(new HlPHPLineContinue(8,7));
    c->items.append(new HlCStringChar(9,2));
    c->items.append(new HlCharDetect(8,1,'\''));

 // PHP in HTML Tag
  contextList[31] = c = new HlContext(0,31);
    c->items.append(hphpkeyword = new HlKeyword(1,31));
    c->items.append(hphpdataType = new HlKeyword(2,31));
    c->items.append(new HlCFloat(6,31));
    c->items.append(new HlCOct(4,31));
    c->items.append(new HlCHex(5,31));
    c->items.append(new HlCInt(3,31));
    c->items.append(new HlCChar(7,31));
    c->items.append(new HlCharDetect(15,39,'$'));
    c->items.append(new HlCharDetect(8,32,'"'));
    c->items.append(new Hl2CharDetect(10,33,'/','/'));
    c->items.append(new Hl2CharDetect(10,34,'/','*'));
    c->items.append(new HlCharDetect(10,33,'#'));
    c->items.append(new Hl2CharDetect(12,13,'?','>'));
//    c->items.append(new HlCPrep(11,4));
  contextList[32] = c = new HlContext(8,32);
    c->items.append(new HlCStringChar(9,32));
    c->items.append(new HlCharDetect(19,39,'$'));
    c->items.append(new HlCharDetect(8,31,'"'));
    c->items.append(new HlPHPLineContinue(8,37));
  contextList[33] = new HlContext(10,31);
  contextList[34] = c = new HlContext(10,34);
    c->items.append(new Hl2CharDetect(10,31,'*','/'));
  contextList[35] = c = new HlContext(11,31);
    c->items.append(new HlLineContinue(11,38));
    c->items.append(new HlRangeDetect(12,35,'"','"'));
    c->items.append(new HlRangeDetect(12,35,'<','>'));
    c->items.append(new Hl2CharDetect(10,33,'/','/'));
    c->items.append(new Hl2CharDetect(10,36,'/','*'));
  contextList[36] = c = new HlContext(10,36);
    c->items.append(new Hl2CharDetect(10,35,'*','/'));
  contextList[37] = new HlContext(0,32);
  contextList[38] = new HlContext(0,35);
  contextList[39] = c = new HlContext(15,31);
    c->items.append(new HlPhpVar(15,32));

 // PHP in JavaScript
  contextList[41] = c = new HlContext(0,41);
    c->items.append(jsphpkeyword = new HlKeyword(1,41));
    c->items.append(jsphpdataType = new HlKeyword(2,41));
    c->items.append(new HlCFloat(6,41));
    c->items.append(new HlCOct(4,41));
    c->items.append(new HlCHex(5,41));
    c->items.append(new HlCInt(3,41));
    c->items.append(new HlCChar(7,41));
    c->items.append(new HlCharDetect(15,49,'$'));
    c->items.append(new HlCharDetect(8,42,'"'));
    c->items.append(new Hl2CharDetect(10,43,'/','/'));
    c->items.append(new Hl2CharDetect(10,44,'/','*'));
    c->items.append(new HlCharDetect(10,43,'#'));
    c->items.append(new Hl2CharDetect(12,21,'?','>'));
//    c->items.append(new HlCPrep(11,4));
  contextList[42] = c = new HlContext(8,41);
    c->items.append(new HlCStringChar(9,42));
    c->items.append(new HlCharDetect(19,49,'$'));
    c->items.append(new HlCharDetect(8,41,'"'));
    c->items.append(new HlPHPLineContinue(8,47));
  contextList[43] = new HlContext(10,41);
  contextList[44] = c = new HlContext(10,44);
    c->items.append(new Hl2CharDetect(10,41,'*','/'));
  contextList[45] = c = new HlContext(11,41);
    c->items.append(new HlLineContinue(11,48));
    c->items.append(new HlRangeDetect(12,45,'"','"'));
    c->items.append(new HlRangeDetect(12,45,'<','>'));
    c->items.append(new Hl2CharDetect(10,43,'/','/'));
    c->items.append(new Hl2CharDetect(10,46,'/','*'));
  contextList[46] = c = new HlContext(10,46);
    c->items.append(new Hl2CharDetect(10,45,'*','/'));
  contextList[47] = new HlContext(0,42);
  contextList[48] = new HlContext(0,45);
  contextList[49] = c = new HlContext(15,41);
    c->items.append(new HlPhpVar(15,42));

  setKeywords(keyword, dataType);

  jskeyword->addList(jsKeywords);
  jsdataType->addList(jsTypes);

  jsphpkeyword->addList(phpKeywords);
  jsphpdataType->addList(phpTypes);

  hphpkeyword->addList(phpKeywords);
  hphpdataType->addList(phpTypes);
}

void PhpHighlight::setKeywords(HlKeyword *keyword, HlKeyword *dataType)
{
  keyword->addList(phpKeywords);
  dataType->addList(phpTypes);
}

JSHighlight::JSHighlight(const char *name) : GenHighlight(name) {
  iWildcards = "*.js";
  iMimetypes = "text/x-js-src";
}

JSHighlight::~JSHighlight() {
}

void JSHighlight::createItemData(ItemDataList &list)
{
  list.append(new ItemData("Normal Text",dsNormal));
  list.append(new ItemData("Keyword",dsKeyword));
  list.append(new ItemData("Data Type",dsDataType));
  list.append(new ItemData("Decimal",dsDecVal));
  list.append(new ItemData("Octal",dsBaseN));
  list.append(new ItemData("Hex",dsBaseN));
  list.append(new ItemData("Float",dsFloat));
  list.append(new ItemData("Char",dsChar));
  list.append(new ItemData("String",dsString));
  list.append(new ItemData("String Char",dsChar));
  list.append(new ItemData("Comment",dsComment));
}

void JSHighlight::makeContextList() {
  HlContext *c;
  HlKeyword *keyword, *dataType;

  //JavaScript

  contextList[0] = c = new HlContext(0,0);
    c->items.append(keyword = new HlKeyword(1,0));
    c->items.append(dataType = new HlKeyword(2,0));
    c->items.append(new HlCFloat(6,0));
    c->items.append(new HlCOct(4,0));
    c->items.append(new HlCHex(5,0));
    c->items.append(new HlCInt(3,0));
    c->items.append(new HlCChar(7,0));
    c->items.append(new HlCharDetect(8,22,'"'));
    c->items.append(new Hl2CharDetect(10,23,'/','/'));
    c->items.append(new Hl2CharDetect(10,24,'/','*'));
//    c->items.append(new HlCPrep(11,25));
  contextList[22] = c = new HlContext(8,0);
    c->items.append(new HlLineContinue(8,27));
    c->items.append(new HlCStringChar(9,22));
    c->items.append(new HlCharDetect(8,0,'"'));
  contextList[23] = new HlContext(10,0);
  contextList[24] = c = new HlContext(10,24);
    c->items.append(new Hl2CharDetect(10,0,'*','/'));
  contextList[25] = c = new HlContext(11,0);
    c->items.append(new HlLineContinue(11,28));
    c->items.append(new HlRangeDetect(12,25,'"','"'));
    c->items.append(new HlRangeDetect(12,25,'<','>'));
    c->items.append(new Hl2CharDetect(10,23,'/','/'));
    c->items.append(new Hl2CharDetect(10,26,'/','*'));
  contextList[26] = c = new HlContext(10,26);
    c->items.append(new Hl2CharDetect(10,25,'*','/'));
  contextList[27] = new HlContext(0,22);
  contextList[28] = new HlContext(0,25);

  setKeywords(keyword, dataType);
}

void JSHighlight::setKeywords(HlKeyword *keyword, HlKeyword *dataType) {

  keyword->addList(jsKeywords);
  dataType->addList(jsTypes);

}

SqlHighlight::SqlHighlight(const char *name) : GenHighlight(name) {
  iWildcards = "*.sql;*.SQL;*.pgsql;*.PGSQL";
  iMimetypes = "text/x-sql";
}

SqlHighlight::~SqlHighlight() {
}

void SqlHighlight::createItemData(ItemDataList &list) {
  list.append(new ItemData("Normal Text",dsNormal));
  list.append(new ItemData("Keyword",dsKeyword));
  list.append(new ItemData("Data Type",dsDataType));
  list.append(new ItemData("Decimal",dsDecVal));
  list.append(new ItemData("Octal",dsBaseN));
  list.append(new ItemData("Hex",dsBaseN));
  list.append(new ItemData("Float",dsFloat));
  list.append(new ItemData("Char",dsChar));
  list.append(new ItemData("String",dsString));
  list.append(new ItemData("String Char",dsChar));
  list.append(new ItemData("Comment",dsComment));
  list.append(new ItemData("Others",dsOthers,Qt::magenta,Qt::green,false,false));
}

void SqlHighlight::makeContextList() {
  HlContext *c;
  HlKeyword *keyword, *dataType;

  contextList[0] = c = new HlContext(0,0);
//    c->items.append(keyword = new HlCaseKeyword(1,0));
//    c->items.append(dataType = new HlCaseKeyword(2,0));
    c->items.append(keyword = new HlKeyword(1,0));
    c->items.append(dataType = new HlKeyword(2,0));
    c->items.append(new HlCFloat(6,0));
    c->items.append(new HlCOct(4,0));
    c->items.append(new HlCHex(5,0));
    c->items.append(new HlCInt(3,0));
    c->items.append(new HlCChar(7,0));
    c->items.append(new HlCharDetect(8,1,'\''));
    c->items.append(new Hl2CharDetect(10,2,'-','-'));
    c->items.append(new Hl2CharDetect(10,3,'/','*'));
    c->items.append(new HlCPrep(11,4));
  contextList[1] = c = new HlContext(8,0);
    c->items.append(new HlLineContinue(8,6));
    c->items.append(new HlCStringChar(9,1));
    c->items.append(new HlCharDetect(8,0,'\''));
  contextList[2] = new HlContext(10,0);
  contextList[3] = c = new HlContext(10,3);
    c->items.append(new Hl2CharDetect(10,0,'*','/'));
  contextList[4] = c = new HlContext(11,0);
    c->items.append(new HlLineContinue(11,7));
    c->items.append(new HlRangeDetect(12,4,'"','"'));
    c->items.append(new HlRangeDetect(12,4,'<','>'));
    c->items.append(new Hl2CharDetect(10,2,'-','-'));
    c->items.append(new Hl2CharDetect(10,5,'/','*'));
  contextList[5] = c = new HlContext(10,5);
    c->items.append(new Hl2CharDetect(10,4,'*','/'));
  contextList[6] = new HlContext(0,1);
  contextList[7] = new HlContext(0,4);

  setKeywords((HlKeyword*)keyword, (HlKeyword*)dataType);
}

void SqlHighlight::setKeywords(HlKeyword *keyword, HlKeyword *dataType) {
  keyword->addList(sqlKeywords);
  dataType->addList(sqlTypes);
}

// *******************************************************************
// end of quanta highlightings
// *******************************************************************


HlManager *HlManager::s_pSelf = 0;

HlManager::HlManager() : QObject(0L) {

  hlList.setAutoDelete(true);
  hlList.append(new Highlight(I18N_NOOP("Normal")));
  hlList.append(new CHighlight(     "C"        ));
  hlList.append(new CppHighlight(   "C++"      ));
  hlList.append(new ObjcHighlight(  "Objective-C"));
  hlList.append(new JavaHighlight(  "Java"     ));
  hlList.append(new HtmlHighlight(  "HTML"     ));
  hlList.append(new BashHighlight(  "Bash"     ));
  hlList.append(new ModulaHighlight("Modula 2" ));
  hlList.append(new AdaHighlight(   "Ada"      ));
  hlList.append(new PythonHighlight("Python"   ));
  hlList.append(new PerlHighlight(  "Perl"     ));
  hlList.append(new SatherHighlight("Sather"   ));
  hlList.append(new LatexHighlight( "Latex"    ));
  hlList.append(new IdlHighlight("IDL"));
}

HlManager::~HlManager() {
}

HlManager *HlManager::self()
{
  if ( !s_pSelf )
    s_pSelf = new HlManager;
  return s_pSelf;
}

Highlight *HlManager::getHl(int n) {
  if (n < 0 || n >= (int) hlList.count()) n = 0;
  return hlList.at(n);
}

int HlManager::defaultHl() {
  KConfig *config;

  config = kapp->config();
  config->setGroup("General Options");
  return nameFind(config->readEntry("Highlight"));
}


int HlManager::nameFind(const QString &name) {
  int z;

  for (z = hlList.count() - 1; z > 0; z--) {
    if (hlList.at(z)->iName == name) break;
  }
  return z;
}

int HlManager::wildcardFind(const QString &fileName) {
  Highlight *highlight;
  int p1, p2;
  QString w;
  for (highlight = hlList.first(); highlight != 0L; highlight = hlList.next()) {
    p1 = 0;
    w = highlight->getWildcards();
    while (p1 < (int) w.length()) {
      p2 = w.find(';',p1);
      if (p2 == -1) p2 = w.length();
      if (p1 < p2) {
        QRegExp regExp(w.mid(p1,p2 - p1),true,true);
        if (regExp.match(fileName) == 0) return hlList.at();
      }
      p1 = p2 + 1;
    }
  }
  return -1;
}

int HlManager::mimeFind(const QByteArray &contents, const QString &fname)
{
/*
  // fill the detection buffer with the contents of the text
  const int HOWMANY = 1024;
  char buffer[HOWMANY];
  int number=0, len;

  for (int index=0; index<doc->lastLine(); index++)
  {
    len = doc->textLength(index);

    if (number+len > HOWMANY)
      break;

    memcpy(&buffer[number], doc->textLine(index)->getText(), len);
    number += len;
  }
*/
  // detect the mime type
  KMimeMagicResult *result;
  result = KMimeMagic::self()->findBufferFileType(contents, fname);

  Highlight *highlight;
  int p1, p2;
  QString w;

  for (highlight = hlList.first(); highlight != 0L; highlight = hlList.next())
  {
    w = highlight->getMimetypes();

    p1 = 0;
    while (p1 < (int) w.length()) {
      p2 = w.find(';',p1);
      if (p2 == -1) p2 = w.length();
      if (p1 < p2) {
        QRegExp regExp(w.mid(p1,p2 - p1),true,true);
        if (regExp.match(result->mimeType()) == 0) return hlList.at();
      }
      p1 = p2 + 1;
    }
  }

  return -1;
}

int HlManager::makeAttribs(Highlight *highlight, Attribute *a, int maxAttribs) {
  ItemStyleList defaultStyleList;
  ItemStyle *defaultStyle;
  ItemFont defaultFont;
  ItemDataList itemDataList;
  ItemData *itemData;
  int nAttribs, z;
  QFont font;

  defaultStyleList.setAutoDelete(true);
  getDefaults(defaultStyleList, defaultFont);

  itemDataList.setAutoDelete(true);
  highlight->getItemDataList(itemDataList);
  nAttribs = itemDataList.count();
  for (z = 0; z < nAttribs; z++) {
    itemData = itemDataList.at(z);
    if (itemData->defStyle) {
      // default style
      defaultStyle = defaultStyleList.at(itemData->defStyleNum);
      a[z].col = defaultStyle->col;
      a[z].selCol = defaultStyle->selCol;
      //font.setBold(defaultStyle->bold);
      font.setItalic(defaultStyle->italic);
    } else {
      // custom style
      a[z].col = itemData->col;
      a[z].selCol = itemData->selCol;
      //font.setBold(itemData->bold);
      font.setItalic(itemData->italic);
    }
    if (itemData->defFont) {
      font.setFamily(defaultFont.family);
      font.setPointSize(defaultFont.size);
//      KCharset(defaultFont.charset).setQFont(font);
    } else {
      font.setFamily(itemData->family);
      font.setPointSize(itemData->size);
//      KCharset(itemData->charset).setQFont(font);
    }
    a[z].setFont(font);
  }
  for (; z < maxAttribs; z++) {
    a[z].col = black;
    a[z].selCol = black;
    a[z].setFont(font);
  }
  return nAttribs;
}

int HlManager::defaultStyles() {
  return 10;
}

const char * HlManager::defaultStyleName(int n) {
  static const char *names[] = {
    I18N_NOOP("Normal"),
    I18N_NOOP("Keyword"),
    I18N_NOOP("Data Type"),
    I18N_NOOP("Decimal/Value"),
    I18N_NOOP("Base-N Integer"),
    I18N_NOOP("Floating Point"),
    I18N_NOOP("Character"),
    I18N_NOOP("String"),
    I18N_NOOP("Comment"),
    I18N_NOOP("Others")};

  return names[n];
}

void HlManager::getDefaults(ItemStyleList &list, ItemFont &font) {
  KConfig *config;
  int z;
  ItemStyle *i;
  QString s;
  QRgb col, selCol;

  list.setAutoDelete(true);
  //ItemStyle(color, selected color, bold, italic)
  list.append(new ItemStyle(black,white,false,false));     //normal
  list.append(new ItemStyle(black,white,true,false));      //keyword
  list.append(new ItemStyle(darkRed,white,false,false));   //datatype
  list.append(new ItemStyle(blue,cyan,false,false));       //decimal/value
  list.append(new ItemStyle(darkCyan,cyan,false,false));   //base n
  list.append(new ItemStyle(darkMagenta,cyan,false,false));//float
  list.append(new ItemStyle(magenta,magenta,false,false)); //char
  list.append(new ItemStyle(red,red,false,false));         //string
  list.append(new ItemStyle(darkGray,gray,false,true));    //comment
  list.append(new ItemStyle(darkGreen,green,false,false)); //others

  config = kapp->config();
  config->setGroup("Default Item Styles");
  for (z = 0; z < defaultStyles(); z++) {
    i = list.at(z);
    s = config->readEntry(defaultStyleName(z));
    if (!s.isEmpty()) {
      sscanf(s.latin1(),"%X,%X,%d,%d",&col,&selCol,&i->bold,&i->italic);
      i->col.setRgb(col);
      i->selCol.setRgb(selCol);
    }
  }

  config->setGroup("Default Font");
  QFont defaultFont = KGlobalSettings::fixedFont();
  font.family = config->readEntry("Family", defaultFont.family());
  font.size = config->readNumEntry("Size", defaultFont.pointSize());
  font.charset = config->readEntry("Charset","ISO-8859-1");
}

void HlManager::setDefaults(ItemStyleList &list, ItemFont &font) {
  KConfig *config;
  int z;
  ItemStyle *i;
  char s[64];

  config = kapp->config();
  config->setGroup("Default Item Styles");
  for (z = 0; z < defaultStyles(); z++) {
    i = list.at(z);
    sprintf(s,"%X,%X,%d,%d",i->col.rgb(),i->selCol.rgb(),i->bold, i->italic);
    config->writeEntry(defaultStyleName(z),s);
  }

  config->setGroup("Default Font");
  config->writeEntry("Family",font.family);
  config->writeEntry("Size",font.size);
  config->writeEntry("Charset",font.charset);

  emit changed();
}


int HlManager::highlights() {
  return (int) hlList.count();
}

const char * HlManager::hlName(int n) {
  return hlList.at(n)->iName;
}

void HlManager::getHlDataList(HlDataList &list) {
  int z;

  for (z = 0; z < (int) hlList.count(); z++) {
    list.append(hlList.at(z)->getData());
  }
}

void HlManager::setHlDataList(HlDataList &list) {
  int z;

  for (z = 0; z < (int) hlList.count(); z++) {
    hlList.at(z)->setData(list.at(z));
  }
  //notify documents about changes in highlight configuration
  emit changed();
}

StyleChanger::StyleChanger( QWidget *parent )
  : QWidget(parent)
{
  QLabel *label;

  QGridLayout *glay = new QGridLayout( this, 4, 3, 0, KDialog::spacingHint() );
  CHECK_PTR(glay);
  glay->addColSpacing( 1, KDialog::spacingHint() ); // Looks better
  glay->setColStretch( 2, 10 );

  col = new KColorButton(this);
  CHECK_PTR(col);
  connect(col,SIGNAL(changed(const QColor &)),this,SLOT(changed()));
  label = new QLabel(col,i18n("Normal:"),this);
  CHECK_PTR(label);
  glay->addWidget(label,0,0);
  glay->addWidget(col,1,0);

  selCol = new KColorButton(this);
  CHECK_PTR(selCol);
  connect(selCol,SIGNAL(changed(const QColor &)),this,SLOT(changed()));
  label = new QLabel(selCol,i18n("Selected:"),this);
  CHECK_PTR(label);
  glay->addWidget(label,2,0);
  glay->addWidget(selCol,3,0);

  bold = new QCheckBox(i18n("Bold"),this);
  CHECK_PTR(bold);
  connect(bold,SIGNAL(clicked()),SLOT(changed()));
  glay->addWidget(bold,1,2);

  italic = new QCheckBox(i18n("Italic"),this);
  CHECK_PTR(italic);
  connect(italic,SIGNAL(clicked()),SLOT(changed()));
  glay->addWidget(italic,2,2);
}

void StyleChanger::setRef(ItemStyle *s) {

  style = s;
  col->setColor(style->col);
  selCol->setColor(style->selCol);
  bold->setChecked(style->bold);
  italic->setChecked(style->italic);

}

void StyleChanger::setEnabled(bool enable) {

  col->setEnabled(enable);
  selCol->setEnabled(enable);
  bold->setEnabled(enable);
  italic->setEnabled(enable);
}

void StyleChanger::changed() {

  if (style) {
    style->col = col->color();
    style->selCol = selCol->color();
    style->bold = bold->isChecked();
    style->italic = italic->isChecked();
  }
}





FontChanger::FontChanger( QWidget *parent )
  : QWidget(parent)
{
  QLabel *label;
  QStringList fontList;

  QVBoxLayout *vlay = new QVBoxLayout( this, 0, KDialog::spacingHint() );
  CHECK_PTR(vlay);

  familyCombo = new QComboBox(true,this);
  CHECK_PTR(familyCombo);
  label = new QLabel( familyCombo,i18n("Family:"), this );
  CHECK_PTR(label);
  vlay->addWidget(label);
  vlay->addWidget(familyCombo);
  connect( familyCombo, SIGNAL(activated(const QString&)),
           this, SLOT(familyChanged(const QString&)));
  KFontChooser::getFontList(fontList, false);
  familyCombo->insertStringList(fontList);


  sizeCombo = new QComboBox(true,this);
  CHECK_PTR(sizeCombo);
  label = new QLabel(sizeCombo,i18n("Size:"),this);
  CHECK_PTR(label);
  vlay->addWidget(label);
  vlay->addWidget(sizeCombo);
  connect( sizeCombo, SIGNAL(activated(int)),
           this, SLOT(sizeChanged(int)) );
  for( int i=0; fontSizes[i] != 0; i++ ){
    sizeCombo->insertItem(QString().setNum(fontSizes[i]));
  }


  charsetCombo = new QComboBox(true,this);
  CHECK_PTR(charsetCombo);
  label = new QLabel(charsetCombo,i18n("Charset:"),this);
  CHECK_PTR(label);
  vlay->addWidget(label);
  vlay->addWidget(charsetCombo);
  connect( charsetCombo, SIGNAL(activated(const QString&)),
           this, SLOT(charsetChanged(const QString&)) );
}


void FontChanger::setRef(ItemFont *f) {
  int z;

  font = f;
  for (z = 0; z < (int) familyCombo->count(); z++) {
    if (font->family == familyCombo->text(z)) {
      familyCombo->setCurrentItem(z);
      goto found;
    }
  }
  font->family = familyCombo->text(0);
found:

  for (z = 0; fontSizes[z] > 0; z++) {
    if (font->size == fontSizes[z]) {
      sizeCombo->setCurrentItem(z);
      break;
    }
  }
  displayCharsets();
}

void FontChanger::familyChanged(const QString& family) {

  font->family = family;
  displayCharsets();
}

void FontChanger::sizeChanged(int n) {

  font->size = fontSizes[n];;
}

void FontChanger::charsetChanged(const QString& charset) {

  font->charset = charset;
  //KCharset(chset).setQFont(font);
}

void FontChanger::displayCharsets() {
  int z;
  QString charset;
  KCharsets *charsets;

  charsets = KGlobal::charsets();
  QStringList lst = charsets->availableCharsetNames(font->family);
//  QStrList lst = charsets->displayable(font->family);
  charsetCombo->clear();
  for(z = 0; z < (int) lst.count(); z++) {
    charset = *lst.at(z);
    charsetCombo->insertItem(charset);
    if (/*(QString)*/ font->charset == charset) charsetCombo->setCurrentItem(z);
  }
  charset = "any";
  charsetCombo->insertItem(charset);
  if (/*(QString)*/ font->charset == charset) charsetCombo->setCurrentItem(z);
}

//---------


HighlightDialog::HighlightDialog( HlManager *hlManager, ItemStyleList *styleList,
                                  ItemFont *font,
                                  HlDataList *highlightDataList,
                                  int hlNumber, QWidget *parent,
                                  const char *name, bool modal )
  :KDialogBase(KDialogBase::Tabbed, i18n("Highlight Settings"), Ok|Cancel, Ok, parent, name, modal),
   defaultItemStyleList(styleList), hlData(0L)
{

  // defaults =========================================================

  QFrame *page1 = addPage(i18n("&Defaults"));
  QGridLayout *grid = new QGridLayout(page1,2,2,0,spacingHint());

  QVGroupBox *dvbox1 = new QVGroupBox( i18n("Default Item Styles"), page1 );
  /*QLabel *label = */new QLabel( i18n("Item:"), dvbox1 );
  QComboBox *styleCombo = new QComboBox( false, dvbox1 );
  defaultStyleChanger = new StyleChanger( dvbox1 );
  for( int i = 0; i < hlManager->defaultStyles(); i++ ) {
    styleCombo->insertItem(i18n(hlManager->defaultStyleName(i)));
  }
  connect(styleCombo, SIGNAL(activated(int)), this, SLOT(defaultChanged(int)));
  grid->addWidget(dvbox1,0,0);

  QVGroupBox *dvbox2 = new QVGroupBox( i18n("Default Font"), page1 );
  defaultFontChanger = new FontChanger( dvbox2 );
  defaultFontChanger->setRef(font);
  grid->addWidget(dvbox2,0,1);

  grid->setRowStretch(1,1);
  grid->setColStretch(1,1);

  defaultChanged(0);

  // highlight modes =====================================================

  QFrame *page2 = addPage(i18n("&Highlight Modes"));
  grid = new QGridLayout(page2,3,2,0,spacingHint());

  QVGroupBox *vbox1 = new QVGroupBox( i18n("Config Select"), page2 );
  grid->addWidget(vbox1,0,0);
  QVGroupBox *vbox2 = new QVGroupBox( i18n("Item Style"), page2 );
  grid->addWidget(vbox2,1,0);
  QVGroupBox *vbox3 = new QVGroupBox( i18n("Highlight Auto Select"), page2 );
  grid->addWidget(vbox3,0,1);
  QVGroupBox *vbox4 = new QVGroupBox( i18n("Item Font"), page2 );
  grid->addWidget(vbox4,1,1);

  grid->setRowStretch(2,1);
  grid->setColStretch(1,1);

  QLabel *label = new QLabel( i18n("Highlight:"), vbox1 );
  hlCombo = new QComboBox( false, vbox1 );
  connect( hlCombo, SIGNAL(activated(int)),
           this, SLOT(hlChanged(int)) );
  for( int i = 0; i < hlManager->highlights(); i++) {
    hlCombo->insertItem(hlManager->hlName(i));
  }
  hlCombo->setCurrentItem(hlNumber);


  label = new QLabel( i18n("Item:"), vbox1 );
  itemCombo = new QComboBox( false, vbox1 );
  connect( itemCombo, SIGNAL(activated(int)), this, SLOT(itemChanged(int)) );

  label = new QLabel( i18n("File Extensions:"), vbox3 );
  wildcards  = new QLineEdit( vbox3 );
  label = new QLabel( i18n("Mime Types:"), vbox3 );
  mimetypes = new QLineEdit( vbox3 );


  styleDefault = new QCheckBox(i18n("Default"), vbox2 );
  connect(styleDefault,SIGNAL(clicked()),SLOT(changed()));
  styleChanger = new StyleChanger( vbox2 );


  fontDefault = new QCheckBox(i18n("Default"), vbox4 );
  connect(fontDefault,SIGNAL(clicked()),SLOT(changed()));
  fontChanger = new FontChanger( vbox4 );

  hlDataList = highlightDataList;
  hlChanged(hlNumber);
}


void HighlightDialog::defaultChanged(int z)
{
  defaultStyleChanger->setRef(defaultItemStyleList->at(z));
}


void HighlightDialog::hlChanged(int z)
{
  writeback();

  hlData = hlDataList->at(z);

  wildcards->setText(hlData->wildcards);
  mimetypes->setText(hlData->mimetypes);

  itemCombo->clear();
  for (ItemData *itemData = hlData->itemDataList.first(); itemData != 0L;
    itemData = hlData->itemDataList.next()) {
    itemCombo->insertItem(i18n(itemData->name));
  }

  itemChanged(0);
}

void HighlightDialog::itemChanged(int z)
{
  itemData = hlData->itemDataList.at(z);

  styleDefault->setChecked(itemData->defStyle);
  styleChanger->setRef(itemData);

  fontDefault->setChecked(itemData->defFont);
  fontChanger->setRef(itemData);
}

void HighlightDialog::changed()
{
  itemData->defStyle = styleDefault->isChecked();
  itemData->defFont = fontDefault->isChecked();
}

void HighlightDialog::writeback() {
  if (hlData) {
    hlData->wildcards = wildcards->text();
    hlData->mimetypes = mimetypes->text();
  }
}

void HighlightDialog::done(int r) {
  writeback();
  QDialog::done(r);
}

#include "highlight.moc"
