<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
 <tag name="element">
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
   <tooltip>Name for the element. Cannot be used if ref is present.</tooltip>
   <whatsthis>Name for the element. Cannot be used if ref is present.</whatsthis>
   <location col="1" row="1"/>
  </attr>

  <label>
   <text>ref</text>
   <location col="0" row="2"/>
  </label>
  <attr name="ref" type="input">
   <tooltip>Reference to a named element. Cannot be used if name is present.</tooltip>
   <whatsthis>Reference to a named element. Cannot be used if name is present.</whatsthis>
   <location col="1" row="2"/>
  </attr>

  <label>
   <text>type</text>
   <location col="0" row="3"/>
  </label>
  <attr name="type" type="input">
   <tooltip>Name of a built-in data type, simpleType, or complexType.</tooltip>
   <whatsthis>Name of a built-in data type, simpleType, or complexType.</whatsthis>
   <location col="1" row="3"/>
  </attr>

  <label>
   <text>minOccurs</text>
   <location col="0" row="4"/>
  </label>
  <attr name="minOccurs" type="input">
   <tooltip>Minimum number of times the element can occur. Value can be >=0. Default is 1. Only usable with ref.</tooltip>
   <whatsthis>Minimum number of times the element can occur. Value can be >=0. Default is 1. Only usable with ref.</whatsthis>
   <location col="1" row="4"/>
  </attr>

  <label>
   <text>maxOccurs</text>
   <location col="0" row="5"/>
  </label>
  <attr name="maxOccurs" type="input">
   <tooltip>Maximum number of times the element can occur. Value can be >=0 or unbounded. Default is 1. Only usable with ref.</tooltip>
   <whatsthis>Maximum number of times the element can occur. Value can be >=0 or unbounded. Default is 1. Only usable with ref.</whatsthis>
   <location col="1" row="5"/>
  </attr>

  <label>
   <text>nullable</text>
   <location col="0" row="6"/>
  </label>
  <attr name="nullable" type="list">
   <items>
    <item>false</item>
    <item>true</item>
   </items>
   <tooltip>Whether an explicit null value can be assigned to the element. Default is false.</tooltip>
   <whatsthis>Whether an explicit null value can be assigned to the element. Default is false.</whatsthis>
   <location col="1" row="6"/>
  </attr>

  <label>
   <text>abstract</text>
   <location col="0" row="7"/>
  </label>
  <attr name="abstract" type="list">
   <items>
    <item>false</item>
    <item>true</item>
   </items>
   <location col="1" row="7"/>
  </attr>

  <label>
   <text>block</text>
   <location col="0" row="8"/>
  </label>
  <attr name="block" type="input">
   <location col="1" row="8"/>
  </attr>

  <label>
   <text>final</text>
   <location col="0" row="9"/>
  </label>
  <attr name="final" type="input">
   <location col="1" row="9"/>
  </attr>

  <label>
   <text>default</text>
   <location col="0" row="10"/>
  </label>
  <attr name="default" type="input">
   <tooltip>Default value for the element. Only usable if the element's content is a simple type or text-only.</tooltip>
   <whatsthis>Default value for the element. Only usable if the element's content is a simple type or text-only.</whatsthis>
   <location col="1" row="10"/>
  </attr>

  <label>
   <text>fixed</text>
   <location col="0" row="11"/>
  </label>
  <attr name="fixed" type="input">
   <tooltip>Fixed value for the element. Only usable if the element's content is a simple type or text-only.</tooltip>
   <whatsthis>Fixed value for the element. Only usable if the element's content is a simple type or text-only.</whatsthis>
   <location col="1" row="11"/>
  </attr>

  <label>
   <text>form</text>
   <location col="0" row="12"/>
  </label>
  <attr name="form" type="list">
   <items>
    <item>qualified</item>
    <item>unqualified</item>
   </items>
   <location col="1" row="12"/>
  </attr>

  <label>
   <text>substitutionGroup</text>
   <location col="0" row="13"/>
  </label>
  <attr name="substitutionGroup" type="input">
   <tooltip>Name of an element that can be substituted with this one. Only usable if the parent element is schema.</tooltip>
   <whatsthis>Name of an element that can be substituted with this one. Only usable if the parent element is schema.</whatsthis>
   <location col="1" row="13"/>
  </attr>

  <stoppingtags>
   <stoppingtag name="include" />
   <stoppingtag name="import" />
   <stoppingtag name="redefine" />
   <stoppingtag name="group" />
   <stoppingtag name="attributeGroup" />
   <stoppingtag name="element" />
   <stoppingtag name="attribute" />
   <stoppingtag name="notation" />
  </stoppingtags>
  <children>
   <child name="annotation" />
   <child name="simpleType" />
   <child name="complexType" />
   <child name="unique" />
   <child name="key" />
   <child name="keyref" />
  </children>
 </tag>
</TAGS>
