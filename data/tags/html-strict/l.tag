<!DOCTYPE TAGS>
<TAGS>
<tag name="label" hasCore="1" hasI18n="1" hasScript="1">
	<attr name="for" type="input">
		<text>For</text>
		<textlocation col="0" row="0" />
		<location col="1" row="0" colspan="2" />
	</attr>
	<attr name="accesskey" type="input">
		<text>Accesskey</text>
		<textlocation col="0" row="1" />
		<location col="1" row="1" colspan="2" />
	</attr>
	<attr name="onfocus" type="input">
		<text>Onfocus</text>
		<textlocation col="0" row="2" />
		<location col="1" row="2" colspan="2" />
	</attr>
	<attr name="onblur" type="input">
		<text>Onblur</text>
		<textlocation col="0" row="3" />
		<location col="1" row="3" colspan="2" />
	</attr>

	<spacer orientation="v">
		<location col="0" row="11" colspan="2" />
	</spacer>

	<kafkainfos>
		<rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="no" cursorcanenter="true">
		</rootnode>
	</kafkainfos>
</tag>
<tag name="legend" hasCore="1" hasI18n="1" hasScript="1">
	<attr name="accesskey" type="input">
		<text>Accesskey</text>
		<textlocation col="0" row="0" />
		<location col="1" row="0" colspan="2" />
	</attr>
	<attr name="align" type="list">
		<text>Align</text>
		<textlocation col="0" row="1" />
		<items>
			<item>left</item>
			<item>right</item>
			<item>top</item>
			<item>bottom</item>
		</items>
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
<tag name="location" single="1">
	<kafkainfos>
		<rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="no" cursorcanenter="false">
		</rootnode>
	</kafkainfos>
</tag>
<tag name="li" hasCore="1" hasI18n="1" hasScript="1" optional="1">

    <attr name="type" type="list">
        <text>Type</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" colspan="2" />
        <items>
            <item>disk</item>
            <item>square</item>
            <item>circle</item>
            <item>1</item>
            <item>a</item>
            <item>A</item>
            <item>i</item>
            <item>I</item>
        </items>
    </attr>

    <attr name="value" type="input">
        <text>Value</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1" colspan="2" />
    </attr>

    <attr name="compact" type="check">
        <text>compact</text>
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

    <stoppingtags>
        <stoppingtag name="li" />
    </stoppingtags>

    <kafkainfos>
        <rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="left" cursorcanenter="true">
        </rootnode>
    </kafkainfos>

</tag>
<tag name="link" hasScript="1" hasCore="1" hasI18n="1" single="1">
    <attr name="rel" type="list">
        <text>Rel</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" colspan="2"  />
        <items>
                <item>Alternate</item>
                <item>StyleSheet</item>
                <item>Start</item>
                <item>Next</item>
                <item>Prev</item>
                <item>Contents</item>
                <item>Index</item>
                <item>Glossary</item>
                <item>Copyright</item>
                <item>Made</item>
                <item>Chapter</item>
                <item>Section</item>
                <item>Subsection</item>
                <item>Appendix</item>
                <item>Help</item>
                <item>Bookmark</item>
        </items>
    </attr>

    <attr name="rev" type="list">
        <text>Rev</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1" colspan="2" />
        <items>
                <item>Alternate</item>
                <item>StyleSheet</item>
                <item>Start</item>
                <item>Next</item>
                <item>Prev</item>
                <item>Contents</item>
                <item>Index</item>
                <item>Glossary</item>
                <item>Copyright</item>
                <item>Made</item>
                <item>Chapter</item>
                <item>Section</item>
                <item>Subsection</item>
                <item>Appendix</item>
                <item>Help</item>
                <item>Bookmark</item>
        </items>
    </attr>

    <attr name="href" type="url">
        <text>HREF</text>
        <textlocation col="0" row="2" />
        <location col="1" row="2" colspan="2" />
    </attr>

    <attr name="type" type="list">
        <text>Type</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" colspan="2" />
        <items>
                <item>text/html</item>
                <item>image/jpeg</item>
                <item>image/gif</item>
                <item>image/png</item>
                <item>model/vrml</item>
                <item>video/quicktime</item>
                <item>application/java</item>
                <item>text/css</item>
                <item>text/javascript</item>
        </items>
    </attr>

    <attr name="target" type="list">
        <text>Target</text>
        <textlocation col="0" row="4" />
        <location col="1" row="4" colspan="2"  />
        <items>
                <item>_blank</item>
                <item>_self</item>
                <item>_parent</item>
                <item>_top</item>
        </items>
    </attr>

    <attr name="media" type="list">
        <text>Media</text>
        <textlocation col="0" row="5" />
        <location col="1" row="5" colspan="2" />
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

    <attr name="hreflang" type="input">
        <text>HREF lang</text>
        <textlocation col="0" row="6" />
        <location col="1" row="6" colspan="2" />
    </attr>

    <attr name="charset" type="input">
        <text>Charset</text>
        <textlocation col="0" row="7" />
        <location col="1" row="7" colspan="2" />
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
</TAGS>
