<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
 <tag name="import">
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
   <text>namespace</text>
   <location col="0" row="1"/>
  </label>
  <attr name="namespace" type="input">
   <tooltip>URI of the namespace to import.</tooltip>
   <whatsthis>URI of the namespace to import.</whatsthis>
   <location col="1" row="1"/>
  </attr>

  <label>
   <text>schemaLocation</text>
   <location col="0" row="2"/>
  </label>
  <attr name="schemaLocation" type="input">
   <tooltip>URI to the schema for the imported namespace.</tooltip>
   <whatsthis>URI to the schema for the imported namespace.</whatsthis>
   <location col="1" row="2"/>
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
