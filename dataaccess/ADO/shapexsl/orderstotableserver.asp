<HTML>
<HEAD>
<TITLE>ADO & XML Order Processing</TITLE>

<!-- #include file="..\adovbs.inc" -->
<%

	Response.Write "<H3>Server-side processing</H3>"
	Response.Write "Page Generated @ " & Now() & "<BR/>"

	' Get Web Server Name for use in building URLs
	dim sWebServerName
	sWebServerName = Request.ServerVariables("Server_Name")

	' Grab the raw SQL query from the form variable/URL.
	dim sSQL
	sSQL = Request.QueryString("SQL")

	' Wrap it in the proper SQL XML query dialect tags.
	Dim sQuery
    sQuery = "<ROOT xmlns:sql='urn:schemas-microsoft-com:xml-sql'><sql:query>" & sSQL
	sQuery = sQuery & " FOR XML AUTO</sql:query></ROOT>"

	' Create and open a connection to a SQL2000 server.
    Dim adoConn
    Set adoConn = Server.CreateObject("ADODB.Connection")

    Dim sConn
    sConn = "Provider=SQLOLEDB;Data Source=(local);Initial Catalog=Northwind;Integrated Security=SSPI"

	Response.write "Connect String = " & sConn & "<BR/>"

    adoConn.ConnectionString = sConn
    adoConn.Open

	' Create a command object to execute the query above.
	Dim adoCmd
    Set adoCmd = Server.CreateObject("ADODB.Command")
    Set adoCmd.ActiveConnection = adoConn

	Response.write "Query String = " & sQuery & "<BR/>"

	' Open a stream object to hold the raw XML output from SQL Server.
    Dim adoStreamResults
    Set adoStreamResults = Server.CreateObject("ADODB.Stream")
    adoStreamResults.Open

	' Fill in the correct command properties so that SQL2000 recognizes this
	' as an XML query and knows we want the output in a stream, then execute.
    adoCmd.CommandText = sQuery
	adoCmd.Dialect = "{5D531CB2-E6Ed-11D2-B252-00C04F681B71}"
	adoCmd.Properties("Output Stream") = adoStreamResults
	adoCmd.Execute , , 1024

	Dim sResults 
	adoStreamResults.Position = 0
	sResults = adoStreamResults.ReadText
	Response.write "Len(sResults) = " & len(sResults) & "<BR>"
	Response.write "<H3>Server-side processing of XML/XSL Transform</H3>"

 	' Since we are processing the data on the server-side we
	' need to turn the XML into HTML before we stream it out.
    Dim xmlDoc, htmlOut, xslDoc, xslt, xproc

  	' Load document with source XML (from the query).
    Set xmlDoc = CreateObject("MSXML2.FreeThreadedDOMDocument")
    xmlDoc.async = False
    If Not (xmlDoc.LoadXML(sResults)) Then
        Response.write "xmlDoc.Load Returned: " & xmlDoc.parseError.reason & "<BR>"
    End If

  	' Load document with transform (XSL).
    Set xslDoc = CreateObject("MSXML2.FreeThreadedDOMDocument")
    xslDoc.async = False
    If Not (xslDoc.Load(Server.MapPath("tablefromxmlhier.xsl"))) Then
        Response.write "xslDoc.Load Returned: " & xslDoc.parseError.reason & "<BR>"
    End If

    Set htmlOut = CreateObject("MSXML2.FreeThreadedDOMDocument")

	' Run the transform to put the results in xmlOut
    Set xslt = CreateObject("MSXML2.XSLTemplate")
    Set xslt.stylesheet = xslDoc
    Set xproc = xslt.createProcessor
    xproc.input = xmlDoc
    xproc.output = htmlOut
    xproc.Transform

	' Write the newly-created HTML table into the response stream.
	Response.write htmlOut.XML

%>
</HEAD>
<BODY>
<LINK REL="STYLESHEET" HREF="..\ADOStyle.css">
</BODY>
</HTML>
