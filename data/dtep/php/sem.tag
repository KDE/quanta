<!DOCTYPE tags>
<tags>
	<tag name="ftok" type="function" returnType="int">
		<attr name="pathname" type="string"></attr>
		<attr name="proj" type="string"></attr>
	</tag>
	<tag name="msg_get_queue" type="function" returnType="resource">
		<attr name="key" type="int"></attr>
		<attr name="perms" type="int" status="optional"></attr>
	</tag>
	<tag name="msg_receive" type="function" returnType="bool">
		<attr name="queue" type="resource"></attr>
		<attr name="desiredmsgtype" type="int"></attr>
		<attr name="&amp;msgtype" type="int"></attr>
		<attr name="maxsize" type="int"></attr>
		<attr name="&amp;message" type="mixed"></attr>
		<attr name="unserialize" type="bool" status="optional"></attr>
		<attr name="flags" type="int" status="optional"></attr>
		<attr name="&amp;errorcode" type="int" status="optional"></attr>
	</tag>
	<tag name="msg_remove_queue" type="function" returnType="bool">
		<attr name="queue" type="resource"></attr>
	</tag>
	<tag name="msg_send" type="function" returnType="bool">
		<attr name="queue" type="resource"></attr>
		<attr name="msgtype" type="int"></attr>
		<attr name="message" type="mixed"></attr>
		<attr name="serialize" type="bool" status="optional"></attr>
		<attr name="blocking" type="bool" status="optional"></attr>
		<attr name="&amp;errorcode" type="int" status="optional"></attr>
	</tag>
	<tag name="msg_set_queue" type="function" returnType="bool">
		<attr name="queue" type="resource"></attr>
		<attr name="data" type="array"></attr>
	</tag>
	<tag name="msg_stat_queue" type="function" returnType="array">
		<attr name="queue" type="resource"></attr>
	</tag>
	<tag name="sem_acquire" type="function" returnType="bool">
		<attr name="sem_identifier" type="resource"></attr>
	</tag>
	<tag name="sem_get" type="function" returnType="resource">
		<attr name="key" type="int"></attr>
		<attr name="max_acquire" type="int" status="optional"></attr>
		<attr name="perm" type="int" status="optional"></attr>
		<attr name="auto_release" type="int" status="optional"></attr>
	</tag>
	<tag name="sem_release" type="function" returnType="bool">
		<attr name="sem_identifier" type="resource"></attr>
	</tag>
	<tag name="sem_remove" type="function" returnType="bool">
		<attr name="sem_identifier" type="resource"></attr>
	</tag>
	<tag name="shm_attach" type="function" returnType="int">
		<attr name="key" type="int"></attr>
		<attr name="memsize" type="int" status="optional"></attr>
		<attr name="perm" type="int" status="optional"></attr>
	</tag>
	<tag name="shm_detach" type="function" returnType="bool">
		<attr name="shm_identifier" type="int"></attr>
	</tag>
	<tag name="shm_get_var" type="function" returnType="mixed">
		<attr name="shm_identifier" type="int"></attr>
		<attr name="variable_key" type="int"></attr>
	</tag>
	<tag name="shm_put_var" type="function" returnType="bool">
		<attr name="shm_identifier" type="int"></attr>
		<attr name="variable_key" type="int"></attr>
		<attr name="variable" type="mixed"></attr>
	</tag>
	<tag name="shm_remove_var" type="function" returnType="int">
		<attr name="shm_identifier" type="int"></attr>
		<attr name="variable_key" type="int"></attr>
	</tag>
	<tag name="shm_remove" type="function" returnType="int">
		<attr name="shm_identifier" type="int"></attr>
	</tag>
</tags>