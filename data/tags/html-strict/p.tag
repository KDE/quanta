<!DOCTYPE TAGS>
<TAGS>
<tag name="p" hasScript="1" hasCore="1" hasI18n="1" optional="1">

    <attr name="align" type="list">
        <text>Alignment</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" />
        <items>
                <item>left</item>
                <item>center</item>
                <item>right</item>
                <item>justify</item>
        </items>
    </attr>


    <spacer orientation="v">
        <location col="0" row="9" colspan="2" />
    </spacer>

    <stoppingtags>
        <stoppingtag name="p" />
        <stoppingtag name="li" />
        <stoppingtag name="td" />
        <stoppingtag name="tr" />
        <stoppingtag name="th" />
        <stoppingtag name="dt" />
        <stoppingtag name="dd" />
    </stoppingtags>

    <kafkainfos>
        <rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="left" cursorcanenter="true">
        </rootnode>
    </kafkainfos>

</tag>
<tag name="param" single="1">
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
	<attr name="valuetype" type="input">
		<text>Value Type</text>
		<textlocation col="0" row="2" />
		<location col="1" row="2" colspan="2" />
	</attr>
	<attr name="type" type="input">
		<text>Type</text>
		<textlocation col="0" row="3" />
		<location col="1" row="3" colspan="2" />
	</attr>
	<attr name="id" type="input">
		<text>Id</text>
		<textlocation col="0" row="4" />
		<location col="1" row="4" colspan="2" />
	</attr>

	<spacer orientation="v">
		<location col="0" row="11" colspan="2" />
	</spacer>

	<kafkainfos>
		<rootnode canbedeleted="false" canbemodified="false" canhavecursorfocus="no" cursorcanenter="false">
		</rootnode>
	</kafkainfos>
</tag>
<tag name="pre" hasCore="1" hasI18n="1" hasScript="1">
	<attr name="width" type="input">
		<text>Width</text>
		<textlocation col="0" row="0" />
		<location col="1" row="0" colspan="2" />
	</attr>
	<attr name="xml:space" type="input">
		<text>Xml:Space</text>
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
</TAGS>
