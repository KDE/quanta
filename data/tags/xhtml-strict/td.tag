<!DOCTYPE TAGS>
<TAGS>
<tag name="td" hasScript="1" hasCore="1" hasI18n="1" optional="1">

    <attr name="width" type="input">
        <text>Width</text>
        <textlocation col="0" row="0" />
        <tooltip>use % for percent or it will set pixels</tooltip>
        <location col="1" row="0" />
    </attr>

    <attr name="colspan" type="list">
        <text>Col span</text>
        <textlocation col="2" row="0" />
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

    <attr name="height" type="input">
        <text>Height</text>
        <textlocation col="0" row="1" />
        <tooltip>Deprecated in HTML 4.0 - not used in IE 5</tooltip>
        <location col="1" row="1" />
    </attr>

    <attr name="rowspan" type="list">
        <text>Row span</text>
        <textlocation col="2" row="1" />
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

    <attr name="align" type="list">
        <text>Align</text>
        <textlocation col="0" row="2" />
        <items>
            <item>left</item>
            <item>center</item>
            <item>right</item>
        </items>
        <location col="1" row="2" colspan="1" />
    </attr>

    <attr name="valign" type="list">
        <text>VAlign</text>
        <textlocation col="2" row="2"  />
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

    <attr name="bgcolor" type="color">
        <text>Bg color</text>
        <textlocation col="0" row="5"  />
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
    
    <attr name="headers"> </attr>
    <attr name="abbr"> </attr>
    <attr name="scope"> </attr>
    <attr name="axis"> </attr>
    <attr name="char"> </attr>
    <attr name="charoff"> </attr>
    <attr name="background"> </attr>

    <stoppingtags>
        <stoppingtag>td</stoppingtag>
        <stoppingtag>tr</stoppingtag>
        <stoppingtag>th</stoppingtag>
    </stoppingtags>

</tag>
</TAGS>
