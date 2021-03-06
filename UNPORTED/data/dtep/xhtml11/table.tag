<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
<tag name="table" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="width" type="input">
        <text>width</text>
        <textlocation col="0" row="0" />
        <tooltip>Use % for percent or it will set pixels</tooltip>
        <location col="1" row="0" />
    </attr>

    <attr name="cellspacing" type="input">
        <text>cellspacing</text>
        <textlocation col="2" row="0" />
        <location col="3" row="0" />
    </attr>

    <attr name="border" type="input">
        <text>border</text>
        <textlocation col="0" row="1" />
        <tooltip>Set in pixels</tooltip>
        <location col="1" row="1" />
    </attr>

    <attr name="cellpadding" type="input">
        <text>cellpadding</text>
        <textlocation col="2" row="1" />
        <location col="3" row="1" />
    </attr>

    <attr name="frame" type="list">
        <text>frame</text>
        <textlocation col="0" row="2"  />
        <items>
            <item>above</item>
            <item>below</item>
            <item>border</item>
            <item>box</item>
            <item>hsides</item>
            <item>lhs</item>
            <item>rhs</item>
            <item>void</item>
            <item>vsides</item>
        </items>
        <location col="1" row="2" colspan="2" />
    </attr>

    <attr name="rules" type="list">
        <text>rules</text>
        <textlocation col="0" row="3"  />
        <items>
            <item>none</item>
            <item>groups</item>
            <item>rows</item>
            <item>cols</item>
            <item>all</item>
        </items>
        <location col="1" row="3" colspan="2" />
    </attr>

	<attr name="summary" type="input">
        <text>summary</text>
        <textlocation col="0" row="5" />
        <location col="1" row="5" colspan="2"/>
    </attr>

<children>
<child name="caption" />
<child name="col" />
<child name="colgroup" />
<child name="thead" />
<child name="tfoot" />
<child name="tbody" />
<child name="tr" usage="required" />
</children>

</tag>
</TAGS>
