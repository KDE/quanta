<!DOCTYPE tags>
<tags>
	<tag name="ming_setcubicthreshold" type="function" returnType="void">
		<attr name="threshold" type="int"></attr>
	</tag>
	<tag name="ming_setscale" type="function" returnType="void">
		<attr name="scale" type="int"></attr>
	</tag>
	<tag name="ming_useswfversion" type="function" returnType="void">
		<attr name="version" type="int"></attr>
	</tag>
	<tag name="swfaction" type="function" returnType="object">
		<attr name="script" type="string"></attr>
	</tag>
	<tag name="swfbitmap->getheight" type="function" returnType="int">
	</tag>
	<tag name="swfbitmap->getwidth" type="function" returnType="int">
	</tag>
	<tag name="swfbitmap" type="function" returnType="object">
		<attr name="file" type="mixed"></attr>
		<attr name="alphafile" type="mixed" status="optional"></attr>
	</tag>
	<tag name="swfbutton_keypress" type="function" returnType="int">
		<attr name="str" type="string"></attr>
	</tag>
	<tag name="swfbutton->addaction" type="function" returnType="void">
		<attr name="action" type="resource"></attr>
		<attr name="flags" type="int"></attr>
	</tag>
	<tag name="swfbutton->addshape" type="function" returnType="void">
		<attr name="shape" type="resource"></attr>
		<attr name="flags" type="int"></attr>
	</tag>
	<tag name="swfbutton->setaction" type="function" returnType="void">
		<attr name="action" type="resource"></attr>
	</tag>
	<tag name="swfbutton->setdown" type="function" returnType="void">
		<attr name="shape" type="resource"></attr>
	</tag>
	<tag name="swfbutton->sethit" type="function" returnType="void">
		<attr name="shape" type="resource"></attr>
	</tag>
	<tag name="swfbutton->setover" type="function" returnType="void">
		<attr name="shape" type="resource"></attr>
	</tag>
	<tag name="swfbutton->setup" type="function" returnType="void">
		<attr name="shape" type="resource"></attr>
	</tag>
	<tag name="swfbutton" type="function" returnType="object">
	</tag>
	<tag name="swfdisplayitem->addcolor" type="function" returnType="void">
		<attr name="red" type="int" status="optional"></attr>
		<attr name="green" type="int" status="optional"></attr>
		<attr name="blue" type="int" status="optional"></attr>
		<attr name="a" type="int" status="optional"></attr>
	</tag>
	<tag name="swfdisplayitem->move" type="function" returnType="void">
		<attr name="dx" type="int"></attr>
		<attr name="dy" type="int"></attr>
	</tag>
	<tag name="swfdisplayitem->moveto" type="function" returnType="void">
		<attr name="x" type="int"></attr>
		<attr name="y" type="int"></attr>
	</tag>
	<tag name="swfdisplayitem->multcolor" type="function" returnType="void">
		<attr name="red" type="int" status="optional"></attr>
		<attr name="green" type="int" status="optional"></attr>
		<attr name="blue" type="int" status="optional"></attr>
		<attr name="a" type="int" status="optional"></attr>
	</tag>
	<tag name="swfdisplayitem->remove" type="function" returnType="void">
	</tag>
	<tag name="swfdisplayitem->rotate" type="function" returnType="void">
		<attr name="ddegrees" type="float"></attr>
	</tag>
	<tag name="swfdisplayitem->rotateto" type="function" returnType="void">
		<attr name="degrees" type="float"></attr>
	</tag>
	<tag name="swfdisplayitem->scale" type="function" returnType="void">
		<attr name="dx" type="int"></attr>
		<attr name="dy" type="int"></attr>
	</tag>
	<tag name="swfdisplayitem->scaleto" type="function" returnType="void">
		<attr name="x" type="int"></attr>
		<attr name="y" type="int"></attr>
	</tag>
	<tag name="swfdisplayitem->setdepth" type="function" returnType="void">
		<attr name="depth" type="float"></attr>
	</tag>
	<tag name="swfdisplayitem->setname" type="function" returnType="void">
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="swfdisplayitem->setratio" type="function" returnType="void">
		<attr name="ratio" type="float"></attr>
	</tag>
	<tag name="swfdisplayitem->skewx" type="function" returnType="void">
		<attr name="ddegrees" type="float"></attr>
	</tag>
	<tag name="swfdisplayitem->skewxto" type="function" returnType="void">
		<attr name="degrees" type="float"></attr>
	</tag>
	<tag name="swfdisplayitem->skewy" type="function" returnType="void">
		<attr name="ddegrees" type="float"></attr>
	</tag>
	<tag name="swfdisplayitem->skewyto" type="function" returnType="void">
		<attr name="degrees" type="float"></attr>
	</tag>
	<tag name="swfdisplayitem" type="function" returnType="object">
	</tag>
	<tag name="swffill->moveto" type="function" returnType="void">
		<attr name="x" type="int"></attr>
		<attr name="y" type="int"></attr>
	</tag>
	<tag name="swffill->rotateto" type="function" returnType="void">
		<attr name="degrees" type="float"></attr>
	</tag>
	<tag name="swffill->scaleto" type="function" returnType="void">
		<attr name="x" type="int"></attr>
		<attr name="y" type="int"></attr>
	</tag>
	<tag name="swffill->skewxto" type="function" returnType="void">
		<attr name="x" type="float"></attr>
	</tag>
	<tag name="swffill->skewyto" type="function" returnType="void">
		<attr name="y" type="float"></attr>
	</tag>
	<tag name="SWFFill" type="function" returnType="object">
	</tag>
	<tag name="swffont->getwidth" type="function" returnType="int">
		<attr name="string" type="string"></attr>
	</tag>
	<tag name="swffont" type="function" returnType="object">
		<attr name="filename" type="string"></attr>
	</tag>
	<tag name="swfgradient->addentry" type="function" returnType="void">
		<attr name="ratio" type="float"></attr>
		<attr name="red" type="int"></attr>
		<attr name="green" type="int"></attr>
		<attr name="blue" type="int"></attr>
		<attr name="a" type="int" status="optional"></attr>
	</tag>
	<tag name="swfgradient" type="function" returnType="object">
	</tag>
	<tag name="swfmorph->getshape1" type="function" returnType="mixed">
	</tag>
	<tag name="swfmorph->getshape2" type="function" returnType="mixed">
	</tag>
	<tag name="swfmorph" type="function" returnType="object">
	</tag>
	<tag name="swfmovie->add" type="function" returnType="void">
		<attr name="instance" type="resource"></attr>
	</tag>
	<tag name="swfmovie->nextframe" type="function" returnType="void">
	</tag>
	<tag name="swfmovie->output" type="function" returnType="int">
		<attr name="compression" type="int" status="optional"></attr>
	</tag>
	<tag name="swfmovie->remove" type="function" returnType="void">
		<attr name="instance" type="resource"></attr>
	</tag>
	<tag name="swfmovie->save" type="function" returnType="int">
		<attr name="filename" type="string"></attr>
		<attr name="compression" type="int" status="optional"></attr>
	</tag>
	<tag name="swfmovie->setbackground" type="function" returnType="void">
		<attr name="red" type="int"></attr>
		<attr name="green" type="int"></attr>
		<attr name="blue" type="int"></attr>
	</tag>
	<tag name="swfmovie->setdimension" type="function" returnType="void">
		<attr name="width" type="int"></attr>
		<attr name="height" type="int"></attr>
	</tag>
	<tag name="swfmovie->setframes" type="function" returnType="void">
		<attr name="numberofframes" type="string"></attr>
	</tag>
	<tag name="swfmovie->setrate" type="function" returnType="void">
		<attr name="rate" type="int"></attr>
	</tag>
	<tag name="swfmovie->streammp3" type="function" returnType="void">
		<attr name="mp3File" type="mixed"></attr>
	</tag>
	<tag name="swfmovie" type="function" returnType="object">
	</tag>
	<tag name="swfshape->addfill" type="function" returnType="void">
		<attr name="red" type="int"></attr>
		<attr name="green" type="int"></attr>
		<attr name="blue" type="int"></attr>
		<attr name="a" type="int" status="optional"></attr>
	</tag>
	<tag name="swfshape->drawcurve" type="function" returnType="void">
		<attr name="controldx" type="int"></attr>
		<attr name="controldy" type="int"></attr>
		<attr name="anchordx" type="int"></attr>
		<attr name="anchordy" type="int"></attr>
	</tag>
	<tag name="swfshape->drawcurveto" type="function" returnType="void">
		<attr name="controlx" type="int"></attr>
		<attr name="controly" type="int"></attr>
		<attr name="anchorx" type="int"></attr>
		<attr name="anchory" type="int"></attr>
	</tag>
	<tag name="swfshape->drawline" type="function" returnType="void">
		<attr name="dx" type="int"></attr>
		<attr name="dy" type="int"></attr>
	</tag>
	<tag name="swfshape->drawlineto" type="function" returnType="void">
		<attr name="x" type="int"></attr>
		<attr name="y" type="int"></attr>
	</tag>
	<tag name="swfshape->movepen" type="function" returnType="void">
		<attr name="dx" type="int"></attr>
		<attr name="dy" type="int"></attr>
	</tag>
	<tag name="swfshape->movepento" type="function" returnType="void">
		<attr name="x" type="int"></attr>
		<attr name="y" type="int"></attr>
	</tag>
	<tag name="swfshape->setleftfill" type="function" returnType="void">
		<attr name="fill" type="swfgradient"></attr>
	</tag>
	<tag name="swfshape->setline" type="function" returnType="void">
		<attr name="width" type="int"></attr>
		<attr name="red" type="int" status="optional"></attr>
		<attr name="green" type="int" status="optional"></attr>
		<attr name="blue" type="int" status="optional"></attr>
		<attr name="a" type="int" status="optional"></attr>
	</tag>
	<tag name="swfshape->setrightfill" type="function" returnType="void">
		<attr name="fill" type="swfgradient"></attr>
	</tag>
	<tag name="swfshape" type="function" returnType="object">
	</tag>
	<tag name="swfsprite->add" type="function" returnType="void">
		<attr name="object" type="resource"></attr>
	</tag>
	<tag name="swfsprite->nextframe" type="function" returnType="void">
	</tag>
	<tag name="swfsprite->remove" type="function" returnType="void">
		<attr name="object" type="resource"></attr>
	</tag>
	<tag name="swfsprite->setframes" type="function" returnType="void">
		<attr name="numberofframes" type="int"></attr>
	</tag>
	<tag name="swfsprite" type="function" returnType="object">
	</tag>
	<tag name="swftext->addstring" type="function" returnType="void">
		<attr name="string" type="string"></attr>
	</tag>
	<tag name="swftext->getwidth" type="function" returnType="void">
		<attr name="string" type="string"></attr>
	</tag>
	<tag name="swftext->moveto" type="function" returnType="void">
		<attr name="x" type="int"></attr>
		<attr name="y" type="int"></attr>
	</tag>
	<tag name="swftext->setcolor" type="function" returnType="void">
		<attr name="red" type="int"></attr>
		<attr name="green" type="int"></attr>
		<attr name="blue" type="int"></attr>
		<attr name="a" type="int" status="optional"></attr>
	</tag>
	<tag name="swftext->setfont" type="function" returnType="void">
		<attr name="font" type="string"></attr>
	</tag>
	<tag name="swftext->setheight" type="function" returnType="void">
		<attr name="height" type="int"></attr>
	</tag>
	<tag name="swftext->setspacing" type="function" returnType="void">
		<attr name="spacing" type="float"></attr>
	</tag>
	<tag name="swftext" type="function" returnType="object">
	</tag>
	<tag name="swftextfield->addstring" type="function" returnType="void">
		<attr name="string" type="string"></attr>
	</tag>
	<tag name="swftextfield->align" type="function" returnType="void">
		<attr name="alignement" type="int"></attr>
	</tag>
	<tag name="swftextfield->setbounds" type="function" returnType="void">
		<attr name="width" type="int"></attr>
		<attr name="height" type="int"></attr>
	</tag>
	<tag name="swftextfield->setcolor" type="function" returnType="void">
		<attr name="red" type="int"></attr>
		<attr name="green" type="int"></attr>
		<attr name="blue" type="int"></attr>
		<attr name="a" type="int" status="optional"></attr>
	</tag>
	<tag name="swftextfield->setfont" type="function" returnType="void">
		<attr name="font" type="string"></attr>
	</tag>
	<tag name="swftextfield->setheight" type="function" returnType="void">
		<attr name="height" type="int"></attr>
	</tag>
	<tag name="swftextfield->setindentation" type="function" returnType="void">
		<attr name="width" type="int"></attr>
	</tag>
	<tag name="swftextfield->setleftmargin" type="function" returnType="void">
		<attr name="width" type="int"></attr>
	</tag>
	<tag name="swftextfield->setlinespacing" type="function" returnType="void">
		<attr name="height" type="int"></attr>
	</tag>
	<tag name="swftextfield->setmargins" type="function" returnType="void">
		<attr name="left" type="int"></attr>
		<attr name="right" type="int"></attr>
	</tag>
	<tag name="swftextfield->setname" type="function" returnType="void">
		<attr name="name" type="string"></attr>
	</tag>
	<tag name="swftextfield->setrightmargin" type="function" returnType="void">
		<attr name="width" type="int"></attr>
	</tag>
	<tag name="swftextfield" type="function" returnType="object">
		<attr name="flags" type="int" status="optional"></attr>
	</tag>
</tags>