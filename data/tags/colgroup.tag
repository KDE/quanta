<!DOCTYPE TAGS>
<TAGS>
<tag name="colgroup">

    <label>
        <text>Width </text>
        <location col="0" row="0" />
    </label>

    <attr name="width" type="input">
        <tooltip>use % for percent or it will set pixels
You can add multiple values with commas</tooltip>
        <location col="1" row="0" />
    </attr>

    <label>
        <text>Span </text>
        <location col="0" row="1" />
    </label>

    <attr name="span" type="list">
        <items>
            <item>1</item>
            <item>2</item>
            <item>3</item>
            <item>4</item>
            <item>5</item>
            <item>6</item>
            <item>7</item>
            <item>8</item>
            <item>9</item>
        </items>
        <location col="1" row="1" />
    </attr>
    
    <label>
        <text>Align</text>
        <location col="0" row="2"  />
    </label>

    <attr name="align" type="list">
        <items>
            <item>left</item>
            <item>center</item>
            <item>right</item>
        </items>
        <location col="1" row="2" />
    </attr>

    <label>
        <text>VAlign</text>
        <location col="0" row="3"  />
    </label>

    <attr name="valign" type="list">
        <items>
            <item>baseline</item>
            <item>bottom</item>
            <item>bottom</item>
            <item>center</item>
            <item>top</item>
        </items>
        <location col="1" row="3" colspan="1" />
    </attr>

    <label>
        <text>Character </text>
        <location col="0" row="4" />
    </label>

    <attr name="char" type="input">
        <tooltip>alignment character for cells</tooltip>
        <location col="1" row="4" />
    </attr>

    <label>
        <text>Character Offset </text>
        <location col="0" row="5" />
    </label>

    <attr name="charoff" type="input">
        <tooltip>alignment character offset</tooltip>
        <location col="1" row="5" />
    </attr>

    <spacer orientation="v">
        <location col="0" row="6" colspan="4" />
    </spacer>

</tag>
</TAGS>
