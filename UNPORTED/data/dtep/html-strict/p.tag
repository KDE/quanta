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
  <attr name="id" type="list" source="dcop" interface="QuantaIf" method="idSelectors()" arguments="" >
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
<child name="#text" />
 <child name="tt" />
 <child name="i" />
 <child name="b" />
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
 <child name="br" />
 <child name="script" />
 <child name="map" />
 <child name="q" />
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
