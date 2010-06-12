<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
 <tag name="list">
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
   <text>itemType</text>
   <location col="0" row="1"/>
  </label>
  <attr name="itemType" type="input">
   <tooltip>Name of a built-in data type or simpleType. If the content contains a simpleType element, then itemType is not allowed, else itemType is required.</tooltip>
   <whatsthis>Name of a built-in data type or simpleType. If the content contains a simpleType element, then itemType is not allowed, else itemType is required.</whatsthis>
   <location col="1" row="1"/>
  </attr>
  <children>
   <child name="annotation" />
   <child name="simpleType" />
  </children>
 </tag>
</TAGS>
