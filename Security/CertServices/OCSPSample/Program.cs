// This	example	demonstrates the following
//1. Setting the Responder Property	NumberOfBackEndConnections.
//2. Getting SigningFlags RevocationConfiguration property.



using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CertificateAdministration.Interop;
using System.Runtime.InteropServices;
using OCSPSample.Properties;

// you must	include	references to Microsoft.CertificateAdministration.Interop.dll
// found under %SystemRoot%\System32

namespace OCSPSample
{
	class Program
	{
		static string strServerName = "contososerver"; // change this to the computer name of your responder
		static string strConfigName = "contosoconfig"; // change this to any existing revocation configuration on your responder

		static string strResponderProperty = "NumberOfBackEndConnections"; // responder property
		static string strConfigurationProperty = "SigningFlags"; // revocation configuration property
		static void Main(string[] args)
		{
			System.Console.WriteLine(Resources.strConnecting +  strServerName);

			IOCSPAdmin IOcspAdmin = (IOCSPAdmin)(new OCSPAdminClass());			
			// connect to the responder 'strServerName'

			try
			{
				IOcspAdmin.GetConfiguration(strServerName,
												false // bForce is false, that means fail if the responder service is offline
												);

				System.Console.WriteLine(Resources.strConnected);
			}
			catch (COMException e)
			{
				System.Console.WriteLine(Resources.strNotConnected + e.Message);
				return;
			}

			// Setting the number of back end connections property
			int nBackEndConnections = 20;
			System.Console.WriteLine(strResponderProperty + " " + Resources.strPropertySet + nBackEndConnections.ToString());
			object objVal = nBackEndConnections;
			IOCSPProperty IOcspPropertyNumberOfBackendConnections;
			bool fCreate = false;
			bool fSet = false;

			// checking if the property exists
			try
			{
				IOcspPropertyNumberOfBackendConnections = (IOCSPProperty)(IOcspAdmin.OCSPServiceProperties.get_ItemByName(strResponderProperty));
				int nPreValue = (int)(IOcspPropertyNumberOfBackendConnections.Value);
				System.Console.WriteLine(strResponderProperty + " " + Resources.strPropertyValue + nPreValue.ToString());
				IOcspPropertyNumberOfBackendConnections.Value = objVal;
				fSet = true;
			}
			catch (COMException e)
			{
					System.Console.WriteLine(strResponderProperty + " " + Resources.strPropertyNotRetrieved + e.Message);
					fCreate = true;
			}

			if (fCreate)
			{
				try
				{
					IOcspAdmin.OCSPServiceProperties.CreateProperty(strResponderProperty, ref objVal);
					System.Console.WriteLine(strResponderProperty + " " + Resources.strPropertyCreated);
					fSet = true;
				}
				catch (COMException e)
				{
					System.Console.WriteLine(strResponderProperty + " " + Resources.strPropertyNotCreated + e.Message);
				}
			}

			if (fSet)
			{
				// apply the property on the responder for the property to be persisted
				try
				{
					IOcspAdmin.SetConfiguration(strServerName,
												false //bForce is false which means to fail if the responder service is offline
												);
					System.Console.WriteLine(strResponderProperty + " " + Resources.strPropertyApplied);
				}
				catch (COMException e)
				{
					System.Console.WriteLine(Resources.strApplyChangesFailed + e.Message);
				}
			}

			// Get the existing Revocation information with ID as strConfigName
			IOCSPCAConfiguration IOcspCAConfiguration;
			try
			{
				System.Console.WriteLine(Resources.strRevCfgRetrieving + strConfigName);
				IOcspCAConfiguration = (IOCSPCAConfiguration)(IOcspAdmin.OCSPCAConfigurationCollection.get_ItemByName(strConfigName));
				System.Console.WriteLine(Resources.strRevCfgRetrieved);
			}
			catch (COMException e)
			{
				System.Console.WriteLine(Resources.strRevCfgRetrieveFailed + e.Message);
				return;
			}


			try
			{
				uint uSigningFlags = IOcspCAConfiguration.SigningFlags;
				System.Console.WriteLine(strConfigurationProperty + " " + Resources.strPropertyValue + uSigningFlags.ToString());
			}
			catch (COMException e)
			{
				System.Console.WriteLine(strConfigurationProperty + " " + Resources.strPropertyDoesNotExist + e.Message);
			}

		}
	}
}
