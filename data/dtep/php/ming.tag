<!DOCTYPE tags>
<tags>
	<tag name="ming_setcubicthreshold" type="function" returnType="void" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="threshold" type="int" status="required"></attr>
	</tag>
	<tag name="ming_setscale" type="function" returnType="void" version="PHP 4 &gt;= 4.0.5, PHP 5">
		<attr name="scale" type="int" status="required"></attr>
	</tag>
	<tag name="ming_useswfversion" type="function" returnType="void" version="PHP 4 &gt;= 4.2.0, PHP 5">
		<attr name="version" type="int" status="required"></attr>
	</tag>
	<tag name="swfaction" type="function" returnType="object" version="PHP 4 &gt;= 4.0.5">
		<attr name="script" type="string" status="required"></attr>
	</tag>
	<tag name="swfbitmap->getheight" type="function" returnType="int" version="">
	</tag>
	<tag name="swfbitmap->getwidth" type="function" returnType="int" version="">
	</tag>
	<tag name="swfbitmap" type="function" returnType="object" version="PHP 4 &gt;= 4.0.5">
		<attr name="file" type="mixed" status="required"></attr>
		<attr name="alphafile" type="mixed" status="optional"></attr>
	</tag>
	<tag name="swfbutton_keypress" type="function" returnType="int" version="PHP 4 &gt;= 4.0.5">
		<attr name="str" type="string" status="required"></attr>
	</tag>
	<tag name="swfbutton->addaction" type="function" returnType="void" version="">
		<attr name="action" type="resource" status="required"></attr>
		<attr name="flags" type="int" status="required"></attr>
	</tag>
	<tag name="swfbutton->addshape" type="function" returnType="void" version="">
		<attr name="shape" type="resource" status="required"></attr>
		<attr name="flags" type="int" status="required"></attr>
	</tag>
	<tag name="swfbutton->setaction" type="function" returnType="void" version="">
		<attr name="action" type="resource" status="required"></attr>
	</tag>
	<tag name="swfbutton->setdown" type="function" returnType="void" version="">
		<attr name="shape" type="resource" status="required"></attr>
	</tag>
	<tag name="swfbutton->sethit" type="function" returnType="void" version="">
		<attr name="shape" type="resource" status="required"></attr>
	</tag>
	<tag name="swfbutton->setover" type="function" returnType="void" version="">
		<attr name="shape" type="resource" status="required"></attr>
	</tag>
	<tag name="swfbutton->setup" type="function" returnType="void" version="">
		<attr name="shape" type="resource" status="required"></attr>
	</tag>
	<tag name="swfbutton" type="function" returnType="object" version="PHP 4 &gt;= 4.0.5">
	</tag>
	<tag name="swfdisplayitem->addcolor" type="function" returnType="void" version="">
		<attr name="red" type="int" status="optional"></attr>
		<attr name="green" type="int" status="optional"></attr>
		<attr name="blue" type="int" status="optional"></attr>
		<attr name="a" type="int" status="optional"></attr>
	</tag>
	<tag name="swfdisplayitem->move" type="function" returnType="void" version="">
		<attr name="dx" type="int" status="required"></attr>
		<attr name="dy" type="int" status="required"></attr>
	</tag>
	<tag name="swfdisplayitem->moveto" type="function" returnType="void" version="">
		<attr name="x" type="int" status="required"></attr>
		<attr name="y" type="int" status="required"></attr>
	</tag>
	<tag name="swfdisplayitem->multcolor" type="function" returnType="void" version="">
		<attr name="red" type="int" status="optional"></attr>
		<attr name="green" type="int" status="optional"></attr>
		<attr name="blue" type="int" status="optional"></attr>
		<attr name="a" type="int" status="optional"></attr>
	</tag>
	<tag name="swfdisplayitem->remove" type="function" returnType="void" version="">
	</tag>
	<tag name="swfdisplayitem->rotate" type="function" returnType="void" version="">
		<attr name="ddegrees" type="float" status="required"></attr>
	</tag>
	<tag name="swfdisplayitem->rotateto" type="function" returnType="void" version="">
		<attr name="degrees" type="float" status="required"></attr>
	</tag>
	<tag name="swfdisplayitem->scale" type="function" returnType="void" version="">
		<attr name="dx" type="int" status="required"></attr>
		<attr name="dy" type="int" status="required"></attr>
	</tag>
	<tag name="swfdisplayitem->scaleto" type="function" returnType="void" version="">
		<attr name="x" type="int" status="required"></attr>
		<attr name="y" type="int" status="required"></attr>
	</tag>
	<tag name="swfdisplayitem->setdepth" type="function" returnType="void" version="">
		<attr name="depth" type="float" status="required"></attr>
	</tag>
	<tag name="swfdisplayitem->setname" type="function" returnType="void" version="">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="swfdisplayitem->setratio" type="function" returnType="void" version="">
		<attr name="ratio" type="float" status="required"></attr>
	</tag>
	<tag name="swfdisplayitem->skewx" type="function" returnType="void" version="">
		<attr name="ddegrees" type="float" status="required"></attr>
	</tag>
	<tag name="swfdisplayitem->skewxto" type="function" returnType="void" version="">
		<attr name="degrees" type="float" status="required"></attr>
	</tag>
	<tag name="swfdisplayitem->skewy" type="function" returnType="void" version="">
		<attr name="ddegrees" type="float" status="required"></attr>
	</tag>
	<tag name="swfdisplayitem->skewyto" type="function" returnType="void" version="">
		<attr name="degrees" type="float" status="required"></attr>
	</tag>
	<tag name="swfdisplayitem" type="function" returnType="object" version="">
	</tag>
	<tag name="swffill->moveto" type="function" returnType="void" version="">
		<attr name="x" type="int" status="required"></attr>
		<attr name="y" type="int" status="required"></attr>
	</tag>
	<tag name="swffill->rotateto" type="function" returnType="void" version="">
		<attr name="degrees" type="float" status="required"></attr>
	</tag>
	<tag name="swffill->scaleto" type="function" returnType="void" version="">
		<attr name="x" type="int" status="required"></attr>
		<attr name="y" type="int" status="required"></attr>
	</tag>
	<tag name="swffill->skewxto" type="function" returnType="void" version="">
		<attr name="x" type="float" status="required"></attr>
	</tag>
	<tag name="swffill->skewyto" type="function" returnType="void" version="">
		<attr name="y" type="float" status="required"></attr>
	</tag>
	<tag name="SWFFill" type="function" returnType="object" version="">
	</tag>
	<tag name="swffont->getwidth" type="function" returnType="int" version="">
		<attr name="string" type="string" status="required"></attr>
	</tag>
	<tag name="swffont" type="function" returnType="object" version="PHP 4 &gt;= 4.0.5">
		<attr name="filename" type="string" status="required"></attr>
	</tag>
	<tag name="swfgradient->addentry" type="function" returnType="void" version="">
		<attr name="ratio" type="float" status="required"></attr>
		<attr name="red" type="int" status="required"></attr>
		<attr name="green" type="int" status="required"></attr>
		<attr name="blue" type="int" status="required"></attr>
		<attr name="a" type="int" status="optional"></attr>
	</tag>
	<tag name="swfgradient" type="function" returnType="object" version="PHP 4 &gt;= 4.0.5">
	</tag>
	<tag name="swfmorph->getshape1" type="function" returnType="mixed" version="">
	</tag>
	<tag name="swfmorph->getshape2" type="function" returnType="mixed" version="">
	</tag>
	<tag name="swfmorph" type="function" returnType="object" version="PHP 4 &gt;= 4.0.5">
	</tag>
	<tag name="swfmovie->add" type="function" returnType="void" version="">
		<attr name="instance" type="resource" status="required"></attr>
	</tag>
	<tag name="swfmovie->nextframe" type="function" returnType="void" version="">
	</tag>
	<tag name="swfmovie->output" type="function" returnType="int" version="">
		<attr name="compression" type="int" status="optional"></attr>
	</tag>
	<tag name="swfmovie->remove" type="function" returnType="void" version="">
		<attr name="instance" type="resource" status="required"></attr>
	</tag>
	<tag name="swfmovie->save" type="function" returnType="int" version="">
		<attr name="filename" type="string" status="required"></attr>
		<attr name="compression" type="int" status="optional"></attr>
	</tag>
	<tag name="swfmovie->setbackground" type="function" returnType="void" version="">
		<attr name="red" type="int" status="required"></attr>
		<attr name="green" type="int" status="required"></attr>
		<attr name="blue" type="int" status="required"></attr>
	</tag>
	<tag name="swfmovie->setdimension" type="function" returnType="void" version="">
		<attr name="width" type="int" status="required"></attr>
		<attr name="height" type="int" status="required"></attr>
	</tag>
	<tag name="swfmovie->setframes" type="function" returnType="void" version="">
		<attr name="numberofframes" type="string" status="required"></attr>
	</tag>
	<tag name="swfmovie->setrate" type="function" returnType="void" version="">
		<attr name="rate" type="int" status="required"></attr>
	</tag>
	<tag name="swfmovie->streammp3" type="function" returnType="void" version="">
		<attr name="mp3File" type="mixed" status="required"></attr>
	</tag>
	<tag name="swfmovie" type="function" returnType="object" version="PHP 4 &gt;= 4.0.5">
	</tag>
	<tag name="swfshape->addfill" type="function" returnType="void" version="">
		<attr name="red" type="int" status="required"></attr>
		<attr name="green" type="int" status="required"></attr>
		<attr name="blue" type="int" status="required"></attr>
		<attr name="a" type="int" status="optional"></attr>
	</tag>
	<tag name="swfshape->drawcurve" type="function" returnType="void" version="">
		<attr name="controldx" type="int" status="required"></attr>
		<attr name="controldy" type="int" status="required"></attr>
		<attr name="anchordx" type="int" status="required"></attr>
		<attr name="anchordy" type="int" status="required"></attr>
	</tag>
	<tag name="swfshape->drawcurveto" type="function" returnType="void" version="">
		<attr name="controlx" type="int" status="required"></attr>
		<attr name="controly" type="int" status="required"></attr>
		<attr name="anchorx" type="int" status="required"></attr>
		<attr name="anchory" type="int" status="required"></attr>
	</tag>
	<tag name="swfshape->drawline" type="function" returnType="void" version="">
		<attr name="dx" type="int" status="required"></attr>
		<attr name="dy" type="int" status="required"></attr>
	</tag>
	<tag name="swfshape->drawlineto" type="function" returnType="void" version="">
		<attr name="x" type="int" status="required"></attr>
		<attr name="y" type="int" status="required"></attr>
	</tag>
	<tag name="swfshape->movepen" type="function" returnType="void" version="">
		<attr name="dx" type="int" status="required"></attr>
		<attr name="dy" type="int" status="required"></attr>
	</tag>
	<tag name="swfshape->movepento" type="function" returnType="void" version="">
		<attr name="x" type="int" status="required"></attr>
		<attr name="y" type="int" status="required"></attr>
	</tag>
	<tag name="swfshape->setleftfill" type="function" returnType="void" version="">
		<attr name="fill" type="swfgradient" status="required"></attr>
	</tag>
	<tag name="swfshape->setline" type="function" returnType="void" version="">
		<attr name="width" type="int" status="required"></attr>
		<attr name="red" type="int" status="optional"></attr>
		<attr name="green" type="int" status="optional"></attr>
		<attr name="blue" type="int" status="optional"></attr>
		<attr name="a" type="int" status="optional"></attr>
	</tag>
	<tag name="swfshape->setrightfill" type="function" returnType="void" version="">
		<attr name="fill" type="swfgradient" status="required"></attr>
	</tag>
	<tag name="swfshape" type="function" returnType="object" version="PHP 4 &gt;= 4.0.5">
	</tag>
	<tag name="swfsprite->add" type="function" returnType="void" version="">
		<attr name="object" type="resource" status="required"></attr>
	</tag>
	<tag name="swfsprite->nextframe" type="function" returnType="void" version="">
	</tag>
	<tag name="swfsprite->remove" type="function" returnType="void" version="">
		<attr name="object" type="resource" status="required"></attr>
	</tag>
	<tag name="swfsprite->setframes" type="function" returnType="void" version="">
		<attr name="numberofframes" type="int" status="required"></attr>
	</tag>
	<tag name="swfsprite" type="function" returnType="object" version="PHP 4 &gt;= 4.0.5">
	</tag>
	<tag name="swftext->addstring" type="function" returnType="void" version="">
		<attr name="string" type="string" status="required"></attr>
	</tag>
	<tag name="swftext->getwidth" type="function" returnType="void" version="">
		<attr name="string" type="string" status="required"></attr>
	</tag>
	<tag name="swftext->moveto" type="function" returnType="void" version="">
		<attr name="x" type="int" status="required"></attr>
		<attr name="y" type="int" status="required"></attr>
	</tag>
	<tag name="swftext->setcolor" type="function" returnType="void" version="">
		<attr name="red" type="int" status="required"></attr>
		<attr name="green" type="int" status="required"></attr>
		<attr name="blue" type="int" status="required"></attr>
		<attr name="a" type="int" status="optional"></attr>
	</tag>
	<tag name="swftext->setfont" type="function" returnType="void" version="">
		<attr name="font" type="string" status="required"></attr>
	</tag>
	<tag name="swftext->setheight" type="function" returnType="void" version="">
		<attr name="height" type="int" status="required"></attr>
	</tag>
	<tag name="swftext->setspacing" type="function" returnType="void" version="">
		<attr name="spacing" type="float" status="required"></attr>
	</tag>
	<tag name="swftext" type="function" returnType="object" version="PHP 4 &gt;= 4.0.5">
	</tag>
	<tag name="swftextfield->addstring" type="function" returnType="void" version="">
		<attr name="string" type="string" status="required"></attr>
	</tag>
	<tag name="swftextfield->align" type="function" returnType="void" version="">
		<attr name="alignement" type="int" status="required"></attr>
	</tag>
	<tag name="swftextfield->setbounds" type="function" returnType="void" version="">
		<attr name="width" type="int" status="required"></attr>
		<attr name="height" type="int" status="required"></attr>
	</tag>
	<tag name="swftextfield->setcolor" type="function" returnType="void" version="">
		<attr name="red" type="int" status="required"></attr>
		<attr name="green" type="int" status="required"></attr>
		<attr name="blue" type="int" status="required"></attr>
		<attr name="a" type="int" status="optional"></attr>
	</tag>
	<tag name="swftextfield->setfont" type="function" returnType="void" version="">
		<attr name="font" type="string" status="required"></attr>
	</tag>
	<tag name="swftextfield->setheight" type="function" returnType="void" version="">
		<attr name="height" type="int" status="required"></attr>
	</tag>
	<tag name="swftextfield->setindentation" type="function" returnType="void" version="">
		<attr name="width" type="int" status="required"></attr>
	</tag>
	<tag name="swftextfield->setleftmargin" type="function" returnType="void" version="">
		<attr name="width" type="int" status="required"></attr>
	</tag>
	<tag name="swftextfield->setlinespacing" type="function" returnType="void" version="">
		<attr name="height" type="int" status="required"></attr>
	</tag>
	<tag name="swftextfield->setmargins" type="function" returnType="void" version="">
		<attr name="left" type="int" status="required"></attr>
		<attr name="right" type="int" status="required"></attr>
	</tag>
	<tag name="swftextfield->setname" type="function" returnType="void" version="">
		<attr name="name" type="string" status="required"></attr>
	</tag>
	<tag name="swftextfield->setrightmargin" type="function" returnType="void" version="">
		<attr name="width" type="int" status="required"></attr>
	</tag>
	<tag name="swftextfield" type="function" returnType="object" version="PHP 4 &gt;= 4.0.5">
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
</tags>