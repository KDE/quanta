<!DOCTYPE tags>
<tags>
	<tag name="mcve_adduser" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="admin_password" type="string" status="required"></attr>
		<attr name="usersetup" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_adduserarg" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="usersetup" type="resource" status="required"></attr>
		<attr name="argtype" type="int" status="required"></attr>
		<attr name="argval" type="string" status="required"></attr>
	</tag>
	<tag name="mcve_bt" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
	</tag>
	<tag name="mcve_checkstatus" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_chkpwd" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
	</tag>
	<tag name="mcve_chngpwd" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="admin_password" type="string" status="required"></attr>
		<attr name="new_password" type="string" status="required"></attr>
	</tag>
	<tag name="mcve_completeauthorizations" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="&amp;array" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_connect" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
	</tag>
	<tag name="mcve_connectionerror" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
	</tag>
	<tag name="mcve_deleteresponse" type="function" returnType="bool" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_deletetrans" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_deleteusersetup" type="function" returnType="void" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="usersetup" type="resource" status="required"></attr>
	</tag>
	<tag name="mcve_deluser" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="admin_password" type="string" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
	</tag>
	<tag name="mcve_destroyconn" type="function" returnType="void" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
	</tag>
	<tag name="mcve_destroyengine" type="function" returnType="void" version="PHP 4 &gt;= 4.2.0, PHP 5">
	</tag>
	<tag name="mcve_disableuser" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="admin_password" type="string" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
	</tag>
	<tag name="mcve_edituser" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="admin_password" type="string" status="required"></attr>
		<attr name="usersetup" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_enableuser" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="admin_password" type="string" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
	</tag>
	<tag name="mcve_force" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="trackdata" type="string" status="required"></attr>
		<attr name="account" type="string" status="required"></attr>
		<attr name="expdate" type="string" status="required"></attr>
		<attr name="amount" type="float" status="required"></attr>
		<attr name="authcode" type="string" status="required"></attr>
		<attr name="comments" type="string" status="required"></attr>
		<attr name="clerkid" type="string" status="required"></attr>
		<attr name="stationid" type="string" status="required"></attr>
		<attr name="ptrannum" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_getcell" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
		<attr name="column" type="string" status="required"></attr>
		<attr name="row" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_getcellbynum" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
		<attr name="column" type="int" status="required"></attr>
		<attr name="row" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_getcommadelimited" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_getheader" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
		<attr name="column_num" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_getuserarg" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="usersetup" type="resource" status="required"></attr>
		<attr name="argtype" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_getuserparam" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
		<attr name="key" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_gft" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="type" type="int" status="required"></attr>
		<attr name="account" type="string" status="required"></attr>
		<attr name="clerkid" type="string" status="required"></attr>
		<attr name="stationid" type="string" status="required"></attr>
		<attr name="comments" type="string" status="required"></attr>
		<attr name="ptrannum" type="int" status="required"></attr>
		<attr name="startdate" type="string" status="required"></attr>
		<attr name="enddate" type="string" status="required"></attr>
	</tag>
	<tag name="mcve_gl" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="int" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="type" type="int" status="required"></attr>
		<attr name="account" type="string" status="required"></attr>
		<attr name="batch" type="string" status="required"></attr>
		<attr name="clerkid" type="string" status="required"></attr>
		<attr name="stationid" type="string" status="required"></attr>
		<attr name="comments" type="string" status="required"></attr>
		<attr name="ptrannum" type="int" status="required"></attr>
		<attr name="startdate" type="string" status="required"></attr>
		<attr name="enddate" type="string" status="required"></attr>
	</tag>
	<tag name="mcve_gut" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="type" type="int" status="required"></attr>
		<attr name="account" type="string" status="required"></attr>
		<attr name="clerkid" type="string" status="required"></attr>
		<attr name="stationid" type="string" status="required"></attr>
		<attr name="comments" type="string" status="required"></attr>
		<attr name="ptrannum" type="int" status="required"></attr>
		<attr name="startdate" type="string" status="required"></attr>
		<attr name="enddate" type="string" status="required"></attr>
	</tag>
	<tag name="mcve_initconn" type="function" returnType="resource" version="PHP 4 &gt;= 4.2.0, PHP 5">
	</tag>
	<tag name="mcve_initengine" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="location" type="string" status="required"></attr>
	</tag>
	<tag name="mcve_initusersetup" type="function" returnType="resource" version="PHP 4 &gt;= 4.2.0, PHP 5">
	</tag>
	<tag name="mcve_iscommadelimited" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_liststats" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="admin_password" type="string" status="required"></attr>
	</tag>
	<tag name="mcve_listusers" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="admin_password" type="string" status="required"></attr>
	</tag>
	<tag name="mcve_maxconntimeout" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="secs" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_monitor" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
	</tag>
	<tag name="mcve_numcolumns" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_numrows" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_override" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="trackdata" type="string" status="required"></attr>
		<attr name="account" type="string" status="required"></attr>
		<attr name="expdate" type="string" status="required"></attr>
		<attr name="amount" type="float" status="required"></attr>
		<attr name="street" type="string" status="required"></attr>
		<attr name="zip" type="string" status="required"></attr>
		<attr name="cv" type="string" status="required"></attr>
		<attr name="comments" type="string" status="required"></attr>
		<attr name="clerkid" type="string" status="required"></attr>
		<attr name="stationid" type="string" status="required"></attr>
		<attr name="ptrannum" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_parsecommadelimited" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_ping" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
	</tag>
	<tag name="mcve_preauth" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="trackdata" type="string" status="required"></attr>
		<attr name="account" type="string" status="required"></attr>
		<attr name="expdate" type="string" status="required"></attr>
		<attr name="amount" type="float" status="required"></attr>
		<attr name="street" type="string" status="required"></attr>
		<attr name="zip" type="string" status="required"></attr>
		<attr name="cv" type="string" status="required"></attr>
		<attr name="comments" type="string" status="required"></attr>
		<attr name="clerkid" type="string" status="required"></attr>
		<attr name="stationid" type="string" status="required"></attr>
		<attr name="ptrannum" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_preauthcompletion" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="finalamount" type="float" status="required"></attr>
		<attr name="sid" type="int" status="required"></attr>
		<attr name="ptrannum" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_qc" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="clerkid" type="string" status="required"></attr>
		<attr name="stationid" type="string" status="required"></attr>
		<attr name="comments" type="string" status="required"></attr>
		<attr name="ptrannum" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_responseparam" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
		<attr name="key" type="string" status="required"></attr>
	</tag>
	<tag name="mcve_return" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="int" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="trackdata" type="string" status="required"></attr>
		<attr name="account" type="string" status="required"></attr>
		<attr name="expdate" type="string" status="required"></attr>
		<attr name="amount" type="float" status="required"></attr>
		<attr name="comments" type="string" status="required"></attr>
		<attr name="clerkid" type="string" status="required"></attr>
		<attr name="stationid" type="string" status="required"></attr>
		<attr name="ptrannum" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_returncode" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_returnstatus" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_sale" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="trackdata" type="string" status="required"></attr>
		<attr name="account" type="string" status="required"></attr>
		<attr name="expdate" type="string" status="required"></attr>
		<attr name="amount" type="float" status="required"></attr>
		<attr name="street" type="string" status="required"></attr>
		<attr name="zip" type="string" status="required"></attr>
		<attr name="cv" type="string" status="required"></attr>
		<attr name="comments" type="string" status="required"></attr>
		<attr name="clerkid" type="string" status="required"></attr>
		<attr name="stationid" type="string" status="required"></attr>
		<attr name="ptrannum" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_setblocking" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="tf" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_setdropfile" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="directory" type="string" status="required"></attr>
	</tag>
	<tag name="mcve_setip" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="host" type="string" status="required"></attr>
		<attr name="port" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_setssl_files" type="function" returnType="int" version="PHP 4 &gt;= 4.3.3, PHP 5">
		<attr name="sslkeyfile" type="string" status="required"></attr>
		<attr name="sslcertfile" type="string" status="required"></attr>
	</tag>
	<tag name="mcve_setssl" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="host" type="string" status="required"></attr>
		<attr name="port" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_settimeout" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="seconds" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_settle" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="batch" type="string" status="required"></attr>
	</tag>
	<tag name="mcve_text_avs" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="code" type="string" status="required"></attr>
	</tag>
	<tag name="mcve_text_code" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="code" type="string" status="required"></attr>
	</tag>
	<tag name="mcve_text_cv" type="function" returnType="string" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="code" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_transactionauth" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_transactionavs" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_transactionbatch" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_transactioncv" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_transactionid" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_transactionitem" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_transactionssent" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
	</tag>
	<tag name="mcve_transactiontext" type="function" returnType="string" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_transinqueue" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
	</tag>
	<tag name="mcve_transnew" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
	</tag>
	<tag name="mcve_transparam" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
		<attr name="key" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_transsend" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="identifier" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_ub" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
	</tag>
	<tag name="mcve_uwait" type="function" returnType="int" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="microsecs" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_verifyconnection" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="tf" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_verifysslcert" type="function" returnType="bool" version="PHP 4 &gt;= 4.3.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="tf" type="int" status="required"></attr>
	</tag>
	<tag name="mcve_void" type="function" returnType="int" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="conn" type="resource" status="required"></attr>
		<attr name="username" type="string" status="required"></attr>
		<attr name="password" type="string" status="required"></attr>
		<attr name="sid" type="int" status="required"></attr>
		<attr name="ptrannum" type="int" status="required"></attr>
	</tag>
</tags>