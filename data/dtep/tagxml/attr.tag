<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
<tag name="attr">

    <label>
        <text>Name</text>
        <location col="0" row="0" />
    </label>

    <attr name="name" type="input">
        <tooltip>attribute name </tooltip>
        <location col="1" row="0" />
    </attr>

    <label>
        <text>Type</text>
        <location col="0" row="1" />
    </label>

    <attr name="type" type="list">
        <tooltip>attribute type </tooltip>
            <items>
                <item>input</item>
                <item>check</item>
                <item>color</item>
                <item>url</item>
                <item>list</item>
            </items>
        <location col="1" row="1" />
    </attr>

<children>
<child name="items" />
<child name="tooltip" />
<child name="location" />
<child name="label" />
</children>

</tag>
</TAGS>
