<%@ LANGUAGE = VBScript %>
<%  Option Explicit		%>

<!-- http://localhost/ADOSamp/ADOSQL2K/ForXML.asp	-->
<!-- Copyright (c) 2001	 Microsoft Corporation		-->

<HTML>
<HEAD>
<META NAME="GENERATOR" Content="Microsoft Developer Studio">
<META HTTP-EQUIV="Content-Type" content='text/html'; charset='iso-8859-1'>

<TITLE>ADO 2.7 Samples</TITLE>
<!-- Include the ADO Header file   -->
<!-- #include file="..\adovbs.inc" -->
<%
	Response.Write "<H3>Server-side processing</H3>"
	Response.Write "Page Generated @ " & Now() & "<BR/>"

	' Create the ADO Connection Object
    Dim adoConn
    Set adoConn = Server.CreateObject("ADODB.Connection")

	' Create the ADO Connection String
	' Edit to run in your environment
    Dim sConn
    sConn = "Provider=SQLOLEDB;Data Source=(local);Initial Catalog=Northwind;User ID=SA;Password=;"

	Response.write "Connect String = " & sConn & "<BR/>"

	' Set the Connection String and open the Connection
    adoConn.ConnectionString = sConn
    adoConn.Open

    Response.write "ADO Version = " & adoConn.Version & "<BR/>"
    Response.write "adoConn.State = " & adoConn.State & "<BR/>"

	' Create the ADO Command Object and associate the open ADO Connection with it
	Dim adoCmd
    Set adoCmd = Server.CreateObject("ADODB.Command")
    Set adoCmd.ActiveConnection = adoConn

	' Build a Template Query. Template queries can be stored in files,
	' or passed in as strings is the case here.
	' This query will retrieve all data from the products table in the form of an XML document.
	Dim sQuery
	sQuery = "<ROOT xmlns:sql='urn:schemas-microsoft-com:xml-sql'>"
	sQuery = sQuery + "<sql:query>SELECT * FROM PRODUCTS ORDER BY PRODUCTNAME FOR XML AUTO</sql:query>"
	sQuery = sQuery + "</ROOT>"

	Response.write "Query String = " & sQuery & "<BR/>"

	' Instead of passing in the query via Command.CommandText, we are using an ADO Stream object.
	' ADO Stream objects can be used as input (for queries) and as output (for XML documents)
	Dim adoStreamQuery
    Set adoStreamQuery = Server.CreateObject("ADODB.Stream")
    adoStreamQuery.Open
	
	' Write out Template Query to the Stream
	adoStreamQuery.WriteText sQuery, adWriteChar
	
	' Remember to reset the stream pointer to the beginning of the stream,
	' otherwise, the reader of the Stream won't see the query.
	adoStreamQuery.Position = 0

	' Associate our Stream Object with the Command Object
    adoCmd.CommandStream = adoStreamQuery

	' Tell the Command Object and the underlying OLE DB Provider what command dialect 
	' to use to parse this query. In this case we are specifying Template Query syntax (MSSQLXML).
	adoCmd.Dialect = "{5D531CB2-E6Ed-11D2-B252-00C04F681B71}"
 
	Response.write "Pushing XML to client for processing "  & "<BR/>"

	' Instead of writing results to a Recordset Object, we are specifying that the XML results 
	' should be written to the ASP Response Object which is a Stream Object. This will push the 
	' XML results down to the client, to be rendered in the client's browser.
	adoCmd.Properties("Output Stream") = Response

	' Wrap the resulting XML in an XML Data Island.
	Response.write "<XML ID=MyDataIsle>"

	' Execute the Template Query and pass in the adExecuteStream flag indicating that the
	' output of the command will be sent to the Stream Object referred to in the 
	' CommandStream property and not a Recordset Object.
	adoCmd.Execute , , adExecuteStream
	
	' Corresponding close tag for the XML Data Island
	Response.write "</XML>"
%>

<SCRIPT language="VBScript" For="window" Event="onload">
	
	' This code is executed in the client browser at page-load time
	Dim xmlDoc
	Set xmlDoc = MyDataIsle.XMLDocument
	xmlDoc.resolveExternals=false
	xmlDoc.async=false
		
	Dim root, child
	Set root = xmlDoc.documentElement

	' Iterate through the XML Data Island and display each ProductName.
	For each child in root.childNodes
		dim OutputXML
		OutputXML = document.all("log").innerHTML
		document.all("log").innerHTML = OutputXML & "<LI>" & child.getAttribute("ProductName") & "</LI>"
	Next
	
</SCRIPT>

</HEAD>

<BODY>
<LINK REL="STYLESHEET" HREF="..\ADOStyle.css">

	<H3>Client-side processing of XML Document MyDataIsle</H3>
	<UL id=log>
	</UL>

</BODY>
</HTML>
