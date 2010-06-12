<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
 <tag name="simpleType">
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
   <tooltip>Name for the simpleType. Required, if the simpleType element is a child of the schema element; else it is prohibited.</tooltip>
   <whatsthis>Name for the simpleType. Required, if the simpleType element is a child of the schema element; else it is prohibited.</whatsthis>
   <location col="1" row="1"/>
  </attr>

  <stoppingtags>
   <stoppingtag name="include" />
   <stoppingtag name="import" />
   <stoppingtag name="redefine" />
   <stoppingtag name="simpleType" />
   <stoppingtag name="complexType" />
   <stoppingtag name="group" />
   <stoppingtag name="attributeGroup" />
   <stoppingtag name="element" />
   <stoppingtag name="attribute" />
   <stoppingtag name="notation" />
  </stoppingtags>
  <children>
   <child name="annotation" />
   <child name="restriction" />
   <child name="list" />
   <child name="union" />
  </children>
 </tag>
</TAGS>
