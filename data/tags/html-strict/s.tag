<!DOCTYPE TAGS>
<TAGS>
<tag name="script">

    <attr name="type" type="list">
        <text>Type</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" colspan="2" />
        <items>
                <item>text/html</item>
                <item>text/css</item>
		<item>text/php</item>
                <item>text/javascript</item>
                <item>application/x-javascript</item>
        </items>
    </attr>

    <attr name="language" type="list">
        <text>Language</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1" colspan="2" />
        <items>
		<item>PHP</item>
                <item>JavaScript</item>
                <item>JavaScript1.2</item>
                <item>JavaScript1.3</item>
                <item>VBScript</item>
        </items>
    </attr>

    <attr name="src" type="url">
        <text>Source</text>
        <textlocation col="0" row="2" />
        <location col="1" row="2" colspan="2" />
    </attr>

    <attr name="charset" type="input">
        <text>Charset</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" colspan="2" />
    </attr>

    <attr name="defer" type="check">
        <text>Defer (script execution may wait)</text>
        <location col="0" row="4" colspan="2" />
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
        <rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="no" cursorcanenter="false">
        </rootnode>
    </kafkainfos>

</tag>
<tag name="samp" hasCore="1" hasI18n="1" hasScript="1">
	<kafkainfos>
		<rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="no" cursorcanenter="true">
		</rootnode>
	</kafkainfos>
</tag>
<tag name="select" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="name" type="input">
        <text>Name</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" />
    </attr>

    <attr name="multiple" type="check">
        <text>Multiple</text>
        <location col="1" row="1" />
    </attr>

    <attr name="disabled" type="check">
        <text>Disabled</text>
        <location col="1" row="2" />
    </attr>

    <attr name="tabindex" type="input">
        <text>Tab index</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" />
    </attr>

    <attr name="onfocus" type="input">
        <text>On focus</text>
        <textlocation col="0" row="4" />
        <location col="1" row="4" />
    </attr>

    <attr name="onblur" type="input">
        <text>On blur</text>
        <textlocation col="0" row="5" />
        <location col="1" row="5" />
    </attr>

    <attr name="onchange" type="input">
        <text>On change</text>
        <textlocation col="0" row="6" />
        <location col="1" row="6" />
    </attr>

    <attr name="size"> </attr>

    <spacer orientation="v">
        <location col="0" row="9" colspan="2" />
    </spacer>

    <stoppingtags>
        <stoppingtag name="select" />
    </stoppingtags>

    <kafkainfos>
        <rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="singlenodeanditself" cursorcanenter="false">
        </rootnode>
    </kafkainfos>

</tag>
<tag name="small" hasCore="1" hasI18n="1" hasScript="1">
	<kafkainfos>
		<rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="no" cursorcanenter="true">
		</rootnode>
	</kafkainfos>
</tag>
<tag name="span" hasCore="1" hasI18n="1" hasScript="1">
	<kafkainfos>
		<rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="no" cursorcanenter="true">
		</rootnode>
	</kafkainfos>
</tag>
<tag name="strong" hasCore="1" hasI18n="1" hasScript="1">
	<kafkainfos>
		<rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="no" cursorcanenter="true">
		</rootnode>
	</kafkainfos>
</tag>
<tag name="style" hasI18n="1">

    <attr name="type" type="list">
        <text>Type</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" colspan="2" />
        <items>
                <item>text/css</item>
        </items>
    </attr>

    <attr name="media" type="list">
        <text>Media</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1" colspan="2" />
        <items>
                <item>screen</item>
                <item>tty</item>
                <item>tv</item>
                <item>projection</item>
                <item>handheld</item>
                <item>print</item>
                <item>braille</item>
                <item>aural</item>
                <item>all</item>
        </items>
    </attr>

    <attr name="title" type="input">
        <text>Title</text>
        <textlocation col="0" row="2" />
        <location col="1" row="2" colspan="2" />
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
<tag name="sub" hasCore="1" hasI18n="1" hasScript="1">
	<kafkainfos>
		<rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="no" cursorcanenter="true">
		</rootnode>
	</kafkainfos>
</tag>
<tag name="sup" hasCore="1" hasI18n="1" hasScript="1">
	<kafkainfos>
		<rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="no" cursorcanenter="true">
		</rootnode>
	</kafkainfos>
</tag>
</TAGS>
