<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
<tag name="caption" hasCore="1" hasI18n="1" hasScript="1">
<children>
<child name="#text" />
 <child name="tt" />
 <child name="i" />
 <child name="b" />
 <child name="big" />
 <child name="small" />
  <child name="em" />
 <child name="strong" />
 <child name="dfn" />
 <child name="code" />
 <child name="samp" />
 <child name="kbd" />
 <child name="var" />
 <child name="cite" />
 <child name="abbr" />
 <child name="acronym" />
  <child name="a" />
 <child name="img" />
 <child name="object" />
 <child name="br" />
 <child name="script" />
 <child name="map" />
 <child name="q" />
 <child name="sub" />
 <child name="sup" />
 <child name="span" />
 <child name="bdo" />
  <child name="input" />
 <child name="select" />
 <child name="textarea" />
 <child name="label" />
 <child name="button" />
</children>
</tag>

<tag name="cite" hasCore="1" hasI18n="1" hasScript="1">
<children>
<child name="#text" />
 <child name="tt" />
 <child name="i" />
 <child name="b" />
 <child name="big" />
 <child name="small" />
  <child name="em" />
 <child name="strong" />
 <child name="dfn" />
 <child name="code" />
 <child name="samp" />
 <child name="kbd" />
 <child name="var" />
 <child name="cite" />
 <child name="abbr" />
 <child name="acronym" />
  <child name="a" />
 <child name="img" />
 <child name="object" />
 <child name="br" />
 <child name="script" />
 <child name="map" />
 <child name="q" />
 <child name="sub" />
 <child name="sup" />
 <child name="span" />
 <child name="bdo" />
  <child name="input" />
 <child name="select" />
 <child name="textarea" />
 <child name="label" />
 <child name="button" />
</children>
</tag>

<tag name="code" hasCore="1" hasI18n="1" hasScript="1">
<children>
<child name="#text" />
 <child name="tt" />
 <child name="i" />
 <child name="b" />
 <child name="big" />
 <child name="small" />
  <child name="em" />
 <child name="strong" />
 <child name="dfn" />
 <child name="code" />
 <child name="samp" />
 <child name="kbd" />
 <child name="var" />
 <child name="cite" />
 <child name="abbr" />
 <child name="acronym" />
  <child name="a" />
 <child name="img" />
 <child name="object" />
 <child name="br" />
 <child name="script" />
 <child name="map" />
 <child name="q" />
 <child name="sub" />
 <child name="sup" />
 <child name="span" />
 <child name="bdo" />
  <child name="input" />
 <child name="select" />
 <child name="textarea" />
 <child name="label" />
 <child name="button" />
</children>
</tag>

<tag name="col" hasScript="1" hasCore="1" hasI18n="1" single="1">

    <attr name="width" type="input">
        <text>Width</text>
        <textlocation col="0" row="0" />
        <tooltip>use % for percent or it will set pixels</tooltip>
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
            <item>center</item>
            <item>top</item>
        </items>
        <location col="1" row="3" />
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


</tag>

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
<children>
<child name="col" usage="required" />
</children>
    <stoppingtags>
        <stoppingtag name="colgroup" />
        <stoppingtag name="tbody" />
        <stoppingtag name="thead" />
        <stoppingtag name="tfoot" />
        <stoppingtag name="tr" />
    </stoppingtags>

</tag>
</TAGS>
