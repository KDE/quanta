<!DOCTYPE TAGS>
<TAGS>
<tag name="b" hasCore="1" hasI18n="1" hasScript="1">
	<kafkainfos>
		<rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="no" cursorcanenter="true">
		</rootnode>
	</kafkainfos>
</tag>
<tag name="base" single="1">
    <attr name="href" type="input">
        <text>HREF</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" colspan="2" />
    </attr>

    <attr name="target" type="list">
        <text>Target</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1" colspan="2" />
        <items>
                <item>_blank</item>
                <item>_self</item>
                <item>_parent</item>
                <item>_top</item>
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
<tag name="bdo" hasCore="1">
	<attr name="dir" type="input">
		<text>Dir</text>
		<textlocation col="0" row="0" />
		<location col="1" row="0" colspan="2" />
	</attr>
	<attr name="lang" type="input">
		<text>Lang</text>
		<textlocation col="0" row="1" />
		<location col="1" row="1" colspan="2" />
	</attr>

	<spacer orientation="v">
		<location col="0" row="11" colspan="2" />
	</spacer>

	<kafkainfos>
		<rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="no" cursorcanenter="true">
		</rootnode>
	</kafkainfos>
</tag>
<tag name="big" hasCore="1" hasI18n="1" hasScript="1">
	<kafkainfos>
		<rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="no" cursorcanenter="true">
		</rootnode>
	</kafkainfos>
</tag>
<tag name="blockquote" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="cite" type="url">
        <text>Cite</text>
        <textlocation col="0" row="0" />
        <tooltip>This is not widely supported</tooltip>
        <location col="1" row="0" colspan="2" />
    </attr>

    <attr name="dir" type="list">
        <text>Dir</text>
        <textlocation col="0" row="1" />
        <tooltip>This is not widely supported</tooltip>
        <location col="1" row="1" colspan="2" />
        <items>
                <item>LTR</item>
                <item>RTL</item>
        </items>
    </attr>

    <spacer orientation="h">
        <location col="0" row="2" colspan="3" />
    </spacer>

    <spacer orientation="v">
        <location col="0" row="9" colspan="3" />
    </spacer>

    <kafkainfos>
        <rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="no" cursorcanenter="true">
        </rootnode>
    </kafkainfos>

</tag>
<tag name="body" hasScript="1" hasCore="1" hasI18n="1">
    <attr name="background" type="url">
        <text>BG image</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" colspan="2"  />
    </attr>

    <attr name="bgcolor" type="color">
        <text>BG color</text>
        <textlocation col="0" row="1" />
        <tooltip>Depricated in HTML 4.0 </tooltip>
        <location col="2" row="1"  />
    </attr>

    <attr name="text" type="color">
        <text>Text color</text>
        <textlocation col="0" row="2" />
        <tooltip>Depricated in HTML 4.0 </tooltip>
        <location col="2" row="2"  />
    </attr>

    <attr name="link" type="color">
        <text>Link color</text>
        <textlocation col="0" row="3" />
        <tooltip>Depricated in HTML 4.0 </tooltip>
        <location col="2" row="3"  />
    </attr>

    <attr name="alink" type="color">
        <text>Active link color</text>
        <textlocation col="0" row="4" />
        <tooltip>Depricated in HTML 4.0 </tooltip>
        <location col="2" row="4"  />
    </attr>

    <attr name="vlink" type="color">
        <text>Visited link color</text>
        <textlocation col="0" row="5" />
        <tooltip>Depricated in HTML 4.0 </tooltip>
        <location col="2" row="5"  />
    </attr>

    <attr name="onload" type="input">
        <text>On load</text>
        <textlocation col="0" row="7" />
        <location col="1" row="7" colspan="2" />
    </attr>

    <attr name="onunload" type="input">
        <text>On unload</text>
        <textlocation col="0" row="8" />
        <location col="1" row="8" colspan="2" />
    </attr>

    <attr name="topmargin"> </attr>
    <attr name="leftmargin"> </attr>
    <attr name="marginwidth"> </attr>
    <attr name="marginheight"> </attr>

    <kafkainfos>
        <rootnode canbedeleted="false" canbemodified="false" canhavecursorfocus="no" cursorcanenter="false">
        </rootnode>
    </kafkainfos>

</tag>
<tag name="br" hasCore="1" single="1">

    <attr name="clear" type="list">
        <text>Clear</text>
        <textlocation col="0" row="0" />
        <tooltip>Depricated in HTML 4.0</tooltip>
        <location col="1" row="0" />
        <items>
                <item>left</item>
                <item>all</item>
                <item>right</item>
                <item>none</item>
        </items>
    </attr>


    <spacer orientation="v">
        <location col="0" row="9" colspan="2" />
    </spacer>

    <kafkainfos>
        <rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="singlenode" cursorcanenter="false">
        </rootnode>
    </kafkainfos>

</tag>
<tag name="button" hasCore="1" hasI18n="1" hasScript="1">
	<attr name="name" type="input">
		<text>Name</text>
		<textlocation col="0" row="0" />
		<location col="1" row="0" colspan="2" />
	</attr>
	<attr name="value" type="input">
		<text>Value</text>
		<textlocation col="0" row="1" />
		<location col="1" row="1" colspan="2" />
	</attr>
	<attr name="type" type="list">
		<text>Type</text>
		<textlocation col="0" row="2" />
		<items>
			<item>BUTTON</item>
			<item>SUBMIT</item>
			<item>RESET</item>
		</items>
		<location col="1" row="2" colspan="2" />
	</attr>
	<attr name="disabled" type="check">
		<text>Disabled</text>
		<textlocation col="0" row="3" />
		<location col="1" row="3" colspan="2" />
	</attr>

	<spacer orientation="v">
		<location col="0" row="11" colspan="2" />
	</spacer>

	<kafkainfos>
		<rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="singlenodeanditself" cursorcanenter="false">
		</rootnode>
	</kafkainfos>
</tag>
</TAGS>
