<!DOCTYPE tags>
<tags>
	<tag name="gmp_abs" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_add" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
		<attr name="b" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_and" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
		<attr name="b" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_clrbit" type="function" returnType="void" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="&amp;a" type="resource" status="required"></attr>
		<attr name="index" type="int" status="required"></attr>
	</tag>
	<tag name="gmp_cmp" type="function" returnType="int" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
		<attr name="b" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_com" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_div_q" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
		<attr name="b" type="resource" status="required"></attr>
		<attr name="round" type="int" status="optional"></attr>
	</tag>
	<tag name="gmp_div_qr" type="function" returnType="array" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="n" type="resource" status="required"></attr>
		<attr name="d" type="resource" status="required"></attr>
		<attr name="round" type="int" status="optional"></attr>
	</tag>
	<tag name="gmp_div_r" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="n" type="resource" status="required"></attr>
		<attr name="d" type="resource" status="required"></attr>
		<attr name="round" type="int" status="optional"></attr>
	</tag>
	<tag name="gmp_divexact" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="n" type="resource" status="required"></attr>
		<attr name="d" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_fact" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="int" status="required"></attr>
	</tag>
	<tag name="gmp_gcd" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
		<attr name="b" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_gcdext" type="function" returnType="array" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
		<attr name="b" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_hamdist" type="function" returnType="int" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
		<attr name="b" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_init" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="number" type="mixed" status="required"></attr>
		<attr name="base" type="int" status="optional"></attr>
	</tag>
	<tag name="gmp_intval" type="function" returnType="int" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="gmpnumber" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_invert" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
		<attr name="b" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_jacobi" type="function" returnType="int" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
		<attr name="p" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_legendre" type="function" returnType="int" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
		<attr name="p" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_mod" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="n" type="resource" status="required"></attr>
		<attr name="d" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_mul" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
		<attr name="b" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_neg" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_or" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
		<attr name="b" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_perfect_square" type="function" returnType="bool" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_popcount" type="function" returnType="int" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_pow" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="base" type="resource" status="required"></attr>
		<attr name="exp" type="int" status="required"></attr>
	</tag>
	<tag name="gmp_powm" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="base" type="resource" status="required"></attr>
		<attr name="exp" type="resource" status="required"></attr>
		<attr name="mod" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_prob_prime" type="function" returnType="int" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
		<attr name="reps" type="int" status="optional"></attr>
	</tag>
	<tag name="gmp_random" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="limiter" type="int" status="required"></attr>
	</tag>
	<tag name="gmp_scan0" type="function" returnType="int" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
		<attr name="start" type="int" status="required"></attr>
	</tag>
	<tag name="gmp_scan1" type="function" returnType="int" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
		<attr name="start" type="int" status="required"></attr>
	</tag>
	<tag name="gmp_setbit" type="function" returnType="void" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="&amp;a" type="resource" status="required"></attr>
		<attr name="index" type="int" status="required"></attr>
		<attr name="set_clear" type="bool" status="optional"></attr>
	</tag>
	<tag name="gmp_sign" type="function" returnType="int" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_sqrt" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_sqrtrem" type="function" returnType="array" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_strval" type="function" returnType="string" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="gmpnumber" type="resource" status="required"></attr>
		<attr name="base" type="int" status="optional"></attr>
	</tag>
	<tag name="gmp_sub" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
		<attr name="b" type="resource" status="required"></attr>
	</tag>
	<tag name="gmp_xor" type="function" returnType="resource" version="PHP 4 &gt;= 4.0.4, PHP 5">
		<attr name="a" type="resource" status="required"></attr>
		<attr name="b" type="resource" status="required"></attr>
	</tag>
</tags>