<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
 <tag name="label" hasCore="1" />
 <tag name="legalnotice" hasCore="1" />
 <tag name="lhs" hasCore="1" />
 <tag name="lineannotation" hasCore="1" />
 <tag name="link" hasCore="1">
  <attr name="linkend" type="input">
   <text>linkend</text>
   <textlocation row="0" col="0" />
   <location row="0" col="1" />
  </attr>
  <attr name="endterm" type="input">
   <text>endterm</text>
   <textlocation row="1" col="0" />
   <location row="1" col="1" />
  </attr>
  <attr name="type" type="input">
   <text>type</text>
   <textlocation row="2" col="0" />
   <location row="2" col="1" />
  </attr>
 </tag>
 <tag name="listitem" hasCore="1">
  <attr name="override" type="input">
   <text>override</text>
   <textlocation row="0" col="0" />
   <location row="0" col="1" />
  </attr>
 </tag>
 <tag name="literal" hasCore="1">
  <attr name="moreinfo" type="list">
   <text>moreinfo</text>
   <textlocation row="0" col="0" />
   <location row="0" col="1" />
   <items>
    <item>none</item>
    <item>refentry</item>
   </items>
  </attr>
 </tag>
 <tag name="literallayout" hasCore="1">
  <attr name="width" type="input">
   <text>width</text>
   <textlocation row="0" col="0" />
   <location row="0" col="1" />
  </attr>
  <attr name="format" type="list">
   <text>format</text>
   <textlocation row="1" col="0" />
   <location row="1" col="1" />
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
  <attr name="class" type="list">
   <text>class</text>
   <textlocation row="3" col="0" />
   <location row="3" col="1" />
   <items>
    <item>monospaced</item>
    <item>normal</item>
   </items>
  </attr>
 </tag>
</TAGS>