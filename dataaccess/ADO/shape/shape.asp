<%@ LANGUAGE = VBScript %>
<%  Option Explicit		%>

<!-- http://localhost/ADOSamp/SHAPE/Shape.asp	-->
<!-- Copyright (c) 2001 Microsoft Corporation		-->

<HTML>
<HEAD>
<META NAME="GENERATOR" Content="Microsoft Developer Studio"/>
<META HTTP-EQUIV="Content-Type" content="text/html; charset='iso-8859-1'"/>
<TITLE>OpenXML</TITLE>

<!-- #include file="..\adovbs.inc" -->
<%
		Response.Write "Page Generated @ " & Now() & "<BR/>"

		Dim sConn
		sConn = "Provider=MSDataShape;Data Provider=SQLOLEDB;Data Source=(local);Initial Catalog=Northwind;Integrated Security=SSPI"

		Response.write "Connect String = " & sConn & "<BR/>"

		Dim adoConn
		Set adoConn = Server.CreateObject("ADODB.Connection")
		adoConn.ConnectionString = sConn
		adoConn.Open

		Response.write "ADO Version = " & adoConn.Version & "<BR/>"
		Response.write "adoConn.State = " & adoConn.State & "<BR/>"
		Response.write "<BR/>"

		Dim strOrders
		Dim strProducts
		

		'The example shape command constructs a two level hierarchy. Products is the 
		'parent or root recordset. Order details is the child recordset. 
		'The shape command is constructed by nesting the two shape commands. 
		'
		'The inner shape command (strOrders) defines the child (OrderDetails) recordset. 
		'It contains all of the columns from the northwind Order Details table plus the 
		'Amount column added by the shape povider. The shape provider takes the recordset 
		'produced by the data provider command (in this case "Select * from [Order Details]") 
		'and adds to it any columns defined in the APPENDs clause (in this case the 
		'CALC (calculated) column named amount). 
		  
	    strOrders = "SHAPE {Select * from [Order Details]} AS OrderDetails APPEND CALC (CCur((UnitPrice*Quantity)*(1- Discount))) As Amount"

		'The Outer shape command (stProducts) defines the root recordset. It has the same
		'basic form as the inner command. There is a data provider command (in this case 
		'"select ProductID,ProductName,SupplierID from Products") and an APPENDs clause 
		'which defines columns to be added to the recordset. In this case two columns are
		'added. A chapter column (orders) and an aggregate column (sales). The chapter 
		'column creates a hierarichal relationship between this recordset and a child recordset. 
		'The inner shape command is part of the chapter column definition and defines the child
		'recordset. The RELATEs clause "RELATE ProductID TO ProductID" defines the 
		'relationship between the two recordsets. The aggreate column in this case is a 
		'SUM over the amount column in the child recordset. 

		strProducts = "SHAPE {select ProductID,ProductName,SupplierID from Products} AS Products APPEND ((" & strOrders & ") RELATE ProductID to ProductID) AS Orders,SUM(Orders.Amount) As Sales"

		Response.write "Query = " & strProducts & "<BR/>"

		Response.write "<BR/>"

		Dim adoCmd
		Set adoCmd = Server.CreateObject("ADODB.Command")
		Set adoCmd.ActiveConnection = adoConn
		adoCmd.CommandText = strProducts
		adoCmd.CommandType = adCmdText
		

	    Dim adoRSProducts
		Set adoRSProducts = adoCmd.Execute()

		Dim adoRSOrders
		'ADO exposes a shape chapter column as a recordset which contains rows from
		'the child recordset which match the parent row. 
		Set adoRSOrders = adoRSProducts.fields("Orders").value


		dim indx
		indx = 0
		while indx < 4
			'Note this example depends on ADO's stay in sync feature implicitly closing and 
			'     repoening adoRSOrders when the parent recordset's row position is changed. 
			'     If for your application this behavior is  undesirable you can turn it off 
			'     by setting the StayInSync property to false. In this example if StayInSync
			'     was false it would be necessary to move "Set adoRSOrders .." inside the
			'     outer while loop. 
			
			Response.write "Product ID= " & adoRSProducts.Fields("ProductID").Value & ",Product Name= " & adoRSProducts.Fields("ProductName").Value & ", Sales= " & adoRSProducts.Fields("Sales").Value &"<BR/>"
			while adoRSOrders.EOF = FALSE
				Response.write "....Product ID= " & adoRSOrders.Fields("ProductID").Value & ", Unit Price= " & adoRSOrders.Fields("UnitPrice").Value &", Quantity= " & adoRSOrders.Fields("Quantity").Value &", Discount= " & adoRSOrders.Fields("Discount").Value &", Sale Amount= " & adoRSOrders.Fields("Amount").Value &"<BR/>"
				adoRSOrders.MoveNext
			Wend
			adoRSProducts.MoveNext
			indx = indx + 1
		wend

		adoRSProducts.Close
		adoRSOrders.close

		
%>
</HEAD>
<BODY>
<LINK REL="STYLESHEET" HREF="..\ADOStyle.css">
</BODY>
</HTML>
