<?php
/* Please note: This file was slightly modified please see original version. */

/*****************************************************************************
 * The contents of this file are subject to the RECIPROCAL PUBLIC LICENSE
 * Version 1.1 ("License"); You may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 * http://opensource.org/licenses/rpl.php. Software distributed under the
 * License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND,
 * either express or implied.
 *
 * @author:  Mr. Milk (aka Marcelo Leite)
 * @email:   mrmilk@anysoft.com.br
 * @version: 0.9 beta
 * @date:    2007-07-07
 *
 *****************************************************************************/
if (isDownload()) {
  downloadFile();
} elseif(isCommandLine()) {
  processBatch();
}?>
<html>
  <head>
  <meta http-equiv='Content-Type' content='text/html; charset=UTF-8'>
  <script>
    var menus = [
      [ "Indentation and General Formatting", "", [
        [ "indent_size", "&nbsp;characters per indentation level" ],
        [ "indent_with_tabs", "Indent with tabs instead of spaces" ],
        [ "keep_redundant_lines", "Keep redundant lines" ],
        [ "space_inside_parentheses", "Space inside parentheses" ],
        [ "space_outside_parentheses", "Space outside parentheses" ],
        [ "space_after_comma", "Space after comma" ]
      ]],
      [ "Operators", "(=, .=, +=, -=, *=, /=, &=, |=, <<<, ==, ===, !=, !==, >, >=, <, <=, -, +, *, /, %, &&, ||, AND, OR, XOR, ?, :)", [
        [ "space_around_assignment", "Space around assignment (= .= += -= *= /= <<<)" ],
        [ "align_var_assignment", "Align block +3 assigned variables" ],
        [ "space_around_comparison", "Space around comparison (== === != !== > >= < <=)" ],
        [ "space_around_arithmetic", "Space around arithmetic (- + * / %)" ],
        [ "space_around_logical", "Space around logical (&& || AND OR XOR << >>)" ],
        [ "space_around_colon_question", "Space around question/colon (? :)" ]
      ]],
      [ "Functions, Classes and Objects", "(keywords: function, class, interface, abstract, public, protected, private, final, ->, ::)", [
        [ "line_before_function", "Blank line before keyword" ],
        [ "line_before_curly_function", "Opening bracket on next line" ],
        [ "line_after_curly_function", "Blank line below opening bracket" ],
        [ "space_around_obj_operator", "Space around object operator (->)" ],
        [ "space_around_double_colon", "Space around double colon (::)" ]
      ]],
      [ "Control Structures", "(if, elseif, else, for, foreach, while, do, switch, break)", [
        [ "space_after_if", "Space between keyword and opening parentheses" ],
        [ "else_along_curly", "Keep else/elseif along with bracket" ],
        [ "line_before_curly", "Opening bracket on next line" ],
        [ "add_missing_braces", "Add missing brackets to single line structs" ],
        [ "line_after_break", "Blank line after case \"break\"" ],
        [ "space_inside_for", "Space between \"for\" elements" ],
        [ "indent_case", "Extra indent for \"Case\" and \"Default\"" ]
      ]],
      [ "Arrays and Concatenation", "(array, dot, double arrow =>)", [
        [ "line_before_array", "Opening array parentheses on next line" ],
        [ "vertical_array", "Non-empty arrays as vertical block" ],
        [ "align_array_assignment", "Align block +3 assigned array elements" ],
        [ "space_around_double_arrow", "Space around double arrow" ],
        [ "vertical_concat", "Concatenation as vertical block" ],
        [ "space_around_concat", "Space around concat elements" ]
      ]],
      [ "Comments", "(//, #, /* */, /** */)", [
        [ "line_before_comment_multi", "Blank line before multi-line comment (/*)" ],
        [ "line_after_comment_multi", "Blank line after multi-line comment (/*)" ],
        [ "line_before_comment", "Blank line before single line comments (//)" ],
        [ "line_after_comment", "Blank line after single line comments (//)" ]
      ]]
    ];

    window.onload = function() {

      t = gEBI("g").cloneNode(true);
      r = gEBI("e").cloneNode(true);
      i = gEBI("i").cloneNode(true);
      loadFileMenu();
      for(y=0; y<(menus.length); y++) {
        h = menus[y][0];
        k = menus[y][1];
        addGroup(h, k, y+1);
        for(z=0; z<(menus[y][2].length); z++) {
          n = menus[y][2][z][0];
          d = menus[y][2][z][1];
          addItem(n, d, z, y+1);
        }
        if(getCookie("menu"+(y+1))=="on") {
          collapse(gEBN("a")[y+1]);
        }
      }
      gEBI("nowrap").checked = getCookie("nowrap")=="on" ? true : false;
      gEBI("iso8859").checked = getCookie("iso8859")=="on" ? true : false;
      gEBN("textarea")[1].checked = getCookie("textarea")!="on" ? true : false;
      gEBN("textarea")[0].checked = !gEBN("textarea")[1].checked;
      if(gEBN("textarea")[1].checked) {
        switchPanels(gEBN("textarea")[1]);
      }

    }

    function addGroup(h, k, y) {
      gEBI("m").parentNode.insertBefore(t.cloneNode(true), gEBI("m"));
      gEBN("h")[y].innerHTML = h;
      gEBN("k")[y].innerHTML = k;
    }

    function addItem(n, d, z, y) {
      c = n=="indent_size" ? i.cloneNode(true) : r.cloneNode(true);
      gEBTN('tbody',gEBN("g")[y])[0].appendChild(c);
      tr = gEBTN("tr",gEBN("g")[y])[z+2];
      td = gEBTN("td",tr)[0];
      gEBTN("input",td)[0].name = n;
      gEBTN("input",td)[0].id = n;
      gEBTN("span",td)[0].innerHTML = d;
      ck = getCookie(n);
      if(ck && ck=="on") {
        gEBTN("input",td)[0].checked = true;
        gEBTN("input",td)[0].value = "on";
      }
      else if(ck!=null) {
        gEBTN("input",td)[0].value = ck;
      }
    }

    function switchPanels(el) {
      if(el.name=="textarea") {
        if(gEBN("textarea")[0].checked) {
          sDIS(gEBI("tx"));
          sDIS(gEBI("hl"),"none");
        }
        else {
          sDIS(gEBI("tx"),"none");
          sDIS(gEBI("hl"));
        }
      }
    }

    function setCookie() {

      var cooky = "";
      var ta_index = 0;
      var elems = gEBTN("input");
      for(i=0; i<elems.length; i++) {
        switch(elems[i].type) {
          case "text":
            if(elems[i].value!="" && elems[i].name!="") {
              cooky += elems[i].name+":"+elems[i].value+"#";
            }
            break;
          case "checkbox":
            if(elems[i].checked) {
              cooky += elems[i].name+":on#";
            }
            break;
          case "radio":
            ta_index++;
            if(elems[i].checked && ta_index==1) {
              cooky += elems[i].name+":on#";
            }
        }
      }
      elems = gEBN("a");
      for(i=0; i<elems.length; i++) {
        tb = elems[i].parentNode.parentNode.parentNode;
        if(gEBTN("tr",tb)[1].style.display=="") {
          cooky += "menu"+i+":on#";
        }
      }
      document.cookie = "options="+cooky+"; expires="+(new Date(2020, 01, 01)).toGMTString();
    }

    function getCookie(ckn)  {
      var res = document.cookie.match(ckn+':(.*?)(#|$)');
      return res ? res[1] : null;
    }

    function loadFileMenu() {
      gEBN("h")[0].innerHTML = "Choose Source Code";
      gEBN("k")[0].innerHTML = "(select file, paste code into text box or use sample)";
      sDIS(gEBN("k")[0].parentNode);
      gEBTN("tbody",gEBN("g")[0])[0].appendChild(gEBI("f"));
      gEBTN("tbody",gEBN("g")[0])[0].appendChild(gEBI("x"));
      gEBTN("tbody",gEBN("g")[0])[0].appendChild(gEBI("s"));
      gEBN("a")[0].style.color = "red";
      if(getCookie("menu0")=="off") {
        collapse(gEBN("a")[0]);
      }
    }

    function collapse(el) {
      var ai = 0;
      while(ai<gEBN("a").length && gEBN("a")[ai++]!=el);
      tb = el.parentNode.parentNode.parentNode;
      el.style.color = el.style.color=="" ? "red" : "";
      for(i=1; i<gEBTN("tr",tb).length; i++) {
        tr = gEBTN("tr",tb)[i];
        if(i==1) {
          st = tr.style.display=="none" ? "" : "none";
        }
        sDIS(tr,st);
      }
      window.focus();
    }

    function hideLeft(el) {
      if(el.innerHTML=="hide left panel") {
        el.innerHTML = "show left panel";
        gEBI("hide_top").appendChild(el);
        sDIS(gEBI("left"),"none");
        sDIS(gEBI("hide_top"));
      }
      else {
        el.innerHTML = "hide left panel";
        gEBI("hide_left").appendChild(el);
        sDIS(gEBI("left"));
        sDIS(gEBI("hide_top"),"none");
      }
    }

    function submitCode(el) {
      if(gEBI('file').value!='') {
        gEBI('ta').value = '';
      }
      if(el.name=="sc") {
        gEBI("download").value = 2;
      }
      else if(el.name=="dl") {
        gEBI("download").value = 1;
      }
      else {
        gEBI("download").value = 0;
      }
      var elems = gEBTN("input");
      for(i=0; i<elems.length; i++) {
        if(elems[i].type=="checkbox" && elems[i].checked) {
          elems[i].value = "on";
        }
      }
      document.stylist.submit();
    }

    var t = null;
    var r = null;
    var i = null;
    var h = null;
    var k = null;
    var n = null;
    var d = null;

    function gEBI(id) {
      return document.getElementById(id);
    }
    function gEBN(nm, el) {
      return el==undefined ? document.getElementsByName(nm) : el.getElementsByName(nm);
    }
    function gEBTN(tn, el) {
      return el==undefined ? document.getElementsByTagName(tn) : el.getElementsByTagName(tn);
    }
    function sDIS(el,st) {
      el.style.display = st==undefined ? "" : st;
    }

  </script>

  <style>
  body, table {
    font-family: "Lucida Grande", "trebuchet ms", verdana, Tahoma, Arial, Verdana, Helvetica, sans-serif;
    color: #444444;
    font-size: 11px;
    background-color: #ffffff;
    margin-top: 0px;
  }
  input {
    font-family: "Lucida Grande", "trebuchet ms", verdana, Tahoma, Arial, Verdana, Helvetica, sans-serif;
    border: 1px solid #888888;
    font-size: 10px;
    background-color: #ffffff;
  }
  textarea {
    font-family: Courier New, Bitstream Vera Sans Mono, Courier, sans-serif;
    border: 0px;
    font-size: 12px;
    background-color: #ffffff;
  }
  a {
    text-decoration: none;
    color: black;
    font-weight: normal;
  }
  a:hover {
    color: #0094F2;
  }
  table {
    border: 1px solid #e7e7e7;
  }
  td {
    padding: 2px 5px 2px 5px;
  }
  h1 {
    font-variant: small-caps;
    font-size: 20px;
    margin: 0px;
  }
  hr {
    height: 1px;
    margin-bottom: 5px;
  }
  .button {
    font-size: 10px;
    color: #666666;
    margin: 0px;
  }
  .check {
    border: 0px;
    vertical-align: middle;
  }
  .h {
    font-size: 11px;
    color: #666666;
    font-weight: bold;
    text-align: left;
    margin: 0px;
    border-bottom: 1px solid #e7e7e7;
    padding-bottom: 3px;
    padding-right: 0px;
  }
  .k {
    font-style: italic;
  }
  .l {
    border-bottom: 1px solid #e7e7e7;
  }
  </style>

  </head>
  <body>
  <table width="100%" height="98%" cellpadding=0 cellspacing=0 border=0 style="margin-top: 10px; border: 0px;">
  <form action="phpStylist.php" method="POST" name="stylist" enctype="multipart/form-data">
  <tr height=1>
    <td valign=top style="padding: 0px;">
      <table cellpadding=0 cellspacing=0 border=0 style="border: 0px;">
      <tr>
        <td style="padding: 0px;"><h1>phpStylist by Mr. Milk</h1></td>
        <td style="padding-left: 20px;"valign=bottom><i>“because your code may not work, but it must look professional!!!”</i></td>
      </tr>
      </table>
      <hr>
    </td>
  </tr>
  <tr>
    <td valign=top style="padding: 0px;">
      <table width="100%" height="100%" cellpadding=0 cellspacing=0 border=0 style="border: 0px">
      <tr>
        <td width="290" valign=top id="left">
          <table id="g" name="g" width="290" style="margin-bottom: 10px;" cellpadding=0 cellspacing=0 border=0>
            <tr>
              <td id="h" name="h" class="h" nowrap></td>
              <td align=right class="l" nowrap><a href="#" name="a" title="expand/collapse" onclick="collapse(this); setCookie(); return false;">•••</a></td>
            </tr>
            <tr style="display: none;">
              <td id="k" name="k" class="k" colspan=2></td>
            </tr>
          </table>
          <table id="m" style="display: none; margin-bottom: 10px;">
            <tr id="e" name="e" style="display: none;">
              <td colspan=2 nowrap>
                <input type="checkbox" class="check" onclick="setCookie();" value="off"><span></span>
              </td>
            </tr>
            <tr id="i" name="i" style="display: none;">
              <td colspan=2>
                <input type="text" size=2 value="2" onchange="setCookie();"><span></span>
              </td>
            </tr>
            <tr id="f" name="f">
              <td colspan=2 style="padding-top: 10px;">
                <input type="file" name="file" id="file" size=39>
              </td>
            </tr>
            <tr id="x" name="x">
              <td colspan=2 style="padding-top: 0px;">
                <input type="radio" class="check" id="textarea" name="textarea" onclick="switchPanels(this); setCookie();" value="on" checked>show editable text box<br>
                <input type="radio" class="check" id="textarea" name="textarea" onclick="switchPanels(this); setCookie();" value="off">show syntax highlighted code<br>
                <input type="checkbox" class="check" id="iso8859" name="iso8859" onclick="setCookie();" value="off">use ISO-8859-1 charset (default UTF-8)<br>
                <input type="checkbox" class="check" id="nowrap" name="nowrap" onclick="setCookie();" value="off">do not wrap lines in right panel
              </td>
            </tr>
            <tr id="s" name="s">
              <td colspan=2 style="padding-bottom: 10px;">
                <input type="button" name="pv" onclick="submitCode(this);" value=" Preview " class="button">&nbsp;
                <input type="button" name="dl" onclick="submitCode(this);" value=" Download " class="button">&nbsp;
                <input type="button" name="sc" onclick="submitCode(this);" value="Use Sample" class="button">
                <input type="hidden" name="download" id="download">
              </td>
            </tr>
          </table>
          <div id="hide_left"><a href="#" id="hide" onclick="hideLeft(this);" style="color: #0094F2" class="k">hide left panel</a></div>
        </td>
        <td valign=top style="padding-bottom: 10px;">
          <table width="100%" height="100%" cellpadding=0 cellspacing=0 border=0>
            <tr height=1>
              <td style="border-bottom: 1px solid #f5f5f5; padding: 0px; margin: 0px;">
                <div id="hide_top" style="display: none; margin-left: 5px;"></div>
              </td>
            </tr>
            <?echo parseFile();?>
          </table>
        </td>
      </tr>
      </table>
    </td>
  </tr>
  </table>
  </form>
  </body>
</html>

<?php
function parseFile($download = false)
{
  global $HTTP_POST_FILES;
  $code = "";
  $stylist = new phpStylist();
  if ((isset($_REQUEST['file']) && $_REQUEST['file']!="") || (isset($HTTP_POST_FILES['file']['tmp_name']) && $HTTP_POST_FILES['file']['tmp_name'] != "none" && $HTTP_POST_FILES['file']['tmp_name'] != "")) {
    $file = isset($_REQUEST['file']) ? $_REQUEST['file'] : $HTTP_POST_FILES['file']['tmp_name'];
    $code = loadFile($file);
    if (isset($_REQUEST["iso8859"]) && $_REQUEST["iso8859"]!="off" && !isDownload()) {
      $code = utf8_encode($code);
    }
  }
  elseif (isset($_REQUEST["download"]) && $_REQUEST["download"] == '2') {
    $code = getSampleCode();
  }
  elseif (isset($_REQUEST["ta"]) && $_REQUEST["ta"] != '') {
    $code = stripslashes($_REQUEST["ta"]);
    if (isset($_REQUEST["iso8859"]) && $_REQUEST["iso8859"]!="off" && isDownload()) {
      $code = utf8_decode($code);
    }
  }
  if (!empty($code)) {
    foreach ($_REQUEST as $k => $v) {
      if (isset($stylist->options[strtoupper($k)])) {
        $stylist->options[strtoupper($k)] = $v != "off";
      }
    }
    if (isset($_REQUEST["indent_with_tabs"]) && $_REQUEST["indent_with_tabs"] != "off") {
      $stylist->indent_char = "\t";
    }
    if (isset($_REQUEST["indent_size"]) && $_REQUEST["indent_size"] != "" && $_REQUEST["indent_size"] != "null") {
      $stylist->indent_size = $_REQUEST["indent_size"];
    }
    if (strpos($code, '<?') === false) {
      $code = '<?php ' . $code . ' ?' . '>';
    }
    $formatted = $stylist->formatCode($code);
    $highlight = highlight_string($formatted, true);
    if (isset($HTTP_POST_FILES['file']['tmp_name']) && file_exists($HTTP_POST_FILES['file']['tmp_name'])) {
      unlink($HTTP_POST_FILES['file']['tmp_name']);
    }
    $nowrap   = isset($_REQUEST["nowrap"]) && $_REQUEST["nowrap"] == 'on' ? "nowrap" : "";
    $tawrap   = $nowrap == "" ? "" : "wrap='off' ";
    $display  = isset($_REQUEST["textarea"]) && $_REQUEST["textarea"] == 'on' ? "" : "style='display: none;'";
    $textarea = "<tr id='tx' $display><td valign=top><textarea $tawrap style='width: 100%; height: 100%;' id='ta' name='ta'>" . htmlspecialchars($formatted) . "</textarea></td></tr>";
    $display  = isset($_REQUEST["textarea"]) && $_REQUEST["textarea"] == 'on' ? "style='display: none;'" : "";
    $frame    = "<tr id='hl' $display><td valign=top $nowrap>$highlight</td></tr>";
  }
  else {
    $nowrap   = isset($_REQUEST["nowrap"]) && $_REQUEST["nowrap"] == 'on' ? "nowrap" : "";
    $tawrap   = $nowrap == "" ? "" : "wrap='off' ";
    $textarea = "<tr id='tx'><td valign=top><textarea $tawrap style='width: 100%; height=100%;' id='ta' name='ta'></textarea></td></tr>";
    $frame    = "<tr id='hl' style='display: none;'><td valign=top $nowrap></td></tr>";
  }
  if ($download) {
    return isset($formatted) ? $formatted : "";
  }
  else {
    return $textarea . $frame;
  }
}

function isCommandLine() {
  global $argv;
  return ((is_array($argv) && count($argv)>0) || (is_array($_SERVER['argv']) && count($_SERVER['argv'])>0) || (is_array($GLOBALS['HTTP_SERVER_VARS']['argv']) && count($GLOBALS['HTTP_SERVER_VARS']['argv'])>0));
}

function processBatch() {

  global $argv;
  if (is_array($argv)) {
    $options = $argv;
  }
  elseif (is_array($_SERVER['argv'])) {
    $options = $_SERVER['argv'];
  }
  elseif (is_array($GLOBALS['HTTP_SERVER_VARS']['argv'])) {
    $options = $GLOBALS['HTTP_SERVER_VARS']['argv'];
  }
  foreach($options as $index=>$option) {
    if($option=="--help") {
      echo "phpStylist v0.8 by Mr. Milk\n";
      echo "usage: phpStylist source_file options\n\n";
      echo "Indentation and General Formatting:\n";
      echo "--indent_size n\n";
      echo "--indent_with_tabs\n";
      echo "--keep_redundant_lines\n";
      echo "--space_inside_parentheses\n";
      echo "--space_outside_parentheses\n";
      echo "--space_after_comma\n\n";
      echo "Operators:\n";
      echo "--space_around_assignment\n";
      echo "--align_var_assignment\n";
      echo "--space_around_comparison\n";
      echo "--space_around_arithmetic\n";
      echo "--space_around_logical\n";
      echo "--space_around_colon_question\n\n";
      echo "Functions, Classes and Objects:\n";
      echo "--line_before_function\n";
      echo "--line_before_curly_function\n";
      echo "--line_after_curly_function\n";
      echo "--space_around_obj_operator\n";
      echo "--space_around_double_colon\n\n";
      echo "Control Structures:\n";
      echo "--space_after_if\n";
      echo "--else_along_curly\n";
      echo "--line_before_curly\n";
      echo "--add_missing_braces\n";
      echo "--line_after_break\n";
      echo "--space_inside_for\n";
      echo "--indent_case\n\n";
      echo "Arrays and Concatenation:\n";
      echo "--line_before_array\n";
      echo "--vertical_array\n";
      echo "--align_array_assignment\n";
      echo "--space_around_double_arrow\n";
      echo "--vertical_concat\n";
      echo "--space_around_concat\n\n";
      echo "Comments:\n";
      echo "--line_before_comment_multi\n";
      echo "--line_after_comment_multi\n";
      echo "--line_before_comment\n";
      echo "--line_after_comment\n";
      exit;
    }
    if($index==1) {
      $_REQUEST["file"] = $option;
    }
    elseif($option=="--indent_size") {
      $_REQUEST["indent_size"] = $options[$index+1];
    }
    elseif($index>0 && $options[$index-1]!="indent_size") {
      $_REQUEST[substr($option, 2)] = "on";
    }
  }
  $_REQUEST["download"] = 2;
  $str = parseFile(true);
  echo $str;
  exit;

}

function isDownload()
{
  return isset($_REQUEST["download"]) && $_REQUEST["download"] == '1';
}

function downloadFile()
{
  global $HTTP_POST_FILES;
  $charset = isset($_REQUEST["iso8859"]) && $_REQUEST["iso8859"]!="off" ? "ISO-8859-1" : "UTF-8";
  $str = parseFile(true);
  if ($str != "") {
    $fn = isset($HTTP_POST_FILES['file']) && $HTTP_POST_FILES['file']['name'] != '' ? $HTTP_POST_FILES['file']['name'] : "phpStylist.php";
    header("Expires: Wed, 20 Jun 2007 00:00:00 GMT");
    header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
    header("Cache-Control: no-store, no-cache, must-revalidate");
    header("Cache-Control: post-check=0, pre-check=0", false);
    header("Pragma: no-cache");
    header("Content-Type: text/plain; charset=".$charset);
    header('Content-Disposition: attachment; filename="' . $fn . '";');
    header("Pragma: public");
    header("Expires: 0");
    header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
    header("Content-Transfer-Encoding: binary");
  }
  echo $str;
  exit;
}

function loadFile($filename)
{
  $code = "";
  $file = null;
  if($filename == "-" || $filename == "STDIN") {
    $file = "php://stdin";
  } else {
    $file = "$filename";
  }
  $f = fopen($file, "rb");
  if($f) {
    while (!feof($f)) {
      $code .= fread($f, 8192);
    }
    fclose($f);
  }
  return $code;
}

function getSampleCode()
{
  $sample = 'PD9waHANCi8qKioNCiAqIHBocFN0eWxpc3QgZGVtb25zdHJhdGlvbiBjb2RlDQogKiBieSBNci4gTWlsayAoYWthIE1hcmNlbG8gTGVpdGUpDQogKiBtcm1pbGtAYW55c29mdC5jb20uYnINCiAqLw0KDQokd29ya19ob3VycyA9IDEyOw0KJHRpbWUgPSAkd29ya19ob3VycyoyOw0KJGRldmVsb3BlciA9ICRwb29yZ3V5Ow0KJGdvZCA9ICQkbXJnYXRlczsNCiR3YWdlID0gJHdvcmtfaG91cnMvMjsNCiRib251cyA9ICR3YWdlICogMC4wMDAwMTsNCiR3YXJuZWQgPSBmYWxzZTsNCiRwcm9wZXJ0aWVzID0gYXJyYXkoImdhdGVzIj0+YXJyYXkoImV2ZXJ5dGhpbmciPT4ieWVzIiwgImFueXRoaW5nIj0+InllcyIsICJhbnl0aGluZyBlbHNlIj0+InllcyIsICJ5b3UiPT4iY29tcGxldGVseSIpLCAiZGV2ZWxvcGVyIj0+YXJyYXkoImFuIG9sZCBjYXIiPT4iMiBpbnN0YWxsbWVudHMiLCAibW9kZGVkIGNvbXB1dGVyIj0+ImFsbW9zdCIsICJiZWVyIG11ZyI9PiJzdXJlISIpKTsNCiR3YXJuaW5nID0gIk1yLiAiLiRkZXZlbG9wZXIuIiwgeW91IG1hZGUgIi4gJGdvZC4gIiB1bmhhcHB5IHRvZGF5LiI7DQovLyBkZXZlbG9wZXIncyBtaXNlcmFibGUgbGlmZQ0KZm9yKCR5b3U9JGRldmVsb3BlcjskeW91PCRnb2Q7JHlvdSsrKSB7DQogIC8vIGxldCdzIHVzZSBoaXMgcG90ZW50aWFsDQogIGlmKCR3b3JrPD0kdGltZSo0KSB7DQogICAgJHdvcmsrKzsNCiAgICAkc2xlZXAtLTsNCiAgICAkZW5yaWNoR2F0ZXMoKTsNCiAgfQ0KICAvLyBjaGlja2VuaW5nIG91dCBodWg/DQogIGVsc2UgaWYoJHRpcmVkKSB7DQogICAgaWYoISR3YXJuZWQpIHsNCiAgICAgICRzY2FyZVRvRGVhdGgoJGRldmVsb3Blciwkd2FybmluZyk7DQogICAgICAkYm9udXMvPTI7DQogICAgfQ0KICAgIGVsc2UgaWYoJGJvbnVzID4gMCkNCiAgICAgICRib251cz0wOw0KICAgIGVsc2Ugew0KICAgICAgZmlyZSgkZGV2ZWxvcGVyKTsNCiAgICAgICRyZXBsYWNlbWVudCA9IGZpbmRJZGlvdCgpOw0KICAgICAgaGlyZSgkcmVwbGFjZW1lbnQsICR3YWdlLzIpOw0KICAgIH0NCiAgfQ0KICAvLyB0aGlzIGd1eSBjYW4gdGFrZSBpdA0KICBlbHNlaWYoJHdhZ2U+MCkNCiAgICAkd2FnZS0tOw0KICAvLyBoYXZlIHdlIHJlYWxseSBmb3VuZCB3aG8gd2UndmUgYmVlbiBsb29raW5nIGZvcj8NCiAgZWxzZSB7DQogICAgJGVucmljaEdhdGVzKCk7DQogICAgJGhlYWx0aF9sZXZlbCA9IGVtZXJnZW5jeVJvb20oJGRldmVsb3Blcik7DQogICAgLyogc2FuaXR5IGNoZWNrISAqLw0KICAgIHN3aXRjaCgkaGVhbHRoX2xldmVsKSB7DQogICAgICBjYXNlICJncmVhdCI6DQogICAgICAgIGZpcmUoJGRvY3Rvcik7DQogICAgICAgIHVuc2V0KCRwcm9wZXJ0aWVzWyJkZXZlbG9wZXIiXSk7DQogICAgICAgIHVuc2V0KCRwcm9wZXJ0aWVzWyJnYXRlcyJdWyJ5b3UiXSk7DQogICAgICAgIGN1Y2tvb3NOZXN0KCRkZXZlbG9wZXIpOw0KICAgICAgICAkcmVwbGFjZW1lbnQgPSBmaW5kSWRpb3QoKTsNCiAgICAgICAgaGlyZSgkcmVwbGFjZW1lbnQsICRtaW5fd2FnZSk7DQogICAgICAgIGJyZWFrOw0KICAgICAgY2FzZSAiZ29vZCI6DQogICAgICBjYXNlICJiYWQiOg0KICAgICAgICBiYWNrVG9Xb3JrKCk7DQogICAgICBicmVhazsNCiAgICAgIGNhc2UgInRlcm1pbmFsIjoNCiAgICAgICAgZmlyZSgkZGV2ZWxvcGVyKTsNCiAgICAgICAgJHJlcGxhY2VtZW50ID0gJGhhc19icm90aGVyID8gImJyb3RoZXIiIDogImZyaWVuZCI7DQogICAgICAgIGhpcmUoJHJlcGxhY2VtZW50LCAkbWluX3dhZ2UvMik7DQogICAgICBicmVhazsNCiAgICAgIGRlZmF1bHQ6DQogICAgfQ0KICB9DQp9DQovKioqDQogKiBGdW5jdGlvbnMNCiAqLw0KDQpmdW5jdGlvbiBlbWVyZ2VuY3lSb29tKCR3aG8pIHsNCiAgJGhlYWx0aHkgPWZhbHNlOw0KICAkaW50ZXJuID0gMDsNCiAgd2hpbGUoISRoZWFsdGh5ICYmICRoZWFsdGh5IT0iZW5kIikNCiAgICAkaGVhbHRoeSA9IHNjcnVic0ludGVybk9waW5pb25zKCR3aG8sICsrJGludGVybik7DQogICRpbnRlcm4gPSAwOw0KICB3aGlsZSghJGhlYWx0aHkgJiYgJGhlYWx0aHkhPSJlbmQiKQ0KICAgICRoZWFsdGh5ID0gZXJJbnRlcm5PcGluaW9ucygkd2hvLCAkaW50ZXJuKyspOw0KICByZXR1cm4gJGhlYWx0aHk7DQp9DQoNCmZ1bmN0aW9uIGhpcmUoJHdobywgJHdhZ2UpIHsNCiAgJGRldmVsb3BlciA9IG5ldyBTbGF2ZSgpOw0KICAkZGV2ZWxvcGVyLT5wZXJzb24gPSAkd2hvOw0KICAkZGV2ZWxvcGVyLT5zZXRTYWxhcnkoJHdhZ2UpOw0KICAkY29zdCA9IGhyUm91dGluZXM6OmdldEhpcmluZ0Nvc3QoKTsNCiAgJGRldmVsb3Blci0+c2V0RGVidCgkY29zdCk7DQp9DQoNCmZ1bmN0aW9uIGZpcmUoJHdobykgew0KICBjYWxsU2VjdXJpdHkoKTsNCiAgZ2V0SGltT3V0KCR3aG8pOw0KfQ0KDQo/Pg0K';
  return base64_decode($sample);
}

class phpStylist
{
  var $indent_size = 2;
  var $indent_char = " ";
  var $block_size  = 3;
  var $options = array(
    "SPACE_INSIDE_PARENTHESES"    => false,
    "SPACE_OUTSIDE_PARENTHESES"   => false,
    "SPACE_INSIDE_FOR"            => false,
    "SPACE_AFTER_IF"              => false,
    "SPACE_AFTER_COMMA"           => false,
    "SPACE_AROUND_OBJ_OPERATOR"   => false,
    "SPACE_AROUND_DOUBLE_COLON"   => false,
    "SPACE_AROUND_DOUBLE_ARROW"   => false,
    "SPACE_AROUND_ASSIGNMENT"     => false,
    "SPACE_AROUND_COMPARISON"     => false,
    "SPACE_AROUND_COLON_QUESTION" => false,
    "SPACE_AROUND_LOGICAL"        => false,
    "SPACE_AROUND_ARITHMETIC"     => false,
    "SPACE_AROUND_CONCAT"         => false,
    "LINE_BEFORE_FUNCTION"        => false,
    "LINE_BEFORE_CURLY"           => false,
    "LINE_BEFORE_CURLY_FUNCTION"  => false,
    "LINE_AFTER_CURLY_FUNCTION"   => false,
    "LINE_BEFORE_ARRAY"           => false,
    "LINE_BEFORE_COMMENT"         => false,
    "LINE_AFTER_COMMENT"          => false,
    "LINE_BEFORE_COMMENT_MULTI"   => false,
    "LINE_AFTER_COMMENT_MULTI"    => false,
    "LINE_AFTER_BREAK"            => false,
    "VERTICAL_CONCAT"             => false,
    "VERTICAL_ARRAY"              => false,
    "INDENT_CASE"                 => false,
    "KEEP_REDUNDANT_LINES"        => false,
    "ADD_MISSING_BRACES"          => false,
    "ALIGN_ARRAY_ASSIGNMENT"      => false,
    "ALIGN_VAR_ASSIGNMENT"        => false,
    "ELSE_ALONG_CURLY"            => false,
  );
  var $_new_line = "\n";
  var $_indent   = 0;
  var $_for_idx  = 0;
  var $_code     = "";
  var $_log      = false;
  var $_pointer  = 0;
  var $_tokens   = 0;

  function phpStylist()
  {
    define("S_OPEN_CURLY", "{");
    define("S_CLOSE_CURLY", "}");
    define("S_OPEN_BRACKET", "[");
    define("S_CLOSE_BRACKET", "]");
    define("S_OPEN_PARENTH", "(");
    define("S_CLOSE_PARENTH", ")");
    define("S_SEMI_COLON", ";");
    define("S_COMMA", ",");
    define("S_CONCAT", ".");
    define("S_COLON", ":");
    define("S_QUESTION", "?");
    define("S_EQUAL", "=");
    define("S_EXCLAMATION", "!");
    define("S_IS_GREATER", ">");
    define("S_IS_SMALLER", "<");
    define("S_MINUS", "-");
    define("S_PLUS", "+");
    define("S_TIMES", "*");
    define("S_DIVIDE", "/");
    define("S_MODULUS", "%");
    define("S_REFERENCE", "&");
    define("S_QUOTE", '"');
    define("S_AT", "@");
    define("S_DOLLAR", "$");
    define("S_ABSTRACT", "abstract");
    define("S_INTERFACE", "interface");
    define("S_FINAL", "final");
    define("S_PUBLIC", "public");
    define("S_PRIVATE", "private");
    define("S_PROTECTED", "protected");
    if (defined("T_ML_COMMENT")) {
      define("T_DOC_COMMENT", T_ML_COMMENT);
    }
    elseif (defined("T_DOC_COMMENT")) {
      define("T_ML_COMMENT", T_DOC_COMMENT);
    }
  }

  function formatCode($source = '')
  {
    $in_for        = false;
    $in_break      = false;
    $in_function   = false;
    $in_concat     = false;
    $space_after   = false;
    $curly_open    = false;
    $array_level   = 0;
    $arr_parenth   = array();
    $switch_level  = 0;
    $if_level      = 0;
    $if_pending    = 0;
    $else_pending  = false;
    $if_parenth    = array();
    $switch_arr    = array();
    $halt_parser   = false;
    $after         = false;
    $this->_tokens = token_get_all($source);
    foreach ($this->_tokens as $index => $token) {
      list($id, $text) = $this->_get_token($token);
      $this->_pointer = $index;
      if ($halt_parser && $id != S_QUOTE) {
        $this->_append_code($text, false);
        continue;
      }
      if (substr(phpversion(), 0, 1) == "4" && $id == T_STRING) {
        switch (strtolower(trim($text))) {
          case S_ABSTRACT:
          case S_INTERFACE:
          case S_FINAL:
          case S_PUBLIC:
          case S_PRIVATE:
          case S_PROTECTED:
            $id = T_PUBLIC;
          default:
        }
      }
      switch ($id) {
        case S_OPEN_CURLY:
          $condition = $in_function ? $this->options["LINE_BEFORE_CURLY_FUNCTION"] : $this->options["LINE_BEFORE_CURLY"];
          $this->_set_indent( + 1);
          $this->_append_code((!$condition ? ' ' : $this->_get_crlf_indent(false, - 1)) . $text . $this->_get_crlf($this->options["LINE_AFTER_CURLY_FUNCTION"] && $in_function && !$this->_is_token_lf()) . $this->_get_crlf_indent());
          $in_function = false;
          break;

        case S_CLOSE_CURLY:
          if ($curly_open) {
            $curly_open = false;
            $this->_append_code(trim($text));
          }
          else {
            if (($in_break || $this->_is_token(S_CLOSE_CURLY)) && $switch_level > 0 && $switch_arr["l" . $switch_level] > 0 && $switch_arr["s" . $switch_level] == $this->_indent - 2) {
              if ($this->options["INDENT_CASE"]) {
                $this->_set_indent( - 1);
              }
              $switch_arr["l" . $switch_level]--;
              $switch_arr["c" . $switch_level]--;
            }
            while ($switch_level > 0 && $switch_arr["l" . $switch_level] == 0 && $this->options["INDENT_CASE"]) {
              unset($switch_arr["s" . $switch_level]);
              unset($switch_arr["c" . $switch_level]);
              unset($switch_arr["l" . $switch_level]);
              $switch_level--;
              if ($switch_level > 0) {
                $switch_arr["l" . $switch_level]--;
              }
              $this->_set_indent( - 1);
              $this->_append_code($this->_get_crlf_indent() . $text . $this->_get_crlf_indent());
              $text = '';
            }
            if ($text != '') {
              $this->_set_indent( - 1);
              $this->_append_code($this->_get_crlf_indent() . $text . $this->_get_crlf_indent());
            }
          }
          break;

        case S_SEMI_COLON:
          if (($in_break || $this->_is_token(S_CLOSE_CURLY)) && $switch_level > 0 && $switch_arr["l" . $switch_level] > 0 && $switch_arr["s" . $switch_level] == $this->_indent - 2) {
            if ($this->options["INDENT_CASE"]) {
              $this->_set_indent( - 1);
            }
            $switch_arr["l" . $switch_level]--;
            $switch_arr["c" . $switch_level]--;
          }
          if ($in_concat) {
            $this->_set_indent( - 1);
            $in_concat = false;
          }
          $this->_append_code($text . $this->_get_crlf($this->options["LINE_AFTER_BREAK"] && $in_break) . $this->_get_crlf_indent($in_for));
          while ($if_pending > 0) {
            $text = $this->options["ADD_MISSING_BRACES"] ? "}" : "";
            $this->_set_indent( - 1);
            if ($text != "") {
              $this->_append_code($this->_get_crlf_indent() . $text . $this->_get_crlf_indent());
            }
            else {
              $this->_append_code($this->_get_crlf_indent());
            }
            $if_pending--;
            if ($this->_is_token(array(T_ELSE, T_ELSEIF))) {
              break;
            }
          }
          if ($this->_for_idx == 0) {
            $in_for = false;
          }
          $in_break = false;
          $in_function = false;
          break;

        case S_OPEN_BRACKET:
        case S_CLOSE_BRACKET:
          $this->_append_code($text);
          break;

        case S_OPEN_PARENTH:
          if ($if_level > 0) {
            $if_parenth["i" . $if_level]++;
          }
          if ($array_level > 0) {
            $arr_parenth["i" . $array_level]++;
            if ($this->_is_token(array(T_ARRAY), true) && !$this->_is_token(S_CLOSE_PARENTH)) {
              $this->_set_indent( + 1);
              $this->_append_code((!$this->options["LINE_BEFORE_ARRAY"] ? '' : $this->_get_crlf_indent(false, - 1)) . $text . $this->_get_crlf_indent());
              break;
            }
          }
          $this->_append_code($this->_get_space($this->options["SPACE_OUTSIDE_PARENTHESES"] || $space_after) . $text . $this->_get_space($this->options["SPACE_INSIDE_PARENTHESES"]));
          $space_after = false;
          break;

        case S_CLOSE_PARENTH:
          if ($array_level > 0) {
            $arr_parenth["i" . $array_level]--;
            if ($arr_parenth["i" . $array_level] == 0) {
              $comma = substr(trim($this->_code), - 1) != "," && $this->options['VERTICAL_ARRAY'] ? "," : "";
              $this->_set_indent( - 1);
              $this->_append_code($comma . $this->_get_crlf_indent() . $text . $this->_get_crlf_indent());
              unset($arr_parenth["i" . $array_level]);
              $array_level--;
              break;
            }
          }
          $this->_append_code($this->_get_space($this->options["SPACE_INSIDE_PARENTHESES"]) . $text . $this->_get_space($this->options["SPACE_OUTSIDE_PARENTHESES"]));
          if ($if_level > 0) {
            $if_parenth["i" . $if_level]--;
            if ($if_parenth["i" . $if_level] == 0) {
              if (!$this->_is_token(S_OPEN_CURLY) && !$this->_is_token(S_SEMI_COLON)) {
                $text = $this->options["ADD_MISSING_BRACES"] ? "{" : "";
                $this->_set_indent( + 1);
                $this->_append_code((!$this->options["LINE_BEFORE_CURLY"] || $text == "" ? ' ' : $this->_get_crlf_indent(false, - 1)) . $text . $this->_get_crlf_indent());
                $if_pending++;
              }
              unset($if_parenth["i" . $if_level]);
              $if_level--;
            }
          }
          break;

        case S_COMMA:
          if ($array_level > 0) {
            $this->_append_code($text . $this->_get_crlf_indent($in_for));
          }
          else {
            $this->_append_code($text . $this->_get_space($this->options["SPACE_AFTER_COMMA"]));
            if ($this->_is_token(S_OPEN_PARENTH)) {
              $space_after = $this->options["SPACE_AFTER_COMMA"];
            }
          }
          break;

        case S_CONCAT:
          $condition = $this->options["SPACE_AROUND_CONCAT"];
          if ($this->_is_token(S_OPEN_PARENTH)) {
            $space_after = $condition;
          }
          if ($this->options["VERTICAL_CONCAT"]) {
            if (!$in_concat) {
              $in_concat = true;
              $this->_set_indent( + 1);
            }
            $this->_append_code($this->_get_space($condition) . $text . $this->_get_crlf_indent());
          }
          else {
            $this->_append_code($this->_get_space($condition) . $text . $this->_get_space($condition));
          }
          break;

        case T_CONCAT_EQUAL:
        case T_DIV_EQUAL:
        case T_MINUS_EQUAL:
        case T_PLUS_EQUAL:
        case T_MOD_EQUAL:
        case T_MUL_EQUAL:
        case T_AND_EQUAL:
        case T_OR_EQUAL:
        case T_XOR_EQUAL:
        case T_SL_EQUAL:
        case T_SR_EQUAL:
        case S_EQUAL:
          $condition = $this->options["SPACE_AROUND_ASSIGNMENT"];
          if ($this->_is_token(S_OPEN_PARENTH)) {
            $space_after = $condition;
          }
          $this->_append_code($this->_get_space($condition) . $text . $this->_get_space($condition));
          break;

        case T_IS_EQUAL:
        case S_IS_GREATER:
        case T_IS_GREATER_OR_EQUAL:
        case T_IS_SMALLER_OR_EQUAL:
        case S_IS_SMALLER:
        case T_IS_IDENTICAL:
        case T_IS_NOT_EQUAL:
        case T_IS_NOT_IDENTICAL:
          $condition = $this->options["SPACE_AROUND_COMPARISON"];
          if ($this->_is_token(S_OPEN_PARENTH)) {
            $space_after = $condition;
          }
          $this->_append_code($this->_get_space($condition) . $text . $this->_get_space($condition));
          break;

        case T_BOOLEAN_AND:
        case T_BOOLEAN_OR:
        case T_LOGICAL_AND:
        case T_LOGICAL_OR:
        case T_LOGICAL_XOR:
        case T_SL:
        case T_SR:
          $condition = $this->options["SPACE_AROUND_LOGICAL"];
          if ($this->_is_token(S_OPEN_PARENTH)) {
            $space_after = $condition;
          }
          $this->_append_code($this->_get_space($condition) . $text . $this->_get_space($condition));
          break;

        case T_DOUBLE_COLON:
          $condition = $this->options["SPACE_AROUND_DOUBLE_COLON"];
          $this->_append_code($this->_get_space($condition) . $text . $this->_get_space($condition));
          break;

        case S_COLON:
          if ($switch_level > 0 && $switch_arr["l" . $switch_level] > 0 && $switch_arr["c" . $switch_level] < $switch_arr["l" . $switch_level]) {
            $switch_arr["c" . $switch_level]++;
            if ($this->options["INDENT_CASE"]) {
              $this->_set_indent( + 1);
            }
            $this->_append_code($text . $this->_get_crlf_indent());
          }
          else {
            $condition = $this->options["SPACE_AROUND_COLON_QUESTION"];
            if ($this->_is_token(S_OPEN_PARENTH)) {
              $space_after = $condition;
            }
            $this->_append_code($this->_get_space($condition) . $text . $this->_get_space($condition));
          }
          if (($in_break || $this->_is_token(S_CLOSE_CURLY)) && $switch_level > 0 && $switch_arr["l" . $switch_level] > 0) {
            if ($this->options["INDENT_CASE"]) {
              $this->_set_indent( - 1);
            }
            $switch_arr["l" . $switch_level]--;
            $switch_arr["c" . $switch_level]--;
          }
          break;

        case S_QUESTION:
          $condition = $this->options["SPACE_AROUND_COLON_QUESTION"];
          if ($this->_is_token(S_OPEN_PARENTH)) {
            $space_after = $condition;
          }
          $this->_append_code($this->_get_space($condition) . $text . $this->_get_space($condition));
          break;

        case T_DOUBLE_ARROW:
          $condition = $this->options["SPACE_AROUND_DOUBLE_ARROW"];
          if ($this->_is_token(S_OPEN_PARENTH)) {
            $space_after = $condition;
          }
          $this->_append_code($this->_get_space($condition) . $text . $this->_get_space($condition));
          break;

        case S_MINUS:
        case S_PLUS:
        case S_TIMES:
        case S_DIVIDE:
        case S_MODULUS:
          $condition = $this->options["SPACE_AROUND_ARITHMETIC"];
          if ($this->_is_token(S_OPEN_PARENTH)) {
            $space_after = $condition;
          }
          $this->_append_code($this->_get_space($condition) . $text . $this->_get_space($condition));
          break;

        case T_OBJECT_OPERATOR:
          $condition = $this->options["SPACE_AROUND_OBJ_OPERATOR"];
          $this->_append_code($this->_get_space($condition) . $text . $this->_get_space($condition));
          break;

        case T_FOR:
          $in_for = true;
        case T_FOREACH:
        case T_WHILE:
        case T_DO:
        case T_IF:
        case T_SWITCH:
          $space_after = $this->options["SPACE_AFTER_IF"];
          $this->_append_code($text . $this->_get_space($space_after), false);
          if ($id == T_SWITCH) {
            $switch_level++;
            $switch_arr["s" . $switch_level] = $this->_indent;
            $switch_arr["l" . $switch_level] = 0;
            $switch_arr["c" . $switch_level] = 0;
          }
          $if_level++;
          $if_parenth["i" . $if_level] = 0;
          break;

        case T_FUNCTION:
        case T_CLASS:
        case T_INTERFACE:
        case T_FINAL:
        case T_ABSTRACT:
        case T_PUBLIC:
        case T_PROTECTED:
        case T_PRIVATE:
          if (!$in_function) {
            if ($this->options["LINE_BEFORE_FUNCTION"]) {
              $this->_append_code($this->_get_crlf($after || !$this->_is_token(array(T_COMMENT, T_ML_COMMENT, T_DOC_COMMENT), true)) . $this->_get_crlf_indent() . $text . $this->_get_space());
              $after = false;
            }
            else {
              $this->_append_code($text . $this->_get_space(), false);
            }
            $in_function = true;
          }
          else {
            $this->_append_code($this->_get_space() . $text . $this->_get_space());
          }
          break;

        case T_START_HEREDOC:
          $this->_append_code($this->_get_space($this->options["SPACE_AROUND_ASSIGNMENT"]) . $text);
          break;

        case T_END_HEREDOC:
          $this->_append_code($this->_get_crlf() . $text . $this->_get_crlf_indent());
          break;

        case T_COMMENT:
        case T_ML_COMMENT:
        case T_DOC_COMMENT:
          if (is_array($this->_tokens[$index - 1])) {
            $pad = $this->_tokens[$index - 1][1];
            $i   = strlen($pad) - 1;
            $k   = "";
            while (substr($pad, $i, 1) != "\n" && substr($pad, $i, 1) != "\r" && $i >= 0) {
              $k .= substr($pad, $i--, 1);
            }
            $text = preg_replace("/\r?\n$k/", $this->_get_crlf_indent(), $text);
          }
          $after = $id == (T_COMMENT && preg_match("/^\/\//", $text)) ? $this->options["LINE_AFTER_COMMENT"] : $this->options["LINE_AFTER_COMMENT_MULTI"];
          $before = $id == (T_COMMENT && preg_match("/^\/\//", $text)) ? $this->options["LINE_BEFORE_COMMENT"] : $this->options["LINE_BEFORE_COMMENT_MULTI"];
          if ($prev = $this->_is_token(S_OPEN_CURLY, true, $index, true)) {
            $before = $before && !$this->_is_token_lf(true, $prev);
          }
          $after = $after && (!$this->_is_token_lf() || !$this->options["KEEP_REDUNDANT_LINES"]);
          if ($before) {
            $this->_append_code($this->_get_crlf(!$this->_is_token(array(T_COMMENT), true)) . $this->_get_crlf_indent() . trim($text) . $this->_get_crlf($after) . $this->_get_crlf_indent());
          }
          else {
            $this->_append_code(trim($text) . $this->_get_crlf($after) . $this->_get_crlf_indent(), false);
          }
          break;

        case T_DOLLAR_OPEN_CURLY_BRACES:
        case T_CURLY_OPEN:
          $curly_open = true;
        case T_NUM_STRING:
        case T_BAD_CHARACTER:
          $this->_append_code(trim($text));
          break;

        case T_EXTENDS:
        case T_IMPLEMENTS:
        case T_INSTANCEOF:
        case T_AS:
          $this->_append_code($this->_get_space() . $text . $this->_get_space());
          break;

        case S_DOLLAR:
        case S_REFERENCE:
        case T_INC:
        case T_DEC:
          $this->_append_code(trim($text), false);
          break;

        case T_WHITESPACE:
          $redundant = "";
          if ($this->options["KEEP_REDUNDANT_LINES"]) {
            $lines          = preg_match_all("/\r?\n/", $text, $matches);
            $lines          = $lines > 0 ? $lines - 1 : 0;
            $redundant      = $lines > 0 ? str_repeat($this->_new_line, $lines) : "";
            $current_indent = $this->_get_indent();
            if (substr($this->_code, strlen($current_indent) * - 1) == $current_indent && $lines > 0) {
              $redundant .= $current_indent;
            }
          }
          if($this->_is_token(array(T_OPEN_TAG), true)) {
            $this->_append_code($text, false);
          }
          else {
            $this->_append_code($redundant . trim($text), false);
          }
          break;

        case S_QUOTE:
          $this->_append_code($text, false);
          $halt_parser = !$halt_parser;
          break;

        case T_ARRAY:
          if ($this->options["VERTICAL_ARRAY"]) {
            $next = $this->_is_token(array(T_DOUBLE_ARROW), true);
            $next |= $this->_is_token(S_EQUAL, true);
            $next |= $array_level>0;
            if ($next) {
              $next = $this->_is_token(S_OPEN_PARENTH, false, $index, true);
              if ($next) {
                $next = !$this->_is_token(S_CLOSE_PARENTH, false, $next);
              }
            }
            if ($next) {
              $array_level++;
              $arr_parenth["i" . $array_level] = 0;
            }
          }
        case T_STRING:
        case T_CONSTANT_ENCAPSED_STRING:
        case T_ENCAPSED_AND_WHITESPACE:
        case T_VARIABLE:
        case T_CHARACTER:
        case T_STRING_VARNAME:
        case S_AT:
        case S_EXCLAMATION:
        case T_OPEN_TAG:
        case T_OPEN_TAG_WITH_ECHO:
          $this->_append_code($text, false);
          break;

        case T_CLOSE_TAG:
          $this->_append_code($text, !$this->_is_token_lf(true));
          break;

        case T_CASE:
        case T_DEFAULT:
          if ($switch_arr["l" . $switch_level] > 0 && $this->options["INDENT_CASE"]) {
            $switch_arr["c" . $switch_level]--;
            $this->_set_indent( - 1);
            $this->_append_code($this->_get_crlf_indent() . $text . $this->_get_space());
          }
          else {
            $switch_arr["l" . $switch_level]++;
            $this->_append_code($text . $this->_get_space(), false);
          }
          break;

        case T_INLINE_HTML:
          $this->_append_code($text, false);
          break;

        case T_BREAK:
        case T_CONTINUE:
          $in_break = true;
        case T_VAR:
        case T_GLOBAL:
        case T_STATIC:
        case T_CONST:
        case T_ECHO:
        case T_PRINT:
        case T_INCLUDE:
        case T_INCLUDE_ONCE:
        case T_REQUIRE:
        case T_REQUIRE_ONCE:
        case T_DECLARE:
        case T_EMPTY:
        case T_ISSET:
        case T_UNSET:
        case T_DNUMBER:
        case T_LNUMBER:
        case T_RETURN:
        case T_EVAL:
        case T_EXIT:
        case T_LIST:
        case T_CLONE:
        case T_NEW:
        case T_FUNC_C:
        case T_CLASS_C:
        case T_FILE:
        case T_LINE:
          $this->_append_code($text . $this->_get_space(), false);
          break;

        case T_ELSEIF:
          $space_after  = $this->options["SPACE_AFTER_IF"];
          $added_braces = $this->_is_token(S_SEMI_COLON, true) && $this->options["ADD_MISSING_BRACES"];
          $condition    = $this->options['ELSE_ALONG_CURLY'] && ($this->_is_token(S_CLOSE_CURLY, true) || $added_braces);
          $this->_append_code($this->_get_space($condition) . $text . $this->_get_space($space_after), $condition);
          $if_level++;
          $if_parenth["i" . $if_level] = 0;
          break;

        case T_ELSE:
          $added_braces = $this->_is_token(S_SEMI_COLON, true) && $this->options["ADD_MISSING_BRACES"];
          $condition = $this->options['ELSE_ALONG_CURLY'] && ($this->_is_token(S_CLOSE_CURLY, true) || $added_braces);
          $this->_append_code($this->_get_space($condition) . $text, $condition);
          if (!$this->_is_token(S_OPEN_CURLY) && !$this->_is_token(array(T_IF))) {
            $text = $this->options["ADD_MISSING_BRACES"] ? "{" : "";
            $this->_set_indent( + 1);
            $this->_append_code((!$this->options["LINE_BEFORE_CURLY"] || $text == "" ? ' ' : $this->_get_crlf_indent(false, - 1)) . $text . $this->_get_crlf_indent());
            $if_pending++;
          }
          break;

        default:
          $this->_append_code($text . ' ', false);
          break;
      }
    }
    return $this->_align_operators();
  }

  function _get_token($token)
  {
    if (is_string($token)) {
      return array($token, $token);
    }
    else {
      return $token;
    }
  }

  function _append_code($code = "", $trim = true)
  {
    if ($trim) {
      $this->_code = rtrim($this->_code) . $code;
    }
    else {
      $this->_code .= $code;
    }
  }

  function _get_crlf_indent($in_for = false, $increment = 0)
  {
    if ($in_for) {
      $this->_for_idx++;
      if ($this->_for_idx > 2) {
        $this->_for_idx = 0;
      }
    }
    if ($this->_for_idx == 0 || !$in_for) {
      return $this->_get_crlf() . $this->_get_indent($increment);
    }
    else {
      return $this->_get_space($this->options["SPACE_INSIDE_FOR"]);
    }
  }

  function _get_crlf($true = true)
  {
    return $true ? $this->_new_line : "";
  }

  function _get_space($true = true)
  {
    return $true ? " " : "";
  }

  function _get_indent($increment = 0)
  {
    return str_repeat($this->indent_char, ($this->_indent + $increment) * $this->indent_size);
  }

  function _set_indent($increment)
  {
    $this->_indent += $increment;
    if ($this->_indent < 0) {
      $this->_indent = 0;
    }
  }

  function _is_token($token, $prev = false, $i = 99999, $idx = false)
  {
    if ($i == 99999) {
      $i = $this->_pointer;
    }
    if ($prev) {
      while (--$i >= 0 && is_array($this->_tokens[$i]) && $this->_tokens[$i][0] == T_WHITESPACE);
    }
    else {
      while (++$i < count($this->_tokens)-1 && is_array($this->_tokens[$i]) && $this->_tokens[$i][0] == T_WHITESPACE);
    }
    if (is_string($this->_tokens[$i]) && $this->_tokens[$i] == $token) {
      return $idx ? $i : true;
    }
    elseif (is_array($token) && is_array($this->_tokens[$i])) {
      if (in_array($this->_tokens[$i][0], $token)) {
        return $idx ? $i : true;
      }
      elseif ($prev && $this->_tokens[$i][0] == T_OPEN_TAG) {
        return $idx ? $i : true;
      }
    }
    return false;
  }

  function _is_token_lf($prev = false, $i = 99999)
  {
    if ($i == 99999) {
      $i = $this->_pointer;
    }
    if ($prev) {
      $count = 0;
      while (--$i >= 0 && is_array($this->_tokens[$i]) && $this->_tokens[$i][0] == T_WHITESPACE && strpos($this->_tokens[$i][1], "\n") === false);
    }
    else {
      $count = 1;
      while (++$i < count($this->_tokens) && is_array($this->_tokens[$i]) && $this->_tokens[$i][0] == T_WHITESPACE && strpos($this->_tokens[$i][1], "\n") === false);
    }
    if (is_array($this->_tokens[$i]) && preg_match_all("/\r?\n/", $this->_tokens[$i][1], $matches) > $count) {
      return true;
    }
    return false;
  }

  function _pad_operators($found)
  {
    global $quotes;
    $pad_size = 0;
    $result   = "";
    $source   = explode($this->_new_line, $found[0]);
    $position = array();
    array_pop($source);
    foreach ($source as $k => $line) {
      if (preg_match("/'quote[0-9]+'/", $line)) {
        preg_match_all("/'quote([0-9]+)'/", $line, $holders);
        for ($i = 0; $i < count($holders[1]); $i++) {
          $line = preg_replace("/" . $holders[0][$i] . "/", str_repeat(" ", strlen($quotes[0][$holders[1][$i]])), $line);
        }
      }
      if (strpos($line, "=") > $pad_size) {
        $pad_size = strpos($line, "=");
      }
      $position[$k] = strpos($line, "=");
    }
    foreach ($source as $k => $line) {
      $padding = str_repeat(" ", $pad_size - $position[$k]);
      $padded  = preg_replace("/^([^=]+?)([\.\+\*\/\-\%]?=)(.*)$/", "\\1{$padding}\\2\\3" . $this->_new_line, $line);
      $result .= $padded;
    }
    return $result;
  }

  function _parse_block($blocks)
  {
    global $quotes;
    $pad_chars = "";
    $holders = array();
    if ($this->options['ALIGN_ARRAY_ASSIGNMENT']) {
      $pad_chars .= ",";
    }
    if ($this->options['ALIGN_VAR_ASSIGNMENT']) {
      $pad_chars .= ";";
    }
    $php_code = $blocks[0];
    preg_match_all("/\/\*.*?\*\/|\/\/[^\n]*|#[^\n]|([\"'])[^\\\\]*?(?:\\\\.[^\\\\]*?)*?\\1/s", $php_code, $quotes);
    $quotes[0] = array_values(array_unique($quotes[0]));
    for ($i = 0; $i < count($quotes[0]); $i++) {
      $patterns[]    = "/" . preg_quote($quotes[0][$i], '/') . "/";
      $holders[]     = "'quote$i'";
      $quotes[0][$i] = str_replace('\\\\', '\\\\\\\\', $quotes[0][$i]);
    }
    if (count($holders) > 0) {
      $php_code = preg_replace($patterns, $holders, $php_code);
    }
    $php_code = preg_replace_callback("/(?:.+=.+[" . $pad_chars . "]\r?\n){" . $this->block_size . ",}/", array($this, "_pad_operators"), $php_code);
    for ($i = count($holders) - 1; $i >= 0; $i--) {
      $holders[$i] = "/" . $holders[$i] . "/";
    }
    if (count($holders) > 0) {
      $php_code = preg_replace($holders, $quotes[0], $php_code);
    }
    return $php_code;
  }

  function _align_operators()
  {
    if ($this->options['ALIGN_ARRAY_ASSIGNMENT'] || $this->options['ALIGN_VAR_ASSIGNMENT']) {
      return preg_replace_callback("/<\?.*?\?" . ">/s", array($this, "_parse_block"), $this->_code);
    }
    else {
      return $this->_code;
    }
  }
}
?>
