<!DOCTYPE TAGS>
<TAGS>
<tag name="select" hasCore="1">

    <attr name="title" status="implied" type="input">
        <text>Title</text> 
        <textlocation col="0" row="0" />
        <location col="1" row="0" colspan="3"/>
    </attr>

    <attr name="name" status="implied" type="input">
        <text>Variable Name</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1"/>
    </attr>

    <attr name="value" status="implied" type="input">
        <text>Value</text>
        <textlocation col="0" row="2" />
        <location col="1" row="2" />
    </attr>

    <attr name="iname" status="implied" type="input">
        <text>Index variable name</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" />
    </attr>

    <attr name="ivalue" status="implied" type="input">
        <text>Index variable deafult</text>
        <textlocation col="0" row="4" />
        <location col="1" row="4" />
    </attr>

    <attr name="multiple" defaultValue="false" type="list">
        <text>Select multiple values</text>
        <textlocation col="0" row="5" />
             <items>
	         <item>true</item>
	         <item>false</item>
	     </items>
        <location col="1" row="5" />
    </attr>

    <attr name="tabindex" status="implied" type="input">
        <text>Tab index</text>
        <textlocation col="0" row="6" />
        <location col="1" row="6" />
    </attr>

    <attr name="xml:lang" status="implied" type="input">
        <text>Override xml:lang</text>
        <textlocation col="0" row="7" />
        <location col="1" row="7" />
    </attr>

</tag>
</TAGS>
