<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
 <tag name="all">
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
   <text>minOccurs</text>
   <location col="0" row="1"/>
  </label>
  <attr name="minOccurs" type="list">
   <items>
    <item>0</item>
    <item>1</item>
   </items>
   <tooltip>Minimum number of times the element can occur. Value can be 0 or 1. Default is 1.</tooltip>
   <whatsthis>Minimum number of times the element can occur. Value can be 0 or 1. Default is 1.</whatsthis>
   <location col="1" row="1"/>
  </attr>

  <label>
   <text>maxOccurs</text>
   <location col="0" row="2"/>
  </label>
  <attr name="maxOccurs" type="list">
   <items>
    <item>1</item>
   </items>
   <tooltip>Maximum number of times the element can occur. The value must be 1.</tooltip>
   <whatsthis>Maximum number of times the element can occur. The value must be 1.</whatsthis>
   <location col="1" row="2"/>
  </attr>
  <children>
   <child name="annotation" />
   <child name="element" />
  </children>
 </tag>
</TAGS>
