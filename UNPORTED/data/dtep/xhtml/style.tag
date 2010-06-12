<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
<tag name="style" hasI18n="1">

    <attr name="id" type="list" source="dcop" interface="QuantaIf" method="idSelectors()" arguments="" >
        <text>Id</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" colspan="2" />
    </attr>

    <attr name="type" type="list">
        <text>Type</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1" colspan="2" />
        <items>
                <item>text/css</item>
        </items>
    </attr>

    <attr name="media" type="list">
        <text>Media</text>
        <textlocation col="0" row="2" />
        <location col="1" row="2" colspan="2" />
        <items>
                <item>all</item>
                <item>aural</item>
                <item>braille</item>
                <item>embossed</item>
                <item>handheld</item>
                <item>print</item>
                <item>projection</item>
                <item>screen</item>
                <item>tty</item>
                <item>tv</item>
        </items>
    </attr>

    <attr name="title" type="input">
        <text>Title</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" colspan="2" />
    </attr>

    <attr name="xml:space" type="list">
        <text>Xml:space</text>
        <textlocation col="0" row="4" />
        <location col="1" row="4" colspan="2" />
        <items>
                <item>preserve</item>
        </items>
    </attr>
    <children>
	<child name="#text" />
   </children>
</tag>
</TAGS>
