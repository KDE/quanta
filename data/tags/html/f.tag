<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
<tag name="fieldset" hasCore="1" hasI18n="1" hasScript="1">
<children>
<child name="a" />
<child name="abbr" />
<child name="acronym" />
<child name="address" />
<child name="attr" />
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
<child name="legend" />
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
<tag name="form" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="action" type="input">
        <text>Action</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" colspan="3" />
    </attr>

    <attr name="method" type="list">
        <text>Method</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1"/>
        <items>
                <item>GET</item>
                <item>POST</item>
        </items>
    </attr>

    <attr name="enctype" type="list">
        <text>Enctype</text>
        <textlocation col="0" row="2" />
        <location col="1" row="2" colspan="3"/>
        <items>
           <item>application/x-www-form-urlencoded</item>
           <item>multipart/form-data</item>
        </items>
    </attr>


    <attr name="target" type="list">
        <text>Target</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" />
        <items>
                <item>_blank</item>
                <item>_self</item>
                <item>_parent</item>
                <item>_top</item>
        </items>
    </attr>

    <attr name="accept-charset" type="input">
        <text>Accept charset</text>
        <textlocation col="2" row="3" />
        <location col="3" row="3"/>
    </attr>

    <attr name="name"> </attr>
    <attr name="onsubmit"> </attr>
    <attr name="onreset"> </attr>

<children>
<child name="address" />
<child name="blockquote" />
<child name="del" />
<child name="div" />
<child name="dl" />
<child name="fieldset" />
<child name="h1" />
<child name="h2" />
<child name="h3" />
<child name="h4" />
<child name="h5" />
<child name="h6" />
<child name="hr" />
<child name="ins" />
<child name="noscript" />
<child name="ol" />
<child name="p" />
<child name="pre" />
<child name="script" />
<child name="table" />
<child name="ul" />
</children>

</tag>
</TAGS>
