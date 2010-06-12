<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
 <tag name="group">
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
   <tooltip>Name for the group. Only usable when used as a child element to schema and ref is not used.</tooltip>
   <whatsthis>Name for the group. Only usable when used as a child element to schema and ref is not used.</whatsthis>
   <location col="1" row="1"/>
  </attr>

  <label>
   <text>ref</text>
   <location col="0" row="2"/>
  </label>
  <attr name="ref" type="input">
   <tooltip>Reference to a named group. Cannot be present when name is.</tooltip>
   <whatsthis></whatsthis>
   <location col="1" row="2"/>
  </attr>

  <label>
   <text>minOccurs</text>
   <location col="0" row="3"/>
  </label>
  <attr name="minOccurs" type="input">
   <tooltip>Minimum number of times the element can occur. Value can be >=0. Default is 1. Only usable with ref.</tooltip>
   <whatsthis>Minimum number of times the element can occur. Value can be >=0. Default is 1. Only usable with ref.</whatsthis>
   <location col="1" row="3"/>
  </attr>

  <label>
   <text>maxOccurs</text>
   <location col="0" row="4"/>
  </label>
  <attr name="maxOccurs" type="input">
   <tooltip>Maximum number of times the element can occur. Value can be >=0 or unbounded. Default is 1. Only usable with ref.</tooltip>
   <whatsthis>Maximum number of times the element can occur. Value can be >=0 or unbounded. Default is 1. Only usable with ref.</whatsthis>
   <location col="1" row="4"/>
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
   <child name="all" />
   <child name="choice" />
   <child name="sequence" />
  </children>
 </tag>
</TAGS>
