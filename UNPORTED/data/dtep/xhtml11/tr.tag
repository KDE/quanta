<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
<tag name="tr" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="align" type="list">
        <text>align</text>
        <textlocation col="0" row="0" />
        <items>
            <item>left</item>
            <item>center</item>
            <item>right</item>
            <item>justify</item>
            <item>char</item>
        </items>
        <location col="1" row="0" colspan="1" />
    </attr>

    <attr name="valign" type="list">
        <text>valign</text>
        <textlocation col="0" row="1"  />
        <items>
            <item>baseline</item>
            <item>bottom</item>
            <item>middle</item>
            <item>top</item>
        </items>
        <location col="1" row="1" colspan="1" />
    </attr>

    <attr name="char" type="input" />
    <attr name="charoff" type="input" />

    <stoppingtags>
        <stoppingtag name="tr" />
        <stoppingtag name="tfoot" />
    </stoppingtags>

<children>
<child name="th" />
<child name="td" />
</children>
</tag>
</TAGS>
