<!DOCTYPE TAGS>
<TAGS>
<tag name="img" hasCore="1" single="1">

    <attr name="alt" status="required" type="input">
        <text>Alt text</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0"/>
    </attr>

    <attr name="src" status="required" type="url">
        <text>URL</text> 
        <textlocation col="0" row="1" />
        <location col="1" row="1" colspan="3"/>
    </attr>

    <attr name="localsrc" status="implied" type="input">
        <text>Local icon name</text>
        <textlocation col="0" row="2" />
        <location col="1" row="2" />
    </attr>

    <attr name="vspace" defaultValue="0" type="input">
        <text>Vertical Space</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" />
    </attr>

    <attr name="hspace" defaultValue="0" type="input">
        <text>Horizontal Space</text>
        <textlocation col="0" row="4" />
        <location col="1" row="4" />
    </attr>

    <attr name="align" defaultValue="bottom" type="list">
        <text>Alignment to text</text>
        <textlocation col="0" row="5" />
        <location col="1" row="5"/>
        <items>
                <item>top</item>
                <item>middle</item>
                <item>bottom</item>
        </items>
    </attr>

    <attr name="height" status="implied" type="input">
        <text>Height</text>
        <textlocation col="0" row="6" />
        <location col="1" row="6"/>
    </attr>

    <attr name="width" status="implied" type="input">
        <text>Width</text>
        <textlocation col="0" row="7" />
        <location col="1" row="7"/>
    </attr>

    <attr name="xml:lang" status="implied" type="input">
        <text>Override xml:lang</text>
        <textlocation col="0" row="8" />
        <location col="1" row="8"/>
    </attr>

</tag>
</TAGS>
