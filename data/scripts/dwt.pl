#!/usr/bin/perl

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

# Where is our configuration file?
$CONFFILE="$ENV{HOME}/.dwt";

# Check command line arguments
if ($ARGV[1] eq '') {
  print "usage: dwt.pl project filename\n";
  exit;
}
$project=$ARGV[0];
$filename=$ARGV[1];

# Get full Project path from configuration file
$projectpath=&GetProjectPath($project);
if ($projectpath eq '') {
  print "project '$project' not found in $CONFFILE\n";
  exit;
}

# Read file into buffer
open HF, $filename or die "Cannot open: $filename";
@v=<HF>; $source=join("",@v); $source=&Dos2Unix($source);
close HF;

# Extract template name from buffer
$templatename=&GetTemplateName($source);
if ($templatename eq '') {
  print "Could not extract template name from $filename\n";
  exit; 
}

# Open template file
$templatename=&BuildPathName($templatename,$projectpath);
open HF, $templatename or die "Cannot open template file: $templatename";
@v=<HF>; $templatesource=join("",@v);
$templatesource=&Dos2Unix($templatesource);
close HF;

# Correct links in template
$templatesource=&CorrectLinks($templatesource,$projectpath,$filename);

# Rebuild file using template
$p1=index($templatesource,"<!-- #BeginEditable ");
while ($p1>=0) {
  $p1=index($templatesource,"\"",$p1);
  if ($p1>=0) {
    $p1=$p1+1;
    $p2=index($templatesource,"\"",$p1);
    $region=substr($templatesource,$p1,$p2-$p1);
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
        print "Could not find region '$region' in template $templatename";
        exit;
      }
    } else {
      print "Could not find region '$region' in $filename";
      exit;
    }
  }
  $p1=index($templatesource,"<!-- #BeginEditable ",$p1);
} # while

# Editable blocks inserted, now build frame
$templatesource=&RemoveHtmlTags($templatesource);
$templatesource=&AddHtmlTags($templatesource,$source);

# Save new file
open HF,">$filename" or die "Cannot create $filename.new";
print HF $templatesource;
close HF;

# Extract template filename 
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
    }
  }  
  return $templatename;
}

# Add path specification to template filename, so it ca be opened
sub BuildPathName() {
  my $templatename=$_[0];
  my $prp=$_[1];
  $templatename="$prp/$templatename";
  # Remove unneeded slashes 
  $templatename=~s/\/\//\//g;
  return $templatename;
}

# Remove header and footer from Template
sub RemoveHtmlTags() {
  my $p1;
  my $templatesource=$_[0];
  $p1=index($templatesource,"<head>"); # 6 chars
  if ($p1>=0) {
    $p1=$p1+6;
    $templatesource=substr($templatesource,$p1);
  } else {
    print "Could not find <head> tag in template $templatename";
    exit;
  }
  $p1=index($templatesource,"</body>"); 
  if ($p1>=0) {
    $templatesource=substr($templatesource,0,$p1);
  } else {
    print "Could not find </body> tag in template $templatename";
    exit;
  }
  return $templatesource;
}

# Add header and footer tags from source file to template
sub AddHtmlTags() {
  my ($p1,$hstart,$hend);
  my $templatesource=$_[0];
  my $source=$_[1];

  $p1=index($source,"<head>"); # 6 chars
  if ($p1>=0) {
    $p1=$p1+6;
    $hstart=substr($source,0,$p1);
  } else {
    print "Could not find <head> tag in $filename";
    exit;
  }
  $p1=index($source,"</body>"); 
  if ($p1>=0) {
    $hend=substr($source,$p1);
  } else {
    print "Could not find </body> tag in $filename";
    exit;
  }
  return "$hstart$templatesource$hend"; 
}

# Remove '\r' (DOS CRLF Format -> UNIX)
sub Dos2Unix() {
  my $v=$_[0];
  $v=~s/\r//gm;
  return $v;
}

# Get project path from configuration file ~/.dwt
sub GetProjectPath() {
  my $pr=$_[0];
  my $prp="";
  my $line;
  my @v;
  open HF, "$CONFFILE" or die "Cannot open configuration file $CONFFILE - See README for help\n";
  while ($line=<HF> and $prp eq '') {
    @v=split('=',$line);
    if ($v[0] eq $pr) {
      $prp=$v[1];
      chomp $prp;
    }
  }
  close HF;
  return $prp;
}

# Correct template links in new created buffer
sub CorrectLinks() {
  my $ts=$_[0];
  my $prp=$_[1];
  my $filename=$_[2];
  my $pwd=$ENV{PWD};
  my $l;
  # Convert filename to full pathname. Maybe there is a better way to do that?
  # Could not find answer how to expand filename to full path at comp.lang.perl.*
  $filename="$pwd/$filename";
  $filename=~s/\/\//\//g;
  $filename=~s/\/\.//g;
  # Remove filename
  $l=rindex($filename,'/');
  $filename=substr($filename,0,$l);
  # Get remaining path specification 
  $l=length($prp); $filename=substr($filename,$l);
  $ts=~s/\.\.\//$filename/g;
  return $ts;
}

# EOF

