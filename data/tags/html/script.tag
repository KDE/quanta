<!DOCTYPE TAGS>
<TAGS>
<tag name="script">

    <label>
        <text>Type </text>
        <location col="0" row="0" />
    </label>

    <label>
        <text>Language </text>
        <location col="0" row="1" />
    </label>

    <label>
        <text>Source </text>
        <location col="0" row="2" />
    </label>

    <label>
        <text>Charset </text>
        <location col="0" row="3" />
    </label>


    <attr name="type" type="list">
        <location col="1" row="0" colspan="2" />
        <items>
                <item>text/html</item>
                <item>text/css</item>
                <item>text/javascript</item>
                <item>application/x-javascript</item>
        </items>
    </attr>

    <attr name="language" type="list">
        <location col="1" row="1" colspan="2" />
        <items>
                <item>JavaScript</item>
                <item>JavaScript1.2</item>
                <item>JavaScript1.3</item>
                <item>VBScript</item>
        </items>
    </attr>

    <attr name="src" type="url">
        <location col="1" row="2" colspan="2" />
    </attr>

    <attr name="charset" type="input">
        <location col="1" row="3" colspan="2" />
    </attr>

    <attr name="defer" type="check">
        <text> Defer (script execution may wait ) </text>
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

</tag>
</TAGS>
