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

    <attr name="src" type="url">
        <text>Source</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1" colspan="2" />
    </attr>

    <attr name="charset" type="input">
        <text>Charset</text>
        <textlocation col="0" row="2" />
        <location col="1" row="2" colspan="2" />
    </attr>

    <attr name="defer" type="check">
        <text>Defer (script execution may wait)</text>
        <location col="0" row="3" colspan="2" />
    </attr>

	<attr name="xml:space" type="list">
        <text>xml:space</text>
        <textlocation col="0" row="4" />
        <location col="1" row="4" colspan="2" />
        <items>
                <item>preserve</item>
        </items>
    </attr>
	
</tag>
</TAGS>
