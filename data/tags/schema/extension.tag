<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
 <tag name="extension">
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
   <text>base</text>
   <location col="0" row="1"/>
  </label>
  <attr name="base" type="input">
   <tooltip>Name of a built-in data type, simpleType, or complexType.</tooltip>
   <whatsthis>Name of a built-in data type, simpleType, or complexType.</whatsthis>
   <location col="1" row="1"/>
  </attr>
  <children>
   <child name="annotation" />
   <child name="group" />
   <child name="all" />
   <child name="choice" />
   <child name="sequence" />
   <child name="attribute" />
   <child name="attributeGroup" />
   <child name="anyAttribute" />
  </children>
 </tag>
</TAGS>
