<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
<tag name="form" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="action" type="input">
        <text>action</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" colspan="3" />
    </attr>

    <attr name="method" type="list">
        <text>method</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1"/>
        <items>
                <item>get</item>
                <item>post</item>
        </items>
    </attr>

    <attr name="enctype" type="list">
        <text>enctype</text>
        <textlocation col="0" row="2" />
        <location col="1" row="2" colspan="3"/>
        <items>
           <item>application/x-www-form-urlencoded</item>
           <item>multipart/form-data</item>
        </items>
    </attr>

    <attr name="accept" type="list">
        <text>enctype</text>
        <textlocation col="0" row="2" />
        <location col="1" row="3" colspan="3"/>
        <items>
          <item>text/html</item>
          <item>image/jpeg</item>
          <item>image/gif</item>
          <item>image/png</item>
          <item>model/vrml</item>
          <item>application/java</item>
          <item>application/mathml+xml</item>
          <item>application/rss+xml</item>
          <item>application/xhtml+xml</item>
          <item>application/xml</item>
          <item>text/css</item>
          <item>text/javascript</item>
          <item>text/xml</item>
        </items>
    </attr>

    <attr name="accept-charset" type="input">
        <text>Accept charset</text>
        <textlocation col="2" row="3" />
        <location col="3" row="3"/>
    </attr>

    <attr name="onsubmit" type="input" />
    <attr name="onreset" type="input" />

<children>
<child name="address" />
<child name="blockquote" />
<child name="del" />
<child name="div" />
<child name="dl" />
<child name="fieldset" />
<child name="h1" />
<child name="h2" />
<child name="h3" />
<child name="h4" />
<child name="h5" />
<child name="h6" />
<child name="hr" />
<child name="ins" />
<child name="noscript" />
<child name="ol" />
<child name="p" />
<child name="pre" />
<child name="script" />
<child name="table" />
<child name="ul" />
</children>
</tag>
</TAGS>
