<!DOCTYPE tags>
<tags>
	<tag name="openal_buffer_create" type="function" returnType="resource">
	</tag>
	<tag name="openal_buffer_data" type="function" returnType="bool">
		<attr name="buffer" type="resource"></attr>
		<attr name="format" type="int"></attr>
		<attr name="data" type="string"></attr>
		<attr name="freq" type="int"></attr>
	</tag>
	<tag name="openal_buffer_destroy" type="function" returnType="bool">
		<attr name="buffer" type="resource"></attr>
	</tag>
	<tag name="openal_buffer_get" type="function" returnType="int">
		<attr name="buffer" type="resource"></attr>
		<attr name="property" type="int"></attr>
	</tag>
	<tag name="openal_buffer_loadwav" type="function" returnType="bool">
		<attr name="buffer" type="resource"></attr>
		<attr name="wavfile" type="string"></attr>
	</tag>
	<tag name="openal_context_create" type="function" returnType="resource">
		<attr name="device" type="resource"></attr>
	</tag>
	<tag name="openal_context_current" type="function" returnType="bool">
		<attr name="context" type="resource"></attr>
	</tag>
	<tag name="openal_context_destroy" type="function" returnType="bool">
		<attr name="context" type="resource"></attr>
	</tag>
	<tag name="openal_context_process" type="function" returnType="bool">
		<attr name="context" type="resource"></attr>
	</tag>
	<tag name="openal_context_suspend" type="function" returnType="bool">
		<attr name="context" type="resource"></attr>
	</tag>
	<tag name="openal_device_close" type="function" returnType="bool">
		<attr name="device" type="resource"></attr>
	</tag>
	<tag name="openal_device_open" type="function" returnType="resource">
		<attr name="device_desc" type="string" status="optional"></attr>
	</tag>
	<tag name="openal_listener_get" type="function" returnType="mixed">
		<attr name="property" type="int"></attr>
	</tag>
	<tag name="openal_listener_set" type="function" returnType="bool">
		<attr name="property" type="int"></attr>
		<attr name="setting" type="mixed"></attr>
	</tag>
	<tag name="openal_source_create" type="function" returnType="resource">
	</tag>
	<tag name="openal_source_destroy" type="function" returnType="resource">
		<attr name="source" type="resource"></attr>
	</tag>
	<tag name="openal_source_get" type="function" returnType="mixed">
		<attr name="source" type="resource"></attr>
		<attr name="property" type="int"></attr>
	</tag>
	<tag name="openal_source_pause" type="function" returnType="bool">
		<attr name="source" type="resource"></attr>
	</tag>
	<tag name="openal_source_play" type="function" returnType="bool">
		<attr name="source" type="resource"></attr>
	</tag>
	<tag name="openal_source_rewind" type="function" returnType="bool">
		<attr name="source" type="resource"></attr>
	</tag>
	<tag name="openal_source_set" type="function" returnType="bool">
		<attr name="source" type="resource"></attr>
		<attr name="property" type="int"></attr>
		<attr name="setting" type="mixed"></attr>
	</tag>
	<tag name="openal_source_stop" type="function" returnType="bool">
		<attr name="source" type="resource"></attr>
	</tag>
	<tag name="openal_stream" type="function" returnType="resource">
		<attr name="source" type="resource"></attr>
		<attr name="format" type="int"></attr>
		<attr name="rate" type="int"></attr>
	</tag>
</tags>