#!/usr/bin/perl
#
# Template Magic for Quanta:
#    This script is intended to be used as a sample of some of the possibilities of integrating
#  Kommander and Quanta.  It's hoped that this can serve as a basic example of how Kommander works
#  and how it can speed you're web development.
#
#  This script exists in the public domain and may be freely copied and redistributed.  Please just
#  let me know that you used my script and what you're doing (I like new ideas).
#
#  As usual, if this script by some means manages to do any harm to you, your computer, your animal,
#  your relatives or your animal relatives, feel free to vent to me in email and I'll try to fix the
#  script.  You'll have to take care of the livestock.
#
# Template variables are of the form [ varname:vartype ] where vartype can be either
# 'file', 'filename' or 'text'.  Filename and file are two slight variations on the same theme
# as both allow you to select a file from your filesystem, file will insert the contents of that
# file whereas filename will simply insert the filename (with the path).  Text is simply a rich
# text field that can take a good little bit of text (the limitation if any will exist in
# Kommander or your computers memory).
#
# Cheers,
#   --Robert Nickel <robert@artnickel.com>
#     Quanta Development Team Groupie
#'

use strict;
# disable buffereing - otherwise everything waits till we quit
$| = 1;

my $tmversion = "2.0";

#my $template=`kmdr-executor /tmp/TM.kmdr`;

# adapt - passed parameter is template

my $template = @ARGV[0];

open(IFH, "< $template") || die("Couldn't read $template\n");

my ($tmp,@ar,@OrderedVars,%Vars,%Vals,$i,$k,$v,$l,$r,$ns,@filechars,$fname,@VarData,@VD,$j);

my $max_ui_height=800;  #Maximum screen height of ui dialog
my $calcHeight=0;  #Height of ui dialog, up to a max of $max_ui_height
parseTemplateVars();

close(IFH);

srand (time ^ $$ ^ unpack "%L*", `ps axww | gzip`);
for (48..57,65..90,97..122) { push @filechars, chr(); } #Chars for tmp filename
$fname="/tmp/";
for (0..10) { $fname.=$filechars[rand 62]; }
$fname.=".quanta.template";

#uncomment this line if you're having grief with the generated kmdr file.'
#and comment out the rm -f a little farther down.
#$fname="/tmp/quanta_template.kmdr";

open(OFH, "> $fname") || die("Couldn't create temp file $fname\n");
&createUI();
close(OFH);

@VarData=`kmdr-executor $fname`; #We now have var:value one per line.

#Ok.  Let's get that pesky template loaded and output it to something useful.'
#print "Data from kommander:\n";
$j=0;
for $i (@VarData) {
  chomp($i);
  if($i=~/^\+\+\+\+\z/) {  #end of record marker
    $j++;
  } else { #add the contents to the end of the string
    if(length($VD[$j])>0) { $VD[$j].=" "; }
    $VD[$j].=$i;
  }
}

for $i (@VD) {
  chomp($i);
  if($i ne "") {
    ($k,$v)=split(/:/,$i,2);  #This should give us key:value pair separated.
#    print "$k: '$v'\n";
    if(exists($Vars{$k})) {
      $Vals{$k}=$v;
    } else { die("Inconsistent variable: $k.\nBailing....\n"); }
  }
}

open(IFH, "< $template") || die("Couldn't read $template\n");  #reopen file to reset file pointer

&insertTemplateValues();

close(IFH);

#cleanup temp file
`rm -f $fname`;
exit(0);

sub usage() {
  my $msg=shift();
  print "$msg\n\nUsage: $0 <path-to-template>\n";
  exit(1);
}

sub parseTemplateVars() {
  $calcHeight=40; #base height for button.
  @OrderedVars=();  #initialize this!  Who knows?
  while($tmp=<IFH>) {
    chomp($tmp);
    if($tmp=~/\[\s*\S+\s*\]/) {   #Only parse on matches - Changed delimieters from |<- ->| to [ ]
      $tmp=~s/^[^\[]*(\[\s*\S+\s*\])[^\[]*/$1/g;
      $l=index($tmp,"[");
      while($l>=0) {
        $r=index($tmp,"]");
        $ns=substr($tmp,$l+1,$r-1);
        $ns=~s/\[\s*(\S+)\s*\]/$1/;
        $ns=~s/\s//g; #no spaces!
        ($k,$v)=split(/:/,$ns,2);
        if(exists($Vars{$k}) && $v ne $Vars{$k}) {
          die("Multiple definitions for variable $k - '$v' != '$Vars{$k}'.\n");
        }
        if(! exists($Vars{$k})) { push @OrderedVars, ["$k","$v"]; }
        $Vars{$k}=$v;
        $calcHeight+=$v eq "text" ? 35 : 25;
        $tmp=substr($tmp,$r+1);
        $l=index($tmp,"[");
      }
    }
  }

=pod
  print "="x80 . "\n";
  print "Variables found:\n";
  for $i (keys %Vars) {
    print "$i - $Vars{$i}\n";
  }
=cut  
}

sub insertTemplateValues() {
  my ($subcntr, $pre, $post);

  while($tmp=<IFH>) {
    $subcntr=0; #sanity.  Make sure we don't run away with substitutions.'
    while($tmp=~/\[\s*\S+\s*\]/ && $subcntr<100) {   #Only do substitutions on matches.
      for $k (keys %Vars) {
        $tmp=~s/\[\s*$k:$Vars{$k}\s*]/$Vals{$k}/;
      }
      $subcntr++;
    }
    print $tmp;
  }
}

sub header() {
  #----------- Header -----------
  if($calcHeight>$max_ui_height) { $calcHeight=$max_ui_height; } #Max!
  print OFH <<E
  <!DOCTYPE UI><UI version="3.0" stdsetdef="1">
  <class>TemplateVarList</class>
  <widget class="Dialog">
      <property name="name">
          <cstring>TemplateVarList</cstring>
      </property>
      <property name="geometry">
          <rect>
              <x>0</x>
              <y>0</y>
              <width>600</width>
              <height>$calcHeight</height>
          </rect>
      </property>
      <property name="caption">
          <string>Template Magic (for Quanta 3.1.9) version $tmversion</string>
      </property>
E
;
}

sub footer() {
  #----------- Footer -----------
  my $theSpot=$calcHeight-30;
  print OFH <<E
        <widget class="CloseButton">
          <property name="name">
              <cstring>CloseButton2</cstring>
          </property>
          <property name="geometry">
              <rect>
                  <x>300</x>
                  <y>$theSpot</y>
                  <width>121</width>
                  <height>21</height>
              </rect>
          </property>
          <property name="text">
              <string>Create Document</string>
          </property>
          <property name="associations" stdset="0">
              <stringlist>
                  <string>
E
;

  $i=0;
  for $k (keys %Vars) {
    if($i>0) { print OFH "\n"; }
    $i++;
    print OFH "echo -e '$k:\@$k\\n++++'";
  }
print OFH <<E
                </string>
              </stringlist>
          </property>
          <property name="writeStdout">
              <bool>true</bool>
          </property>
      </widget>
  </widget>
  <layoutdefaults spacing="6" margin="11"/>
  </UI>

E
;
}

sub createUI() {
=pod
Structure for label widget: height is 20

    <widget class="QLabel">
        <property name="name">
            <cstring>TextLabel2_2</cstring>
        </property>
        <property name="geometry">
            <rect>
                <x>10</x>
                <y>40</y>
                <width>130</width>
                <height>20</height>
            </rect>
        </property>
        <property name="text">
            <string>Variable2-FileInput</string>
        </property>
    </widget>

Structure for a file widget: height is 20

    <widget class="FileSelector">
        <property name="name">
            <cstring>FileSelector2</cstring>
        </property>
        <property name="geometry">
            <rect>
                <x>140</x>
                <y>50</y>
                <width>441</width>
                <height>20</height>
            </rect>
        </property>
    </widget>

Structure for a text widget: height is 30

    <widget class="TextEdit">
        <property name="name">
            <cstring>TextEdit1</cstring>
        </property>
        <property name="geometry">
            <rect>
                <x>140</x>
                <y>10</y>
                <width>440</width>
                <height>30</height>
            </rect>
        </property>
        <property name="vScrollBarMode">
            <enum>AlwaysOff</enum>
        </property>
        <property name="hScrollBarMode">
            <enum>AlwaysOff</enum>
        </property>
    </widget>
.
=cut

  header();

  my %Layouts;
  my ($y_loc)=0;
  for $k (0..$#OrderedVars) {
    $Layouts{$OrderedVars[$k][0]}{y}=$y_loc;
    print OFH <<E
    <widget class="QLabel">
        <property name="name">
            <cstring>LBL_$OrderedVars[$k][0]</cstring>
        </property>
        <property name="geometry">
            <rect>
                <x>10</x>
                <y>$y_loc</y>
                <width>130</width>
                <height>20</height>
            </rect>
        </property>
        <property name="text">
            <string>$OrderedVars[$k][0]:</string>
        </property>
    </widget>
E
;
    SWITCH: for ($OrderedVars[$k][1]) {
      /file|filename/ && do { $y_loc+=25; last SWITCH; };
      /text/ && do { $y_loc+=35; last SWITCH; };
    }
  }

  for $k (0..$#OrderedVars) {

    SWITCH: for ($OrderedVars[$k][1]) {
      /file|filename/ && do {
    print OFH <<E
    <widget class="FileSelector">
        <property name="name">
            <cstring>$OrderedVars[$k][0]</cstring>
        </property>
        <property name="geometry">
            <rect>
                <x>140</x>
                <y>$Layouts{$OrderedVars[$k][0]}{y}</y>
                <width>440</width>
                <height>20</height>
            </rect>
        </property>
        <property name="associations" stdset="0">
            <stringlist>
                <string>\@widgetText</string>
            </stringlist>
        </property>
    </widget>
E
;
        last SWITCH;
      };
      /text/ && do {
    print OFH <<E
    <widget class="TextEdit">
        <property name="name">
            <cstring>$OrderedVars[$k][0]</cstring>
        </property>
        <property name="geometry">
            <rect>
                <x>140</x>
                <y>$Layouts{$OrderedVars[$k][0]}{y}</y>
                <width>440</width>
                <height>30</height>
            </rect>
        </property>
        <property name="associations" stdset="0">
            <stringlist>
                <string>\@widgetText</string>
            </stringlist>
        </property>
        <property name="vScrollBarMode">
            <enum>AlwaysOff</enum>
        </property>
        <property name="hScrollBarMode">
            <enum>AlwaysOff</enum>
        </property>
    </widget>
E
;
        last SWITCH;
      };
    }
  }

  footer();
}
