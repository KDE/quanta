<!DOCTYPE TAGS>
<TAGS>
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

    <spacer orientation="v">
        <location col="0" row="5" colspan="4" />
    </spacer>
    
    <attr name="bgcolor" type="color">
        <text>Bg color</text>
        <textlocation col="0" row="5" />
        <location col="1" row="5" colspan="1" />
        <tooltip>Depricated in HTML 4.0</tooltip>
    </attr>


    <spacer orientation="v">
        <location col="0" row="9" colspan="4" />
    </spacer>
   
    <attr name="char"> </attr>
    <attr name="charoff"> </attr>

    <stoppingtags>
        <stoppingtag name="tr" />
        <stoppingtag name="tfoot" />
    </stoppingtags>
</tag>
</TAGS>
