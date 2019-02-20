
 HeadDump: requests the default HTML document from the server and prints 
			along with HTTP headers.
			Microsoft Corporation (C). L. B.
	
 PURPOSE: 
 This sample demonstrates how to create and submit HTTP 
 request. Sample requests the default HTML document from the
 server and then display it along with the HTTP transaction headers.
 This sample also allows to access password protected pages. It 
 checks for HTTP server response code and it is "401 Access Denied"
 it asks password and user name and then resubmit request.


 This sample features: 
  - SSL/PCT support
  - NTLM Authentication (when IE 3.0 or above is installed)
  - Basic Authentication
  - Generating standard user name/password UI to get user credentials.
  - Getting user name and password without UI
  - getting any document of any MIME type
  - translation of Internet error codes to the extended error messages
  - ability to save remote object to a local file (when no local file
	 specify output goes to STDOUT).

 Usage:
 To create headdump.exe you need to link wininet.lib or just use provided make  file and type 'nmake' at the command prompt.

   headdump.exe -h host [-o object] [-p proxy] [-u] [-s] [-?] [-f file]
		-h host is a HTTP server such as www.server.com 
		-p proxy is optional proxy, in form: http://proxy:80
		-o object is optional document to GET, in form: /foobar.htm
		-u use this flag to bring user name and password UI (optional)
		-f save remote object as local file  (optional)
		-? get help
		-s use SSL (optional)

 Samples:
 To get help on command line parameters:
	C:> headdump.exe -?	
	
 Note that HeadDump can also invoke ISAPI dll or CGI like this:
 	C:> headdump.exe -s www.server.name -o /scripts/myisapi.dll 

 Since sample generates large output, you may want to pipe to more:
 	C:> headdump.exe -s www.server.name -o /scripts/myisapi.dll  | more

 To save remote HTML object as file:
	C:> headdump.exe -s www.server.name -o /exe_files/foo.exe -f foo.exe
