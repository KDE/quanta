<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
<tag name="td" hasCore="1" hasI18n="1" optional="1">

    <attr name="colspan" type="input">
        <text>colspan</text>
        <textlocation col="2" row="0" />
        <location col="3" row="0" />
    </attr>

    <attr name="rowspan" type="input">
        <text>rowspan</text>
        <textlocation col="2" row="1" />
        <location col="3" row="1" />
    </attr>

    <attr name="align" type="list">
        <text>align</text>
        <textlocation col="0" row="2" />
        <items>
            <item>left</item>
            <item>center</item>
            <item>right</item>
            <item>justify</item>
            <item>char</item>
        </items>
        <location col="1" row="2" colspan="1" />
    </attr>

    <attr name="valign" type="list">
        <text>valign</text>
        <textlocation col="2" row="2"  />
        <items>
            <item>baseline</item>
            <item>bottom</item>
            <item>middle</item>
            <item>top</item>
        </items>
        <location col="3" row="2" colspan="1" />
    </attr>

    <attr name="headers" type="input" />
    <attr name="abbr" type="input" />
    <attr name="scope" type="list">
        <text>scope</text>
        <textlocation col="0" row="3"  />
        <items>
            <item>row</item>
            <item>col</item>
            <item>rowgroup</item>
            <item>colgroup</item>
        </items>
        <location col="0" row="3" colspan="1" />
    </attr>
    <attr name="axis" type="input" />

    <stoppingtags>
        <stoppingtag name="td" />
        <stoppingtag name="tr" />
        <stoppingtag name="th" />
    </stoppingtags>

<children>
<child name="#text" />
<child name="a" />
<child name="abbr" />
<child name="acronym" />
<child name="address" />
<child name="blockquote" />
<child name="br" />
<child name="cite" />
<child name="code" />
<child name="dfn" />
<child name="div" />
<child name="dl" />
<child name="em" />
<child name="form" />
<child name="h1" />
<child name="h2" />
<child name="h3" />
<child name="h4" />
<child name="h5" />
<child name="h6" />
<child name="img" />
<child name="input" />
<child name="kbd" />
<child name="label" />
<child name="object" />
<child name="ol" />
<child name="p" />
<child name="pre" />
<child name="q" />
<child name="samp" />
<child name="select" />
<child name="span" />
<child name="strong" />
<child name="textarea" />
<child name="ul" />
<child name="var" />
</children>
</tag>
</TAGS>
