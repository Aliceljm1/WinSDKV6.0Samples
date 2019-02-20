<%@ LANGUAGE = VBScript %>
<%  Option Explicit		%>

<!-- http://localhost/ADOSamp/ADOSQL2K/XPATH.asp	-->
<!-- Copyright (c) 2001 Microsoft Corporation		-->

<HTML>
<HEAD>
<TITLE>ADO 2.7 Samples</TITLE>

<!-- Include the ADO Header file   -->
<!-- #include file="..\adovbs.inc" -->
<%

	' Using the ASP Request Object, get the CompanyName specified in the URL.
	dim sCompanyName
	sCompanyName = Request.QueryString("CompanyName")

	' If no CompanyName was specified, display instructions.
	If Len(sCompanyName) = 0 then
		Response.redirect "http://localhost/ADOSamp/ADOSQL2K/XPATHErr.asp"
	Else

		Response.Write "<H3>Server-side processing</H3>"
		Response.Write "Page Generated @ " & Now() & "<BR/>"
		Response.Write "sCompanyName = " & sCompanyName & "<BR/>"

		' Create the XPath Command String
		' This command searches Customer elements containing the CompanyName
		' string passed in via the Request Object.
		Dim sCmd
		sCmd = "Customer[@CompanyName=" & sCompanyName & "]"

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

		' Tell the Command Object and the underlying OLE DB Provider what command dialect 
		' to use to parse this query. In this case we are specifying XPath syntax.
		adoCmd.Dialect = "{ec2a4293-e898-11d2-b1b7-00c04f680c56}"

		' Instead of writing results to a Recordset Object, we are specifying that the XML results 
		' should be written to the ASP Response Object which is a Stream Object. This will push the 
		' XML results down to the client, to be rendered in the client's browser.
		adoCmd.Properties("Output Stream") = Response

		' In order to map XML elements and attributes to SQL Tables and Columns,
		' a mapping schema must be used. Two new properties must be set indicating 
		' the name and location of the mapping schema.
		' Note that the Base Path property is specified by a physical path and may
		' need to be modified to run in your environment.
		adoCmd.Properties("Base Path") = "C:\INETPUB\WWWROOT\ADOSamp\ADOSQL2K"
		adoCmd.Properties("Mapping Schema") = "Orders.xml"

		' Store the XPath command in the CommandText property.
		adoCmd.CommandText = sCmd

		Response.write  "adoCmd.CommandText = " & adoCmd.CommandText  & "<BR/>"

		' Wrap the resulting XML in an XML Data Island.
		Response.write "<XML ID=MyDataIsle>"

		' Execute the XPath Query and pass in the adExecuteStream flag indicating that the
		' output of the command will be sent to the Stream Object referred to in the 
		' CommandStream property and not a Recordset Object.
		adoCmd.Execute , , adExecuteStream

		' Write the corresponding close tag for the XML Data Island
		Response.write "</XML>"

	End If

%>

<SCRIPT language="VBScript" For="window" Event="onload">
	
	Dim xmlDoc
	Set xmlDoc = MyDataIsle.XMLDocument
	xmlDoc.resolveExternals=false
	xmlDoc.async=false

	Dim root, child
	Set root = xmlDoc.documentElement

	OutputHeader = document.all("Header").innerHTML 
	OutputHeader = OutputHeader & "CustomerID: " & root.getAttribute("CustomerID")
	document.all("Header").innerHTML = OutputHeader 
	
	' Display the Order # and Order Date for each order in the XML results
	For each child in root.childNodes
		dim OutputOrders, OrderList
		OutputOrders = document.all("Orders").innerHTML 
		OrderList = "<LI> Order # " & child.getAttribute("OrderID") & ", Date: " & child.getAttribute("OrderDate") & "</LI>"
		TotalPage = OutputOrders & OrderList
		document.all("Orders").innerHTML = TotalPage
	Next

</SCRIPT>

</HEAD>
<BODY>
	<LINK REL="STYLESHEET" HREF="..\ADOStyle.css">

	<H3>Client-side processing of XML Document MyDataIsle</H3>
	<DIV id=Header></DIV>
	<UL id=Orders></UL>
</BODY>
</HTML>
