<!DOCTYPE tags>
<tags>
	<tag name="Memcache::add" type="function" returnType="bool">
		<attr name="key" type="string"></attr>
		<attr name="var" type="mixed"></attr>
		<attr name="flag" type="int" status="optional"></attr>
		<attr name="expire" type="int" status="optional"></attr>
	</tag>
	<tag name="Memcache::close" type="function" returnType="bool">
	</tag>
	<tag name="Memcache::connect" type="function" returnType="bool">
		<attr name="host" type="string"></attr>
		<attr name="port" type="int" status="optional"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
	</tag>
	<tag name="memcache_debug" type="function" returnType="bool">
		<attr name="on_off" type="int"></attr>
	</tag>
	<tag name="Memcache::decrement" type="function" returnType="int">
		<attr name="key" type="string"></attr>
		<attr name="value" type="int" status="optional"></attr>
	</tag>
	<tag name="Memcache::delete" type="function" returnType="bool">
		<attr name="key" type="string"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
	</tag>
	<tag name="Memcache::flush" type="function" returnType="bool">
	</tag>
	<tag name="Memcache::get" type="function" returnType="mixed">
		<attr name="key" type="string"></attr>
	</tag>
	<tag name="Memcache::getStats" type="function" returnType="array">
	</tag>
	<tag name="Memcache::getVersion" type="function" returnType="string">
	</tag>
	<tag name="Memcache::increment" type="function" returnType="int">
		<attr name="key" type="string"></attr>
		<attr name="value" type="int" status="optional"></attr>
	</tag>
	<tag name="Memcache::pconnect" type="function" returnType="bool">
		<attr name="host" type="string"></attr>
		<attr name="port" type="int" status="optional"></attr>
		<attr name="timeout" type="int" status="optional"></attr>
	</tag>
	<tag name="Memcache::replace" type="function" returnType="bool">
		<attr name="key" type="string"></attr>
		<attr name="var" type="mixed"></attr>
		<attr name="flag" type="int" status="optional"></attr>
		<attr name="expire" type="int" status="optional"></attr>
	</tag>
	<tag name="Memcache::set" type="function" returnType="bool">
		<attr name="key" type="string"></attr>
		<attr name="var" type="mixed"></attr>
		<attr name="flag" type="int" status="optional"></attr>
		<attr name="expire" type="int" status="optional"></attr>
	</tag>
</tags>