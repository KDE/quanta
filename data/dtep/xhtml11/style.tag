<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
<tag name="style" hasI18n="1">
     <attr name="id" type="list" source="dcop" interface="QuantaIf" method="idSelectors()" arguments="" >
        <text>id</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" colspan="2" />
    </attr>
    <attr name="type" type="list">
        <text>type</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1" colspan="2" />
        <items>
                <item>text/css</item>
        </items>
    </attr>

    <attr name="media" type="list">
        <text>media</text>
        <textlocation col="0" row="2" />
        <location col="1" row="2" colspan="2" />
        <items>
                <item>screen</item>
                <item>tty</item>
                <item>tv</item>
                <item>projection</item>
                <item>handheld</item>
                <item>print</item>
                <item>braille</item>
                <item>aural</item>
                <item>all</item>
        </items>
    </attr>

    <attr name="title" type="input">
        <text>title</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" colspan="2" />
    </attr>

    <attr name="xml:space" type="list">
        <text>xml:space</text>
        <textlocation col="0" row="4" />
        <location col="1" row="4" colspan="2" />
        <items>
                <item>preserve</item>
        </items>
    </attr>

    <attr name="defer" type="check">
        <text>defer</text>
        <textlocation col="0" row="5" />
        <location col="1" row="5" colspan="2" />
    </attr>
    <children>
	    <child name="#text" />
    </children>
</tag>
</TAGS>
