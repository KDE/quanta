<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
<tag name="p" hasScript="1" hasCore="1" hasI18n="1" optional="1">

    <attr name="align" type="list">
        <text>Alignment</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" />
        <items>
                <item>left</item>
                <item>center</item>
                <item>right</item>
                <item>justify</item>
        </items>
    </attr>

    <stoppingtags>
        <stoppingtag name="p" />
        <stoppingtag name="li" />
        <stoppingtag name="td" />
        <stoppingtag name="tr" />
        <stoppingtag name="th" />
        <stoppingtag name="dt" />
        <stoppingtag name="dd" />
    </stoppingtags>

<children>
<child name="a" />
<child name="abbr" />
<child name="acronym" />
<child name="b" />
<child name="bdo" />
<child name="big" />
<child name="br" />
<child name="button" />
<child name="cite" />
<child name="code" />
<child name="del" />
<child name="dfn" />
<child name="em" />
<child name="i" />
<child name="img" />
<child name="input" />
<child name="ins" />
<child name="kbd" />
<child name="label" />
<child name="map" />
<child name="object" />
<child name="q" />
<child name="samp" />
<child name="script" />
<child name="select" />
<child name="small" />
<child name="span" />
<child name="strong" />
<child name="sub" />
<child name="sup" />
<child name="textarea" />
<child name="tt" />
<child name="var" />
</children>
</tag>
<tag name="param" single="1">
  <attr name="name" type="input">
    <text>Name</text>
    <textlocation col="0" row="0" />
    <location col="1" row="0" colspan="2" />
  </attr>
  <attr name="value" type="input">
    <text>Value</text>
    <textlocation col="0" row="1" />
    <location col="1" row="1" colspan="2" />
  </attr>
  <attr name="valuetype" type="input">
    <text>Value Type</text>
    <textlocation col="0" row="2" />
    <location col="1" row="2" colspan="2" />
  </attr>
  <attr name="type" type="input">
    <text>Type</text>
    <textlocation col="0" row="3" />
    <location col="1" row="3" colspan="2" />
  </attr>
  <attr name="id" type="input">
    <text>Id</text>
    <textlocation col="0" row="4" />
    <location col="1" row="4" colspan="2" />
  </attr>

</tag>
<tag name="pre" hasCore="1" hasI18n="1" hasScript="1">
  <attr name="width" type="input">
    <text>Width</text>
    <textlocation col="0" row="0" />
    <location col="1" row="0" colspan="2" />
  </attr>
  <attr name="xml:space" type="input">
    <text>Xml:Space</text>
    <textlocation col="0" row="1" />
    <location col="1" row="1" colspan="2" />
  </attr>
<children>
<child name="a" />
<child name="abbr" />
<child name="acronym" />
<child name="b" />
<child name="bdo" />
<child name="br" />
<child name="button" />
<child name="cite" />
<child name="code" />
<child name="del" />
<child name="dfn" />
<child name="em" />
<child name="i" />
<child name="input" />
<child name="ins" />
<child name="kbd" />
<child name="label" />
<child name="map" />
<child name="q" />
<child name="samp" />
<child name="script" />
<child name="select" />
<child name="span" />
<child name="strong" />
<child name="textarea" />
<child name="tt" />
<child name="var" />
</children>
</tag>
</TAGS>
