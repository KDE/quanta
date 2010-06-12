<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
 <tag name="include">
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
   <text>schemaLocation</text>
   <location col="0" row="1"/>
  </label>
  <attr name="schemaLocation" type="input">
   <tooltip>URI to the schema to include in the target namespace of the containing schema.</tooltip>
   <whatsthis>URI to the schema to include in the target namespace of the containing schema.</whatsthis>
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
  </children>
 </tag>
</TAGS>
