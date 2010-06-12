<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
 <tag name="schema">
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
   <text>version</text>
   <location col="0" row="1"/>
  </label>
  <attr name="version" type="input">
   <tooltip>Version of the schema.</tooltip>
   <whatsthis>Version of the schema.</whatsthis>
   <location col="1" row="1"/>
  </attr>

  <label>
   <text>targetNamespace</text>
   <location col="0" row="2"/>
  </label>
  <attr name="targetNamespace" type="input">
   <tooltip>URI reference of the namespace of this schema.</tooltip>
   <whatsthis>URI reference of the namespace of this schema.</whatsthis>
   <location col="1" row="2"/>
  </attr>

  <label>
   <text>xmlns</text>
   <location col="0" row="3"/>
  </label>
  <attr name="xmlns" type="input">
   <tooltip>URI reference for one or more namespaces for use in this schema. If no prefix is used, then components of that namespace may be used unqualified.</tooltip>
   <whatsthis>URI reference for one or more namespaces for use in this schema. If no prefix is used, then components of that namespace may be used unqualified.</whatsthis>
   <location col="1" row="3"/>
  </attr>

  <label>
   <text>attributeFormDefault</text>
   <location col="0" row="4"/>
  </label>
  <attr name="attributeFormDefault" type="list">
   <items>
    <item>qualified</item>
    <item>unqualified</item>
   </items>
   <tooltip>Default form for all attributes within this schema.</tooltip>
   <whatsthis>Default form for all attributes within this schema.</whatsthis>
   <location col="1" row="4"/>
  </attr>

  <label>
   <text>elementFormDefault</text>
   <location col="0" row="5"/>
  </label>
  <attr name="elementFormDefault" type="list">
   <items>
    <item>qualified</item>
    <item>unqualified</item>
   </items>
   <tooltip>Default form for all elements within this schema.</tooltip>
   <whatsthis>Default form for all elements within this schema.</whatsthis>
   <location col="1" row="5"/>
  </attr>

  <label>
   <text>blockDefault</text>
   <location col="0" row="6"/>
  </label>
  <attr name="blockDefault" type="input">
   <location col="1" row="6"/>
  </attr>

  <label>
   <text>finalDefault</text>
   <location col="0" row="7"/>
  </label>
  <attr name="finalDefault" type="input">
   <location col="1" row="7"/>
  </attr>
  <children>
   <child name="include" />
   <child name="import" />
   <child name="redefine" />
   <child name="annotation" />
   <child name="simpleType" />
   <child name="complexType" />
   <child name="group" />
   <child name="attributeGroup" />
   <child name="element" />
   <child name="attribute" />
   <child name="notation" />
  </children>
 </tag>
</TAGS>
