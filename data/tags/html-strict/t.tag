<!DOCTYPE TAGS>
<TAGS>
<tag name="table" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="width" type="input">
        <text>Width</text>
        <textlocation col="0" row="0" />
        <tooltip>use % for percent or it will set pixels</tooltip>
        <location col="1" row="0" />
    </attr>

    <attr name="cellspacing" type="input">
        <text>Cell spacing</text>
        <textlocation col="2" row="0" />
        <location col="3" row="0" />
    </attr>

    <attr name="border" type="input">
        <text>Border</text>
        <textlocation col="0" row="1" />
        <location col="1" row="1" />
    </attr>

    <attr name="cellpadding" type="input">
        <text>Cell padding</text>
        <textlocation col="2" row="1" />
        <location col="3" row="1" />
    </attr>

    <attr name="align" type="list">
        <text>Align</text>
        <textlocation col="0" row="2" />
        <items>
            <item>left</item>
            <item>center</item>
            <item>right</item>
        </items>
        <location col="1" row="2" colspan="2" />
    </attr>

    <attr name="bgcolor" type="color">
        <text>Background color</text>
        <textlocation col="0" row="3"  />
        <location col="1" row="3" colspan="2" />
        <tooltip>Depricated in HTML 4.0</tooltip>
    </attr>

    <spacer orientation="v">
        <location col="0" row="4" colspan="4" />
    </spacer>

    <attr name="frame" type="list">
        <text>Frame</text>
        <textlocation col="0" row="5"  />
        <items>
            <item>above</item>
            <item>below</item>
            <item>border</item>
            <item>box</item>
            <item>hsides</item>
            <item>LHS</item>
            <item>RHS</item>
            <item>void</item>
            <item>vsides</item>
        </items>
        <location col="1" row="5" colspan="2" />
        <tooltip>Not supported in Netscape 4x</tooltip>
    </attr>

    <spacer orientation="v">
        <location col="0" row="9" colspan="4" />
    </spacer>

    <attr name="summary"> </attr>
    <attr name="fr"> </attr>
    <attr name="rules"> </attr>
    <attr name="background"> </attr>
    <attr name="height"> </attr>

    <kafkainfos>
        <rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="singlenodeanditself" cursorcanenter="false">
        </rootnode>
    </kafkainfos>

</tag>
<tag name="tbody" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="align" type="list">
        <text>Align</text>
        <textlocation col="0" row="0" />
        <items>
            <item>left</item>
            <item>center</item>
            <item>right</item>
        </items>
        <location col="1" row="0" colspan="1" />
    </attr>

    <attr name="valign" type="list">
        <text>VAlign</text>
        <textlocation col="0" row="1"  />
        <items>
            <item>baseline</item>
            <item>bottom</item>
            <item>bottom</item>
            <item>center</item>
            <item>top</item>
        </items>
        <location col="1" row="1" colspan="1" />
    </attr>

    <spacer orientation="v">
        <location col="0" row="5" colspan="4" />
    </spacer>

    <attr name="bgcolor" type="color">
        <text>Bg color</text>
        <textlocation col="0" row="5" />
        <location col="1" row="5" colspan="1" />
        <tooltip>Depricated in HTML 4.0</tooltip>
    </attr>


    <spacer orientation="v">
        <location col="0" row="9" colspan="4" />
    </spacer>

    <attr name="char"> </attr>
    <attr name="charoff"> </attr>

    <kafkainfos>
        <rootnode canbedeleted="false" canbemodified="false" canhavecursorfocus="no" cursorcanenter="false">
        </rootnode>
    </kafkainfos>

</tag>
<tag name="td" hasScript="1" hasCore="1" hasI18n="1" optional="1">

    <attr name="width" type="input">
        <text>Width</text>
        <textlocation col="0" row="0" />
        <tooltip>use % for percent or it will set pixels</tooltip>
        <location col="1" row="0" />
    </attr>

    <attr name="colspan" type="list">
        <text>Col span</text>
        <textlocation col="2" row="0" />
        <items>
            <item>1</item>
            <item>2</item>
            <item>3</item>
            <item>4</item>
            <item>5</item>
            <item>6</item>
            <item>7</item>
            <item>8</item>
            <item>9</item>
        </items>
        <location col="3" row="0" />
    </attr>

    <attr name="height" type="input">
        <text>Height</text>
        <textlocation col="0" row="1" />
        <tooltip>Deprecated in HTML 4.0 - not used in IE 5</tooltip>
        <location col="1" row="1" />
    </attr>

    <attr name="rowspan" type="list">
        <text>Row span</text>
        <textlocation col="2" row="1" />
        <items>
            <item>1</item>
            <item>2</item>
            <item>3</item>
            <item>4</item>
            <item>5</item>
            <item>6</item>
            <item>7</item>
            <item>8</item>
            <item>9</item>
        </items>
        <location col="3" row="1" />
    </attr>

    <attr name="align" type="list">
        <text>Align</text>
        <textlocation col="0" row="2" />
        <items>
            <item>left</item>
            <item>center</item>
            <item>right</item>
        </items>
        <location col="1" row="2" colspan="1" />
    </attr>

    <attr name="valign" type="list">
        <text>VAlign</text>
        <textlocation col="2" row="2"  />
        <items>
            <item>baseline</item>
            <item>bottom</item>
            <item>bottom</item>
            <item>center</item>
            <item>top</item>
        </items>
        <location col="3" row="2" colspan="1" />
    </attr>

    <spacer orientation="v">
        <location col="0" row="5" colspan="4" />
    </spacer>

    <attr name="bgcolor" type="color">
        <text>Bg color</text>
        <textlocation col="0" row="5"  />
        <location col="1" row="5" colspan="1" />
        <tooltip>Depricated in HTML 4.0</tooltip>
    </attr>

    <attr name="nowrap" type="check">
        <text>NoWrap</text>
        <location col="0" row="6" colspan="2" />
        <tooltip>Depricated in HTML 4.0</tooltip>
    </attr>

    <spacer orientation="v">
        <location col="0" row="9" colspan="4" />
    </spacer>

    <attr name="headers"> </attr>
    <attr name="abbr"> </attr>
    <attr name="scope"> </attr>
    <attr name="axis"> </attr>
    <attr name="char"> </attr>
    <attr name="charoff"> </attr>
    <attr name="background"> </attr>

    <stoppingtags>
        <stoppingtag>td</stoppingtag>
        <stoppingtag>tr</stoppingtag>
        <stoppingtag>th</stoppingtag>
    </stoppingtags>

    <kafkainfos>
        <rootnode canbedeleted="false" canbemodified="false" canhavecursorfocus="no" cursorcanenter="false">
        </rootnode>
    </kafkainfos>

</tag>
<tag name="textarea" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="name" type="input">
      <text>Name</text>
      <textlocation col="0" row="0" colspan="1" />
      <location col="1" row="0" colspan="4" />
    </attr>

    <attr name="cols" type="input">
      <text>Columns</text>
      <textlocation col="0" row="1" colspan="1" />
      <location col="1" row="1" colspan="1" />
    </attr>

    <attr name="rows" type="input">
      <text>Rows</text>
      <textlocation col="2" row="1" colspan="1" />
      <location col="3" row="1" colspan="1" />
    </attr>

    <attr name="tabindex" type="input">
      <text>Tab index</text>
      <textlocation col="0" row="2" colspan="1" />
      <location col="1" row="2" colspan="1" />
    </attr>

    <spacer>
      <location col="0" row="3" colspan="4" />
    </spacer>

    <attr name="disabled" type="check">
      <text>Disabled</text>
      <location col="0" row="4" colspan="2" />
    </attr>

    <attr name="readonly" type="check">
      <text>Read only</text>
      <location col="0" row="5" colspan="2" />
    </attr>

    <spacer>
      <location col="0" row="9" colspan="4" />
    </spacer>

    <attr name="accesskey"> </attr>
    <attr name="tab"> </attr>
    <attr name="onfocus"> </attr>
    <attr name="onblur"> </attr>
    <attr name="onselect"> </attr>
    <attr name="onchange"> </attr>

    <kafkainfos>
        <rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="singlenodeanditself" cursorcanenter="false">
        </rootnode>
    </kafkainfos>

</tag>
<tag name="tfoot" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="align" type="list">
        <text>Align</text>
        <textlocation col="0" row="0" />
        <items>
            <item>left</item>
            <item>center</item>
            <item>right</item>
        </items>
        <location col="1" row="0" colspan="1" />
    </attr>

    <attr name="valign" type="list">
        <text>VAlign</text>
        <textlocation col="0" row="1"  />
        <items>
            <item>baseline</item>
            <item>bottom</item>
            <item>bottom</item>
            <item>center</item>
            <item>top</item>
        </items>
        <location col="1" row="1" colspan="1" />
    </attr>

    <spacer orientation="v">
        <location col="0" row="5" colspan="4" />
    </spacer>

    <attr name="bgcolor" type="color">
        <text>Bg color</text>
        <textlocation col="0" row="5" />
        <location col="1" row="5" colspan="1" />
        <tooltip>Depricated in HTML 4.0</tooltip>
    </attr>


    <spacer orientation="v">
        <location col="0" row="9" colspan="4" />
    </spacer>

    <attr name="char"> </attr>
    <attr name="charoff"> </attr>

    <kafkainfos>
        <rootnode canbedeleted="false" canbemodified="false" canhavecursorfocus="no" cursorcanenter="false">
        </rootnode>
    </kafkainfos>

</tag>
<tag name="th" hasScript="1" hasCore="1" hasI18n="1" optional="1">

    <attr name="width" type="input">
        <text>Width</text>
        <textlocation col="0" row="0" />
        <tooltip>use % for percent or it will set pixels</tooltip>
        <location col="1" row="0" />
    </attr>

    <attr name="colspan" type="list">
        <text>Col span</text>
        <textlocation col="2" row="0" />
        <items>
            <item>1</item>
            <item>2</item>
            <item>3</item>
            <item>4</item>
            <item>5</item>
            <item>6</item>
            <item>7</item>
            <item>8</item>
            <item>9</item>
        </items>
        <location col="3" row="0" />
    </attr>

    <attr name="height" type="input">
        <text>Height</text>
        <textlocation col="0" row="1" />
        <tooltip>Deprecated in HTML 4.0 - not used in IE 5</tooltip>
        <location col="1" row="1" />
    </attr>

    <attr name="rowspan" type="list">
        <text>Row span</text>
        <textlocation col="2" row="1" />
        <items>
            <item>1</item>
            <item>2</item>
            <item>3</item>
            <item>4</item>
            <item>5</item>
            <item>6</item>
            <item>7</item>
            <item>8</item>
            <item>9</item>
        </items>
        <location col="3" row="1" />
    </attr>

    <attr name="align" type="list">
        <text>Align</text>
        <textlocation col="0" row="2" />
        <items>
            <item>left</item>
            <item>center</item>
            <item>right</item>
        </items>
        <location col="1" row="2" colspan="1" />
    </attr>

    <attr name="valign" type="list">
        <text>VAlign</text>
        <textlocation col="2" row="2"  />
        <items>
            <item>baseline</item>
            <item>bottom</item>
            <item>bottom</item>
            <item>center</item>
            <item>top</item>
        </items>
        <location col="3" row="2" colspan="1" />
    </attr>

    <spacer orientation="v">
        <location col="0" row="5" colspan="4" />
    </spacer>

    <attr name="bgcolor" type="color">
        <text>Bg color</text>
        <textlocation col="0" row="5"  />
        <location col="1" row="5" colspan="1" />
        <tooltip>Depricated in HTML 4.0</tooltip>
    </attr>

    <attr name="nowrap" type="check">
        <text>NoWrap</text>
        <location col="0" row="6" colspan="2" />
        <tooltip>Depricated in HTML 4.0</tooltip>
    </attr>

    <spacer orientation="v">
        <location col="0" row="9" colspan="4" />
    </spacer>

    <attr name="headers"> </attr>
    <attr name="abbr"> </attr>
    <attr name="scope"> </attr>
    <attr name="axis"> </attr>
    <attr name="char"> </attr>
    <attr name="charoff"> </attr>

    <kafkainfos>
        <rootnode canbedeleted="false" canbemodified="false" canhavecursorfocus="no" cursorcanenter="false">
        </rootnode>
    </kafkainfos>

</tag>
<tag name="thead" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="align" type="list">
        <text>Align</text>
        <textlocation col="0" row="0" />
        <items>
            <item>left</item>
            <item>center</item>
            <item>right</item>
        </items>
        <location col="1" row="0" colspan="1" />
    </attr>

    <attr name="valign" type="list">
        <text>VAlign</text>
        <textlocation col="0" row="1"  />
        <items>
            <item>baseline</item>
            <item>bottom</item>
            <item>bottom</item>
            <item>center</item>
            <item>top</item>
        </items>
        <location col="1" row="1" colspan="1" />
    </attr>

    <spacer orientation="v">
        <location col="0" row="5" colspan="4" />
    </spacer>

    <attr name="bgcolor" type="color">
        <text>Bg color</text>
        <textlocation col="0" row="5" />
        <location col="1" row="5" colspan="1" />
        <tooltip>Depricated in HTML 4.0</tooltip>
    </attr>


    <spacer orientation="v">
        <location col="0" row="9" colspan="4" />
    </spacer>

    <attr name="char"> </attr>
    <attr name="charoff"> </attr>

    <kafkainfos>
        <rootnode canbedeleted="false" canbemodified="false" canhavecursorfocus="no" cursorcanenter="false">
        </rootnode>
    </kafkainfos>

</tag>
<tag name="title" hasI18n="1">
	<kafkainfos>
		<rootnode canbedeleted="false" canbemodified="false" canhavecursorfocus="no" cursorcanenter="false">
		</rootnode>
	</kafkainfos>
</tag>
<tag name="tr" hasScript="1" hasCore="1" hasI18n="1" optional="1">

    <attr name="align" type="list">
        <text>Align</text>
        <textlocation col="0" row="0" />
        <items>
            <item>left</item>
            <item>center</item>
            <item>right</item>
        </items>
        <location col="1" row="0" colspan="1" />
    </attr>

    <attr name="valign" type="list">
        <text>VAlign</text>
        <textlocation col="0" row="1"  />
        <items>
            <item>baseline</item>
            <item>bottom</item>
            <item>bottom</item>
            <item>center</item>
            <item>top</item>
        </items>
        <location col="1" row="1" colspan="1" />
    </attr>

    <spacer orientation="v">
        <location col="0" row="5" colspan="4" />
    </spacer>

    <attr name="bgcolor" type="color">
        <text>Bg color</text>
        <textlocation col="0" row="5" />
        <location col="1" row="5" colspan="1" />
        <tooltip>Depricated in HTML 4.0</tooltip>
    </attr>


    <spacer orientation="v">
        <location col="0" row="9" colspan="4" />
    </spacer>

    <attr name="char"> </attr>
    <attr name="charoff"> </attr>

    <stoppingtags>
        <stoppingtag>tr</stoppingtag>
        <stoppingtag>tfoot</stoppingtag>
    </stoppingtags>

    <kafkainfos>
        <rootnode canbedeleted="false" canbemodified="false" canhavecursorfocus="no" cursorcanenter="false">
        </rootnode>
    </kafkainfos>

</tag>
<tag name="tt" hasCore="1" hasI18n="1" hasScript="1">
	<kafkainfos>
		<rootnode canbedeleted="true" canbemodified="true" canhavecursorfocus="no" cursorcanenter="true">
		</rootnode>
	</kafkainfos>
</tag>
</TAGS>
