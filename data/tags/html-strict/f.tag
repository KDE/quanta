<!DOCTYPE TAGS>
<TAGS>
<tag name="fieldset" hasCore="1" hasI18n="1" hasScript="1">
	<kafkainfos>
		<rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="no" cursorcanenter="false">
		</rootnode>
	</kafkainfos>
</tag>
<tag name="form" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="action" type="input">
        <text>Action</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" colspan="3" />
    </attr>

    <attr name="method" type="list">
        <text>Method</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1"/>
        <items>
                <item>GET</item>
                <item>POST</item>
        </items>
    </attr>

    <attr name="enctype" type="list">
        <text>Enctype</text>
        <textlocation col="0" row="2" />
        <location col="1" row="2" colspan="3"/>
        <items>
           <item>application/x-www-form-urlencoded</item>
           <item>multipart/form-data</item>
        </items>
    </attr>


    <attr name="target" type="list">
        <text>Target</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" />
        <items>
                <item>_blank</item>
                <item>_self</item>
                <item>_parent</item>
                <item>_top</item>
        </items>
    </attr>

    <attr name="accept-charset" type="input">
        <text>Accept charset</text>
        <textlocation col="2" row="3" />
        <location col="3" row="3"/>
    </attr>

    <attr name="name"> </attr>
    <attr name="onsubmit"> </attr>
    <attr name="onreset"> </attr>

    <spacer orientation="v">
        <location col="0" row="9" colspan="3" />
    </spacer>

    <kafkainfos>
        <rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="no" cursorcanenter="true">
        </rootnode>
    </kafkainfos>

</tag>
</TAGS>
