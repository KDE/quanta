<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
 <tag name="union">
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
   <text>memberTypes</text>
   <location col="0" row="1"/>
  </label>
  <attr name="memberTypes" type="input">
   <tooltip>List of built-in data types or simpleType elements defined in a schema.</tooltip>
   <whatsthis>List of built-in data types or simpleType elements defined in a schema.</whatsthis>
   <location col="1" row="1"/>
  </attr>
  <children>
   <child name="annotation" />
   <child name="simpleType" />
  </children>
 </tag>
</TAGS>
