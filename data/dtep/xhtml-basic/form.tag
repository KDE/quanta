<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
<tag name="form" hasCore="1" hasI18n="1">

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

<children>
<child name="address" />
<child name="blockquote" />
<child name="div" />
<child name="dl" />
<child name="h1" />
<child name="h2" />
<child name="h3" />
<child name="h4" />
<child name="h5" />
<child name="h6" />
<child name="ol" />
<child name="p" />
<child name="pre" />
<child name="table" />
<child name="ul" />
</children>

</tag>
</TAGS>
