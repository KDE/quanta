<!DOCTYPE TAGS>
<TAGS>
<tag name="frame">

    <label>
        <text>Name</text>
        <location col="0" row="0" />
    </label>

    <label>
        <text>Src</text>
        <location col="0" row="1" />
    </label>

    <label>
        <text>Long desc</text>
        <location col="0" row="3" />
    </label>

    <label>
        <text>Frame border</text>
        <location col="0" row="4" />
    </label>

    <label>
        <text>Margin width</text>
        <location col="0" row="5" />
    </label>

    <label>
        <text>Margin height</text>
        <location col="0" row="6" />
    </label>

    <label>
        <text><!-- No Resize --> </text>
        <location col="0" row="7" />
    </label>

    <label>
        <text>Scrolling</text>
        <location col="0" row="8" />
    </label>



    <attr name="name" type="input">
        <location col="1" row="0" colspan="2"/>
    </attr>

    <attr name="src" type="url">
        <location col="1" row="1"  colspan="2"/>
    </attr>

    <attr name="longdesc" type="url">
        <location col="1" row="3"  colspan="2"/>
    </attr>

    <attr name="frameborder" type="list">
        <location col="1" row="4"  colspan="2"/>
        <items>
                <item>yes</item>
                <item>no</item>
        </items>
    </attr>

    <attr name="marginwidth" type="input">
        <location col="1" row="5" colspan="2"/>
    </attr>

    <attr name="marginheight" type="input">
        <location col="1" row="6" colspan="2"/>
    </attr>

    <attr name="noresize" type="check">
        <text>No resize</text>
        <location col="1" row="7" colspan="2"/>
    </attr>

    <attr name="scrolling" type="list">
        <location col="1" row="8" colspan="2"/>
        <items>
                <item>auto</item>
                <item>yes</item>
                <item>no</item>
        </items>
    </attr>

    <spacer orientation="h">
        <location col="0" row="8" />
    </spacer>

    <spacer orientation="h">
        <location col="1" row="8" />
    </spacer>

    <spacer orientation="h">
        <location col="2" row="8" />
    </spacer>

    <spacer orientation="v">
        <location col="0" row="9" colspan="3" />
    </spacer>

</tag>
</TAGS>
