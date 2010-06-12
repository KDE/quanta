<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE TAGS>
<TAGS>
<tag name="tag">

    <label>
        <text>Tag</text>
        <location col="0" row="0" />
    </label>

    <attr name="name" type="input">
        <tooltip>tag name </tooltip>
        <location col="1" row="0" />
    </attr>

    <label>
        <text>Has Core</text>
        <location col="0" row="1" />
    </label>

    <attr name="hasCore" type="list">
        <tooltip>Uses Core Page</tooltip>
        <location col="1" row="1" />
        <items>
          <item>0</item>
          <item>1</item>
        </items>
    </attr>

    <label>
        <text>Has I18n</text>
        <location col="0" row="2" />
    </label>

    <attr name="hasI18n" type="list">
        <tooltip>Uses I18n Page</tooltip>
        <location col="1" row="2" />
        <items>
          <item>0</item>
          <item>1</item>
        </items>
    </attr>

    <label>
        <text>Has Script</text>
        <location col="0" row="3" />
    </label>

    <attr name="hasScript" type="list">
        <tooltip>Uses Script Page</tooltip>
        <location col="1" row="3" />
        <items>
          <item>0</item>
          <item>1</item>
        </items>
    </attr>

    <label>
        <text>Single Tag</text>
        <location col="0" row="4" />
    </label>

    <attr name="single" type="list">
        <tooltip>Single tag requiring a / at the end</tooltip>
        <location col="1" row="4" />
        <items>
          <item>0</item>
          <item>1</item>
        </items>
    </attr>

</tag>
</TAGS>
