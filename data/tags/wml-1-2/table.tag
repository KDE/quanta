<!DOCTYPE TAGS>
<TAGS>
<tag name="table" hasCore="1">

    <attr name="title" status="implied" type="input">
        <text>Title</text>
        <textlocation col="0" row="0" />
        <tooltip>A label of the table</tooltip>
        <location col="1" row="0" />
    </attr>

    <attr name="align" status="implied" type="list">
        <text>Align</text>
        <textlocation col="0" row="1" />
        <items>
            <item>left</item>
            <item>center</item>
            <item>right</item>
        </items>
        <location col="1" row="1" colspan="2" />
    </attr>

    <attr name="columns" status="required" type="input">
        <text>Number of columns</text>
        <textlocation col="0" row="2"  />
        <location col="1" row="2" />
        <tooltip>Must be greater than zero</tooltip>
    </attr>
    
    <attr name="xml:lang" status="implied" type="input">
        <text>Override xml:lang</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" />
    </attr>

</tag>
</TAGS>
