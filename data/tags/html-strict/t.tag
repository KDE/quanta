<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
<tag name="table" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="width" type="input">
        <text>Width</text>
        <textlocation col="0" row="0" />
        <tooltip>use % for percent or it will set pixels</tooltip>
        <location col="1" row="0" />
    </attr>

    <attr name="cellspacing" type="input">
        <text>Cell spacing</text>
        <textlocation col="2" row="0" />
        <location col="3" row="0" />
    </attr>

    <attr name="border" type="input">
        <text>Border</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1" />
    </attr>

    <attr name="cellpadding" type="input">
        <text>Cell padding</text>
        <textlocation col="2" row="1" />
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
        <location col="1" row="2" colspan="2" />
    </attr>

    <attr name="bgcolor" type="color">
        <text>Background color</text>
        <textlocation col="0" row="3"  />
        <location col="1" row="3" colspan="2" />
        <tooltip>Depricated in HTML 4.0</tooltip>
    </attr>

    <attr name="frame" type="list">
        <text>Frame</text>
        <textlocation col="0" row="5"  />
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


    <attr name="summary"> </attr>
    <attr name="fr"> </attr>
    <attr name="rules"> </attr>
    <attr name="background"> </attr>
    <attr name="height"> </attr>
<children>
<child name="caption" />
<child name="col" />
<child name="colgroup" />
<child name="thead" />
<child name="tfoot" />
<child name="tbody" />
<child name="tr" />
</children>

</tag>
<tag name="tbody" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="align" type="list">
        <text>Align</text>
        <textlocation col="0" row="0" />
        <items>
            <item>left</item>
            <item>center</item>
            <item>right</item>
        </items>
        <location col="1" row="0" colspan="1" />
    </attr>

    <attr name="valign" type="list">
        <text>VAlign</text>
        <textlocation col="0" row="1"  />
        <items>
            <item>baseline</item>
            <item>bottom</item>
            <item>bottom</item>
            <item>center</item>
            <item>top</item>
        </items>
        <location col="1" row="1" colspan="1" />
    </attr>

    <attr name="bgcolor" type="color">
        <text>Bg color</text>
        <textlocation col="0" row="5" />
        <location col="1" row="5" colspan="1" />
        <tooltip>Depricated in HTML 4.0</tooltip>
    </attr>


    <attr name="char"> </attr>
    <attr name="charoff"> </attr>

<children>
<child name="tr" />
</children>

</tag>
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

    <attr name="headers"> </attr>
    <attr name="abbr"> </attr>
    <attr name="scope"> </attr>
    <attr name="axis"> </attr>
    <attr name="char"> </attr>
    <attr name="charoff"> </attr>
    <attr name="background"> </attr>

    <stoppingtags>
        <stoppingtag name="td" />
        <stoppingtag name="tr" />
        <stoppingtag name="th" />
    </stoppingtags>
<children>
<child name="a" />
<child name="abbr" />
<child name="acronym" />
<child name="address" />
<child name="b" />
<child name="bdo" />
<child name="big" />
<child name="blockquote" />
<child name="br" />
<child name="button" />
<child name="cite" />
<child name="code" />
<child name="del" />
<child name="dfn" />
<child name="div" />
<child name="dl" />
<child name="em" />
<child name="fieldset" />
<child name="form" />
<child name="h1" />
<child name="h2" />
<child name="h3" />
<child name="h4" />
<child name="h5" />
<child name="h6" />
<child name="hr" />
<child name="i" />
<child name="img" />
<child name="input" />
<child name="ins" />
<child name="kbd" />
<child name="label" />
<child name="map" />
<child name="noscript" />
<child name="object" />
<child name="ol" />
<child name="p" />
<child name="pre" />
<child name="q" />
<child name="samp" />
<child name="script" />
<child name="select" />
<child name="small" />
<child name="span" />
<child name="strong" />
<child name="sub" />
<child name="sup" />
<child name="table" />
<child name="textarea" />
<child name="tt" />
<child name="ul" />
<child name="var" />
</children>
</tag>
<tag name="textarea" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="name" type="input">
      <text>Name</text>
      <textlocation col="0" row="0" colspan="1" />
      <location col="1" row="0" colspan="4" />
    </attr>

    <attr name="cols" type="input">
      <text>Columns</text>
      <textlocation col="0" row="1" colspan="1" />
      <location col="1" row="1" colspan="1" />
    </attr>

    <attr name="rows" type="input">
      <text>Rows</text>
      <textlocation col="2" row="1" colspan="1" />
      <location col="3" row="1" colspan="1" />
    </attr>

    <attr name="tabindex" type="input">
      <text>Tab index</text>
      <textlocation col="0" row="2" colspan="1" />
      <location col="1" row="2" colspan="1" />
    </attr>

    <attr name="disabled" type="check">
      <text>Disabled</text>
      <location col="0" row="4" colspan="2" />
    </attr>

    <attr name="readonly" type="check">
      <text>Read only</text>
      <location col="0" row="5" colspan="2" />
    </attr>

    <attr name="accesskey"> </attr>
    <attr name="tab"> </attr>
    <attr name="onfocus"> </attr>
    <attr name="onblur"> </attr>
    <attr name="onselect"> </attr>
    <attr name="onchange"> </attr>


</tag>
<tag name="tfoot" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="align" type="list">
        <text>Align</text>
        <textlocation col="0" row="0" />
        <items>
            <item>left</item>
            <item>center</item>
            <item>right</item>
        </items>
        <location col="1" row="0" colspan="1" />
    </attr>

    <attr name="valign" type="list">
        <text>VAlign</text>
        <textlocation col="0" row="1"  />
        <items>
            <item>baseline</item>
            <item>bottom</item>
            <item>bottom</item>
            <item>center</item>
            <item>top</item>
        </items>
        <location col="1" row="1" colspan="1" />
    </attr>

    <attr name="bgcolor" type="color">
        <text>Bg color</text>
        <textlocation col="0" row="5" />
        <location col="1" row="5" colspan="1" />
        <tooltip>Depricated in HTML 4.0</tooltip>
    </attr>

    <attr name="char"> </attr>
    <attr name="charoff"> </attr>
<children>
<child name="tr" />
</children>
</tag>
<tag name="th" hasScript="1" hasCore="1" hasI18n="1" optional="1">

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


    <attr name="headers"> </attr>
    <attr name="abbr"> </attr>
    <attr name="scope"> </attr>
    <attr name="axis"> </attr>
    <attr name="char"> </attr>
    <attr name="charoff"> </attr>
	<stoppingtags>
		<stoppingtag name="td" />
		<stoppingtag name="tr" />
		<stoppingtag name="th" />
	</stoppingtags>
<children>
<child name="a" />
<child name="abbr" />
<child name="acronym" />
<child name="address" />
<child name="b" />
<child name="bdo" />
<child name="big" />
<child name="blockquote" />
<child name="br" />
<child name="button" />
<child name="cite" />
<child name="code" />
<child name="del" />
<child name="dfn" />
<child name="div" />
<child name="dl" />
<child name="em" />
<child name="fieldset" />
<child name="form" />
<child name="h1" />
<child name="h2" />
<child name="h3" />
<child name="h4" />
<child name="h5" />
<child name="h6" />
<child name="hr" />
<child name="i" />
<child name="img" />
<child name="input" />
<child name="ins" />
<child name="kbd" />
<child name="label" />
<child name="map" />
<child name="noscript" />
<child name="object" />
<child name="ol" />
<child name="p" />
<child name="pre" />
<child name="q" />
<child name="samp" />
<child name="script" />
<child name="select" />
<child name="small" />
<child name="span" />
<child name="strong" />
<child name="sub" />
<child name="sup" />
<child name="table" />
<child name="textarea" />
<child name="tt" />
<child name="ul" />
<child name="var" />
</children>

</tag>
<tag name="thead" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="align" type="list">
        <text>Align</text>
        <textlocation col="0" row="0" />
        <items>
            <item>left</item>
            <item>center</item>
            <item>right</item>
        </items>
        <location col="1" row="0" colspan="1" />
    </attr>

    <attr name="valign" type="list">
        <text>VAlign</text>
        <textlocation col="0" row="1"  />
        <items>
            <item>baseline</item>
            <item>bottom</item>
            <item>bottom</item>
            <item>center</item>
            <item>top</item>
        </items>
        <location col="1" row="1" colspan="1" />
    </attr>

    <attr name="bgcolor" type="color">
        <text>Bg color</text>
        <textlocation col="0" row="5" />
        <location col="1" row="5" colspan="1" />
        <tooltip>Depricated in HTML 4.0</tooltip>
    </attr>

    <attr name="char"> </attr>
    <attr name="charoff"> </attr>
<children>
<child name="tr" />
</children>
</tag>
<tag name="title" hasI18n="1" />
<tag name="tr" hasScript="1" hasCore="1" hasI18n="1" optional="1">

    <attr name="align" type="list">
        <text>Align</text>
        <textlocation col="0" row="0" />
        <items>
            <item>left</item>
            <item>center</item>
            <item>right</item>
        </items>
        <location col="1" row="0" colspan="1" />
    </attr>

    <attr name="valign" type="list">
        <text>VAlign</text>
        <textlocation col="0" row="1"  />
        <items>
            <item>baseline</item>
            <item>bottom</item>
            <item>bottom</item>
            <item>center</item>
            <item>top</item>
        </items>
        <location col="1" row="1" colspan="1" />
    </attr>

    <attr name="bgcolor" type="color">
        <text>Bg color</text>
        <textlocation col="0" row="5" />
        <location col="1" row="5" colspan="1" />
        <tooltip>Depricated in HTML 4.0</tooltip>
    </attr>

    <attr name="char"> </attr>
    <attr name="charoff"> </attr>

    <stoppingtags>
        <stoppingtag name="tr" />
        <stoppingtag name="tfoot" />
    </stoppingtags>
<children>
<child name="th" />
<child name="td" />
</children>
</tag>
<tag name="tt" hasCore="1" hasI18n="1" hasScript="1">
<children>
<child name="a" />
<child name="abbr" />
<child name="acronym" />
<child name="b" />
<child name="bdo" />
<child name="big" />
<child name="br" />
<child name="button" />
<child name="cite" />
<child name="code" />
<child name="del" />
<child name="dfn" />
<child name="em" />
<child name="i" />
<child name="img" />
<child name="input" />
<child name="ins" />
<child name="kbd" />
<child name="label" />
<child name="map" />
<child name="object" />
<child name="q" />
<child name="samp" />
<child name="script" />
<child name="select" />
<child name="small" />
<child name="span" />
<child name="strong" />
<child name="sub" />
<child name="sup" />
<child name="textarea" />
<child name="tt" />
<child name="var" />
</children>
</tag>
</TAGS>
