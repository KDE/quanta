<!DOCTYPE TAGS>
<TAGS>
	<tag name="ArrayAppend" type="function" returnType="boolean">
		<attr name="array_name" status="required" />
		<attr name="value" type="string" status="required" />
	</tag>
	<tag name="ArrayAvg" type="function" returnType="number">
		<attr name="array_name" status="required" />
	</tag>
	<tag name="ArrayClear" type="function" returnType="boolean">
		<attr name="array_name" status="required" />
	</tag>
	<tag name="ArrayDeleteAt" type="function" returnType="boolean">
		<attr name="array_name" status="required" />
		<attr name="position" type="int" status="required" />
	</tag>
	<tag name="ArrayInsertAt" type="function" returnType="boolean">
		<attr name="array_name" status="required" />
		<attr name="position" type="int" status="required" />
		<attr name="value" type="string" status="required" />
	</tag>
	<tag name="ArrayIsEmpty" type="function" returnType="boolean">
		<attr name="array_name" status="required" />
	</tag>
	<tag name="ArrayLen" type="function" returnType="int">
		<attr name="array_name" status="required" />
	</tag>
	<tag name="ArrayMax" type="function" returnType="number">
		<attr name="array_name" status="required" />
	</tag>
	<tag name="ArrayMin" type="function" returnType="number">
		<attr name="array_name" status="required" />
	</tag>
	<tag name="ArrayNew" type="function">
		<attr name="dimension {1|2|3}" type="int" status="required" />
	</tag>
	<tag name="ArrayPrepend" type="function" returnType="boolean">
		<attr name="array_name" status="required" />
		<attr name="value" type="string" status="required" />
	</tag>
	<tag name="ArrayResize" type="function" returnType="boolean">
		<attr name="array_name" status="required" />
		<attr name="minimum_size" type="int" status="required" />
	</tag>
	<tag name="ArraySet" type="function" returnType="boolean">
		<attr name="array_name" status="required" />
		<attr name="start_pos" type="int" status="required" />
		<attr name="end_pos" type="int" status="required" />
		<attr name="value" type="string" status="required" />
	</tag>
	<tag name="ArraySort" type="function" returnType="boolean">
		<attr name="array_name" status="required" />
		<attr name="sort_type {numeric|text|textnocase}" status="required" />
		<attr name="sort_order {asc|desc}" status="optional" />
	</tag>
	<tag name="ArraySum" type="function" returnType="number">
		<attr name="array_name" status="required" />
	</tag>
	<tag name="ArraySwap" type="function" returnType="boolean">
		<attr name="array_name" status="required" />
		<attr name="position1" type="int" status="required" />
		<attr name="position2" type="int" status="required" />
	</tag>
	<tag name="ArrayToList" type="function" returnType="list">
		<attr name="array_name" status="required" />
		<attr name="delimiter" status="optional" />
	</tag>
</TAGS>
