<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
 <tag name="complexContent">
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
   <text>mixed</text>
   <location col="0" row="1"/>
  </label>
  <attr name="mixed" type="list">
   <items>
    <item>true</item>
    <item>false</item>
   </items>
   <tooltip>true, if character data is allowed to appear between the child elements. Default is false.</tooltip>
   <whatsthis>true, if character data is allowed to appear between the child elements. Default is false.</whatsthis>
   <location col="1" row="1"/>
  </attr>
  <children>
   <child name="annotation" />
   <child name="restriction" />
   <child name="extension" />
  </children>
 </tag>
</TAGS>
