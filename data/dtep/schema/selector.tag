<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
 <tag name="selector">
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
   <text>xpath</text>
   <location col="0" row="1"/>
  </label>
  <attr name="xpath" type="input">
   <tooltip>An XPath expression, relative to the element being declared, that identifies the child elements to which the identity constraint applies.</tooltip>
   <whatsthis>An XPath expression, relative to the element being declared, that identifies the child elements to which the identity constraint applies.</whatsthis>
   <location col="1" row="1"/>
  </attr>
  <children>
   <child name="annotation" />
  </children>
 </tag>
</TAGS>
