<!DOCTYPE tags>
<tags>
	<tag name="openal_buffer_create" type="function" returnType="resource" version="">
	</tag>
	<tag name="openal_buffer_data" type="function" returnType="bool" version="">
		<attr name="buffer" type="resource" status="required"></attr>
		<attr name="format" type="int" status="required"></attr>
		<attr name="data" type="string" status="required"></attr>
		<attr name="freq" type="int" status="required"></attr>
	</tag>
	<tag name="openal_buffer_destroy" type="function" returnType="bool" version="">
		<attr name="buffer" type="resource" status="required"></attr>
	</tag>
	<tag name="openal_buffer_get" type="function" returnType="int" version="">
		<attr name="buffer" type="resource" status="required"></attr>
		<attr name="property" type="int" status="required"></attr>
	</tag>
	<tag name="openal_buffer_loadwav" type="function" returnType="bool" version="">
		<attr name="buffer" type="resource" status="required"></attr>
		<attr name="wavfile" type="string" status="required"></attr>
	</tag>
	<tag name="openal_context_create" type="function" returnType="resource" version="">
		<attr name="device" type="resource" status="required"></attr>
	</tag>
	<tag name="openal_context_current" type="function" returnType="bool" version="">
		<attr name="context" type="resource" status="required"></attr>
	</tag>
	<tag name="openal_context_destroy" type="function" returnType="bool" version="">
		<attr name="context" type="resource" status="required"></attr>
	</tag>
	<tag name="openal_context_process" type="function" returnType="bool" version="">
		<attr name="context" type="resource" status="required"></attr>
	</tag>
	<tag name="openal_context_suspend" type="function" returnType="bool" version="">
		<attr name="context" type="resource" status="required"></attr>
	</tag>
	<tag name="openal_device_close" type="function" returnType="bool" version="">
		<attr name="device" type="resource" status="required"></attr>
	</tag>
	<tag name="openal_device_open" type="function" returnType="resource" version="">
		<attr name="device_desc" type="string" status="optional"></attr>
	</tag>
	<tag name="openal_listener_get" type="function" returnType="mixed" version="">
		<attr name="property" type="int" status="required"></attr>
	</tag>
	<tag name="openal_listener_set" type="function" returnType="bool" version="">
		<attr name="property" type="int" status="required"></attr>
		<attr name="setting" type="mixed" status="required"></attr>
	</tag>
	<tag name="openal_source_create" type="function" returnType="resource" version="">
	</tag>
	<tag name="openal_source_destroy" type="function" returnType="resource" version="">
		<attr name="source" type="resource" status="required"></attr>
	</tag>
	<tag name="openal_source_get" type="function" returnType="mixed" version="">
		<attr name="source" type="resource" status="required"></attr>
		<attr name="property" type="int" status="required"></attr>
	</tag>
	<tag name="openal_source_pause" type="function" returnType="bool" version="">
		<attr name="source" type="resource" status="required"></attr>
	</tag>
	<tag name="openal_source_play" type="function" returnType="bool" version="">
		<attr name="source" type="resource" status="required"></attr>
	</tag>
	<tag name="openal_source_rewind" type="function" returnType="bool" version="">
		<attr name="source" type="resource" status="required"></attr>
	</tag>
	<tag name="openal_source_set" type="function" returnType="bool" version="">
		<attr name="source" type="resource" status="required"></attr>
		<attr name="property" type="int" status="required"></attr>
		<attr name="setting" type="mixed" status="required"></attr>
	</tag>
	<tag name="openal_source_stop" type="function" returnType="bool" version="">
		<attr name="source" type="resource" status="required"></attr>
	</tag>
	<tag name="openal_stream" type="function" returnType="resource" version="">
		<attr name="source" type="resource" status="required"></attr>
		<attr name="format" type="int" status="required"></attr>
		<attr name="rate" type="int" status="required"></attr>
	</tag>
</tags>