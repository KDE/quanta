<!DOCTYPE TAGS>
<TAGS>
<tag name="card" hasCore="1">

    <attr name="title" status="implied" type="input">
        <text>Title</text> 
        <textlocation col="0" row="0" />
        <location col="1" row="0"/>
    </attr>

    <attr name="newcontext" defaultValue="false" type="list">
        <text>Initialise context</text> 
        <textlocation col="0" row="1" />
        <location col="1" row="1"/>
             <items>
	         <item>true</item>
	         <item>false</item>
	     </items>
    </attr>

    <attr name="ordered" defaultValue="true" type="list">
        <text>Allow user to navigate</text> 
        <textlocation col="0" row="2" />
        <location col="1" row="2"/>
             <items>
	         <item>true</item>
	         <item>false</item>
	     </items>
    </attr>

    <attr name="xml:lang" status="implied" type="input">
        <text>Override xml:lang</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" />
    </attr>

    <attr name="onenterforward" status="implied" type="url">
        <text>URL for go</text> 
        <textlocation col="0" row="4" />
        <location col="1" row="4" colspan="3"/>
    </attr>

    <attr name="onenterbackward" status="implied" type="url">
        <text>URL for prev</text> 
        <textlocation col="0" row="5" />
        <location col="1" row="5" colspan="3"/>
    </attr>

    <attr name="ontimer" status="implied" type="url">
        <text>URL for timer</text> 
        <textlocation col="0" row="6" />
        <location col="1" row="6" colspan="3"/>
    </attr>

</tag>
</TAGS>
