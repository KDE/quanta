<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
 <tag name="attributeGroup">
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
   <tooltip>Name of the attribute group. Cannot be present if ref is used.</tooltip>
   <whatsthis>Name of the attribute group. Cannot be present if ref is used.</whatsthis>
   <location col="1" row="1"/>
  </attr>

  <label>
   <text>ref</text>
   <location col="0" row="2"/>
  </label>
  <attr name="ref" type="input">
   <tooltip>Reference to a named attribute group. Cannot be present if name is used.</tooltip>
   <whatsthis>Reference to a named attribute group. Cannot be present if name is used.</whatsthis>
   <location col="1" row="2"/>
  </attr>

  <stoppingtags>
   <stoppingtag name="include" />
   <stoppingtag name="import" />
   <stoppingtag name="redefine" />
   <stoppingtag name="simpleType" />
   <stoppingtag name="complexType" />
   <stoppingtag name="group" />
   <stoppingtag name="element" />
   <stoppingtag name="notation" />
  </stoppingtags>
  <children>
   <child name="annotation" />
   <child name="attribute" />
   <child name="attributeGroup" />
   <child name="anyAttribute" />
  </children>
 </tag>
</TAGS>
