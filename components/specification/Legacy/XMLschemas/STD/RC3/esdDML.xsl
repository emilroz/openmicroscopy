<?xml version="1.0"?>
<!-- 
SchemaDOC Stylesheet for DDML documents.
Copyright TIBCO Extensibility - 2000-2001

$Id: esdDML.xsl 457 2008-06-30 15:08:37Z andrew $

-->
<s:stylesheet xmlns:s="http://www.w3.org/1999/XSL/Transform" version="1.0" 
	xmlns="http://www.w3.org/TR/REC-html40" 
	xmlns:dml = "http://www.purl.org/NET/ddml/v1" 
	xmlns:DDML = "http://www.purl.org/NET/ddml/v1" 
	xmlns:xalan = "http://xml.apache.org/xalan"
	xmlns:t = "http://www.tibco.com/namespaces/schemadoc/temporary"
	exclude-result-prefixes="xalan t dml DDML">

<s:import href="images.xsl" />
<s:import href="schemadoc_common.xsl" />

<s:param name="imageDir" select="'sdimages'"/>
<s:param name="schemaName" select="''"/>
<s:param name="doImages" select="'true'"/>
<s:param name="doSource" select="'true'"/>

<s:output method="html" indent="yes"/>
<s:strip-space elements="*"/>

<s:key name="elementRefs" match="dml:Ref" use="@Element" />

<!-- ========================================================================
========================================================================== -->		
<s:template match="/">
	<html>
		<head>
			<title>SchemaDOC <s:value-of select="$schemaName"/> </title>
			<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"/>
			<meta name="description" content="SchemaDoc generated by Tibco Extensibility DDML stylesheet."/>
			<meta name="copyright" content="Tibco Extensibility 2001"/>

			<link rel="stylesheet" title="Schemadoc Stylesheet" HREF="../schemadoc.css"/>				
			<script language="javascript" src='../sourceutils.js'>&#160;</script>
			<s:if test="$doImages='true' or $doImages='TRUE'">
				<script language="javascript" src='../svgcheck.js'>&#160;</script>
				<script language="VBScript" src='../svgcheck.vbs'>&#160;</script>
			</s:if>
		</head>
		<body bgcolor="#FFFFFF" marginheight="0" marginwidth="0" topmargin="0" leftmargin="0">
			<s:if test="$doImages='true' or $doImages='TRUE'">
				<!-- removed SVN check -->	 
			</s:if> 	
			<s:apply-templates/>
			<center>
			<s:if test="$doSource='true' or $doSource='TRUE'">
			<table border="0" WIDTH="90%" BGCOLOR="#eeeeee" >
			<tr>
									 <td> 
					<s:apply-templates mode="dtdSource" select="./dml:DocumentDef/dml:dtdsource"/>
				</td>
				 </tr>
			 </table>
			 </s:if>
			</center>
			
		</body>
	</html>
</s:template>

<!-- ========================================================================
========================================================================== -->		
<s:template match="dml:DocumentDef">
		<table width="90%" border="0" cellspacing="4" cellpadding="2" align="center">
			<tr  >
					<td bgcolor="#003366"  colspan="2" >
						<font face="Arial,Helvetica" size="5" color="#FFFFFF">
						
						XML DTD 
						<s:if test="@dml:Name">
							:
							<s:value-of select="@dml:Name"/> - 
						</s:if>
						<s:if test="$schemaName">
							<s:value-of select="concat(' : ', $schemaName)"/>
						</s:if>
					</font>
				</td>
			</tr>
		     <s:if test="dml:DDML.Doc">
			<tr >
				<td width="20%" class="headers" align="right" valign="top" wrap="true">
					Schema Comments:
				</td>
				<td class="descriptions" align="left" bgcolor="#EEEEEE" fgcolor="#FFAA00" valign="top">
					<font color="#990000">
					<s:apply-templates mode="comments" select="./DDML.Doc"/></font>
				</td>
			</tr>
		     </s:if>
			<tr>
				<td width="20%" class="headers" align="right" valign="top" wrap="true">
					Schema has:
				</td>
				<td class="values" bgcolor="eeeeee" align="left" valign="top">
					<s:number value="count(dml:ElementDecl)"/>
									element definitions and 									 
									<s:number value="count(dml:ElementDecl//dml:AttDef)"/>
									element attribute definitions.
				</td>
			</tr>

			<tr>
				<td width="20%" class="headers" align="right" valign="top" wrap="true">
					Possible root elements:
				</td>
				<td class="values" bgcolor="eeeeee" align="left" valign="top">
					<s:call-template name="dumpPossibleRoots" />
				</td>
			</tr>
		</table>
		<p/>
		<p/>		<p/>
		<table width="90%" border="0" cellspacing="0" cellpadding="2" align="center">
	    <tr  >
					<td bgcolor="#003366"  >
						<font face="Arial,Helvetica" size="5" color="#FFFFFF">
							Element list
						</font>
				</td>
			</tr>			
		</table>
		<p/>
		<s:apply-templates mode="anElement" select="dml:ElementDecl"/>

		<P/>
		<P/>
		<P/>
		<s:if test="$doSource='true' or $doSource='TRUE'">

		<table width="90%" border="0" cellspacing="0" cellpadding="2" align="center">
			<tr>
			<td bgcolor="#bbbbbb"  >
				<font face="Arial,Helvetica" size="5" color="#FFFFFF">
					<s:if test="@Name">
						<s:value-of select="@Name"/>
					</s:if>
					Original Schema Source

				</font>
			</td>
			</tr>			
		</table>	
		</s:if>	
	</s:template>
<!--END s:schema***********************************************************-->

<!-- ========================================================================
	First computes a list of possible roots, then dumps then list to the output
-->
<s:template name="dumpPossibleRoots">

	<s:variable name="elementList" select="dml:ElementDecl" />
	<s:variable name="possibleRoots" select="$elementList[ not( key('elementRefs', @Name) ) ]" />
	
	<s:call-template name="listElements">
		<s:with-param name="elementList" select="$possibleRoots" />
		<s:with-param name="emptyMessage" select="'No obvious root elements' " />
	</s:call-template>
	
</s:template>

<!-- ========================================================================
========================================================================== -->		
	<s:template mode="anElement" match="dml:ElementDecl">
		<s:variable name="prefix">
			<s:choose>
				<s:when test="@Prefix"><s:value-of select="@Prefix" />:</s:when>
				<s:otherwise></s:otherwise>
			</s:choose>
		</s:variable>
		<s:variable name="componentPath" select="concat('e:', $prefix, @Name)" />
		
		<center>
			<table width="90%" cellspacing="0" cellpadding="2" border="0" >
				<tr bgcolor="#003366">
					<td>
						<font face="Arial,Helvetica" color="#FFFFFF">
							Element
							<a name="{concat('ELEMENT_', @Name)}">
								<b>
									<s:value-of select="@Name"/>
								</b>
							</a>
						</font>
					</td>
				</tr>
			</table>
		</center>
		<table width="90%" cellspacing="0" cellpadding="0" border="0" bordercolor="#FFFFFF" bordercolordark="#FFFFFF" bordercolorlight="#FFFFFF" align="center">
			<tr>
				<td width="3%" bgcolor="#4444FF" align="left" valign="middle"></td>
				<td width="97%" align="left" valign="top">
					<table width="100%" cellspacing="3" cellpadding="1" border="0" bordercolor="#FFFFFF" align="left">
					
						<!-- insert the diagram row, if appropriate -->
						<s:call-template name="insertImageRow">
							<s:with-param name="doImages" select="$doImages" />
							<s:with-param name="componentPath" select="$componentPath" />
						</s:call-template>
						
						<s:if test="DDML:Doc">
							<tr>
								<td class="tdnames">description</td>
								<td width="85%" class="descriptions"  align="left" bgcolor="#eeeeeeee" wrap="true">
									<font color="#990000">
									<s:apply-templates mode="schemacomments" select="./DDML:Doc"/>
									
									</font>
								</td>
							</tr>
						</s:if>
						<s:if test="DDML:Doc/dml:USAGE">
							<tr>
								<td class="tdnames">usage notes</td>
								<td width="85%" class="descriptions"  align="left" bgcolor="#eeeeeeee" wrap="true">
									<font color="#990000">
									
									<s:apply-templates mode="usagecomments" select="./DDML:Doc"/>
									</font>
								</td>
							</tr>
						</s:if>
						<tr>
							<td class="tdnames" >attributes</td>
							<td width="85%" class="values" bgcolor="#eeeeeeee">
								<s:apply-templates mode="glanceAttrib" select="./dml:AttGroup | ./dml:More"/>
							</td>
						</tr>
				 		<tr>
							<td class="tdnames">uses</td>
							<td width="85%" class="values" bgcolor="#eeeeeeee">
								<s:call-template name="dumpUsesList" />
							</td>
						</tr>
					</table>
				</td>
			</tr>
		</table>
		<table width="90%" cellspacing="0" cellpadding="0" border="0" bordercolor="#FFFFFF" bordercolordark="#FFFFFF" bordercolorlight="#FFFFFF" align="center">
			<tr>
				<td width="3%" bgcolor="#6666FF" align="center" valign="top"></td>
				<td width="97%" align="center" valign="left">
					<table width="100%" cellspacing="3" cellpadding="1" border="0" align="left" valign="top" bordercolor="#FFFFFF" bordercolordark="#FFFFFF" bordercolorlight="#FFFFFF">
						<!-- Find the parents -->
						<TR>
							<TD class="tdnames" >used by</TD>
							<TD class="values" bgcolor="#eeeeee" align="left" valign="top">
								
								<s:call-template name="dumpUsedBy" />
							</TD>
						</TR>
						<tr>
							<td class="tdnames">content</td>
							<td width="85%" class="values" bgcolor="#eeeeeeee">
<s:apply-templates mode="content" select="dml:Model"/> </td>
						</tr>
						<tr>
							<td class="tdnames">occurrence</td>
							<td width="85%" class="values" bgcolor="#eeeeeeee">
								<s:apply-templates mode="occurrence" select="dml:Model"/> </td>
						</tr>
					</table>
				</td>
			</tr>
		</table>
		<s:if test=".//dml:AttDef | ./dml:More/dml:DataType">
			<table width="90%" cellspacing="0" cellpadding="0" border="0"  bordercolor="#FFFFFF" bordercolordark="#FFFFFF" bordercolorlight="#FFFFFF" align="center">
				<tr>
					<td width="3%" bgcolor="#8888FF" align="left" valign="top"></td>
					<td width="97%" align="left" valign="top">
						<table width="100%" cellspacing="3" cellpadding="1" border="0"  align="left" valign="top" bordercolor="#FFFFFF" bordercolordark="#FFFFFF" bordercolorlight="#FFFFFF">
							<s:call-template name="attribDetailHeader"/>
							<s:apply-templates mode="detailAttrib" select=".//dml:AttGroup | .//dml:More"/>
						</table>
						</td>
					</tr>
				</table>
		</s:if>
		<s:if test="$doSource='true' or $doSource='TRUE'">
		<table width="90%" cellspacing="0" cellpadding="0" border="0"  bordercolor="#FFFFFF" bordercolordark="#FFFFFF" bordercolorlight="#FFFFFF" align="center">
				<tr>
					<td width="3%" bgcolor="#bbbbbb" align="left" valign="top"></td>
					<td width="97%" align="left" valign="top">
						<table width="100%" cellspacing="3" cellpadding="1" border="0"  align="left" valign="top" bordercolor="#FFFFFF" bordercolordark="#FFFFFF" bordercolorlight="#FFFFFF">

						<tr>
							<td class="tdnames">source</td>
							<td width="85%" class="source" bgcolor="#eeeeeeee" align="left" valign="top">
									 <s:apply-templates mode="dtdSource" select="./dml:dtdsource"/>
									
							</td>
						</tr>
					</table>
					</td>
				</tr>
			</table>
			</s:if>
		<p/>
	</s:template>

<!-- ========================================================================
	Invoked by outputUsesList to dump a link to a particular element.
-->
<s:template name="elementLinkByName">
	<s:param name="elementName"/>

	<s:if test="/dml:DocumentDef/dml:ElementDecl[@Name=$elementName]">
		<s:value-of select="concat('#ELEMENT_', $elementName)" />
	</s:if>
	
</s:template>

<!-- ========================================================================
	Get the list of things that we reference, then get the list of elements
	off the root that match those names.  Note that our DML generation current
	drops the names of elements that we reference that do not exist.  However,
	if it did not, the logic below would output a sorted list of referenced
	items, with links to those that actually occur in the document.
-->
<s:template name="dumpUsesList">

	<s:variable name="refs" select=".//dml:Ref" />	
	<s:choose>
		<s:when test="$refs">
			<!-- get a sorted list of items that we use. -->
			<s:variable name="refList">
				<s:for-each select="$refs">
					<s:sort select="@Element" />
					<s:value-of select="concat(@Element, ' ')"	/>
				</s:for-each>
			</s:variable>
			
			<!-- dump the list -->
			<s:call-template name="outputUsesList">
				<s:with-param name="list" select="$refList" />
			</s:call-template>
		</s:when>
		<s:otherwise>
			<span class="note">Does not use other elements.</span>
		</s:otherwise>
	</s:choose>

</s:template>

<!-- ========================================================================
	Generates a list of elements.
-->
<s:template name="listElements">
	<s:param name="elementList"/>
	<s:param name="emptyMessage"/>

	<s:choose>
		<s:when test="$elementList" >
			<s:for-each select="$elementList">
				<s:sort select="@Name" />
				
				<a class="element-link" href="{concat('#ELEMENT_', @Name)}">
					<s:value-of select="@Name" />
				</a>
				<s:if test="position() != last()">, </s:if>
			</s:for-each>
		</s:when>
		<s:otherwise>
			<!-- spit out a note that this is not used -->
			<span class="note"><s:value-of select="$emptyMessage" /></span>
		</s:otherwise>
	</s:choose>

</s:template>

<!-- ========================================================================
	Determines who uses elements by looking up their references by key, then
	looking up the ancestors of those references to find the containing element.
-->
<s:template name="dumpUsedBy">
	
	<s:variable name="referenceNodes" select="key('elementRefs', @Name)" />
	<s:variable name="referenceElements" select="$referenceNodes/ancestor::dml:ElementDecl" />
	
	<s:call-template name="listElements">
		<s:with-param name="elementList" select="$referenceElements" />
		<s:with-param name="emptyMessage" select="'Not used by other elements.  Possible root element.'" />
	</s:call-template>
	
</s:template>

<!-- ========================================================================
========================================================================== -->		
<s:template mode="dtdSource" match="dml:dtdsource">
	<s:value-of select="." disable-output-escaping= "yes" />
	&#160;
</s:template>


<!-- ========================================================================
-->		
	<s:template mode="glanceAttrib" match="dml:AttDef">
		<s:value-of select="@Name"/>
		<s:if test="position() != last()" >
			<s:text>, </s:text>
		</s:if>
	</s:template>
<!-- ========================================================================
========================================================================== -->		
	<s:template mode="glanceAttrib" match="dml:More">
		<s:value-of select="./dml:DataType"/>&#160;
	</s:template>
<!-- ========================================================================
========================================================================== -->		
	<s:template name="attribDetailHeader" >

							<tr>
								<th width="15%" class="headers" align="right" valign="top">
									Attribute
								</th>
								<th width="20%" class="headers" align="left" >
									Datatype
								</th>
								<th width="10%" class="headers" align="left" >
									Required
								</th>
								<th width="25%" class="headers" align="left"  >
									Values
								</th>
								<th width="10%" class="headers" align="left" >
									Default
								</th>
								<th width="20%" class="headers" align="left" >
									Comments
								</th>

							</tr>

	</s:template>
<!-- ========================================================================
========================================================================== -->		
	<s:template mode="detailAttrib" match="dml:AttDef">
		<tr>
			<td width="15%" class="headers" align="right" valign="top">
				<b>
					<s:value-of select="@Name"/>
				</b>
			</td>
			<td width="20%" class="values" bgcolor="#eeeeee" align="left" valign="top">
<!--				<A ><s:attribute name="href">#datatype_<s:value-of select="@dml:Type"/></s:attribute> </A>-->
       <s:choose>
        <s:when test="@Type">
          <s:value-of select="@Type"/>
        </s:when>
        <s:otherwise>
          string (default)
        </s:otherwise>
      </s:choose>
      
			</td>
		<td width="10%" class="values" bgcolor="#eeeeee" align="left" valign="top">
      <s:choose>
        <s:when test="@Required">
          <s:value-of select="@Required"/>
        </s:when>
        <s:otherwise>
          no (default)
        </s:otherwise>
      </s:choose>
			&#160;</td>
			<td width="25%" class="values" bgcolor="#eeeeee" align="left" valign="top">

<s:for-each select=".//dml:EnumerationValue">
	<s:value-of select="@Value" />&#160;
</s:for-each>
			</td>
		
			<td width="10%" class="values" bgcolor="#eeeeee" align="left" valign="top">

					<s:value-of select="@AttValue"/>
					&#160;
			</td>
			<td width="20%" class="descriptions" bgcolor="#eeeeee" align="left" valign="top">
					<font color="#990000" size="-1">
						<s:if test="DDML:Doc">
							<s:apply-templates mode="schemacomments" select="./DDML:Doc"/>
						</s:if>
						<s:if test="DDML:Doc/dml:USAGE">
							<br/>Usage Notes:<br/><s:apply-templates mode="usagecomments" select="./DDML:Doc"/>
						</s:if>
					&#160;
					 </font> 				
			</td>
		</tr>
	</s:template>
	
<!-- ========================================================================
========================================================================== -->		
	<s:template mode="detailAttrib" match="dml:More">
		<tr>
			<td width="15%" class="headers" align="right" valign="top">
				<b>
					<s:value-of select="@Name"/>
				</b>
			</td>
			<td width="20%" class="values" bgcolor="#eeeeee" align="left" valign="top">
<!--				<A ><s:attribute name="href">#datatype_<s:value-of select="@dml:Type"/></s:attribute> </A>-->
       <s:choose>
        <s:when test="./dml:DataType">
          <s:value-of select="./dml:DataType"/>
        </s:when>
        <s:otherwise>
          string (default)
        </s:otherwise>
      </s:choose>
      
			</td>
			<td width="20%" class="values" bgcolor="#eeeeee" align="left" valign="top">
&#160;
			</td>
			<td width="25%" class="values" bgcolor="#eeeeee" align="left" valign="top">
&#160;
			</td>
		
			<td width="20%" class="values" bgcolor="#eeeeee" align="left" valign="top">

					&#160;
			</td>
		</tr>
	</s:template>


	<!-- END detailattributetype TYPE -->
<!-- ========================================================================
========================================================================== -->		
		<s:template mode="anElement" match="element">
		<A><s:attribute name="HREF">#ELEMENT_<s:value-of select="@type"/></s:attribute><s:value-of select="@type"/>
		</A>  
	</s:template>

<!-- ========================================================================
========================================================================== -->		
	<s:template match="DDML:Doc" mode="schemacomments">
		 
		<s:choose> 
		<s:when test="not(@usage='USAGE')">
     			<s:value-of select="text()"/>
		</s:when> 
		</s:choose>   
			<!--<s:apply-templates mode="comments" />-->
	</s:template>
	
<!-- ========================================================================
========================================================================== -->		
	<s:template match="DDML:Doc" mode="usagecomments">
		<s:choose> 
		<s:when test="./dml:USAGE">
     			<s:value-of select="./dml:USAGE"/>
		</s:when> 
		</s:choose>   
	</s:template>

	
<!-- ========================================================================
========================================================================== -->		
	<s:template mode="content" match="dml:Model">

		<s:if test="dml:Ref">reference</s:if>
		<s:if test="dml:Choice">choice</s:if>
		<s:if test="dml:Seq">a sequence of elements</s:if>
		<s:if test="dml:Empty">empty </s:if>
		<s:if test="dml:Any">character data and a sequence of elements</s:if>
		<s:if test="dml:PCData">character data</s:if>
		<s:if test="dml:Mixed">character data and a sequence of elements</s:if>
	</s:template>

<!-- ========================================================================
========================================================================== -->		
	<s:template mode="occurrence" match="dml:Seq | dml:Choice |dml:Mixed |dml:Any">
  <s:choose>
    <s:when test="@Frequency">
        <s:value-of select="@Frequency"/>
    </s:when>
    <s:otherwise>
        required (default)
    </s:otherwise>
  </s:choose>	

	</s:template>

<!-- ========================================================================
========================================================================== -->		
	<s:template match="dml:Doc" mode="comments">
		<s:copy>
			<s:apply-templates mode="comments" />
		</s:copy>
	</s:template>
<!-- ##############################################################################    -->

<!--
	$Log$
	Revision 1.1  2006/12/20 22:00:16  siah
	draft STD schema that allows multilingual labels and descriptions

	Revision 1.6  2002/02/12 18:46:47  Eric
	Fixed issue with xslt.dtd, wherein images would not be displayed and exceptions
	thrown.
	
	Revision 1.5  2002/02/06 22:34:10  Eric
	Reenabled schemadoc options, minor improvements...
	
	Revision 1.4  2002/01/22 14:48:23  wraymond
	General: Canon v1.2
	Specific: SchemaDoc Changes
	
	Revision 1.3  2002/01/17 20:40:39  Eric
	Another round of fixes for schemadoc.
	
	Revision 1.2  2002/01/16 19:20:33  Eric
	Moved shared code, fixed uses in XSD, XSD-CR now in sync.
	
	Revision 1.1  2002/01/15 18:37:42  Eric
	Schemadoc test framework and files.
	
-->

</s:stylesheet>
