<!DOCTYPE tags>
<tags>
	<tag name="ftok" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="pathname" type="string" status="required"></attr>
		<attr name="proj" type="string" status="required"></attr>
	</tag>
	<tag name="msg_get_queue" type="function" returnType="resource" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="key" type="int" status="required"></attr>
		<attr name="perms" type="int" status="optional"></attr>
	</tag>
	<tag name="msg_receive" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="queue" type="resource" status="required"></attr>
		<attr name="desiredmsgtype" type="int" status="required"></attr>
		<attr name="&amp;msgtype" type="int" status="required"></attr>
		<attr name="maxsize" type="int" status="required"></attr>
		<attr name="&amp;message" type="mixed" status="required"></attr>
		<attr name="unserialize" type="bool" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="&amp;errorcode" type="int" status="optional"></attr>
	</tag>
	<tag name="msg_remove_queue" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="queue" type="resource" status="required"></attr>
	</tag>
	<tag name="msg_send" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="queue" type="resource" status="required"></attr>
		<attr name="msgtype" type="int" status="required"></attr>
		<attr name="message" type="mixed" status="required"></attr>
		<attr name="serialize" type="bool" status="optional"></attr>
		<attr name="blocking" type="bool" status="optional"></attr>
		<attr name="&amp;errorcode" type="int" status="optional"></attr>
	</tag>
	<tag name="msg_set_queue" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="queue" type="resource" status="required"></attr>
		<attr name="data" type="array" status="required"></attr>
	</tag>
	<tag name="msg_stat_queue" type="function" returnType="array" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="queue" type="resource" status="required"></attr>
	</tag>
	<tag name="sem_acquire" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="sem_identifier" type="resource" status="required"></attr>
	</tag>
	<tag name="sem_get" type="function" returnType="resource" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="key" type="int" status="required"></attr>
		<attr name="max_acquire" type="int" status="optional"></attr>
		<attr name="perm" type="int" status="optional"></attr>
		<attr name="auto_release" type="int" status="optional"></attr>
	</tag>
	<tag name="sem_release" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="sem_identifier" type="resource" status="required"></attr>
	</tag>
	<tag name="sem_remove" type="function" returnType="bool" version="PHP 4 &gt;= 4.1.0, PHP 5">
		<attr name="sem_identifier" type="resource" status="required"></attr>
	</tag>
	<tag name="shm_attach" type="function" returnType="int" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="key" type="int" status="required"></attr>
		<attr name="memsize" type="int" status="optional"></attr>
		<attr name="perm" type="int" status="optional"></attr>
	</tag>
	<tag name="shm_detach" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="shm_identifier" type="int" status="required"></attr>
	</tag>
	<tag name="shm_get_var" type="function" returnType="mixed" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="shm_identifier" type="int" status="required"></attr>
		<attr name="variable_key" type="int" status="required"></attr>
	</tag>
	<tag name="shm_put_var" type="function" returnType="bool" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="shm_identifier" type="int" status="required"></attr>
		<attr name="variable_key" type="int" status="required"></attr>
		<attr name="variable" type="mixed" status="required"></attr>
	</tag>
	<tag name="shm_remove_var" type="function" returnType="int" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="shm_identifier" type="int" status="required"></attr>
		<attr name="variable_key" type="int" status="required"></attr>
	</tag>
	<tag name="shm_remove" type="function" returnType="int" version="PHP 3&gt;= 3.0.6, PHP 4 , PHP 5">
		<attr name="shm_identifier" type="int" status="required"></attr>
	</tag>
</tags>