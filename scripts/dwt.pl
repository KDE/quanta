#!/usr/bin/perl

## dwt - perl script to support dreamweaver templates
## Copyright (C) 2003 Benjamin Stocker <bstocker@4s-hosting.ch>
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

## DESCRIPTION:
## This is a perl script to apply a modified Dreamweaver (DW)
## template to a file using this template. Macromedia
## Dreamweaver manages this automatically whenever you
## change a tepmplate. See DW Documentation for more infos.
## When you have a DW-Project with templates but no DW, you
## cann change templates, but this changes will not be added
## to files using the template. 
## This script can solve this problem. It takes the name of a
## file as argument, scans this file for a template and adds
## all template changes to the file.
##
## Check http://sys.4s-hosting.ch/bstocker/dwt/ 
## for more informations and updates. 
##
## Please, let me know it this script works for you, your questions
## and comments are welcome! 

use Getopt::Std;
use IO::Dir;

# Write debug informations to STDOUT (-d)
$DEBUG=0;
# Version
$VERSION="1.11 (06/9/2003)";
# Display result on screen (-o);
$SCREEN=0;
# Extensions to search
$SEXT="php:php3:php4:htm:html:phtml";
@AEXT=split(':',$SEXT);

&CheckArguments();

## Check command line arguments
if ($ARGV[0] eq '') {
  &Usage;
}
$PROJECTPATH=$ARGV[0];
$PROJECTPATH=&ExpandPath($PROJECTPATH);
unless (-d $PROJECTPATH) {
  print "FATAL: Cannot stat '$PROJECTPATH'. argument must be a directory\n";
	exit;
}

&ScanDir($PROJECTPATH);

# All done!
exit(0);

##
## Scan all directories and files in specified path
##
sub ScanDir($) {
  my $path=$_[0];
	my ($dirname,$newpath,%dhf);
  tie %dhf, IO::Dir, $path;
  if (defined %dhf) {
    foreach $dirname(keys(%dhf)) {
      if ($dirname ne '.' and $dirname ne '..') {
			  $newpath="$path/$dirname";
				&CheckExtension($newpath);
			  if (-d $newpath) {
				  &ScanDir($newpath);
			  }
		  }
    }
  } else {
    die "FATAL: Cannot list $path";
  }
  undef $dhf;
}

##
## Check if extension matches
## 
sub CheckExtension($) {
  my $filename=$_[0];
	my $s;
	foreach $s (@AEXT) {
	  if ($filename=~/\.$s$/i) {
	    &DebugMsg($filename);
			&DoDWT($filename);
	  }
	}
}

##
## Merge template and document
##
sub DoDWT($) {
  my $filename=$_[0];
  my (@v,$source,$templatesource,$templatename);
	my ($p1,$p2,$region,$rbegin,$rend,$rbuf,$q1,$q2);
	
  # Read file into buffer
  open HF, $filename or die "FATAL: Cannot open: $filename";
  @v=<HF>; $source=join("",@v); $source=&Dos2Unix($source);
  close HF;

  # Extract template name from buffer
  $templatename=&GetTemplateName($source);
  if ($templatename eq '') {
    print "NOTE: Could not extract template name from $filename\n";
		return; # function execution ends here! NW would not like that :)
  }

  # Open template file
  $templatename=&BuildPathName($templatename,$PROJECTPATH);
  open HF, $templatename or die "FATAL: Cannot open template file: $templatename";
  @v=<HF>; $templatesource=join("",@v);
  $templatesource=&Dos2Unix($templatesource);
  close HF;

  # Correct links in template
  $templatesource=&CorrectLinks($templatesource,$PROJECTPATH,$filename);

  # Rebuild file using template
  $p1=index($templatesource,"<!-- #BeginEditable ");
  while ($p1>=0) {
    $p1=index($templatesource,"\"",$p1);
    if ($p1>=0) {
      $p1=$p1+1;
      $p2=index($templatesource,"\"",$p1);
      $region=substr($templatesource,$p1,$p2-$p1);
      &DebugMsg("Region: $region");
      # Get region from file and paste into template
      $rbegin="<!-- #BeginEditable \"$region\" -->"; 
      $rend="<!-- #EndEditable -->"; # 21 characters, see below!
      $q1=index($source,$rbegin);
      $q2=index($source,$rend,$q1);
      if ($q1>=0 and $q2>=0) {
        # Extract from file
        $rbuf=substr($source,$q1,$q2-$q1+21); 
        # Paste into template
        $q1=index($templatesource,$rbegin);
        $q2=index($templatesource,$rend,$q1);
        if ($q1>=0 and $q2>=0) {
          $q2=$q2+21;
          $templatesource=substr($templatesource,0,$q1).$rbuf.substr($templatesource,$q2);
        } else {
          print "WARNING: Could not find region '$region' in template $templatename";
        }
      } else {
        print "WARNING: Could not find region '$region' in $filename";
      }
    }
    $p1=index($templatesource,"<!-- #BeginEditable ",$p1);
  } # while

  # Editable blocks inserted, now get library elements
  $templatesource=&InsertLibs($templatesource, $projectpath, $filename);

  # Finally, build frame 
  $templatesource=&RemoveHtmlTags($templatesource);
  $templatesource=&AddHtmlTags($templatesource,$source);

  # Save new file or write it to STDOUT
  if (!$SCREEN) {
    print "Updating $filename\n";
    open HF,">$filename" or die "FATAL: Cannot create $filename";
    print HF $templatesource;
    close HF;
  } else {
    print $templatesource;
  }
}

##
## Extract template filename 
##
sub GetTemplateName() {
  my ($p1,$p2,$v);
  my $source=$_[0]; 
  my $templatename="";

  $p1=index($source, "<!-- #BeginTemplate "); 
  if ($p1>=0) {
    $p1=index($source,"\"",$p1);
    if ($p1>=0) {
      $p1=$p1+1;
      $p2=index($source,"\"",$p1);
      $templatename=substr($source,$p1,$p2-$p1);
      &DebugMsg("Template to be used: $templatename");
    }
  }  
  return $templatename;
}

##
## Add path specification to template filename, so it ca be opened
##
sub BuildPathName() {
  my $templatename=$_[0];
  my $prp=$_[1];
  $templatename="$prp/$templatename";
  # Remove unneeded slashes 
  $templatename=~s/\/\//\//g;
  return $templatename;
}

##
## Remove header and footer from Template
##
sub RemoveHtmlTags() {
  my $p1;
  my $templatesource=$_[0];
  $p1=index($templatesource,"<head>"); # 6 chars
  if ($p1>=0) {
    $p1=$p1+6;
    $templatesource=substr($templatesource,$p1);
  } else {
    print "WARNING: Could not find <head> tag in template $templatename";
  }
  $p1=index($templatesource,"</body>"); 
  if ($p1>=0) {
    $templatesource=substr($templatesource,0,$p1);
  } else {
    print "WARNING: Could not find </body> tag in template $templatename";
  }
  return $templatesource;
}

##
## Add header and footer tags from source file to template
##
sub AddHtmlTags() {
  my ($p1,$hstart,$hend);
  my $templatesource=$_[0];
  my $source=$_[1];

  $p1=index($source,"<head>"); # 6 chars
  if ($p1>=0) {
    $p1=$p1+6;
    $hstart=substr($source,0,$p1);
  } else {
    print "WARNING: Could not find <head> tag in $filename";
  }
  $p1=index($source,"</body>"); 
  if ($p1>=0) {
    $hend=substr($source,$p1);
  } else {
    print "WARNING: Could not find </body> tag in $filename";
  }
  return "$hstart$templatesource$hend"; 
}

##
## Remove '\r' (DOS CRLF Format -> UNIX)
##
sub Dos2Unix() {
  my $v=$_[0];
  $v=~s/\r//gm;
  return $v;
}

##
## Correct template links in new created buffer
##
sub CorrectLinks() {
  my $ts=$_[0];
  my $prp=$_[1];
  my $filename=$_[2];
  my $l;
  
	# Remove filename
  $l=rindex($filename,'/');
  $filename=substr($filename,0,$l);
  # Get remaining path specification 
  $l=length($prp); $filename=substr($filename,$l); 
  $filename.='/'; $filename=~s/\/\//\//;
  $ts=~s/\.\.$filename//g;
  return $ts;
}

##
## Insert library items (if any)
##
sub InsertLibs() {
  my $source=$_[0];
  my $projectpath=$_[1];
  my $sourcepath=$_[2];
  my ($p1, $p2, $p3, $lib, $libpath);
  
  my (@alb, $lb);
  
  # Scan for library items
  $p1=index($source,"<!-- #BeginLibraryItem ");
  while ($p1>=0) {
    # Found, get lib name
    $p1=index($source,"\"",$p1);
    if ($p1>=0) {
      $p1=$p1+1;
      $p2=index($source,"\"",$p1);
      $lib=substr($source,$p1,$p2-$p1);
      &DebugMsg("Library item: $lib");
      # Read library from disk 
      $libpath="$projectpath$lib";
      open LHF, $libpath or die "FATAL: Cannot read library file $libpath";
      @alb=<LHF>; 
      $lb=join("", @alb);
      $lb=&Dos2Unix($lb);
      $lb=&CorrectLinks($lb, $projectpath, $sourcepath);
      close LHF;
      # Replace item
      $p2=index($source, "-->", $p2); 
      if ($p2>=0) {
        $p2=$p2+3;
        $p3=index($source, "<!-- #EndLibraryItem -->", $p2);
        if ($p3>=0) {
          # Build the updated document
          $source=substr($source, 0, $p2) . $lb . substr($source, $p3);  
        } else {
          print "WARNING: End tag for library $lib not found "; 
        }
      } else {
        print "WARNING: Error in library tag $lib. Please correcet markup"; 
      }
    }
    $p1=index($source,"<!-- #BeginLibraryItem ", $p1);
  }
  return $source;
}

sub DebugMsg($) {
  print "# $_[0]\n" if ($DEBUG);
}

##
## Show a small help screen
##
sub Usage() {
  print "Usage: dwt.pl [-dov -e extensions] projectpath\n";
  print "  -d Show debug messages\n";
  print "  -o Show result but do not save it to file\n";
  print "  -v Show version\n";
	print "  -e Set extensions to search for (default: $SEXT)\n";
  exit;
}

##
## Show version
##
sub Version() {
  print "this is dwt $VERSION\n";
  exit;
}

##
## Check command line arguments
##
sub CheckArguments() {
  my (%args, $s);
  
  &getopt('e',\%args);
  foreach $s (keys(%args)) {
    if ($s eq 'o') {
      $SCREEN=1;
    } elsif ($s eq 'd') {
      $DEBUG=1;
    } elsif ($s eq 'v') {
      &Version();
    } elsif ($s eq 'e') {
      @AEXT=split(':',$args{$s});
    } else {
      print "Unknown option: -$s\n";
      &Usage();
    }
  }
}

##
## Convert relative to absolute path
##
sub ExpandPath($) {
  my $filename=$_[0];
  my $pwd=$ENV{PWD};
	# Convert filename to full pathname. Maybe there is a better way to do that?
  # Could not find answer how to expand filename to full path at comp.lang.perl.*
  if (substr($filename,0,1) ne '/') {
    $filename="$pwd/$filename";
    $filename=~s/\/[a-zA-Z0-9]+?\/\.\.\//\//; # Remove ' xxx/../ '
    $filename=~s/\/\//\//g; # Remove ' // '
    $filename=~s/\/\.//g; # Remove ' ./ '
  }
  $filename=~s/\/$//;
	return $filename; 
}

# EOF

