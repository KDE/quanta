<!DOCTYPE TAGS>
<TAGS>
<tag name="input" hasCore="1" single="1">

    <attr name="name" status="required" type="input">
        <text>Name</text> 
        <textlocation col="0" row="0" />
        <location col="1" row="0" colspan="3"/>
    </attr>

    <attr name="type" defaultValue="text" type="list">
        <text>Text Type</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1"/>
             <items>
	         <item>text</item>
		 <item>password</item>
	     </items>
    </attr>

    <attr name="value" status="implied" type="input">
        <text>Value</text>
        <textlocation col="0" row="2" />
        <location col="1" row="2" />
    </attr>

    <attr name="format" status="implied" type="input">
        <text>Format Mask</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" />
    </attr>

    <attr name="emptyok" defaultValue="false" type="list">
        <text>Leave field blank</text>
        <textlocation col="0" row="4" />
        <location col="1" row="4" />
             <items>
	         <item>true</item>
	         <item>false</item>
	     </items>
    </attr>

    <attr name="size" status="implied" type="input">
        <text>Size</text>
        <textlocation col="0" row="5" />
        <location col="1" row="5" />
    </attr>

    <attr name="maxlength" status="implied" type="input">
        <text>Maximum length</text>
        <textlocation col="0" row="6" />
        <location col="1" row="6" />
    </attr>

    <attr name="tabindex" status="implied" type="input">
        <text>Tab index</text>
        <textlocation col="0" row="7" />
        <location col="1" row="7" />
    </attr>

    <attr name="title" status="implied" type="input">
        <text>Title</text>
        <textlocation col="0" row="8" />
        <location col="1" row="8" />
    </attr>

    <attr name="accesskey" status="implied"type="input">
        <text>Access key</text>
        <textlocation col="0" row="9" />
        <location col="1" row="9" />
    </attr>

    <attr name="xml:lang" status="implied" type="input">
        <text>Override xml:lang</text>
        <textlocation col="0" row="10" />
        <location col="1" row="10" />
    </attr>

</tag>
</TAGS>
