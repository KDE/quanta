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
   <stoppingtag>include</stoppingtag>
   <stoppingtag>import</stoppingtag>
   <stoppingtag>redefine</stoppingtag>
   <stoppingtag>simpleType</stoppingtag>
   <stoppingtag>complexType</stoppingtag>
   <stoppingtag>group</stoppingtag>
   <stoppingtag>attributeGroup</stoppingtag>
   <stoppingtag>element</stoppingtag>
   <stoppingtag>attribute</stoppingtag>
   <stoppingtag>notation</stoppingtag>
  </stoppingtags>
 </tag>
</TAGS>
