<!DOCTYPE TAGS>
<TAGS>
<tag name="th" hasScript="1" hasCore="1" hasI18n="1">

    <attr name="colspan" type="list">
        <text>Col span</text>
        <textlocation col="0" row="0" />
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
        <location col="1" row="0" />
    </attr>

   <attr name="rowspan" type="list">
        <text>Row span</text>
        <textlocation col="0" row="1" />
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
        <location col="1" row="1" />
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
        <textlocation col="0" row="3"  />
        <items>
            <item>baseline</item>
            <item>bottom</item>
            <item>center</item>
            <item>top</item>
        </items>
        <location col="1" row="3" colspan="1" />
    </attr>

	<attr name="char" type="input">
        <text>Character</text>
        <textlocation col="0" row="4" />
        <tooltip>alignment character for cells</tooltip>
        <location col="1" row="4" />
    </attr>

    <attr name="charoff" type="input">
        <text>Character offset</text>
        <textlocation col="0" row="5" />
        <tooltip>alignment character offset</tooltip>
        <location col="1" row="5" />
    </attr>    
	
    <attr name="abbr" type="input">
        <text>Abbr</text>
        <textlocation col="0" row="6" />
        <location col="1" row="6" />
    </attr>    
	
    <attr name="scope" type="input">
        <text>Scope</text>
        <textlocation col="0" row="7" />
        <location col="1" row="7" />
    </attr>    
	
    <attr name="axis" type="input">
        <text>Axis</text>
        <textlocation col="0" row="8" />
        <location col="1" row="8" />
    </attr>    
    
	<stoppingtags>
        <stoppingtag>td</stoppingtag>
        <stoppingtag>tr</stoppingtag>
        <stoppingtag>th</stoppingtag>
    </stoppingtags>
	
</tag>
</TAGS>
