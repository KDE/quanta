<!DOCTYPE tags>
<tags>
	<tag name="gmp_abs" type="function" returnType="resource">
		<attr name="a" type="resource"></attr>
	</tag>
	<tag name="gmp_add" type="function" returnType="resource">
		<attr name="a" type="resource"></attr>
		<attr name="b" type="resource"></attr>
	</tag>
	<tag name="gmp_and" type="function" returnType="resource">
		<attr name="a" type="resource"></attr>
		<attr name="b" type="resource"></attr>
	</tag>
	<tag name="gmp_clrbit" type="function" returnType="void">
		<attr name="&amp;a" type="resource"></attr>
		<attr name="index" type="int"></attr>
	</tag>
	<tag name="gmp_cmp" type="function" returnType="int">
		<attr name="a" type="resource"></attr>
		<attr name="b" type="resource"></attr>
	</tag>
	<tag name="gmp_com" type="function" returnType="resource">
		<attr name="a" type="resource"></attr>
	</tag>
	<tag name="gmp_div_q" type="function" returnType="resource">
		<attr name="a" type="resource"></attr>
		<attr name="b" type="resource"></attr>
		<attr name="round" type="int" status="optional"></attr>
	</tag>
	<tag name="gmp_div_qr" type="function" returnType="array">
		<attr name="n" type="resource"></attr>
		<attr name="d" type="resource"></attr>
		<attr name="round" type="int" status="optional"></attr>
	</tag>
	<tag name="gmp_div_r" type="function" returnType="resource">
		<attr name="n" type="resource"></attr>
		<attr name="d" type="resource"></attr>
		<attr name="round" type="int" status="optional"></attr>
	</tag>
	<tag name="" type="function" returnType="">
	</tag>
	<tag name="gmp_divexact" type="function" returnType="resource">
		<attr name="n" type="resource"></attr>
		<attr name="d" type="resource"></attr>
	</tag>
	<tag name="gmp_fact" type="function" returnType="resource">
		<attr name="a" type="int"></attr>
	</tag>
	<tag name="gmp_gcd" type="function" returnType="resource">
		<attr name="a" type="resource"></attr>
		<attr name="b" type="resource"></attr>
	</tag>
	<tag name="gmp_gcdext" type="function" returnType="array">
		<attr name="a" type="resource"></attr>
		<attr name="b" type="resource"></attr>
	</tag>
	<tag name="gmp_hamdist" type="function" returnType="int">
		<attr name="a" type="resource"></attr>
		<attr name="b" type="resource"></attr>
	</tag>
	<tag name="gmp_init" type="function" returnType="resource">
		<attr name="number" type="mixed"></attr>
		<attr name="base" type="int" status="optional"></attr>
	</tag>
	<tag name="gmp_intval" type="function" returnType="int">
		<attr name="gmpnumber" type="resource"></attr>
	</tag>
	<tag name="gmp_invert" type="function" returnType="resource">
		<attr name="a" type="resource"></attr>
		<attr name="b" type="resource"></attr>
	</tag>
	<tag name="gmp_jacobi" type="function" returnType="int">
		<attr name="a" type="resource"></attr>
		<attr name="p" type="resource"></attr>
	</tag>
	<tag name="gmp_legendre" type="function" returnType="int">
		<attr name="a" type="resource"></attr>
		<attr name="p" type="resource"></attr>
	</tag>
	<tag name="gmp_mod" type="function" returnType="resource">
		<attr name="n" type="resource"></attr>
		<attr name="d" type="resource"></attr>
	</tag>
	<tag name="gmp_mul" type="function" returnType="resource">
		<attr name="a" type="resource"></attr>
		<attr name="b" type="resource"></attr>
	</tag>
	<tag name="gmp_neg" type="function" returnType="resource">
		<attr name="a" type="resource"></attr>
	</tag>
	<tag name="gmp_or" type="function" returnType="resource">
		<attr name="a" type="resource"></attr>
		<attr name="b" type="resource"></attr>
	</tag>
	<tag name="gmp_perfect_square" type="function" returnType="bool">
		<attr name="a" type="resource"></attr>
	</tag>
	<tag name="gmp_popcount" type="function" returnType="int">
		<attr name="a" type="resource"></attr>
	</tag>
	<tag name="gmp_pow" type="function" returnType="resource">
		<attr name="base" type="resource"></attr>
		<attr name="exp" type="int"></attr>
	</tag>
	<tag name="gmp_powm" type="function" returnType="resource">
		<attr name="base" type="resource"></attr>
		<attr name="exp" type="resource"></attr>
		<attr name="mod" type="resource"></attr>
	</tag>
	<tag name="gmp_prob_prime" type="function" returnType="int">
		<attr name="a" type="resource"></attr>
		<attr name="reps" type="int" status="optional"></attr>
	</tag>
	<tag name="gmp_random" type="function" returnType="resource">
		<attr name="limiter" type="int"></attr>
	</tag>
	<tag name="gmp_scan0" type="function" returnType="int">
		<attr name="a" type="resource"></attr>
		<attr name="start" type="int"></attr>
	</tag>
	<tag name="gmp_scan1" type="function" returnType="int">
		<attr name="a" type="resource"></attr>
		<attr name="start" type="int"></attr>
	</tag>
	<tag name="gmp_setbit" type="function" returnType="void">
		<attr name="&amp;a" type="resource"></attr>
		<attr name="index" type="int"></attr>
		<attr name="set_clear" type="bool" status="optional"></attr>
	</tag>
	<tag name="gmp_sign" type="function" returnType="int">
		<attr name="a" type="resource"></attr>
	</tag>
	<tag name="gmp_sqrt" type="function" returnType="resource">
		<attr name="a" type="resource"></attr>
	</tag>
	<tag name="gmp_sqrtrem" type="function" returnType="array">
		<attr name="a" type="resource"></attr>
	</tag>
	<tag name="gmp_strval" type="function" returnType="string">
		<attr name="gmpnumber" type="resource"></attr>
		<attr name="base" type="int" status="optional"></attr>
	</tag>
	<tag name="gmp_sub" type="function" returnType="resource">
		<attr name="a" type="resource"></attr>
		<attr name="b" type="resource"></attr>
	</tag>
	<tag name="gmp_xor" type="function" returnType="resource">
		<attr name="a" type="resource"></attr>
		<attr name="b" type="resource"></attr>
	</tag>
</tags>