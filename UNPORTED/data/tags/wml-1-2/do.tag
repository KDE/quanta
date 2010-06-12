<!DOCTYPE TAGS>
<TAGS>
<tag name="do" hasCore="1" >

    <attr name="type" status="required" type="list">
        <text>Type</text>
        <textlocation col="0" row="0" />
        <location col="1" row="0" />
	    <items>
	        <item>accept</item>
		<item>delete</item>
		<item>help</item>
		<item>options</item>
		<item>prev</item>
		<item>reset</item>
		<item>unknown</item>
		<item>vnd.</item>
	    </items>
    </attr>

    <attr name="label" status="implied" type="input">
        <text>Label</text> 
        <textlocation col="0" row="1" />
        <location col="1" row="1"/>
    </attr>

    <attr name="name" status="implied" type="input">
        <text>Name</text>
        <textlocation col="0" row="2" />
        <location col="1" row="2"/>
    </attr>


    <attr name="optional" defaultValue="false" type="list">
        <text>Ignore this element</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" />
             <items>
	         <item>true</item>
	         <item>false</item>
	     </items>
    </attr>

    <attr name="xml:lang" status="implied" type="input">
        <text>Override xml:lang</text>
        <textlocation col="0" row="4" />
        <location col="1" row="4" />
    </attr>

</tag>
</TAGS>
