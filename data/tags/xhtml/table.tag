<!DOCTYPE TAGS>
<TAGS>
<tag name="table" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="width" type="input">
        <text>Width</text>
        <textlocation col="0" row="0" />
        <tooltip>use % for percent or it will set pixels</tooltip>
        <location col="1" row="0" />
    </attr>

    <attr name="cellspacing" type="input">
        <text>Cell spacing</text>
        <textlocation col="2" row="0" />
        <location col="3" row="0" />
    </attr>

    <attr name="border" type="input">
        <text>Border</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1" />
    </attr>

    <attr name="cellpadding" type="input">
        <text>Cell padding</text>
        <textlocation col="2" row="1" />
        <location col="3" row="1" />
    </attr>

    <attr name="align" type="list">
        <text>Align</text>
        <textlocation col="0" row="2" />
        <items>
            <item>left</item>
            <item>center</item>
            <item>right</item>
        </items>
        <location col="1" row="2" colspan="2" />
    </attr>

    <attr name="bgcolor" type="color">
        <text>Background color</text>
        <textlocation col="0" row="3"  />
        <location col="1" row="3" colspan="2" />
        <tooltip>Depricated in HTML 4.0</tooltip>
    </attr>

    <spacer orientation="v">
        <location col="0" row="4" colspan="4" />
    </spacer>

    <attr name="frame" type="list">
        <text>Frame</text>
        <textlocation col="0" row="5"  />
        <items>
            <item>above</item>
            <item>below</item>
            <item>border</item>
            <item>box</item>
            <item>hsides</item>
            <item>LHS</item>
            <item>RHS</item>
            <item>void</item>
            <item>vsides</item>
        </items>
        <location col="1" row="5" colspan="2" />
        <tooltip>Not supported in Netscape 4x</tooltip>
    </attr>

    <spacer orientation="v">
        <location col="0" row="9" colspan="4" />
    </spacer>
    
    <attr name="summary"> </attr>
    <attr name="fr"> </attr>
    <attr name="rules"> </attr>
    <attr name="background"> </attr>
    <attr name="height"> </attr>

</tag>
</TAGS>
