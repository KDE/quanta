<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
 <tag name="complexType">
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
   <text>name</text>
   <location col="0" row="1"/>
  </label>
  <attr name="name" type="input">
   <tooltip>Name for the element.</tooltip>
   <whatsthis>Name for the element.</whatsthis>
   <location col="1" row="1"/>
  </attr>

  <label>
   <text>mixed</text>
   <location col="0" row="2"/>
  </label>
  <attr name="mixed" type="list">
   <items>
    <item>false</item>
    <item>true</item>
   </items>
   <tooltip>true, if character data is allowed to appear between the child elements. Default is false. If simpleContent is used within this type, then mixed is not allowed.</tooltip>
   <whatsthis>true, if character data is allowed to appear between the child elements. Default is false. If simpleContent is used within this type, then mixed is not allowed.</whatsthis>
   <location col="1" row="2"/>
  </attr>

  <label>
   <text>abstract</text>
   <location col="0" row="3"/>
  </label>
  <attr name="abstract" type="list">
   <items>
    <item>true</item>
    <item>false</item>
   </items>
   <tooltip>true, if an element must use a complex type derived from this complex type. Default is false.</tooltip>
   <whatsthis>true, if an element must use a complex type derived from this complex type. Default is false.</whatsthis>
   <location col="1" row="3"/>
  </attr>

  <label>
   <text>block</text>
   <location col="0" row="4"/>
  </label>
  <attr name="block" type="input">
   <location col="1" row="4"/>
  </attr>

  <label>
   <text>final</text>
   <location col="0" row="5"/>
  </label>
  <attr name="final" type="input">
   <location col="1" row="5"/>
  </attr>

  <stoppingtags>
   <stoppingtag name="include" />
   <stoppingtag name="import" />
   <stoppingtag name="redefine" />
   <stoppingtag name="simpleType" />
   <stoppingtag name="complexType" />
   <stoppingtag name="element" />
   <stoppingtag name="notation" />
  </stoppingtags>
  <children>
   <child name="all" />
   <child name="annotation" />
   <child name="anyAttribute" />
   <child name="attribute" />
   <child name="attributeGroup" />
   <child name="choice" />
   <child name="complexContent" />
   <child name="group" />
   <child name="sequence" />
   <child name="simpleContent" />
  </children>
 </tag>
</TAGS>
