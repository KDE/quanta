<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
<tag name="select" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="multiple" type="check">
        <text>multiple</text>
        <location col="1" row="0" />
    </attr>

    <attr name="disabled" type="check">
        <text>disabled</text>
        <location col="1" row="1" />
    </attr>

    <attr name="tabindex" type="input">
        <text>tabindex</text>
        <textlocation col="0" row="2" />
        <location col="1" row="2" />
    </attr>

    <attr name="size" type="input">
        <text>size</text>
        <textlocation col="0" row="3" />
        <location col="1" row="3" />
	</attr>

   <attr name="onfocus" type="input">
        <text>onfocus</text>
        <textlocation col="0" row="4" />
        <location col="1" row="4" />
	</attr>

   <attr name="onblur" type="input">
        <text>onblur</text>
        <textlocation col="0" row="5" />
        <location col="1" row="5" />
	</attr>

   <attr name="onchange" type="input">
        <text>onchange</text>
        <textlocation col="0" row="6" />
        <location col="1" row="6" />
	</attr>

    <stoppingtags>
        <stoppingtag name="select" />
    </stoppingtags>

<children>
<child name="optgroup" />
<child name="option" />
</children>
</tag>
</TAGS>
