<!DOCTYPE TAGS>
<TAGS>
<tag name="colgroup" hasScript="1" hasCore="1" hasI18n="1" optional="1">

    <attr name="width" type="input">
        <text>Width</text>
        <textlocation col="0" row="0" />
        <tooltip>use % for percent or it will set pixels 
	You can add multiple values with commas</tooltip>
        <location col="1" row="0" />
    </attr>

    <attr name="span" type="list">
        <text>Span</text>
        <textlocation col="0" row="1" />
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

    <attr name="align" type="list">
        <text>Align</text>
        <textlocation col="0" row="2"  />
        <items>
            <item>left</item>
            <item>center</item>
            <item>right</item>
        </items>
        <location col="1" row="2" />
    </attr>

    <attr name="valign" type="list">
        <text>VAlign</text>
        <textlocation col="0" row="3"  />
        <items>
            <item>baseline</item>
            <item>bottom</item>
            <item>bottom</item>
            <item>center</item>
            <item>top</item>
        </items>
        <location col="1" row="3" colspan="1" />
    </attr>
    
    <attr name="char" type="input">
        <text>Character</text>
        <textlocation col="0" row="4" />
        <tooltip>alignment character for cells</tooltip>
        <location col="1" row="4" />
    </attr>

    <attr name="charoff" type="input">
        <text>Character offset</text>
        <textlocation col="0" row="5" />
        <tooltip>alignment character offset</tooltip>
        <location col="1" row="5" />
    </attr>

    <spacer orientation="v">
        <location col="0" row="6" colspan="4" />
    </spacer>

</tag>
</TAGS>
