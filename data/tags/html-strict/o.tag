<!DOCTYPE TAGS>
<TAGS>
<tag name="object" hasCore="1" hasI18n="1" hasScript="1">
	<attr name="data" type="url">
		<text>Data</text>
		<textlocation col="0" row="0" />
		<location col="1" row="0" colspan="2" />
	</attr>
	<attr name="classid" type="url">
		<text>ClassId</text>
		<textlocation col="0" row="1" />
		<location col="1" row="1" colspan="2" />
	</attr>
	<attr name="archive" type="input">
		<text>Archive</text>
		<textlocation col="0" row="2" />
		<location col="1" row="2" colspan="2" />
	</attr>
	<attr name="codebase" type="url">
		<text>Codebase</text>
		<textlocation col="0" row="3" />
		<location col="1" row="3" colspan="2" />
	</attr>
	<attr name="width" type="input">
		<text>Width</text>
		<textlocation col="0" row="4" />
		<location col="1" row="4" colspan="2" />
	</attr>
	<attr name="height" type="input">
		<text>Height</text>
		<textlocation col="0" row="5" />
		<location col="1" row="5" colspan="2" />
	</attr>
	<attr name="name" type="input">
		<text>Name</text>
		<textlocation col="0" row="6" />
		<location col="1" row="6" colspan="2" />
	</attr>
	<attr name="usemap" type="url">
		<text>Usemap</text>
		<textlocation col="0" row="7" />
		<location col="1" row="7" colspan="2" />
	</attr>
	<attr name="type" type="input">
		<text>Type</text>
		<textlocation col="0" row="8" />
		<location col="1" row="8" colspan="2" />
	</attr>
	<attr name="codetype" type="input">
		<text>CodeType</text>
		<textlocation col="0" row="9" />
		<location col="1" row="9" colspan="2" />
	</attr>
	<attr name="standby" type="input">
		<text>Standby</text>
		<textlocation col="0" row="10" />
		<location col="1" row="10" colspan="2" />
	</attr>
	<attr name="tabindex" type="input">
		<text>Tabindex</text>
		<textlocation col="0" row="11" />
		<location col="1" row="11" colspan="2" />
	</attr>
	<attr name="declare" type="check">
		<text>Declare</text>
		<textlocation col="0" row="12" />
		<location col="1" row="12" colspan="2" />
	</attr>
	<attr name="align" type="list">
		<text>Align</text>
		<textlocation col="0" row="13" />
		<items>
			<item>baseline</item>
			<item>center</item>
			<item>left</item>
			<item>middle</item>
			<item>right</item>
			<item>textbottom</item>
			<item>textmiddle</item>
			<item>texttop</item>
		</items>
		<location col="1" row="13" colspan="2" />
	</attr>
	<attr name="border" type="input">
		<text>Border</text>
		<textlocation col="0" row="14" />
		<location col="1" row="14" colspan="2" />
	</attr>
	<attr name="hspace" type="input">
		<text>Hspace</text>
		<textlocation col="0" row="15" />
		<location col="1" row="15" colspan="2" />
	</attr>
	<attr name="vspace" type="input">
		<text>VSpace</text>
		<textlocation col="0" row="16" />
		<location col="1" row="16" colspan="2" />
	</attr>

	<spacer orientation="v">
		<location col="0" row="11" colspan="2" />
	</spacer>

	<kafkainfos>
		<rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="singlenodeanditself" cursorcanenter="false">
		</rootnode>
	</kafkainfos>
</tag>
<tag name="ol" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="type" type="list">
        <text>Type</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" colspan="2" />
        <items>
            <item>1</item>
            <item>a</item>
            <item>A</item>
            <item>i</item>
            <item>I</item>
        </items>
    </attr>

    <attr name="start" type="input">
        <text>Start</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1" colspan="2" />
    </attr>


    <attr name="compact" type="check">
        <text>Compact</text>
        <location col="0" row="2" colspan="2" />
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
        <rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="no" cursorcanenter="true">
        </rootnode>
    </kafkainfos>

</tag>
<tag name="option" hasScript="1" hasCore="1" hasI18n="1" optional="1">

    <attr name="value" type="input">
        <text>Value</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" />
    </attr>

    <attr name="selected" type="check">
        <text>Selected</text>
        <location col="1" row="1" />
    </attr>

    <attr name="disabled" type="check">
        <text>Disabled</text>
        <location col="1" row="2" />
    </attr>

    <attr name="label" type="input">
        <text>Label</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" />
    </attr>


    <spacer orientation="v">
        <location col="0" row="9" colspan="2" />
    </spacer>

    <stoppingtags>
	<stoppingtag name="option" />
    </stoppingtags>

    <kafkainfos>
        <rootnode canbedeleted="false" canbemodified="false" canhavecursorfocus="no" cursorcanenter="false">
        </rootnode>
    </kafkainfos>

</tag>
<tag name="optgroup" hasCore="1" hasI18n="1" hasScript="1">
	<attr name="label" type="input">
		<text>Label</text>
		<textlocation col="0" row="0" />
		<location col="1" row="0" colspan="2" />
	</attr>
	<attr name="disabled" type="check">
		<text>Disabled</text>
		<textlocation col="0" row="1" />
		<location col="1" row="1" colspan="2" />
	</attr>

	<spacer orientation="v">
		<location col="0" row="11" colspan="2" />
	</spacer>

	<kafkainfos>
		<rootnode canbedeleted="false" canbemodified="false" canhavecursorfocus="no" cursorcanenter="false">
		</rootnode>
	</kafkainfos>
</tag>
</TAGS>
