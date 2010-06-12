<!DOCTYPE tags>
<tags>	<tag name="abs" type="function" returnType="mixed">
		<attr name="number" type="mixed" status="optional"></attr>
	</tag>
	<tag name="acos" type="function" returnType="float">
		<attr name="arg" type="float" status="optional"></attr>
	</tag>
	<tag name="acosh" type="function" returnType="float">
		<attr name="arg" type="float" status="optional"></attr>
	</tag>
	<tag name="asin" type="function" returnType="float">
		<attr name="arg" type="float" status="optional"></attr>
	</tag>
	<tag name="asinh" type="function" returnType="float">
		<attr name="arg" type="float" status="optional"></attr>
	</tag>
	<tag name="atan2" type="function" returnType="float">
		<attr name="y" type="float" status="optional"></attr>
		<attr name="x" type="float" status="optional"></attr>
	</tag>
	<tag name="atan" type="function" returnType="float">
		<attr name="arg" type="float" status="optional"></attr>
	</tag>
	<tag name="atanh" type="function" returnType="float">
		<attr name="arg" type="float" status="optional"></attr>
	</tag>
	<tag name="base_convert" type="function" returnType="string">
		<attr name="number" type="string" status="optional"></attr>
		<attr name="frombase" type="int" status="optional"></attr>
		<attr name="tobase" type="int" status="optional"></attr>
	</tag>
	<tag name="bindec" type="function" returnType="int">
		<attr name="binary_string" type="string" status="optional"></attr>
	</tag>
	<tag name="ceil" type="function" returnType="float">
		<attr name="value" type="float" status="optional"></attr>
	</tag>
	<tag name="cos" type="function" returnType="float">
		<attr name="arg" type="float" status="optional"></attr>
	</tag>
	<tag name="cosh" type="function" returnType="float">
		<attr name="arg" type="float" status="optional"></attr>
	</tag>
	<tag name="decbin" type="function" returnType="string">
		<attr name="number" type="int" status="optional"></attr>
	</tag>
	<tag name="dechex" type="function" returnType="string">
		<attr name="number" type="int" status="optional"></attr>
	</tag>
	<tag name="decoct" type="function" returnType="string">
		<attr name="number" type="int" status="optional"></attr>
	</tag>
	<tag name="deg2rad" type="function" returnType="float">
		<attr name="number" type="float" status="optional"></attr>
	</tag>
	<tag name="exp" type="function" returnType="float">
		<attr name="arg" type="float" status="optional"></attr>
	</tag>
	<tag name="expm1" type="function" returnType="float">
		<attr name="number" type="float" status="optional"></attr>
	</tag>
	<tag name="floor" type="function" returnType="float">
		<attr name="value" type="float" status="optional"></attr>
	</tag>
	<tag name="getrandmax" type="function" returnType="int">
	</tag>
	<tag name="hexdec" type="function" returnType="int">
		<attr name="hex_string" type="string" status="optional"></attr>
	</tag>
	<tag name="hypot" type="function" returnType="float">
		<attr name="num1" type="float" status="optional"></attr>
		<attr name="num2" type="float" status="optional"></attr>
	</tag>
	<tag name="is_finite" type="function" returnType="bool">
		<attr name="val" type="float" status="optional"></attr>
	</tag>
	<tag name="is_infinite" type="function" returnType="bool">
		<attr name="val" type="float" status="optional"></attr>
	</tag>
	<tag name="is_nan" type="function" returnType="bool">
		<attr name="val" type="float" status="optional"></attr>
	</tag>
	<tag name="lcg_value" type="function" returnType="float">
	</tag>
	<tag name="log10" type="function" returnType="float">
		<attr name="arg" type="float" status="optional"></attr>
	</tag>
	<tag name="log1p" type="function" returnType="float">
		<attr name="number" type="float" status="optional"></attr>
	</tag>
	<tag name="log" type="function" returnType="float">
		<attr name="arg" type="float" status="optional"></attr>
	</tag>
	<tag name="max" type="function" returnType="mixed">
		<attr name="arg1" type="mixed" status="optional"></attr>
		<attr name="arg2" type="mixed" status="optional"></attr>
		<attr name="argn" type="mixed" status="optional"></attr>
	</tag>
	<tag name="min" type="function" returnType="number">
		<attr name="arg1" type="number" status="optional"></attr>
		<attr name="arg2" type="number" status="optional"></attr>
	</tag>
	<tag name="mt_getrandmax" type="function" returnType="int">
	</tag>
	<tag name="mt_rand" type="function" returnType="int">
		<attr name="min" type="int" status="optional"></attr>
		<attr name="max" type="int" status="optional"></attr>
	</tag>
	<tag name="mt_srand" type="function" returnType="void">
		<attr name="seed" type="int" status="optional"></attr>
	</tag>
	<tag name="number_format" type="function" returnType="string">
		<attr name="number" type="float" status="optional"></attr>
		<attr name="decimals" type="int" status="optional"></attr>
		<attr name="dec_point" type="string" status="optional"></attr>
		<attr name="thousands_sep" type="string" status="optional"></attr>
	</tag>
	<tag name="octdec" type="function" returnType="int">
		<attr name="octal_string" type="string" status="optional"></attr>
	</tag>
	<tag name="pi" type="function" returnType="float">
	</tag>
	<tag name="pow" type="function" returnType="number">
		<attr name="base" type="number" status="optional"></attr>
		<attr name="exp" type="number" status="optional"></attr>
	</tag>
	<tag name="rad2deg" type="function" returnType="float">
		<attr name="number" type="float" status="optional"></attr>
	</tag>
	<tag name="rand" type="function" returnType="int">
		<attr name="min" type="int" status="optional"></attr>
		<attr name="max" type="int" status="optional"></attr>
	</tag>
	<tag name="round" type="function" returnType="float">
		<attr name="val" type="float" status="optional"></attr>
		<attr name="precision" type="int" status="optional"></attr>
	</tag>
	<tag name="sin" type="function" returnType="float">
		<attr name="arg" type="float" status="optional"></attr>
	</tag>
	<tag name="sinh" type="function" returnType="float">
		<attr name="arg" type="float" status="optional"></attr>
	</tag>
	<tag name="sqrt" type="function" returnType="float">
		<attr name="arg" type="float" status="optional"></attr>
	</tag>
	<tag name="srand" type="function" returnType="void">
		<attr name="seed" type="int" status="optional"></attr>
	</tag>
	<tag name="tan" type="function" returnType="float">
		<attr name="arg" type="float" status="optional"></attr>
	</tag>
	<tag name="tanh" type="function" returnType="float">
		<attr name="arg" type="float" status="optional"></attr>
	</tag>
</tags>
