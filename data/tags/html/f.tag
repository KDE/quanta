<!DOCTYPE TAGS>
<TAGS>
<tag name="fieldset" hasCore="1" hasI18n="1" hasScript="1">
	<kafkainfos>
		<rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="no" cursorcanenter="true">
		</rootnode>
	</kafkainfos>
</tag>
<tag name="font" hasCore="1" hasI18n="1">

    <attr name="color" type="color">
        <text>Color</text>
        <textlocation col="0" row="2" />
        <tooltip>Font color</tooltip>
        <location col="1" row="2"  />
    </attr>

    <attr name="face" type="input">
        <text>Face</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" colspan="2" />
    </attr>

    <attr name="size" type="list">
        <text>Size</text>
        <textlocation col="0" row="4" />
        <location col="1" row="4"/>
        <items>
                <item>+1</item>
                <item>-1</item>
                <item>+2</item>
                <item>-2</item>
                <item>+3</item>
                <item>-3</item>
                <item>+4</item>
                <item>-4</item>
        </items>
    </attr>

    <spacer orientation="h">
        <location col="0" row="5" />
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

    <kafkainfos>
        <rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="no" cursorcanenter="true">
        </rootnode>
    </kafkainfos>

</tag>
<tag name="frame" hasCore="1" single="1">

    <attr name="name" type="input">
        <text>Name</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" colspan="2"/>
    </attr>

    <attr name="src" type="url">
        <text>Src</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1"  colspan="2"/>
    </attr>

    <attr name="longdesc" type="url">
        <text>Long desc</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3"  colspan="2"/>
    </attr>

    <attr name="frameborder" type="list">
        <text>Frame border</text>
        <textlocation col="0" row="4" />
        <location col="1" row="4"  colspan="2"/>
        <items>
                <item>yes</item>
                <item>no</item>
        </items>
    </attr>

    <attr name="marginwidth" type="input">
        <text>Margin width</text>
        <textlocation col="0" row="5" />
        <location col="1" row="5" colspan="2"/>
    </attr>

    <attr name="marginheight" type="input">
        <text>Margin height</text>
        <textlocation col="0" row="6" />
        <location col="1" row="6" colspan="2"/>
    </attr>

    <attr name="noresize" type="check">
        <text>No resize</text>
        <location col="1" row="7" colspan="2"/>
    </attr>

    <attr name="scrolling" type="list">
        <text>Scrolling</text>
        <textlocation col="0" row="8" />
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

    <kafkainfos>
        <rootnode canbedeleted="false" canbemodified="false" canhavecursorfocus="no" cursorcanenter="false">
        </rootnode>
    </kafkainfos>


</tag>
<tag name="frameset" hasCore="1">

   <attr name="rows" type="input">
        <text>Rows</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" colspan="2"/>
    </attr>

    <attr name="cols" type="input">
        <text>Cols</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1"  colspan="2"/>
    </attr>

    <attr name="onload" type="input">
        <text>On load</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3"  colspan="2"/>
    </attr>

    <attr name="onunload" type="input">
        <text>On unload</text>
        <textlocation col="0" row="4" />
        <location col="1" row="4"  colspan="2"/>
    </attr>

    <attr name="border"> </attr>
    <attr name="frameborder"> </attr>
    <attr name="marginwidth"> </attr>
    <attr name="marginheight"> </attr>
    <attr name="framespacing"> </attr>

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

    <kafkainfos>
        <rootnode canbedeleted="false" canbemodified="false" canhavecursorfocus="no" cursorcanenter="false">
        </rootnode>
    </kafkainfos>

</tag>
</TAGS>