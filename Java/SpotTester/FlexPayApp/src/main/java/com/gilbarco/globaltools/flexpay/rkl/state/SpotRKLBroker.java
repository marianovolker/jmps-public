
package com.gilbarco.globaltools.flexpay.rkl.state;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.Issuer;
import com.gilbarco.globaltools.flexpay.rkl.rklnativeprotocol.RKLNativeMessages;

public class SpotRKLBroker extends RKLBroker
{

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log	logger	= LogFactory.getLog( SpotRKLBroker.class );

	// ******************************************************************
	// STATIC METHODS
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
	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	public SpotRKLBroker()
	{
		super( new Issuer( SpotRKLBroker.class ) );
	}

	// ******************************************************************
	// PUBLIC METHODS (general, getter, setter, interface imp)
	// ******************************************************************
	@Override
	public void initialize()
	{
		super.init( );
	}

	// ******************************************************************
	// PROTECTED METHODS.
	// ******************************************************************

	@SuppressWarnings("incomplete-switch")
	@Override
	protected void handleStatusChanged(RKLNativeStates state)
	{
		try
		{

			switch(state)
			{
				case IDLE :
					break;
				case GET_SPOT_IDENTIFICATION :
					getSpotIdentification( );
					changeState( RKLNativeStates.FINAL_STATE.name( ), "Done. answer not implemented yet" );
					break;
				case GET_SPOT_CERTIFICATE :
					getSpotCertificate( );
					break;
				case SEND_SPOT_CERTIFICATE_VERIFICATION :
					if (!getQueue( ).isEmpty( ))
					{
						sendCertificateVerificationToSpot( getQueue( ).poll( ) );
					}
					else
					{
						showError( state );
					}
					break;
				case SEND_HOST_CERTIFICATE :
					if (!getQueue( ).isEmpty( ))
					{
						sendHostCertificateToSpot( getQueue( ).poll( ) );
					}
					else
					{
						showError( state );
					}
					break;
				case GET_SPOT_NONCE_DATA :
					if (!getQueue( ).isEmpty( ))
					{
						getSpotNonceData( );
					}
					else
					{
						showError( state );
					}
					break;
				case SEND_KEY_MANAGEMENT_DATA :
					if (!getQueue( ).isEmpty( ))
					{
						sendKeyManagementDataToSpot( getQueue( ).poll( ) );
					}
					else
					{
						showError( state );
					}
					break;
				case SEND_KEY_RESPONSE :
					if (!getQueue( ).isEmpty( ))
					{
						sendKeyResponseToSpot( getQueue( ).poll( ) );
					}
					else
					{
						showError( state );
					}
					break;
				case SEND_CLOSE_KEY_SESSION_ANSWER :
					if (!getQueue( ).isEmpty( ))
					{
						sendCloseKeySessionAnswerToSpot( getQueue( ).poll( ) );
					}
					else
					{
						showError( state );
					}
					break;
			}

		}
		catch (EdtServiceException e)
		{
			logger.error( "handleStatusChanged -" + e.getMessage( ));
			changeState( RKLNativeStates.NOTIFY_ERROR.getName( ), "Message " + state.name( ) + " fail." );
		}

	}

	private void showError(RKLNativeStates state)
	{
		logger.error( "Done - " + state.getName( ) + " - parameters error" );
		changeState( RKLNativeStates.NOTIFY_ERROR.name( ), "parameters error" );
		controller.showRKLError( );
	}

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************
	private void getSpotIdentification() throws EdtServiceException
	{
		byte[] message = RKLNativeMessages.getInstance( ).getSpotIdentification( getNextSequenceNumber( ) );
		send( message );
	}

	private void getSpotCertificate() throws EdtServiceException
	{
		byte[] message = RKLNativeMessages.getInstance( ).getSpotCertificate( getNextSequenceNumber( ) );
		send( message );
	}

	private void sendCertificateVerificationToSpot(byte[] verif) throws EdtServiceException
	{
		byte[] message = RKLNativeMessages.getInstance( ).sendCertificateVerificationToSpot( getNextSequenceNumber( ), verif );
		send( message );
	}

	private void sendHostCertificateToSpot(byte[] certif) throws EdtServiceException
	{
		byte[] message = RKLNativeMessages.getInstance( ).sendHostCertificateToSpot( getNextSequenceNumber( ), certif );
		send( message );
	}

	private void getSpotNonceData() throws EdtServiceException
	{
		byte[] message = RKLNativeMessages.getInstance( ).getSpotNonceData( getNextSequenceNumber( ) );
		send( message );
	}

	private void sendKeyManagementDataToSpot(byte[] bs) throws EdtServiceException
	{
		byte[] message = RKLNativeMessages.getInstance( ).sendKeyManagementDataToSpot( getNextSequenceNumber( ), bs );
		send( message );
	}

	private void sendKeyResponseToSpot(byte[] bs) throws EdtServiceException
	{
		byte[] message = RKLNativeMessages.getInstance( ).sendKeyResponseToSpot( getNextSequenceNumber( ), bs );
		send( message );
	}

	private void sendCloseKeySessionAnswerToSpot(byte[] bs) throws EdtServiceException
	{
		byte[] message = RKLNativeMessages.getInstance( ).sendCloseKeySessionToSrvr( getNextSequenceNumber( ), bs );
		send( message );
	}

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
