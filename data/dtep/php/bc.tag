<!DOCTYPE tags>
<tags>
	<tag name="bcadd" type="function" returnType="string">
		<attr name="left_operand" type="string"></attr>
		<attr name="right_operand" type="string"></attr>
		<attr name="scale" type="int" status="optional"></attr>
	</tag>
	<tag name="bccomp" type="function" returnType="int">
		<attr name="left_operand" type="string"></attr>
		<attr name="right_operand" type="string"></attr>
		<attr name="scale" type="int" status="optional"></attr>
	</tag>
	<tag name="bcdiv" type="function" returnType="string">
		<attr name="left_operand" type="string"></attr>
		<attr name="right_operand" type="string"></attr>
		<attr name="scale" type="int" status="optional"></attr>
	</tag>
	<tag name="bcmod" type="function" returnType="string">
		<attr name="left_operand" type="string"></attr>
		<attr name="modulus" type="string"></attr>
	</tag>
	<tag name="bcmul" type="function" returnType="string">
		<attr name="left_operand" type="string"></attr>
		<attr name="right_operand" type="string"></attr>
		<attr name="scale" type="int" status="optional"></attr>
	</tag>
	<tag name="bcpow" type="function" returnType="string">
		<attr name="x" type="string"></attr>
		<attr name="y" type="string"></attr>
		<attr name="scale" type="int" status="optional"></attr>
	</tag>
	<tag name="bcpowmod" type="function" returnType="string">
		<attr name="x" type="string"></attr>
		<attr name="y" type="string"></attr>
		<attr name="modulus" type="string"></attr>
		<attr name="scale" type="int" status="optional"></attr>
	</tag>
	<tag name="bcscale" type="function" returnType="bool">
		<attr name="scale" type="int"></attr>
	</tag>
	<tag name="bcsqrt" type="function" returnType="string">
		<attr name="operand" type="string"></attr>
		<attr name="scale" type="int" status="optional"></attr>
	</tag>
	<tag name="bcsub" type="function" returnType="string">
		<attr name="left_operand" type="string"></attr>
		<attr name="right_operand" type="string"></attr>
		<attr name="scale" type="int" status="optional"></attr>
	</tag>
</tags>