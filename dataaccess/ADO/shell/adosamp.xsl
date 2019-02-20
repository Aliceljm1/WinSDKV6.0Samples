<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/TR/WD-xsl">

    <xsl:template match="/">

    <HTML>
	<HEAD>
	<BASE TARGET="main"/>
	</HEAD>

	<BODY>
	<LINK REL="STYLESHEET" HREF="ADOStyle.css"/>

	<TABLE WIDTH='100%' BORDER='0' >
	<TR><TD ALIGN='center'>
	<A HREF="ADO27.HTM"><IMG SRC='adologo.gif' ALT='ADO 2.7 Rocks!' BORDER='0'></IMG></A>
	</TD></TR>
	<TR><TD ALIGN='center'>
	<H2>ActiveX Data Objects (ADO) 2.7 Samples</H2>
	</TD></TR>
	</TABLE>

	<H3>Click on a sample in the list below. 
		A description and links for running the sample 
		and viewing the code will appear.
		Instructions for setting up and using the sample application 
		will appear in the frame on the right.</H3>
	 
         <OL>
          <xsl:for-each select="root/samples/sample">
     
	    <LI STYLE="cursor: hand" onmouseover="this.style.color='red'" onmouseout="this.style.color=''">
              <xsl:attribute name="onclick">toggle(span<xsl:value-of select="@id"/>)</xsl:attribute>
              <B><xsl:value-of select="title"/></B><BR/>
              <SPAN STYLE="display='none';">
                <xsl:attribute name="ID">span<xsl:value-of select="@id"/></xsl:attribute>
                <xsl:value-of select="desc"/>
                <BR/>
                <A>
                <xsl:attribute name="HREF"><xsl:value-of select="read"/></xsl:attribute> 
                Read the Sample notes
                </A><BR/>
                <A>
                <xsl:attribute name="HREF"><xsl:value-of select="run"/></xsl:attribute> 
                Run the Sample
                </A><BR/>
                <A>
                <xsl:attribute name="HREF"><xsl:value-of select="view"/></xsl:attribute> 
                View the Code
                </A><P/>
              </SPAN>
            </LI>
          </xsl:for-each>
        </OL>

        <xsl:for-each select="root/shortcuts/shortcut">
	 		<A>
			<xsl:attribute name="HREF"><xsl:value-of select="url"/></xsl:attribute>
			</A>
			<xsl:value-of select="desc"/>
            <BR/>
        </xsl:for-each>
		
	<P/>

        <SCRIPT>
		function toggle(e) 
		{
		  if(window.event.srcElement.tagName != "A")
		  {
		    if (e.style.display == "none") 
		    {
			  e.style.display = "";
		    } 
		    else 
		    {
			  e.style.display = "none";
		    }
		  }
		}
        </SCRIPT>

      </BODY>
    </HTML>

 </xsl:template>
</xsl:stylesheet>
