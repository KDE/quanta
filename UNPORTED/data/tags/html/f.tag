<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
<tag name="fieldset" hasCore="1" hasI18n="1" hasScript="1">
<children>
<child name="#text" />
  <child name="tt" />
 <child name="i" />
 <child name="b" />
 <child name="u" />
 <child name="s" />
 <child name="strike" />
 <child name="big" />
 <child name="small" />
  <child name="em" />
 <child name="strong" />
 <child name="dfn" />
 <child name="code" />
 <child name="samp" />
 <child name="kbd" />
 <child name="var" />
 <child name="cite" />
 <child name="abbr" />
 <child name="acronym" />
  <child name="a" />
 <child name="img" />
 <child name="applet" />
 <child name="object" />
 <child name="font" />
 <child name="basefont" />
 <child name="br" />
 <child name="script" />
 <child name="map" />
 <child name="q" />
 <child name="sub" />
 <child name="sup" />
 <child name="span" />
 <child name="bdo" />
 <child name="iframe" />
  <child name="input" />
 <child name="select" />
 <child name="textarea" />
 <child name="label" />
 <child name="button" />
  <child name="p" />
  <child name="h1" />
 <child name="h2" />
 <child name="h3" />
 <child name="h4" />
 <child name="h5" />
 <child name="h6" />
 <child name="ul" />
 <child name="ol" />
 <child name="dir" />
 <child name="menu" />
 <child name="pre" />
 <child name="dl" />
 <child name="div" />
 <child name="center" />
 <child name="noscript" />
 <child name="noframes" />
 <child name="blockquote" />
 <child name="form" />
 <child name="isindex" />
 <child name="hr" />
 <child name="table" />
 <child name="fieldset" />
 <child name="address" />
 <child name="legend" />
</children>
</tag>

<tag name="form" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="action" type="input">
        <text>Action</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" colspan="3" />
    </attr>

    <attr name="method" type="list">
        <text>Method</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1"/>
        <items>
                <item>GET</item>
                <item>POST</item>
        </items>
    </attr>

    <attr name="enctype" type="list">
        <text>Enctype</text>
        <textlocation col="0" row="2" />
        <location col="1" row="2" colspan="3"/>
        <items>
           <item>application/x-www-form-urlencoded</item>
           <item>multipart/form-data</item>
        </items>
    </attr>


    <attr name="target" type="list">
        <text>Target</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" />
        <items>
                <item>_blank</item>
                <item>_self</item>
                <item>_parent</item>
                <item>_top</item>
        </items>
    </attr>

    <attr name="accept-charset" type="input">
        <text>Accept charset</text>
        <textlocation col="2" row="3" />
        <location col="3" row="3"/>
    </attr>

    <attr name="name"> </attr>
    <attr name="onsubmit"> </attr>
    <attr name="onreset"> </attr>

<children>
<child name="#text" />
  <child name="tt" />
 <child name="i" />
 <child name="b" />
 <child name="u" />
 <child name="s" />
 <child name="strike" />
 <child name="big" />
 <child name="small" />
  <child name="em" />
 <child name="strong" />
 <child name="dfn" />
 <child name="code" />
 <child name="samp" />
 <child name="kbd" />
 <child name="var" />
 <child name="cite" />
 <child name="abbr" />
 <child name="acronym" />
  <child name="a" />
 <child name="img" />
 <child name="applet" />
 <child name="object" />
 <child name="font" />
 <child name="basefont" />
 <child name="br" />
 <child name="script" />
 <child name="map" />
 <child name="q" />
 <child name="sub" />
 <child name="sup" />
 <child name="span" />
 <child name="bdo" />
 <child name="iframe" />
  <child name="input" />
 <child name="select" />
 <child name="textarea" />
 <child name="label" />
 <child name="button" />
  <child name="p" />
  <child name="h1" />
 <child name="h2" />
 <child name="h3" />
 <child name="h4" />
 <child name="h5" />
 <child name="h6" />
 <child name="ul" />
 <child name="ol" />
 <child name="dir" />
 <child name="menu" />
 <child name="pre" />
 <child name="dl" />
 <child name="div" />
 <child name="center" />
 <child name="noscript" />
 <child name="noframes" />
 <child name="blockquote" />
 <child name="isindex" />
 <child name="hr" />
 <child name="table" />
 <child name="fieldset" />
 <child name="address" />
</children>

</tag>

<tag name="font" hasCore="1" hasI18n="1">

    <attr name="color" type="color">
        <text>Color</text>
        <textlocation col="0" row="2" />
        <tooltip>Font color</tooltip>
        <location col="1" row="2"  />
    </attr>

    <attr name="face" type="input">
        <text>Face</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" colspan="2" />
    </attr>

    <attr name="size" type="list">
        <text>Size</text>
        <textlocation col="0" row="4" />
        <location col="1" row="4"/>
        <items>
                <item>+1</item>
                <item>-1</item>
                <item>+2</item>
                <item>-2</item>
                <item>+3</item>
                <item>-3</item>
                <item>+4</item>
                <item>-4</item>
        </items>
    </attr>

    <spacer orientation="h">
        <location col="0" row="5" />
    </spacer>

    <spacer orientation="h">
        <location col="1" row="8" />
    </spacer>

    <spacer orientation="h">
        <location col="2" row="8" />
    </spacer>

    <spacer orientation="v">
        <location col="0" row="9" colspan="3" />
    </spacer>

<children>
 <child name="#text" />
 <child name="tt" />
 <child name="i" />
 <child name="b" />
 <child name="u" />
 <child name="s" />
 <child name="strike" />
 <child name="big" />
 <child name="small" />
  <child name="em" />
 <child name="strong" />
 <child name="dfn" />
 <child name="code" />
 <child name="samp" />
 <child name="kbd" />
 <child name="var" />
 <child name="cite" />
 <child name="abbr" />
 <child name="acronym" />
  <child name="a" />
 <child name="img" />
 <child name="applet" />
 <child name="object" />
 <child name="font" />
 <child name="basefont" />
 <child name="br" />
 <child name="script" />
 <child name="map" />
 <child name="q" />
 <child name="sub" />
 <child name="sup" />
 <child name="span" />
 <child name="bdo" />
 <child name="iframe" />
  <child name="input" />
 <child name="select" />
 <child name="textarea" />
 <child name="label" />
 <child name="button" />
</children>
</tag>
</TAGS>
