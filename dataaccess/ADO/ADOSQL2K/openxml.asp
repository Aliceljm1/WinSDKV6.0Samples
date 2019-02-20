<%@ LANGUAGE = VBScript %>
<%  Option Explicit		%>

<!-- http://localhost/ADOSamp/ADOSQL2K/OpenXML.asp	-->
<!-- Copyright (c) 2001 Microsoft Corporation		-->

<HTML>
<HEAD>
<META NAME="GENERATOR" Content="Microsoft Developer Studio"/>
<TITLE>ADO 2.7 Sample</TITLE>

<!-- Include the ADO Header file   -->
<!-- #include file="..\adovbs.inc" -->
<%
		Response.Write "Page Generated @ " & Now() & "<BR/>"

		' Create the ADO Connection String
		Dim sConn
		sConn = "Provider=SQLOLEDB;Data Source=(local);Initial Catalog=Northwind;User ID=SA;Password=;"

		Response.write "Connect String = " & sConn & "<BR/>"

		' Create the ADO Connection Object and open the connection
		Dim adoConn
		Set adoConn = Server.CreateObject("ADODB.Connection")
		adoConn.ConnectionString = sConn
		adoConn.Open

		Response.write "ADO Version = " & adoConn.Version & "<BR/>"
		Response.write "adoConn.State = " & adoConn.State & "<BR/>"

		' Create the XML Document to be processed by the Stored procedure
		Dim sXMLDoc
		sXMLDoc = "<ROOT>"
		sXMLDoc = sXMLDoc & "<Customers CustomerID='VINET' ContactName='Paul Henriot'>"
		sXMLDoc = sXMLDoc & "<Orders CustomerID='VINET' EmployeeID='5' OrderDate='1996-07-04T00:00:00'>"
		sXMLDoc = sXMLDoc & "<Order_0020_Details OrderID='10248' ProductID='11' Quantity='12'/>"
		sXMLDoc = sXMLDoc & "<Order_0020_Details OrderID='10248' ProductID='42' Quantity='10'/>"
		sXMLDoc = sXMLDoc & "</Orders>"
		sXMLDoc = sXMLDoc & "</Customers>"
		sXMLDoc = sXMLDoc & "<Customers CustomerID='LILAS' ContactName='Carlos Gonzlez'>"
		sXMLDoc = sXMLDoc & "<Orders CustomerID='LILAS' EmployeeID='3' OrderDate='1996-08-16T00:00:00'>"
		sXMLDoc = sXMLDoc & "<Order_0020_Details OrderID='10283' ProductID='72' Quantity='3'/>"
		sXMLDoc = sXMLDoc & "</Orders>"
		sXMLDoc = sXMLDoc & "</Customers>"
		sXMLDoc = sXMLDoc & "</ROOT>"

		' Create the query, in this case just the name of the Stored Procedure.
		Dim sQuery
	    sQuery = "SP_OpenXML_Example"
		Response.write "sQuery = " & sQuery & "<BR/>"

		' Create the Command Object and specify the command string, command type and parameters.
		Dim adoCmd
		Set adoCmd = Server.CreateObject("ADODB.Command")
		Set adoCmd.ActiveConnection = adoConn
		adoCmd.CommandText = sQuery
		adoCmd.CommandType = adCmdStoredProc
		adoCmd.Parameters.Refresh
		adoCmd.Parameters.Item(1).Value = sXMLDoc

		' Execute the command, and store the results in the Recordset adoRS.
	    Dim adoRS
		Set adoRS = adoCmd.Execute()

		' Display the contents of the Recordset on the web page.
		While Not adoRS.EOF
			Response.write "CustomerID = " & adoRS.Fields(0).Value & ", ContactName = " & adoRS.Fields(1).Value & "<BR/>"
			adoRS.MoveNext
		Wend

		adoRS.Close

		' Display the SP Return Value on the web page.
		Response.write "ReturnValue = " & adoCmd.Parameters.Item(0).Value & "<BR/>"

%>
</HEAD>
<BODY>
<LINK REL="STYLESHEET" HREF="..\ADOStyle.css">
</BODY>
</HTML>
