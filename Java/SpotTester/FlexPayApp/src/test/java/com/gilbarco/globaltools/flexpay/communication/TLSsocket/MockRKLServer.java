package com.gilbarco.globaltools.flexpay.communication.TLSsocket;


	/**
	 * 
	 */

	import java.io.BufferedInputStream;

	import java.io.BufferedReader;
	import java.io.BufferedWriter;
	import java.io.IOException;
	import java.io.InputStream;
	import java.io.InputStreamReader;
	import java.io.OutputStreamWriter;
	import java.io.PrintWriter;
	import java.security.KeyManagementException;
	import java.security.NoSuchAlgorithmException;
	import java.util.Arrays;

	import javax.net.ssl.SSLContext;
	import javax.net.ssl.SSLServerSocket;
	import javax.net.ssl.SSLServerSocketFactory;
	import javax.net.ssl.SSLSession;
	import javax.net.ssl.SSLSocket;
	import javax.net.ssl.TrustManager;
	import com.gilbarco.globaltools.edt.services.crypto.X509EdtTrustManager;


	public class MockRKLServer
	{
		// ******************************************************************
		// STATIC FIELDS 
		// ******************************************************************

		// ******************************************************************
		// PUBLIC FIELDS.
		// ******************************************************************

		// ******************************************************************
		// PROTECTED FIELDS.
		// ******************************************************************

		// ******************************************************************
		// PRIVATE FIELDS.
		// ******************************************************************
		private volatile boolean blServerUp = false;
		private volatile boolean blShutDown = false;
		private Thread tlsSocketTestingThread = null;
		
		// ******************************************************************
		// CONSTRUCTOR.
		// ******************************************************************

		// ******************************************************************
		// OVERRIDDEN METHODS  (invoked from polymorphic interface).
		// ******************************************************************

		// ******************************************************************
		// PUBLIC METHODS	  (general, getter, setter, interface imp)
		// ******************************************************************

		// ******************************************************************
		// PROTECTED METHODS.
		// ******************************************************************

		// ******************************************************************
		// PRIVATE METHODS.
		// ******************************************************************

		private void startTestingServer()
		{
			tlsSocketTestingThread = new Thread( new TLSTestingServerSocket( ) );
			tlsSocketTestingThread.start( ); // starts the hearing thread
		}

		private boolean isServerUp()
		{
			return blServerUp;
		}

		private void shutdown() throws InterruptedException
		{
			blShutDown = true;
			while( tlsSocketTestingThread.isAlive( ) )
				Thread.sleep( 1000 );
		}


		// ******************************************************************
		// STATIC METHODS 
		// ******************************************************************

		// ******************************************************************
		// INNER TYPES 
		// ******************************************************************
		@SuppressWarnings("serial")
		public class QuitThreadException extends Exception
		{
			public static final String strMessage = "Thread forced to quit.";

			public QuitThreadException()
			{
				super();
			}

			/* (non-Javadoc)
			 * @see java.lang.Throwable#getMessage()
			 */
			@Override
			public String getMessage()
			{
				// super.getMessage( );
				return strMessage;
			}
		}

		public class TLSTestingServerSocket implements Runnable
		{
			// ******************************************************************
			// CONSTANTS
			// ******************************************************************
			private static final int		RKL_DEFAULT_PORT	= 55200;
			private static final String		TL_SV1_2_PROTOCOL	= "TLSv1.2";

			// ******************************************************************
			// PRIVATED FIELDS
			// ******************************************************************
			private X509EdtTrustManager		certs;
			private SSLServerSocketFactory	sslSocketfactory;
			private SSLContext				sslContext;
			private SSLServerSocket			tlsServerSocket;
			private SSLSession				tlsSession;
			private SSLSocket				tlsClientSocket;
			private String[]				suites;
			private String[]				protocols;


			// ******************************************************************
			// PUBLIC METHODS
			// ******************************************************************

			/*
			 * This function runs in a separate thread, that is, this is the thread code.
			 * @see java.lang.Runnable#run()
			 */
			public void run()
			{
				if( blShutDown )
				{
					closeConnection();
					System.out.println( "TLSTestingServerSocket - END" );
					blServerUp = false;
				}

				blServerUp = true;
				System.out.println( "TLSTestingServerSocket - START" );

				try
				{
					if( blShutDown ) throw new QuitThreadException();

					CreateTLSServerSocket( );

					if( blShutDown ) throw new QuitThreadException();

					startSession( );

					if( blShutDown ) throw new QuitThreadException();

					receiveAndSend( );

					if( blShutDown ) throw new QuitThreadException();

					Thread.sleep( 500 );
					closeConnection( );
				}
				catch (QuitThreadException e)
				{
					closeConnection( );
					System.out.println( "TLSTestingServerSocket - " + e.getMessage( ) );
				}
				catch (NoSuchAlgorithmException | KeyManagementException | IOException | InterruptedException e)
				{
					closeConnection( );
					System.out.println( "TLSTestingServerSocket - " + e.getMessage( ) );
				}

				System.out.println( "TLSTestingServerSocket - END" );
				blServerUp = false;
			}


			// ******************************************************************
			// PRIVATED METHODS
			// ******************************************************************

			/**
			 * Creates a TLS server Socket
			 * @throws NoSuchAlgorithmException
			 * @throws KeyManagementException
			 * @throws IOException
			 */
			private void CreateTLSServerSocket() throws NoSuchAlgorithmException, KeyManagementException, IOException
			{
				certs = new X509EdtTrustManager( );

				sslContext = SSLContext.getInstance( TL_SV1_2_PROTOCOL );
				sslContext.init( null, new TrustManager[]
				{ certs }, null );
				sslSocketfactory = (SSLServerSocketFactory) sslContext.getServerSocketFactory( );
				tlsServerSocket = (SSLServerSocket) sslSocketfactory.createServerSocket( RKL_DEFAULT_PORT );

				suites = tlsServerSocket.getSupportedCipherSuites( );
				tlsServerSocket.setEnabledCipherSuites( suites );

				protocols = tlsServerSocket.getSupportedProtocols( );
				tlsServerSocket.setEnabledProtocols( protocols );
			}

			/**
			 * Start Session to accept client connection
			 * @throws IOException
			 */
			private void startSession() throws IOException
			{
				System.out.println( "TLSTestingServerSocket - Waiting for client..." );
				tlsClientSocket = (SSLSocket) tlsServerSocket.accept( );
				tlsClientSocket.setEnabledCipherSuites( suites );
				tlsClientSocket.setEnabledProtocols( protocols );

				tlsClientSocket.startHandshake( );
				tlsSession = tlsClientSocket.getSession( );

				System.out.println( "TLSTestingServerSocket - Connected to " + tlsClientSocket.getRemoteSocketAddress( ) );
				System.out.println( "TLSTestingServerSocket - Protocol used: " + tlsSession.getProtocol( ) );
			}

			/**
			 * Close the TLS socket Connection
			 */
			private void closeConnection()
			{
				try
				{
					tlsServerSocket.close( );
				}
				catch (IOException e)
				{
					e.printStackTrace( );
				}
			}

			private void receiveAndSend()  throws IOException, QuitThreadException
			{
				receiveParseAndReply();
			}

			/**
			 * Receive and Send (echo to the client)
			 * @throws IOException
			 */
			private void receiveAndSendTextLines() throws IOException, QuitThreadException
			{
				BufferedReader in = new BufferedReader(new InputStreamReader(tlsClientSocket.getInputStream()));
				PrintWriter out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(tlsClientSocket.getOutputStream())));
				String inputLine;

				while ((inputLine = in.readLine()) != null)
				{
					if( blShutDown ) throw new QuitThreadException();

					System.out.println("received:" + inputLine);
					out.println(inputLine);
					out.flush();
				}
			}


			/**
			 * Receive and Send (echo to the client)
			 * @throws IOException
			 */
			private void receiveParseAndReply() throws IOException, QuitThreadException
			{
				int nRead = 0;
				BufferedInputStream is = new BufferedInputStream( tlsClientSocket.getInputStream() );
				PrintWriter out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(tlsClientSocket.getOutputStream())));
				byte buffer[] = new byte[2048];

				if( blShutDown ) throw new QuitThreadException();

				is.mark(2048); // to re-read the message again, after printing it, for parsing.

				while ((nRead = is.read( buffer )) >= 0)
				{
					if( blShutDown ) throw new QuitThreadException();

					System.out.printf( "%d bytes received: \n%s\n", nRead, new String(buffer, 0, nRead, "UTF-8") );

					is.reset(); // retrieves the characters read in previous reading.

					out.println("acknowledge");
					out.flush();
					is.mark(2048); // to further roll back on the next reading
				}
			}

		} // end of INNER CLASS


		// ******************************************************************
		// MAIN.
		// ******************************************************************

		/**
		 * main entry point of this helper test application.
		 * Invoked from command line, it enters directly here
		 * @param args
		 */
		public static void main(String[] args)
		{
			System.out.println("\n\nApplication start.\n");

			MockRKLServer server = new MockRKLServer();

			System.out.println("\n\nServer created.\n");
			
			server.startTestingServer( );

			System.out.println("\n\nServer is up hearing messages.\n");

			while( server.isServerUp( ) )
				try
				{
					Thread.sleep(1000);
				}
				catch (InterruptedException e)
				{
					try
					{
						if( server.isServerUp( ) )
							server.shutdown( );
					}
					catch (InterruptedException e1)
					{
						// do nothing, InterruptedException already received by the application
					}

					System.out.println("\n\nReceived interrupt signal. Application exit.\n\n");
					return;
				}

			System.out.println("\n\nServer finished hearing messages. Application exit.\n\n");
		}


	}
