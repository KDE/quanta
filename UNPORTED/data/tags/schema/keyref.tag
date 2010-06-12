<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
 <tag name="keyref">
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
   <tooltip>Name of the keyref element.</tooltip>
   <whatsthis>Name of the keyref element.</whatsthis>
   <location col="1" row="1"/>
  </attr>

  <label>
   <text>refer</text>
   <location col="0" row="2"/>
  </label>
  <attr name="refer" type="input">
   <tooltip>Name of a key or unique element defined in this or another schema.</tooltip>
   <whatsthis>Name of a key or unique element defined in this or another schema.</whatsthis>
   <location col="1" row="2"/>
  </attr>
  <children>
   <child name="annotation" />
   <child name="selector" />
   <child name="field" />
  </children>
 </tag>
</TAGS>
