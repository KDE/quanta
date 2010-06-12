<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
<tag name="form" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="action" type="url">
        <text>action</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" colspan="3" />
    </attr>

    <attr name="method" type="list">
        <text>method</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1"/>
        <items>
                <item>GET</item>
                <item>POST</item>
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

    <attr name="accept-charset" type="input">
        <text>acceptcharset</text>
        <textlocation col="2" row="3" />
        <location col="3" row="3"/>
    </attr>

    <attr name="accept" type="list">
        <text>accept</text>
        <textlocation col="0" row="4" />
        <location col="1" row="4" colspan="3"/>
        <items>
           <item>application/x-www-form-urlencoded</item>
           <item>multipart/form-data</item>
        </items>
    </attr>

    <attr name="onsubmit" type="input">
        <text>onsubmit</text>
        <textlocation col="2" row="5" />
        <location col="3" row="5"/>
    </attr>

    <attr name="onreset" type="input">
        <text>onreset</text>
        <textlocation col="2" row="6" />
        <location col="3" row="6"/>
    </attr>

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
