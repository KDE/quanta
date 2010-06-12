<!DOCTYPE tags>
<tags>	<tag name="ftok" type="function" returnType="int">
		<attr name="pathname" type="string" status="optional"></attr>
		<attr name="proj" type="string" status="optional"></attr>
	</tag>
	<tag name="msg_get_queue" type="function" returnType="int">
		<attr name="key" type="int" status="optional"></attr>
		<attr name="perms" type="int" status="optional"></attr>
	</tag>
	<tag name="msg_receive" type="function" returnType="bool">
		<attr name="queue" type="int" status="optional"></attr>
		<attr name="desiredmsgtype" type="int" status="optional"></attr>
		<attr name="msgtype" type="int" status="optional"></attr>
		<attr name="maxsize" type="int" status="optional"></attr>
		<attr name="message" type="mixed" status="optional"></attr>
		<attr name="unserialize" type="bool" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="errorcode" type="int" status="optional"></attr>
	</tag>
	<tag name="msg_remove_queue" type="function" returnType="bool">
		<attr name="queue" type="int" status="optional"></attr>
	</tag>
	<tag name="msg_send" type="function" returnType="bool">
		<attr name="queue" type="int" status="optional"></attr>
		<attr name="msgtype" type="int" status="optional"></attr>
		<attr name="message" type="mixed" status="optional"></attr>
		<attr name="serialize" type="bool" status="optional"></attr>
		<attr name="blocking" type="bool" status="optional"></attr>
		<attr name="errorcode" type="int" status="optional"></attr>
	</tag>
	<tag name="msg_set_queue" type="function" returnType="bool">
		<attr name="queue" type="int" status="optional"></attr>
		<attr name="data" type="array" status="optional"></attr>
	</tag>
	<tag name="msg_stat_queue" type="function" returnType="array">
		<attr name="queue" type="int" status="optional"></attr>
	</tag>
	<tag name="sem_acquire" type="function" returnType="bool">
		<attr name="sem_identifier" type="int" status="optional"></attr>
	</tag>
	<tag name="sem_get" type="function" returnType="int">
		<attr name="key" type="int" status="optional"></attr>
		<attr name="max_acquire" type="int" status="optional"></attr>
		<attr name="perm" type="int" status="optional"></attr>
	</tag>
	<tag name="sem_release" type="function" returnType="bool">
		<attr name="sem_identifier" type="int" status="optional"></attr>
	</tag>
	<tag name="sem_remove" type="function" returnType="bool">
		<attr name="sem_identifier" type="int" status="optional"></attr>
	</tag>
	<tag name="shm_attach" type="function" returnType="int">
		<attr name="key" type="int" status="optional"></attr>
		<attr name="memsize" type="int" status="optional"></attr>
		<attr name="perm" type="int" status="optional"></attr>
	</tag>
	<tag name="shm_detach" type="function" returnType="int">
		<attr name="shm_identifier" type="int" status="optional"></attr>
	</tag>
	<tag name="shm_get_var" type="function" returnType="mixed">
		<attr name="id" type="int" status="optional"></attr>
		<attr name="variable_key" type="int" status="optional"></attr>
	</tag>
	<tag name="shm_put_var" type="function" returnType="int">
		<attr name="shm_identifier" type="int" status="optional"></attr>
		<attr name="variable_key" type="int" status="optional"></attr>
		<attr name="variable" type="mixed" status="optional"></attr>
	</tag>
	<tag name="shm_remove_var" type="function" returnType="int">
		<attr name="id" type="int" status="optional"></attr>
		<attr name="variable_key" type="int" status="optional"></attr>
	</tag>
	<tag name="shm_remove" type="function" returnType="int">
		<attr name="shm_identifier" type="int" status="optional"></attr>
	</tag>
</tags>
