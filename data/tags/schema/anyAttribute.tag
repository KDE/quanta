<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
 <tag name="anyAttribute">
  <label>
   <text>id</text>
   <location col="0" row="0"/>
  </label>
  <attr name="id" type="input">
   <tooltip>A unique ID for the element.</tooltip>
   <whatsthis>A unique ID for the element.</whatsthis>
   <location col="1" row="0"/>
  </attr>

  <label>
   <text>namespace</text>
   <location col="0" row="1"/>
  </label>
  <attr name="namespace" type="input">
   <tooltip>Namespaces containing the attributes that can be used.</tooltip>
   <whatsthis>Namespaces containing the attributes that can be used.</whatsthis>
   <location col="1" row="1"/>
  </attr>

  <label>
   <text>processContents</text>
   <location col="0" row="2"/>
  </label>
  <attr name="processContents" type="list">
   <items>
    <item>lax</item>
    <item>skip</item>
    <item>strict</item>
   </items>
   <tooltip>How should the XML processor handle validation, in regard to attributes specified by this element?</tooltip>
   <whatsthis>How should the XML processor handle validation, in regard to attributes specified by this element?</whatsthis>
   <location col="1" row="2"/>
  </attr>
  <children>
   <child name="annotation" />
  </children>
 </tag>
</TAGS>
