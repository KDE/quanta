<!DOCTYPE tags>
<tags>
	<tag name="bcadd" type="function" returnType="string" comment="PHP 3, PHP 4 , PHP 5">
		<attr name="left_operand" type="string" status="required"></attr>
		<attr name="right_operand" type="string" status="required"></attr>
		<attr name="scale" type="int" status="optional"></attr>
	</tag>
	<tag name="bccomp" type="function" returnType="int" comment="PHP 3, PHP 4 , PHP 5">
		<attr name="left_operand" type="string" status="required"></attr>
		<attr name="right_operand" type="string" status="required"></attr>
		<attr name="scale" type="int" status="optional"></attr>
	</tag>
	<tag name="bcdiv" type="function" returnType="string" comment="PHP 3, PHP 4 , PHP 5">
		<attr name="left_operand" type="string" status="required"></attr>
		<attr name="right_operand" type="string" status="required"></attr>
		<attr name="scale" type="int" status="optional"></attr>
	</tag>
	<tag name="bcmod" type="function" returnType="string" comment="PHP 3, PHP 4 , PHP 5">
		<attr name="left_operand" type="string" status="required"></attr>
		<attr name="modulus" type="string" status="required"></attr>
	</tag>
	<tag name="bcmul" type="function" returnType="string" comment="PHP 3, PHP 4 , PHP 5">
		<attr name="left_operand" type="string" status="required"></attr>
		<attr name="right_operand" type="string" status="required"></attr>
		<attr name="scale" type="int" status="optional"></attr>
	</tag>
	<tag name="bcpow" type="function" returnType="string" comment="PHP 3, PHP 4 , PHP 5">
		<attr name="x" type="string" status="required"></attr>
		<attr name="y" type="string" status="required"></attr>
		<attr name="scale" type="int" status="optional"></attr>
	</tag>
	<tag name="bcpowmod" type="function" returnType="string" comment="PHP 5">
		<attr name="x" type="string" status="required"></attr>
		<attr name="y" type="string" status="required"></attr>
		<attr name="modulus" type="string" status="required"></attr>
		<attr name="scale" type="int" status="optional"></attr>
	</tag>
	<tag name="bcscale" type="function" returnType="bool" comment="PHP 3, PHP 4 , PHP 5">
		<attr name="scale" type="int" status="required"></attr>
	</tag>
	<tag name="bcsqrt" type="function" returnType="string" comment="PHP 3, PHP 4 , PHP 5">
		<attr name="operand" type="string" status="required"></attr>
		<attr name="scale" type="int" status="optional"></attr>
	</tag>
	<tag name="bcsub" type="function" returnType="string" comment="PHP 3, PHP 4 , PHP 5">
		<attr name="left_operand" type="string" status="required"></attr>
		<attr name="right_operand" type="string" status="required"></attr>
		<attr name="scale" type="int" status="optional"></attr>
	</tag>
</tags>