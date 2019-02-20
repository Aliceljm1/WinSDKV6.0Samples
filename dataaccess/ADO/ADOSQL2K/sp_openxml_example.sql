/* http://localhost/ADOSamp/ADOSQL2K/OpenXML.asp								*/
/* Copyright (c) 2001 Microsoft Corporation										*/

/* ADO 2.7 Sample - Using OpenXML												*/
/* This SQL Stored Procedure is required to run the ADO OpenXML Sample.			*/
/* To create and test the sample stored procedure, execute the following T-SQL.	*/

/* OpenXML allows this stored procedure to take an XML document as input,		*/
/* and creates a virual	Rowset, that T-SQL commands can be executed against.	*/
/* In this sample, an XML document containing customers and orders is passed 	*/
/* into the SP. SQL Server then constructs and in-memory hierarchical Rowset	*/
/* The Select statement then extracts the CustomerID and ContactName fields		*/
/* from the Rowset and returns them to the client as any other SP would			*/

USE Northwind
GO

IF EXISTS (SELECT name FROM sysobjects WHERE name = 'SP_OpenXML_Example' AND type = 'P')
    DROP PROCEDURE SP_OpenXML_Example
GO

CREATE PROCEDURE SP_OpenXML_Example 
	@XMLDoc varchar(2000)
AS
	DECLARE @ReturnCode INT
	DECLARE @iDoc int

	EXECUTE sp_xml_preparedocument @iDoc OUTPUT, @XMLDoc

	SELECT * FROM OpenXML(@iDoc, '/ROOT/Customers',1) 
		WITH (CustomerID varchar(10), ContactName varchar(20)) 

	EXECUTE sp_xml_removedocument @iDoc

	SELECT @ReturnCode = 1
	RETURN @ReturnCode
GO


/* Test the stored procedure */

EXEC SP_OpenXML_Example '<ROOT>
<Customers CustomerID="VINET" ContactName="Paul Henriot">
   <Orders CustomerID="VINET" EmployeeID="5" OrderDate="1996-07-04T00:00:00">
      <Order_0020_Details OrderID="10248" ProductID="11" Quantity="12"/>
      <Order_0020_Details OrderID="10248" ProductID="42" Quantity="10"/>
   </Orders>
</Customers>
<Customers CustomerID="LILAS" ContactName="Carlos Gonzlez">
   <Orders CustomerID="LILAS" EmployeeID="3" OrderDate="1996-08-16T00:00:00">
      <Order_0020_Details OrderID="10283" ProductID="72" Quantity="3"/>
   </Orders>
</Customers>
</ROOT>'

