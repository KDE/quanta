<!DOCTYPE TAGS>
<TAGS>
<tag name="td">

    <label>
        <text>Width </text>
        <location col="0" row="0" />
    </label>

    <attr name="width" type="input">
        <tooltip>use % for percent or it will set pixels</tooltip>
        <location col="1" row="0" />
    </attr>

    <label>
        <text>Col Span </text>
        <location col="2" row="0" />
    </label>

    <attr name="colspan" type="list">
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
        <location col="3" row="0" />
    </attr>

    <label>
        <text>Height </text>
        <location col="0" row="1" />
    </label>

    <attr name="height" type="input">
        <tooltip>Depricated in HTML 4.0 - not used in IE 5</tooltip>
        <location col="1" row="1" />
    </attr>

    <label>
        <text>Row Span </text>
        <location col="2" row="1" />
    </label>

    <attr name="rowspan" type="list">
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
        <location col="1" row="2" colspan="1" />
    </attr>

    <label>
        <text>VAlign</text>
        <location col="2" row="2"  />
    </label>

    <attr name="valign" type="list">
        <items>
            <item>baseline</item>
            <item>bottom</item>
            <item>bottom</item>
            <item>center</item>
            <item>top</item>
        </items>
        <location col="3" row="2" colspan="1" />
    </attr>

    <spacer orientation="v">
        <location col="0" row="5" colspan="4" />
    </spacer>

    <label>
        <text>Bg Color</text>
        <location col="0" row="5"  />
    </label>

    <attr name="bgcolor" type="color">
        <location col="1" row="5" colspan="1" />
        <tooltip>Depricated in HTML 4.0</tooltip>
    </attr>

    <attr name="nowrap" type="check">
        <text>NoWrap</text>
        <location col="0" row="6" colspan="2" />
        <tooltip>Depricated in HTML 4.0</tooltip>
    </attr>

    <spacer orientation="v">
        <location col="0" row="9" colspan="4" />
    </spacer>



</tag>
</TAGS>
