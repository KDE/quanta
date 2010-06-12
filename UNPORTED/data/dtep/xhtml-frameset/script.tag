<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
<tag name="script">

    <attr name="type" type="list">
        <text>Type</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" colspan="2" />
        <items>
            <item>text/html</item>
            <item>text/css</item>
            <item>text/php</item>
            <item>text/javascript</item>
            <item>application/x-javascript</item>
        </items>
    </attr>

    <attr name="id" type="list" source="dcop" interface="QuantaIf" method="idSelectors()" arguments="" >
        <text>Id</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1" colspan="2" />
    </attr>

    <attr name="src" type="url">
        <text>Src</text>
        <textlocation col="0" row="2" />
        <location col="1" row="2" colspan="2" />
    </attr>

    <attr name="charset" type="input">
        <text>Charset</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" colspan="2" />
    </attr>

    <attr name="defer" type="check">
        <text>Defer</text>
        <location col="1" row="4" colspan="2" />
    </attr>

    <attr name="xml:space" type="list">
        <text>Xml:space</text>
        <textlocation col="0" row="5" />
        <location col="1" row="5" colspan="2" />
        <items>
                <item>preserve</item>
        </items>
    </attr>

    <attr name="language" type="input">
        <text>Language</text>
        <textlocation col="0" row="6" />
        <location col="1" row="6" colspan="2" />
    </attr>

</tag>
</TAGS>
