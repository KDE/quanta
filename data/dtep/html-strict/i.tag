<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
<tag name="i" hasCore="1" hasI18n="1" hasScript="1" scope="word">
<children>
<child name="#text" />
 <child name="tt" />
 <child name="i" />
 <child name="b" />
 <child name="big" />
 <child name="small" />
  <child name="em" />
 <child name="strong" />
 <child name="dfn" />
 <child name="code" />
 <child name="samp" />
 <child name="kbd" />
 <child name="var" />
 <child name="cite" />
 <child name="abbr" />
 <child name="acronym" />
  <child name="a" />
 <child name="img" />
 <child name="object" />
 <child name="br" />
 <child name="script" />
 <child name="map" />
 <child name="q" />
 <child name="sub" />
 <child name="sup" />
 <child name="span" />
 <child name="bdo" />
  <child name="input" />
 <child name="select" />
 <child name="textarea" />
 <child name="label" />
 <child name="button" />
</children>
</tag>

<tag name="img" single="1">
  <attr name="src" type="url">
    <text>Src</text>
    <textlocation col="0" row="0" />
    <location col="1" row="0" colspan="2" />
  </attr>
  <attr name="alt" type="input">
    <text>Alt</text>
    <textlocation col="0" row="1" />
    <location col="1" row="1" colspan="2" />
  </attr>
  <attr name="longdesc" type="input">
    <text>Longdesc</text>
    <textlocation col="0" row="2" />
    <location col="1" row="2" colspan="2" />
  </attr>
  <attr name="width" type="input">
    <text>Width</text>
    <textlocation col="0" row="3" />
    <location col="1" row="3" colspan="2" />
  </attr>
  <attr name="height" type="input">
    <text>Height</text>
    <textlocation col="0" row="4" />
    <location col="1" row="4" colspan="2" />
  </attr>
  <attr name="usemap" type="input">
    <text>Usemap</text>
    <textlocation col="0" row="5" />
    <location col="1" row="5" colspan="2" />
  </attr>
  <attr name="ismap" type="input">
    <text>Ismap</text>
    <textlocation col="0" row="6" />
    <location col="1" row="6" colspan="2" />
  </attr>
  <attr name="hspace" type="input">
    <text>HSpace</text>
    <textlocation col="0" row="9" />
    <location col="1" row="9" colspan="2" />
  </attr>
  <attr name="vspace" type="input">
    <text>VSpace</text>
    <textlocation col="0" row="10" />
    <location col="1" row="10" colspan="2" />
  </attr>
  <attr name="name" type="input">
    <text>Name</text>
    <textlocation col="0" row="11" />
    <location col="1" row="11" colspan="2" />
  </attr>

</tag>

<tag name="input" hasScript="1" hasCore="1" hasI18n="1" single="1">

    <attr name="type" type="list">
      <text>Type</text>
      <textlocation col="0" row="0" colspan="1" />
      <items>
        <item>text</item>
        <item>password</item>
        <item>checkbox</item>
        <item>radio</item>
        <item>submit</item>
        <item>reset</item>
        <item>file</item>
        <item>hidden</item>
        <item>image</item>
        <item>button</item>
      </items>
      <location col="1" row="0" colspan="1" />
      <tooltip>What - no kparts?</tooltip>
    </attr>

    <attr name="disabled" type="check">
      <text>Disabled</text>
      <location col="2" row="0" colspan="1" />
      <tooltip>Disable the element</tooltip>
    </attr>

    <attr name="checked" type="check">
      <text>Checked</text>
      <location col="3" row="0" colspan="1" />
      <tooltip>For radio button or check box</tooltip>
    </attr>

    <attr name="name" type="input">
      <text>Name</text>
      <textlocation col="0" row="1" colspan="1" />
      <location col="1" row="1" colspan="1" />
    </attr>

    <attr name="value" type="input">
      <text>Value</text>
      <textlocation col="2" row="1" colspan="1" />
      <location col="3" row="1" colspan="1" />
    </attr>

    <attr name="size" type="input">
      <text>Size</text>
      <textlocation col="0" row="2" colspan="1" />
      <location col="1" row="2" colspan="1" />
      <tooltip>No of characters for text input</tooltip>
    </attr>

    <attr name="maxlength" type="input">
      <text>Max length</text>
      <textlocation col="2" row="2" colspan="1" />
      <location col="3" row="2" colspan="1" />
      <tooltip>Max characters allowed</tooltip>
    </attr>

    <attr name="alt" type="input">
      <text>Alt</text>
      <textlocation col="0" row="3" colspan="1" />
      <location col="1" row="3" colspan="3" />
      <tooltip>Alternate text for image</tooltip>
    </attr>

    <attr name="src" type="url">
      <text>Img src</text>
      <textlocation col="0" row="4" colspan="1" />
      <location col="1" row="4" colspan="3" />
      <tooltip>URL for image to use</tooltip>
    </attr>

    <attr name="usemap" type="url">
<!--  <text>Use map</text>
      <textlocation col="0" row="5" colspan="1" />
      <location col="1" row="5" colspan="3" /> -->
    </attr>

    <attr name="tabindex" type="input">
      <text>Tab index</text>
      <textlocation col="0" row="5" colspan="1" />
      <location col="1" row="5" colspan="1" />
      <tooltip>NA in NS4</tooltip>
    </attr>

    <attr name="readonly" type="check">
      <text>Read only</text>
      <location col="3" row="5" colspan="1" />
      <tooltip>NA in NS4</tooltip>
    </attr>

    <attr name="align" type="list">
      <text>Align</text>
      <textlocation col="0" row="6" colspan="1" />
      <items>
        <item>top</item>
        <item>middle</item>
        <item>bottom</item>
        <item>left</item>
        <item>right</item>
      </items>
      <location col="1" row="6" colspan="1" />
      <tooltip>Depricated in HTML 4.0</tooltip>
    </attr>

    <attr name="accesskey" type="input">
      <text>Accesskey</text>
      <textlocation col="2" row="6" colspan="1" />
      <location col="3" row="6" colspan="1" />
      <tooltip>Shortcut - NA in NS4</tooltip>
    </attr>

    <attr name="onfocus" type="input">
      <text>On focus</text>
      <textlocation col="0" row="8" colspan="1" />
      <location col="1" row="8" colspan="1" />
    </attr>

    <attr name="onselect" type="input">
      <text>On select</text>
      <textlocation col="2" row="8" colspan="1" />
      <location col="3" row="8" colspan="1" />
    </attr>

    <attr name="onblur" type="input">
      <text>On blur</text>
      <textlocation col="0" row="9" colspan="1" />
      <location col="1" row="9" colspan="1" />
    </attr>

    <attr name="onchange" type="input">
      <text>On change</text>
      <textlocation col="2" row="9" colspan="1" />
      <location col="3" row="9" colspan="1" />
    </attr>

    <attr name="accept"> </attr>


</tag>

<tag name="ins" hasCore="1" hasI18n="1" hasScript="1">
  <attr name="cite" type="input">
    <text>Cite</text>
    <textlocation col="0" row="0" />
    <location col="1" row="0" colspan="2" />
  </attr>
  <attr name="datetime" type="input">
    <text>DateTime</text>
    <textlocation col="0" row="1" />
    <location col="1" row="1" colspan="2" />
  </attr>

<children>
<child name="#text" />
 <child name="tt" />
 <child name="i" />
 <child name="b" />
 <child name="big" />
 <child name="small" />
  <child name="em" />
 <child name="strong" />
 <child name="dfn" />
 <child name="code" />
 <child name="samp" />
 <child name="kbd" />
 <child name="var" />
 <child name="cite" />
 <child name="abbr" />
 <child name="acronym" />
  <child name="a" />
 <child name="img" />
 <child name="object" />
 <child name="br" />
 <child name="script" />
 <child name="map" />
 <child name="q" />
 <child name="sub" />
 <child name="sup" />
 <child name="span" />
 <child name="bdo" />
  <child name="input" />
 <child name="select" />
 <child name="textarea" />
 <child name="label" />
 <child name="button" />
  <child name="p" />
 <child name="h1" />
 <child name="h2" />
 <child name="h3" />
 <child name="h4" />
 <child name="h5" />
 <child name="h6" />
 <child name="ul" />
 <child name="ol" />
 <child name="pre" />
 <child name="dl" />
 <child name="div" />
 <child name="noscript" />
 <child name="blockquote" />
 <child name="form" />
 <child name="hr" />
 <child name="table" />
 <child name="fieldset" />
 <child name="address" />
 </children>
</tag>
</TAGS>
