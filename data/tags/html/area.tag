<!DOCTYPE TAGS>
<TAGS>
<tag name="area">

    <label>
        <text>Shape</text>
        <location col="0" row="0" />
    </label>

    <label>
        <text>Coords</text>
        <location col="0" row="1" />
    </label>

    <label>
        <text>HREF</text>
        <location col="0" row="3" />
    </label>

    <label>
        <text>Target</text>
        <location col="0" row="4" />
    </label>

    <label>
        <text>No HREF</text>
        <location col="0" row="5" />
    </label>

    <label>
        <text>Alt</text>
        <location col="0" row="6" />
    </label>

    <label>
        <text>Tab index</text>
        <location col="0" row="7" />
    </label>

    <label>
        <text>OnFocus</text>
        <location col="0" row="8" />
    </label>

    <label>
        <text>OnBlur</text>
        <location col="0" row="9" />
    </label>



    <attr name="shape" type="list">
        <location col="1" row="0" colspan="2"/>
        <items>
                <item>rect</item>
                <item>circle</item>
                <item>poly</item>
                <item>default</item>
        </items>
    </attr>

    <attr name="coords" type="text">
        <location col="1" row="1"  colspan="2"/>
    </attr>

    <attr name="href" type="url">
        <location col="1" row="3"  colspan="2"/>
    </attr>

    <attr name="target" type="list">
        <location col="1" row="4"  colspan="2"/>
        <items>
                <item>_blank</item>
                <item>_self</item>
                <item>_parent</item>
                <item>_top</item>
        </items>
    </attr>

    <attr name="nohref" type="check">
        <text>No HREF</text>
        <location col="1" row="5" colspan="2"/>
    </attr>

    <attr name="alt" type="input">
        <location col="1" row="6" colspan="2"/>
    </attr>

    <attr name="tabindex" type="input">
        <location col="1" row="7" colspan="2"/>
    </attr>

    <attr name="onfocus" type="input">
        <location col="1" row="8" colspan="2"/>
    </attr>

    <attr name="onblur" type="input">
        <location col="1" row="9" colspan="2"/>
    </attr>

</tag>
</TAGS>
