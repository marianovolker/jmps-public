/**
 * 
 */
package com.gilbarco.globaltools.flexpay.crypto;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.PublicKey;
import java.security.Signature;
import java.security.SignatureException;

import javax.security.cert.CertificateException;
import javax.security.cert.X509Certificate;
import javax.xml.bind.DatatypeConverter;

import org.junit.Test;

/**
 * @author gparis
 *
 */



public class SignatureFunctionalTest
{
	
	enum TestResult
	{
		OK, FAIL, ERROR;
	}
	
	private static final int  INT_EOF = -1;
	private static final int  MAX_READING_BUFFER_LEN = 2048;
	private static final int  SIGNATURE_LEN = 304;

	private static final String SMINBODY = "<x></x>";
	private static final String S_PEM_PUBLICKEY_HEADER = "-----BEGIN PUBLIC KEY-----\n";
	private static final String S_PEM_PUBLICKEY_FOOTER = "\n-----END PUBLIC KEY-----\n";
	private static final String S_PEM_CERTIF_HEADER = "-----BEGIN CERTIFICATE-----\n";
	private static final String S_PEM_CERTIF_FOOTER = "\n-----END CERTIFICATE-----\n";	
	private static final String SDEFAULT_DATAFILENAME = "src//test//java//com//gilbarco//globaltools//flexpay//crypto//token1.data";
	private static final String SDEFAULT_SIGNATUREFILENAME = "src//test//java//com//gilbarco//globaltools//flexpay//crypto//token1.signature";
	private static final String SDEFAULT_CERTIFICATEFILENAME = "src//test//java//com//gilbarco//globaltools//flexpay//crypto//sas-certificate.pem";	
	private static byte abReadBuffer[] = new byte[MAX_READING_BUFFER_LEN];


	private static byte hexNibbleToByte(byte c)
	{
		if(c >= '0' && c <= '9')
			return (byte) ( c - '0');
		else if(c >= 'A' && c <= 'F')
			return (byte) ( c - 'A' + 10);
		else if(c >= 'a' && c <= 'f')
			return (byte) ( c - 'a' + 10);
		else
			return (byte) 0xFF;
	}

	private static byte[] getData(String sDataFilename)
	{
		ByteArrayOutputStream  bosData = null;
		FileInputStream fisData = null;

		try
		{
			bosData = new ByteArrayOutputStream();
			fisData = new FileInputStream(sDataFilename);
		}
		catch (FileNotFoundException e)
		{
			System.out.printf("File not found: %s. Test aborted.\n", e.getMessage());
			return null;
		}

		int iread = 0;
		while(iread !=  INT_EOF) // file size could be greater than MAX_READING_BUFFER_LEN
		{
			try
			{
				iread = fisData.read(abReadBuffer, 0, MAX_READING_BUFFER_LEN);
			}
			catch (IOException e)
			{
				System.out.printf("Error reading data file: %s. Test aborted.\n", e.getMessage());
				try
				{
					fisData.close();
				}
				catch (IOException e1)
				{
					System.out.printf("Error closing data file: %s. Test aborted.\n", e1.getMessage());
				}
				return null;
			}

			if(iread > 0)
				bosData.write(abReadBuffer, 0, iread);
		}

		try
		{
			if(bosData.size() >= SMINBODY.length()) // at least some tag was read from file
			{
				fisData.close();
				return bosData.toByteArray();
			}
			else
			{
				System.out.println("Not enough bytes found in data file. Test aborted");
				fisData.close();
			}
		}
		catch (IOException e)
		{
			System.out.printf("Error closing data file: %s. Test aborted.\n", e.getMessage());
		}

		return null;
	}

	private static byte[] getSignature(String sSignatureFilename)
	{
		ByteArrayOutputStream  bosSignature = null;
		FileInputStream fisSignature = null;

		try
		{
			bosSignature = new ByteArrayOutputStream();
			fisSignature = new FileInputStream(sSignatureFilename);
		}
		catch (FileNotFoundException e)
		{
			System.out.printf("%s. Test aborted.\n", e.getMessage());
			return null;
		}

		// Input data file is read
		boolean blContinueOk = false;
		int iread = 0;
		try
		{
			iread = fisSignature.read(abReadBuffer, 0, 3 * SIGNATURE_LEN - 1);
			if(iread >= 3 * SIGNATURE_LEN - 1)
				blContinueOk = true;
			else
			{
				IOException e = new IOException("Premature EOF found, bytes read = "
												+ iread);
				throw e;				
			}
		}
		catch (IOException e) // exception thrown while reading file 
		{
			System.out.printf("Error reading signature file: %s . Test aborted.\n",
								e.getMessage());
			blContinueOk = false;
		}

		// Input data file is closed
		try
		{
			fisSignature.close();
		}
		catch (IOException e1) // exception thrown while closing file
		{
			System.out.printf("Error closing signature file: %s. Test aborted.\n", e1.getMessage());
			blContinueOk = false;
		}

		if( ! blContinueOk )
			return null;

		for(int i = 0; i < SIGNATURE_LEN; i++)
		{
			byte bh, bl, cSeparator;
			String sFormatError = "Format error reading signature file, offset %d, ";

			bh = hexNibbleToByte( abReadBuffer[3*i] );
			bl = hexNibbleToByte( abReadBuffer[3*i + 1] );
			if( bh == 0xFF || bl == 0xFF )
			{
				System.out.printf(sFormatError +"byte: %c%c . Test aborted.\n",
									3*i, abReadBuffer[3*i], abReadBuffer[3*i + 1]);
				return null;
			}

			if(i == SIGNATURE_LEN - 1) // is the last iteration
				cSeparator = ':'; // virtual final separator (actually EOF in file)
			else
				cSeparator = abReadBuffer[3*i + 2];

			if( cSeparator == ':')
				bosSignature.write( 16 * bh + bl );
			else
			{
				System.out.printf(sFormatError + "separator: %c . Test aborted.\n",
									3*i + 2, cSeparator);
				return null;
			}
		}

		return bosSignature.toByteArray();
	}
	
	private static PublicKey getPublicKey(String sPEMCertificateFilename)
	{
		int iFileLength = 0;
		File file = null;
		FileInputStream fisCertif = null;
		
		file = new File(sPEMCertificateFilename);
		iFileLength = (int) file.length();

		try
		{
			fisCertif = new FileInputStream(file);
		}
		catch (FileNotFoundException e)
		{
			System.out.printf("%s. Test aborted.\n", e.getMessage());
			return null;
		}

		// Input certificate's PEM file is read
		boolean blContinueOk = true;
		byte   abEncodedB64Certificate[] = new byte[iFileLength];
		byte[] abBinaryCertificate = null;

		try
		{
			fisCertif.read(abEncodedB64Certificate, 0, iFileLength);
		}
		catch (IOException e) // exception thrown while reading certificate file 
		{
			System.out.printf("Error reading certificate file: %s . Test aborted.\n",
								e.getMessage());
			blContinueOk = false;
		}

		// Input certificate's PEM file is closed
		try
		{
			fisCertif.close();
		}
		catch (IOException e) // exception thrown while closing file
		{
			System.out.printf("Error closing certificate file: %s. Test aborted.\n", e.getMessage());
			blContinueOk = false;
		}

		if( ! blContinueOk )
			return null;

		// Input certificate's data is decoded from Base64 to binary.
		try
		{
			String s2, s1, sEncodedData = new String(abEncodedB64Certificate, "UTF-8");
			s1 = sEncodedData.replace(S_PEM_CERTIF_HEADER, "");
			s2 = s1.replace(S_PEM_CERTIF_FOOTER, "");
			abBinaryCertificate = DatatypeConverter.parseBase64Binary(s2);
		}
		catch (UnsupportedEncodingException e)
		{
			System.out.printf( "Error decoding PEM data: %s\n", e.getMessage() );
			return null;
		}

		// Certificate object is created from  structured binary specification
		try
		{
			X509Certificate certificate = X509Certificate.getInstance(abBinaryCertificate);
			return certificate.getPublicKey(); // public key read from certificate
		}
		catch (CertificateException e)
		{
			System.out.printf( "Error creating certificate from decoded PEM data: %s\n", e.getMessage() );
			return null;
		}
	}

	private static TestResult signatureTest( String sDataFilename, String sSignatureFilename,
											String sPEMCertificateFilename )
	{
		byte[] abSignedData = getData(sDataFilename);
		byte[] abSignature  = getSignature(sSignatureFilename);
		PublicKey key = getPublicKey(sPEMCertificateFilename);

		if(abSignedData == null || abSignature == null || key == null)
			return TestResult.ERROR; // error messages already displayed

		boolean blUpdated = false;
		String  sU = "assigning signed data" , sV = "verifying signature";

		try
		{
			// The signature algorithm object is requested as a generic "Signature" reference
			Signature sig = Signature.getInstance("SHA256withRSA");

			// Key is loaded into signature object and verified
			sig.initVerify(key);
			//sig.initVerify(certificate); // another valid way not used by now

			// Signed data is attached to signature object
			sig.update(abSignedData);
			blUpdated = true;
	
			// Finally the signature bytes are verified
			return sig.verify(abSignature) ? TestResult.OK : TestResult.FAIL;
		}
		catch (NoSuchAlgorithmException e)
		{
			System.out.printf( "SHA256withRSA algorithm is unknown in this system: %s\n", e.getMessage() );
			return TestResult.ERROR;
		}
		catch (InvalidKeyException e)
		{
			System.out.printf( "Invalid public key for verifying signature: %s\n", e.getMessage() );
			return TestResult.ERROR;
		}
		catch (SignatureException e)
		{
			System.out.printf( "Error %s : %s\n", blUpdated ? sU : sV , e.getMessage() );
			return TestResult.ERROR;			
		}
	}

	@Test
	public void signatureTest()
	{
		main(new String[0]);
	}
	

	public static void main(String args[])
	{
		int narg = args.length;
		TestResult res = TestResult.OK;
		String sMsg = null;
		String sDataFilename = null;
		String sSignatureFilename = null;
		String sCertifFilename = null;

		if( narg > 0 && ( args[0].equals("-h") || args[0].equals("--help") ) )
		{
			System.out.println();
			System.out.println("Usage: cryptotest [DataFilename] [SignatureFilename] [CertificateFilename]");
			System.out.println();
			return;
		}

		switch(narg)
		{
			case 0:
				sDataFilename = SDEFAULT_DATAFILENAME;
				sSignatureFilename = SDEFAULT_SIGNATUREFILENAME;
				sCertifFilename = SDEFAULT_CERTIFICATEFILENAME;
				break;

			case 1:
				sDataFilename = args[0];
				sSignatureFilename = SDEFAULT_SIGNATUREFILENAME;
				sCertifFilename = SDEFAULT_CERTIFICATEFILENAME;
				break;

			case 2:
				sDataFilename = args[0];
				sSignatureFilename = args[1];
				sCertifFilename = SDEFAULT_CERTIFICATEFILENAME;
				break;

			case 3:
			default: // all the filenames are specified by parameters
				sDataFilename = args[0];
				sSignatureFilename = args[1];
				sCertifFilename = args[2];;
				break;
		}

		res = signatureTest(sDataFilename, sSignatureFilename, sCertifFilename);
		if(res == TestResult.OK)
			sMsg = "Signature OK and properly verified !!";
		else if(res == TestResult.FAIL)
			sMsg = "Signature ok but verification FAILED.";
		else if(res == TestResult.ERROR)
			sMsg = "ERRORS in application/files.";
		else
			sMsg = "UNKNOWN TEST RESULT !!!";
		
		System.out.printf("%s\n\n", sMsg);
	}

}

