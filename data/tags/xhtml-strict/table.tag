<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
<tag name="table" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="width" type="input">
        <text>width</text>
        <textlocation col="0" row="0" />
        <tooltip>use % for percent or it will set pixels</tooltip>
        <location col="0" row="0" />
    </attr>

    <attr name="cellspacing" type="input">
        <text>cellspacing</text>
        <textlocation col="0" row="1" />
        <location col="0" row="1" />
    </attr>

    <attr name="border" type="input">
        <text>border</text>
        <textlocation col="0" row="2" />
        <location col="0" row="2" />
    </attr>

    <attr name="cellpadding" type="input">
        <text>cellpadding</text>
        <textlocation col="0" row="3" />
        <location col="0" row="3" />
    </attr>

    <attr name="frame" type="list">
        <text>frame</text>
        <textlocation col="0" row="4"  />
        <items>
          <item>void</item>
          <item>above</item>
          <item>below</item>
          <item>hsides</item>
          <item>lhs</item>
          <item>rhs</item>
          <item>vsides</item>
          <item>box</item>
          <item>border</item>
        </items>
        <location col="0" row="4" colspan="2" />
    </attr>

    <attr name="summary" type="input" />

    <attr name="rules" type="list">
        <text>rules</text>
        <textlocation col="0" row="5"  />
        <items>
            <item>none</item>
            <item>groups</item>
            <item>rows</item>
            <item>cols</item>
            <item>all</item>
        </items>
        <location col="0" row="5" colspan="2" />
    </attr>

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
</TAGS>
