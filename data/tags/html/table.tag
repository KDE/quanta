<!DOCTYPE TAGS>
<TAGS>
<tag name="table">

    <label>
        <text>Width </text>
        <location col="0" row="0" />
    </label>

    <attr name="width" type="input">
        <tooltip>use % for percent or it will set pixels</tooltip>
        <location col="1" row="0" />
    </attr>

    <label>
        <text>Cell Spacing </text>
        <location col="2" row="0" />
    </label>

    <attr name="cellspacing" type="input">
        <location col="3" row="0" />
    </attr>

    <label>
        <text>Border </text>
        <location col="0" row="1" />
    </label>

    <attr name="border" type="input">
        <location col="1" row="1" />
    </attr>

    <label>
        <text>Cell Padding </text>
        <location col="2" row="1" />
    </label>

    <attr name="cellpadding" type="input">
        <location col="3" row="1" />
    </attr>

    <label>
        <text>Align </text>
        <location col="0" row="2" />
    </label>

    <attr name="align" type="list">
        <items>
            <item>left</item>
            <item>center</item>
            <item>right</item>
        </items>
        <location col="1" row="2" colspan="2" />
    </attr>

    <label>
        <text>Background Color</text>
        <location col="0" row="3"  />
    </label>

    <attr name="bgcolor" type="color">
        <location col="1" row="3" colspan="2" />
        <tooltip>Depricated in HTML 4.0</tooltip>
    </attr>

    <spacer orientation="v">
        <location col="0" row="4" colspan="4" />
    </spacer>

    <label>
        <text>Frame</text>
        <location col="0" row="5"  />
    </label>

    <attr name="frame" type="list">
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



</tag>
</TAGS>
