<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
<tag name="param" single="1">

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
                <item>image/jpeg</item>
                <item>image/gif</item>
                <item>image/png</item>
                <item>model/vrml</item>
                <item>video/quicktime</item>
                <item>application/java</item>
		<item>audio/basic</item>
		<item>text/html</item>
		<item>text/css</item>
        </items>
    </attr>

    <attr name="value" type="input">
        <text>Value</text>
        <textlocation col="0" row="2" />
        <location col="1" row="2" colspan="1" />
    </attr>

    <attr name="valuetype" type="list">
        <text>Valuetype</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" colspan="2" />
        <items>
                <item>data</item>
                <item>ref</item>
                <item>object</item>
        </items>
    </attr>

   <attr name="name" type="input">
        <text>Name</text>
        <textlocation col="0" row="4" />
        <location col="1" row="4" colspan="2" />
    </attr>

</tag>
</TAGS>
