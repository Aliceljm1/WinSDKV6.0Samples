<HTML>
<HEAD>
<TITLE>ADO & XML Order Processing</TITLE>

<!-- #include file="..\adovbs.inc" -->
<%

	Response.Write "<H3>Client-side processing</H3>"
	Response.Write "Page Generated @ " & Now() & "<BR/>"

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
	adoCmd.Execute , , adExecuteStream

	Dim sResults 
	adoStreamResults.Position = 0
	sResults = adoStreamResults.ReadText
	Response.write "Len(sResults) = " & len(sResults) & "<BR>"

	' Since we are processing the data on the client-side just
	' wrap the raw XML in a data island tag and stream it out.
	Response.write "<XML ID=MyDataIsle>"
	Response.write sResults
	Response.write "</XML>"

%>

<SCRIPT language="VBScript" For="window" Event="onload">
    Dim xmlDoc, htmlOut, xslDoc, xslt, xproc

  	' Grab the XML from the island now that we are client-side.
    Set xmlDoc = MyDataIsle.XMLDocument

	' Create documents to hold the transform and the transformed results.
    Set htmlOut = CreateObject("MSXML2.FreeThreadedDOMDocument")
    Set xslDoc = CreateObject("MSXML2.FreeThreadedDOMDocument")

	' Pull the transform definition in from the server file.
    xslDoc.async = False

    ' Need to fixup URL
    ' If Not xslDoc.Load("c:\inetpub\wwwroot\adosamp\shapexsl\tablefromxmlhier.xsl") Then
    ' The following line works, but only from a remote machine.
    If Not xslDoc.Load("http://localhost/adosamp/shapexsl/tablefromxmlhier.xsl") Then
        document.all("Table").innerHTML = "xslDoc.Load Returned: " & xslDoc.parseError.reason
    End If

    ' Run the transform xmlDoc -> htmlOut
    Set xslt = CreateObject("MSXML2.XSLTemplate")
    Set xslt.stylesheet = xslDoc
    Set xproc = xslt.createProcessor
    xproc.input = xmlDoc
    xproc.output = htmlOut
    xproc.Transform

    ' Put the newly-created HTML table into the visible page.
    document.all("Table").innerHTML = htmlOut.xml

</SCRIPT>

</HEAD>
<BODY>
<LINK REL="STYLESHEET" HREF="..\ADOStyle.css">

	<H3>Client-side processing of XML Document MyDataIsle</H3>
	<DIV id=Table></DIV>
</BODY>
</HTML>
