<!DOCTYPE TAGS>
<TAGS>

<tag name="kafkainfos">

    <spacer orientation="v">
        <location col="0" row="9" colspan="2" />
    </spacer>

</tag>

<tag name="rootnode">
    <label>
        <text>Can Be Deleted</text>
        <location col="0" row="0" />
    </label>

    <attr name="canbedeleted" type="check">
        <tooltip>Indicate if the Node could be deleted in the VPL view.</tooltip>
        <location col="1" row="0" />
    </attr>

    <label>
        <text>Can Be Modified</text>
        <location col="0" row="1" />
    </label>

    <attr name="canbemodified" type="check">
        <tooltip>Indicate if the Node could be modified in the VPL view.</tooltip>
        <location col="1" row="1" />
    </attr>

    <label>
        <text>Can Have Cursor Focus</text>
        <location col="0" row="2" />
    </label>

    <attr name="canhavecursorfocus" type="list">
        <tooltip>Indicate if the Node could have the cursor focus in the VPL view</tooltip>
        <items>
          <item>no</item>
          <item>left</item>
          <item>right</item>
          <item>leftandright</item>
          <item>singlenode</item>
          <item>singlenodeanditself</item>
        </items>
        <location col="1" row="2" />
    </attr>

    <label>
        <text>Cursor Can Enter</text>
	<location col="0" row="3" />
    </label>

    <attr name="cursorcanenter" type="check">
        <tooltip>Indicate if the cursor could enter/leave the Node in the VPL view</tooltip>
        <location col="1" row="3" />
    </attr>

    <spacer orientation="v">
        <location col="0" row="9" colspan="2" />
    </spacer>
</tag>
</TAGS>
