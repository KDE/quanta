<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"
doctype-public="-//W3C//DTD XHTML 1.0 Transitional//EN"
doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"/>

<xsl:template match="/">
 <html>
  <xsl:attribute name="xmlns"><xsl:text>http://www.w3.org/1999/xhtml</xsl:text></xsl:attribute>
  <head>
   <title><xsl:value-of select="INFO/name"/></title>
   <meta>
    <xsl:attribute name="http-equiv"><xsl:text>Content-Type</xsl:text></xsl:attribute>
    <xsl:attribute name="content"><xsl:text>text/xhtml; charset=UTF-8</xsl:text></xsl:attribute>
   </meta>
   <link rel="stylesheet" type="text/css" href="info.css" />
  </head>
  <body>
   <h1><xsl:value-of select="INFO/name"/></h1>
   <table>
    <tr>
     <th>Author:</th>
     <td><xsl:value-of select="INFO/author"/></td>
    </tr>
    <tr>
     <th>Contact:</th>
     <td>
      <a>
       <xsl:attribute name="href">
        <xsl:text>mailto:</xsl:text>
        <xsl:value-of select="INFO/email"/>
       </xsl:attribute>
       <xsl:value-of select="INFO/email"/>
      </a>
     </td>
    </tr>
    <tr>
     <th>Website:</th>
     <td>
      <a>
       <xsl:attribute name="href">
        <xsl:value-of select="INFO/website"/>
       </xsl:attribute>
       <xsl:value-of select="INFO/website"/>
      </a>
     </td>
    </tr>
    <tr>
     <th>Version:</th>
     <td><xsl:value-of select="INFO/version"/></td>
    </tr>
    <tr>
     <th>License:</th>
     <td><xsl:value-of select="INFO/license"/></td>
    </tr>
    <tr>
     <th>About:</th>
     <td><xsl:value-of select="INFO/about"/></td>
    </tr>
   </table>
  <br /><br />
  <div align="center"><hr width="50%" />
  Quanta Information File<br />
  <em>The Quanta Team</em>
  </div>
  </body>
 </html>
</xsl:template>

</xsl:stylesheet>

