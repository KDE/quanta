<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
<tag name="td" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="rowspan" type="input">
        <text>Rowspan</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" />
    </attr>

    <attr name="colspan" type="input">
        <text>Colspan</text>
        <textlocation col="2" row="0" />
        <location col="3" row="0" />
    </attr>

    <attr name="align" type="list">
        <text>Align</text>
        <textlocation col="0" row="1" />
        <items>
            <item>left</item>
            <item>center</item>
            <item>right</item>
            <item>justify</item>
            <item>char</item>
        </items>
        <location col="1" row="1" colspan="1" />
    </attr>

    <attr name="valign" type="list">
        <text>Valign</text>
        <textlocation col="2" row="1"  />
        <items>
            <item>baseline</item>
            <item>bottom</item>
            <item>middle</item>
            <item>top</item>
        </items>
        <location col="3" row="1" colspan="1" />
    </attr>

    <attr name="headers" type="input">
        <text>Headers</text>
        <textlocation col="0" row="2" />
        <location col="1" row="2" colspan="3"/>
    </attr>

    <attr name="abbr" type="input">
        <text>Abbr</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" colspan="3"/>
    </attr>

    <attr name="scope" type="list">
        <text>Scope</text>
        <textlocation col="0" row="4" />
        <location col="1" row="4" colspan="3"/>
        <items>
            <item>row</item>
            <item>col</item>
            <item>rowgroup</item>
            <item>colgroup</item>
        </items>
    </attr>

    <attr name="axis" type="input">
        <text>Axis</text>
        <textlocation col="0" row="5" />
        <location col="1" row="5" colspan="3"/>
    </attr>

    <attr name="char" type="input">
        <text>Char</text>
        <textlocation col="0" row="6" />
        <location col="1" row="6" />
    </attr>

    <attr name="charoff" type="input">
        <text>Charoff</text>
        <textlocation col="0" row="7" />
        <location col="1" row="7" />
    </attr>

    <attr name="height" type="input">
        <text>Height</text>
        <textlocation col="2" row="6" />
        <tooltip>Deprecated</tooltip>
        <location col="3" row="6" />
    </attr>

    <attr name="width" type="input">
        <text>Width</text>
        <textlocation col="2" row="7" />
        <tooltip>use % for percent or it will set pixels</tooltip>
        <location col="3" row="7" />
    </attr>

    <attr name="bgcolor" type="color">
        <text>Bg color</text>
        <textlocation col="0" row="8"  />
        <location col="1" row="8" colspan="3" />
        <tooltip>Deprecated</tooltip>
    </attr>

    <attr name="nowrap" type="check">
        <text>NoWrap</text>
        <location col="1" row="9" colspan="2" />
        <tooltip>Deprecated</tooltip>
    </attr>

    <stoppingtags>
        <stoppingtag name="td" />
        <stoppingtag name="tr" />
        <stoppingtag name="th" />
    </stoppingtags>

<children>
<child name="#text" />
<child name="a" />
<child name="abbr" />
<child name="acronym" />
<child name="address" />
<child name="applet" />
<child name="b" />
<child name="basefont" />
<child name="bdo" />
<child name="big" />
<child name="blockquote" />
<child name="br" />
<child name="button" />
<child name="center" />
<child name="cite" />
<child name="code" />
<child name="del" />
<child name="dfn" />
<child name="dir" />
<child name="div" />
<child name="dl" />
<child name="em" />
<child name="fieldset" />
<child name="font" />
<child name="form" />
<child name="h1" />
<child name="h2" />
<child name="h3" />
<child name="h4" />
<child name="h5" />
<child name="h6" />
<child name="hr" />
<child name="i" />
<child name="iframe" />
<child name="img" />
<child name="input" />
<child name="ins" />
<child name="isindex" />
<child name="kbd" />
<child name="label" />
<child name="map" />
<child name="menu" />
<child name="noframes" />
<child name="noscript" />
<child name="object" />
<child name="ol" />
<child name="p" />
<child name="pre" />
<child name="q" />
<child name="s" />
<child name="samp" />
<child name="script" />
<child name="select" />
<child name="small" />
<child name="span" />
<child name="strike" />
<child name="strong" />
<child name="sub" />
<child name="sup" />
<child name="table" />
<child name="textarea" />
<child name="tt" />
<child name="u" />
<child name="ul" />
<child name="var" />
</children>

</tag>
</TAGS>
