<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
<tag name="script">

    <attr name="type" type="list">
        <text>Type</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" colspan="2" />
        <items>
                <item>text/html</item>
                <item>text/css</item>
    <item>text/php</item>
                <item>text/javascript</item>
                <item>application/x-javascript</item>
        </items>
    </attr>

    <attr name="src" type="url">
        <text>Source</text>
        <textlocation col="0" row="2" />
        <location col="1" row="2" colspan="2" />
    </attr>

    <attr name="charset" type="input">
        <text>Charset</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" colspan="2" />
    </attr>

    <attr name="defer" type="check">
        <text>Defer (script execution may wait)</text>
        <location col="0" row="4" colspan="2" />
    </attr>

<children>
<child name="#text" />
</children>
</tag>

<tag name="samp" hasCore="1" hasI18n="1" hasScript="1">
<children>
<child name="#text" />
 <child name="tt" />
 <child name="i" />
 <child name="b" />
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
 <child name="object" />
 <child name="br" />
 <child name="script" />
 <child name="map" />
 <child name="q" />
 <child name="sub" />
 <child name="sup" />
 <child name="span" />
 <child name="bdo" />
  <child name="input" />
 <child name="select" />
 <child name="textarea" />
 <child name="label" />
 <child name="button" />
 </children>
</tag>

<tag name="select" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="name" type="input">
        <text>Name</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" />
    </attr>

    <attr name="multiple" type="check">
        <text>Multiple</text>
        <location col="1" row="1" />
    </attr>

    <attr name="disabled" type="check">
        <text>Disabled</text>
        <location col="1" row="2" />
    </attr>

    <attr name="tabindex" type="input">
        <text>Tab index</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" />
    </attr>

    <attr name="onfocus" type="input">
        <text>On focus</text>
        <textlocation col="0" row="4" />
        <location col="1" row="4" />
    </attr>

    <attr name="onblur" type="input">
        <text>On blur</text>
        <textlocation col="0" row="5" />
        <location col="1" row="5" />
    </attr>

    <attr name="onchange" type="input">
        <text>On change</text>
        <textlocation col="0" row="6" />
        <location col="1" row="6" />
    </attr>

    <attr name="size"> </attr>

    <stoppingtags>
        <stoppingtag name="select" />
    </stoppingtags>

<children>
<child name="optgroup" />
<child name="option" />
</children>
</tag>

<tag name="small" hasCore="1" hasI18n="1" hasScript="1">
<children>
<child name="#text" />
 <child name="tt" />
 <child name="i" />
 <child name="b" />
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
 <child name="object" />
 <child name="br" />
 <child name="script" />
 <child name="map" />
 <child name="q" />
 <child name="sub" />
 <child name="sup" />
 <child name="span" />
 <child name="bdo" />
  <child name="input" />
 <child name="select" />
 <child name="textarea" />
 <child name="label" />
 <child name="button" /></children>
</tag>

<tag name="span" hasCore="1" hasI18n="1" hasScript="1">
<children>
<child name="#text" />
 <child name="tt" />
 <child name="i" />
 <child name="b" />
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
 <child name="object" />
 <child name="br" />
 <child name="script" />
 <child name="map" />
 <child name="q" />
 <child name="sub" />
 <child name="sup" />
 <child name="span" />
 <child name="bdo" />
  <child name="input" />
 <child name="select" />
 <child name="textarea" />
 <child name="label" />
 <child name="button" />
 </children>
</tag>

<tag name="strong" hasCore="1" hasI18n="1" hasScript="1">
<children>
<child name="#text" />
 <child name="tt" />
 <child name="i" />
 <child name="b" />
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
 <child name="object" />
 <child name="br" />
 <child name="script" />
 <child name="map" />
 <child name="q" />
 <child name="sub" />
 <child name="sup" />
 <child name="span" />
 <child name="bdo" />
  <child name="input" />
 <child name="select" />
 <child name="textarea" />
 <child name="label" />
 <child name="button" />
 </children>
</tag>

<tag name="style" hasI18n="1">

    <attr name="type" type="list">
        <text>Type</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" colspan="2" />
        <items>
                <item>text/css</item>
        </items>
    </attr>

    <attr name="media" type="list">
        <text>Media</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1" colspan="2" />
        <items>
                <item>screen</item>
                <item>tty</item>
                <item>tv</item>
                <item>projection</item>
                <item>handheld</item>
                <item>print</item>
                <item>braille</item>
                <item>aural</item>
                <item>all</item>
        </items>
    </attr>

    <attr name="title" type="input">
        <text>Title</text>
        <textlocation col="0" row="2" />
        <location col="1" row="2" colspan="2" />
    </attr>

<children>
<child name="#text" />
</children>
</tag>

<tag name="sub" hasCore="1" hasI18n="1" hasScript="1">
<children>
<child name="#text" />
 <child name="tt" />
 <child name="i" />
 <child name="b" />
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
 <child name="object" />
 <child name="br" />
 <child name="script" />
 <child name="map" />
 <child name="q" />
 <child name="sub" />
 <child name="sup" />
 <child name="span" />
 <child name="bdo" />
  <child name="input" />
 <child name="select" />
 <child name="textarea" />
 <child name="label" />
 <child name="button" />
 </children>
</tag>

<tag name="sup" hasCore="1" hasI18n="1" hasScript="1">
<children>
<child name="#text" />
 <child name="tt" />
 <child name="i" />
 <child name="b" />
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
 <child name="object" />
 <child name="br" />
 <child name="script" />
 <child name="map" />
 <child name="q" />
 <child name="sub" />
 <child name="sup" />
 <child name="span" />
 <child name="bdo" />
  <child name="input" />
 <child name="select" />
 <child name="textarea" />
 <child name="label" />
 <child name="button" />
 </children>
</tag>
</TAGS>
