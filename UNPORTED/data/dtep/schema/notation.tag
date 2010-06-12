<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
 <tag name="notation">
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
   <text>public</text>
   <location col="0" row="2"/>
  </label>
  <attr name="public" type="input">
   <tooltip>URI corresponding to the public identifier.</tooltip>
   <whatsthis>URI corresponding to the public identifier.</whatsthis>
   <location col="1" row="2"/>
  </attr>

  <label>
   <text>system</text>
   <location col="0" row="3"/>
  </label>
  <attr name="system" type="input">
   <tooltip>URI corresponding to the system identifier.</tooltip>
   <whatsthis>URI corresponding to the system identifier.</whatsthis>
   <location col="1" row="3"/>
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
