<HTML>
<HEAD>
<%
	' This page is just to accept the input variables chosen by the user and
	' build a corresponding SQL statement for use by the next page in doing
	' the real work.
	
	' Get Web Server Name for use in building URLs
	dim sWebServerName
	sWebServerName = Request.ServerVariables("Server_Name")

	'Grab form variables.
	dim nGroupBy, fClient, sString
	nGroupBy = Request.QueryString("GroupBy")
	fClient = Request.QueryString("ClientProc")
	sKeyValue = Request.QueryString("KeyVal")

	' 1 = grouping Orders table by Customers table
	If (nGroupBy = 1) Then
		sString = "SELECT * FROM CUSTOMERS c,ORDERS o WHERE c.customerid = o.customerid"
		sKeyCol = "c.customerid"
	' 2 = grouping Orders table by Employees table
	ElseIf (nGroupBy = 2) Then
		sString = "SELECT * FROM EMPLOYEES e,ORDERS o WHERE e.employeeid = o.employeeid"
		sKeyCol = "e.employeeid"
	' 3 = grouping Orders table by Shippers table
	ElseIf (nGroupBy = 3) Then
		sString = "SELECT * FROM SHIPPERS s,ORDERS o WHERE s.shipperid = o.shipvia"
		sKeyCol = "s.shipperid"
	End If

	' Restrict queries to a particular parent record if so requested.
	If Len(sKeyValue) <> 0 Then
		sString = sString & " AND " & sKeyCol & " = '" & sKeyValue & "'"
	End If

	sString = sString & " ORDER BY " & sKeyCol

	' Redirect to client-processing page or server-processing page.
	If (fClient = "on") Then
		Response.Redirect "http://" & sWebServerName & "/ADOSamp/ShapeXSL/OrdersToTableClient.asp?SQL=" & sString
	Else
		Response.Redirect "http://" & sWebServerName & "/ADOSamp/ShapeXSL/OrdersToTableServer.asp?SQL=" & sString
	End If
%>
</HEAD>
<BODY>
</BODY>
</HTML>
