<!DOCTYPE TAGS>
<TAGS>
<tag name="go" hasCore="1">

    <attr name="href" status="required" type="url">
        <text>HREF</text> 
        <textlocation col="0" row="0" />
        <location col="1" row="0" colspan="3"/>
    </attr>

    <attr name="sendreferer" defaultValue="false" type="list">
        <text>Include URL in request</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1"/>
             <items>
	         <item>true</item>
	         <item>false</item>
	     </items>
    </attr>

    <attr name="method" defaultValue="get" type="list">
        <text>HTTP submission method</text>
        <textlocation col="0" row="2" />
        <location col="1" row="2" />
	    <items>
	        <item>post</item>
		<item>get</item>
	    </items>
    </attr>

    <attr name="enctype" defaultValue="application/x-www-form-urlencoded" type="input">
        <text>Encoding type</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" />
    </attr>

    <attr name="accept-charset" status="implied" type="input">
        <text>Encodings accepted</text>
        <textlocation col="0" row="4" />
        <location col="1" row="4" />
    </attr>

</tag>
</TAGS>
