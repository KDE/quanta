<!DOCTYPE TAGS>
<TAGS>
<tag name="form" hasScript="1" hasCore="1" hasI18n="1">
    
    <attr name="action" type="url">
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

    <attr name="accept-charset" type="input">
        <text>Accept charset</text>
        <textlocation col="2" row="3" />
        <location col="3" row="3"/>
    </attr>

    <attr name="accept" type="list">
        <text>Accept</text>
        <textlocation col="0" row="4" />
        <location col="1" row="4" colspan="3"/>
        <items>
           <item>application/x-www-form-urlencoded</item>
           <item>multipart/form-data</item>
        </items>
    </attr>
    
</tag>
</TAGS>
