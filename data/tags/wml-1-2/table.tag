<!DOCTYPE TAGS>
<TAGS>
<tag name="table">

    <attr name="title" type="input">
        <text>Title</text>
        <textlocation col="0" row="0" />
        <tooltip>A label of the table</tooltip>
        <location col="1" row="0" />
    </attr>

    <attr name="align" type="list">
        <text>Align</text>
        <textlocation col="0" row="1" />
        <items>
            <item>left</item>
            <item>center</item>
            <item>right</item>
        </items>
        <location col="1" row="1" colspan="2" />
    </attr>

    <attr name="columns" type="input">
        <text>Number of columns</text>
        <textlocation col="0" row="2"  />
        <location col="1" row="2" />
        <tooltip>Must be greater than zero</tooltip>
    </attr>

    <spacer orientation="v">
        <location col="0" row="9" colspan="4" />
    </spacer>
    
</tag>
</TAGS>
