<!DOCTYPE TAGS>
<TAGS>
<tag name="go" >

    <attr name="href" type="url">
        <text>HREF</text> 
        <textlocation col="0" row="0" />
        <location col="1" row="0" colspan="3"/>
    </attr>

    <attr name="sendreferer" type="list">
        <text>Include URL in request</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1"/>
             <items>
	         <item>true</item>
	         <item>false</item>
	     </items>
    </attr>

    <attr name="method" type="list">
        <text>HTTP submission method</text>
        <textlocation col="0" row="2" />
        <location col="1" row="2" />
	    <items>
	        <item>post</item>
		<item>get</item>
	    </items>
    </attr>

    <attr name="enctype" type="input">
        <text>Encoding type</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" />
    </attr>

    <attr name="accept-charset" type="input">
        <text>Encodings accepted</text>
        <textlocation col="0" row="4" />
        <location col="1" row="4" />
    </attr>

</tag>
</TAGS>
