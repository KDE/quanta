<!DOCTYPE TAGS>
<TAGS>
<tag name="form">

    <label>
        <text>Action</text>
        <location col="0" row="0" />
    </label>

    <label>
        <text>Method</text>
        <location col="0" row="1" />
    </label>

    <label>
        <text>Enctype</text>
        <location col="0" row="2" />
    </label>

    <label>
        <text>Target</text>
        <location col="0" row="3" />
    </label>

    <label>
        <text>Accept charset</text>
        <location col="2" row="3" />
    </label>


    <attr name="action" type="input">
        <location col="1" row="0" colspan="3" />
    </attr>

    <attr name="method" type="list">
        <location col="1" row="1"/>
        <items>
                <item>GET</item>
                <item>POST</item>
        </items>
    </attr>

    <attr name="enctype" type="list">
        <location col="1" row="2" colspan="3"/>
        <items>
           <item>application/x-www-form-urlencoded</item>
           <item>multipart/form-data</item>
        </items>
    </attr>


    <attr name="target" type="list">
        <location col="1" row="3" />
        <items>
                <item>_blank</item>
                <item>_self</item>
                <item>_parent</item>
                <item>_top</item>
        </items>
    </attr>

    <attr name="accept-charset" type="input">
        <location col="3" row="3"/>
    </attr>


    <spacer orientation="v">
        <location col="0" row="9" colspan="3" />
    </spacer>



</tag>
</TAGS>
