<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
 <tag name="attribute">
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
   <tooltip>Name of the attribute. Cannot be present if ref is used.</tooltip>
   <whatsthis>Name of the attribute. Cannot be present if ref is used.</whatsthis>
   <location col="1" row="1"/>
  </attr>

  <label>
   <text>ref</text>
   <location col="0" row="2"/>
  </label>
  <attr name="ref" type="input">
   <tooltip>Reference to a named attribute. Cannot be present if name is used.</tooltip>
   <whatsthis>Reference to a named attribute. Cannot be present if name is used.</whatsthis>
   <location col="1" row="2"/>
  </attr>

  <label>
   <text>type</text>
   <location col="0" row="3"/>
  </label>
  <attr name="type" type="input">
   <tooltip>Built-in data type or a simple type. If a simpleType element is present, then type cannot be used.</tooltip>
   <whatsthis>Built-in data type or a simple type. If a simpleType element is present, then type cannot be used.</whatsthis>
   <location col="1" row="3"/>
  </attr>

  <label>
   <text>default</text>
   <location col="0" row="4"/>
  </label>
  <attr name="default" type="input">
   <tooltip>Default value for the attribute. Cannot be present if fixed is used.</tooltip>
   <whatsthis>Default value for the attribute. Cannot be present if fixed is used.</whatsthis>
   <location col="1" row="4"/>
  </attr>

  <label>
   <text>fixed</text>
   <location col="0" row="5"/>
  </label>
  <attr name="fixed" type="input">
   <tooltip>Fixed value for the attribute. Cannot be present if default is used.</tooltip>
   <whatsthis>Fixed value for the attribute. Cannot be present if default is used.</whatsthis>
   <location col="1" row="5"/>
  </attr>

  <label>
   <text>form</text>
   <location col="0" row="6"/>
  </label>
  <attr name="form" type="list">
   <items>
    <item>qualified</item>
    <item>unqualified</item>
   </items>
   <tooltip>Form for the attribute. Default value is specified by the attributeFormDefault attribute in the schema element.</tooltip>
   <whatsthis>Form for the attribute. Default value is specified by the attributeFormDefault attribute in the schema element.</whatsthis>
   <location col="1" row="6"/>
  </attr>

  <label>
   <text>use</text>
   <location col="0" row="7"/>
  </label>
  <attr name="use" type="list">
   <items>
    <item>optional</item>
    <item>required</item>
    <item>prohibited</item>
   </items>
   <tooltip>Specifies usage of the attribute.</tooltip>
   <whatsthis>Specifies usage of the attribute.</whatsthis>
   <location col="1" row="7"/>
  </attr>

  <stoppingtags>
   <stoppingtag name="include" />
   <stoppingtag name="import" />
   <stoppingtag name="redefine" />
   <stoppingtag name="complexType" />
   <stoppingtag name="group" />
   <stoppingtag name="attributeGroup" />
   <stoppingtag name="element" />
   <stoppingtag name="attribute" />
   <stoppingtag name="notation" />
  </stoppingtags>
  <children>
   <child name="annotation" />
   <child name="simpleType" />
  </children>
 </tag>
</TAGS>
