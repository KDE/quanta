<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
 <tag name="abbrev" hasCore="1" />
 <tag name="abstract" hasCore="1" />
 <tag name="accel" hasCore="1" />
 <tag name="acronym" hasCore="1" />
 <tag name="action" hasCore="1">
  <attr name="moreinfo" type="list">
   <text>moreinfo</text>
   <location row="1" col="0"/>
   <items>
    <item>none</item>
    <item>refentry</item>
   </items>
  </attr>
 </tag>
 <tag name="address" hasCore="1">
  <attr name="format" type="list">
   <text>format</text>
   <textlocation row="1" col="0"/>
   <location row="1" col="1"/>
   <items>
    <item>linespecific</item>
   </items>
  </attr>
  <attr name="linenumbering" type="list">
   <text>linenumbering</text>
   <textlocation row="2" col="0" />
   <location row="2" col="1" />
   <items>
    <item>numbered</item>
    <item>unnumbered</item>
   </items>
  </attr>
 </tag>
 <tag name="affiliation" hasCore="1" />
 <tag name="anchor" hasCore="1" single="1">
  <attr name="revisionflag" type="list">
   <text>revisionflag</text>
   <textlocation row="0" col="0" />
   <items>
    <item>added</item>
    <item>changed</item>
    <item>deleted</item>
    <item>off</item>
   </items>
   <location row="0" col="1" />
  </attr>
  <attr name="revision" type="input">
   <text>revision</text>
   <textlocation row="1" col="0" />
   <location row="1" col="1" />
  </attr>
  <attr name="arch" type="input">
   <text>arch</text>
   <textlocation row="2" col="0" />
   <location row="2" col="1" />
  </attr>
  <attr name="vendor" type="input">
   <text>vendor</text>
   <textlocation row="3" col="0" />
   <location row="3" col="1" />
  </attr>
  <attr name="security" type="input">
   <text>security</text>
   <textlocation row="4" col="0" />
   <location row="4" col="1" />
  </attr>
  <attr name="pagenum" type="input">
   <text>pagemnu</text>
   <textlocation row="5" col="0" />
   <location row="5" col="1" />
  </attr>
  <attr name="remap" type="input">
   <text>remap</text>
   <textlocation row="6" col="0" />
   <location row="6" col="1" />
  </attr>
   <attr name="xreflabel" type="input">
   <text>xreflabel</text>
   <textlocation row="7" col="0" />
   <location row="7" col="1" />
  </attr>
  <attr name="conformance" type="input">
   <text>conformance</text>
   <textlocation row="8" col="0" />
   <location row="8" col="1" />
  </attr>
  <attr name="os" type="input">
   <text>os</text>
   <textlocation row="9" col="0" />
   <location row="9" col="1" />
  </attr>
  <attr name="userlevel" type="input">
   <text>userlevel</text>
   <textlocation row="10" col="0" />
   <location row="10" col="1" />
  </attr>
  <attr name="id" type="input">
   <text>id</text>
   <textlocation row="11" col="0" />
   <location row="11" col="1" />
  </attr>
  <attr name="role" type="input">
   <text>role</text>
   <textlocation row="12" col="0" />
   <location row="12" col="1" />
  </attr>
  <attr name="condition" type="input">
   <text>condition</text>
   <textlocation row="13" col="0" />
   <location row="13" col="1" />
  </attr>
 </tag>
 <tag name="answer" hasCore="1" />
 <tag name="appendix" hasCore="1">
  <attr name="status" type="input">
   <text>status</text>
   <textlocation row="0" col="0" />
   <location row="0" col="1" />
  </attr>
  <attr name="label" type="input">
   <text>label</text>
   <textlocation row="1" col="0" />
   <location row="1" col="1" />
  </attr>
 </tag>
 <tag name="application" hasCore="1">
  <attr name="moreinfo" type="list">
   <text>moreinfo</text>
   <textlocation row="0" col="0" />
   <items>
    <item>none</item>
    <item>refentry</item>
   </items>
   <location row="0" col="1" />
  </attr>
  <attr name="class" type="list">
   <text>class</text>
   <textlocation row="1" col="0" />
   <items>
    <item>hardware</item>
    <item>software</item>
   </items>
   <location row="1" col="1" />
  </attr>
 </tag>
 <tag name="area" hasCore="1" single="1">
  <attr name="units" type="list">
   <text>units</text>
   <textlocation row="0" col="0" />
   <items>
    <item>calspair</item>
    <item>linecolumn</item>
    <item>linecolumnpair</item>
    <item>linerange</item>
    <item>other</item>
   </items>
   <location row="0" col="1" />
  </attr>
  <attr name="label" type="input">
   <text>label</text>
   <textlocation row="1" col="0" />
   <location row="1" col="1" />
  </attr>
  <attr name="otherunits" type="input">
   <text>otherunits</text>
   <textlocation row="2" col="0" />
   <location row="2" col="1" />
  </attr>
  <attr name="linkends" type="input">
   <text>linkends</text>
   <textlocation row="3" col="0" />
   <location row="3" col="1" />
  </attr>
  <attr name="coords" type="input">
   <text>coords</text>
   <textlocation row="4" col="0" />
   <location row="4" col="1" />
  </attr>
 </tag>
 <tag name="areaset" hasCore="1">
  <attr name="units" type="list">
   <text>units</text>
   <textlocation row="0" col="0" />
   <location row="0" col="1" />
   <items>
    <item>calspair</item>
    <item>linecolumn</item>
    <item>linecolumnpair</item>
    <item>linerange</item>
    <item>other</item>
   </items>
  </attr>
  <attr name="label" type="input">
   <text>label</text>
   <textlocation row="1" col="0" />
   <location row="1" col="1" />
  </attr>
  <attr name="otherunits" type="input">
   <text>otherunits</text>
   <textlocation row="2" col="0" />
   <location row="2" col="1" />
  </attr>
  <attr name="coords" type="input">
   <text>coords</text>
   <textlocation row="3" col="0" />
   <location row="3" col="1" />
  </attr>
 </tag>
 <tag name="areaspec" hasCore="1">
  <attr name="units" type="list">
   <text>units</text>
   <textlocation row="0" col="0" />
   <location row="0" col="1" />
   <items>
    <item>calspair</item>
    <item>linecolumn</item>
    <item>linecolumnpair</item>
    <item>linerange</item>
    <item>other</item>
   </items>
  </attr>
  <attr name="otherunits" type="input">
   <text>otherunits</text>
   <textlocation row="1" col="0" />
   <location row="1" col="1" />
  </attr>
 </tag>
 <tag name="arg" hasCore="1">
  <attr name="rep" type="list">
   <text>rep</text>
   <textlocation row="0" col="0" />
   <items>
    <item>norepeat</item>
    <item>repeat</item>
   </items>
   <location row="0" col="1" />
  </attr>
  <attr name="choice" type="list">
   <text>choice</text>
   <textlocation row="1" col="0" />
   <items>
    <item>opt</item>
    <item>plain</item>
    <item>req</item>
   </items>
   <location row="1" col="1" />
  </attr>
 </tag>
 <tag name="attribution" hasCore="1" />
 <tag name="author" hasCore="1" />
 <tag name="authorgroup" hasCore="1" />
</TAGS>
