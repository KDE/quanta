<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
<tag name="colgroup" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="width" type="input">
        <text>width</text>
        <textlocation col="0" row="0" />
        <tooltip>Use % for percent or it will set pixels
You can add multiple values with commas</tooltip>
        <location col="1" row="0" />
    </attr>

    <attr name="span" type="input">
        <text>span</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1" />
    </attr>

    <attr name="align" type="list">
        <text>align</text>
        <textlocation col="0" row="2"  />
        <items>
            <item>left</item>
            <item>center</item>
            <item>right</item>
            <item>justify</item>
            <item>char</item>
        </items>
        <location col="1" row="2" />
    </attr>

    <attr name="valign" type="list">
        <text>valign</text>
        <textlocation col="0" row="3"  />
        <items>
            <item>baseline</item>
            <item>bottom</item>
            <item>middle</item>
            <item>top</item>
        </items>
        <location col="1" row="3" colspan="1" />
    </attr>

    <attr name="char" type="input">
        <text>char</text>
        <textlocation col="0" row="4" />
        <tooltip>alignment character for cells</tooltip>
        <location col="1" row="4" />
    </attr>

    <attr name="charoff" type="input">
        <text>charoff</text>
        <textlocation col="0" row="5" />
        <tooltip>alignment character offset</tooltip>
        <location col="1" row="5" />
    </attr>
<children>
<child name="col" usage="required" />
</children>

</tag>
</TAGS>
