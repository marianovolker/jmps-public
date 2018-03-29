
package com.gilbarco.globaltools.flexpay.rkl.state;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.edt.services.Issuer;
import com.gilbarco.globaltools.edt.util.EdtConvert;
import com.gilbarco.globaltools.flexpay.rkl.rklnativeprotocol.RKLNativeMessages;

public class ServerRKLBroker extends RKLBroker
{

	// ******************************************************************
	// STATIC FIELDS
	// ******************************************************************
	private static final transient Log	logger	= LogFactory.getLog( ServerRKLBroker.class );

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
	public ServerRKLBroker()
	{
		super( new Issuer( ServerRKLBroker.class ) );
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
				case SEND_SPOT_IDENTIFICATION :
					if (!getQueue( ).isEmpty( ))
					{
						sendIdentificatioRequestToSrvr( getQueue( ).poll( ) );
						changeState( RKLNativeStates.FINAL_STATE.name( ), "Done. answer not implemented yet" );
					}
					else
					{
						showError( state );
					}
					break;
				case SEND_SPOT_CERTIFICATE :
					if (!getQueue( ).isEmpty( ))
					{
						sendSpotCertificateToSrvr( getQueue( ).poll( ) );
					}
					else
					{
						showError( state );
					}
					break;
				case GET_KMS_HOST_CERTIFICATE :
					if (!getQueue( ).isEmpty( ))
					{
						sendSpotCertificateToSrvr( getQueue( ).poll( ) );
					}
					else
					{
						showError( state );
					}
					break;
				case REQUEST_KEY_MANAGEMENT :
					sendGetKeyManagementToSrvr( );
					break;
				case GET_KEYS :
					if (!getQueue( ).isEmpty( ))
					{
						sendGetKeysToSrvr( getQueue( ).poll( ) );
					}
					else
					{
						showError( state );
					}
					break;
				case SEND_CLOSE_KEY_SESSION :
					if (!getQueue( ).isEmpty( ))
					{
						sendCloseKeySessionToSrvr( getQueue( ).poll( ) );
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

	// ******************************************************************
	// PRIVATE METHODS.
	// ******************************************************************

	private void sendIdentificatioRequestToSrvr(byte[] sernum) throws EdtServiceException
	{
		byte[] message = RKLNativeMessages.getInstance( ).getSendSpotIdentification( getNextSequenceNumber( ), sernum );
		send( message );
	}

	private void sendCloseKeySessionToSrvr(byte[] data) throws EdtServiceException
	{
		byte[] message = RKLNativeMessages.getInstance( ).sendCloseKeySessionToSrvr( getNextSequenceNumber( ), data );
		send( message );
	}

	private void sendGetKeysToSrvr(byte[] keydata) throws EdtServiceException
	{
		byte[] message = RKLNativeMessages.getInstance( ).sendGetKeysToSrvr( getNextSequenceNumber( ), keydata );
		send( message );
	}

	private void sendGetKeyManagementToSrvr() throws EdtServiceException
	{
		byte[] message = RKLNativeMessages.getInstance( ).sendGetKeyManagementToSrvr( getNextSequenceNumber( ) );
		send( message );
	}

	private void sendSpotCertificateToSrvr(byte[] certif) throws EdtServiceException
	{
		byte[] message = RKLNativeMessages.getInstance( ).sendSpotCertificateToSrvr( getNextSequenceNumber( ), certif );
		send( message );
	}

	private void showError(RKLNativeStates state)
	{
		logger.error( "Done - " + state.getName( ) + " - parameters error" );
		changeState( RKLNativeStates.NOTIFY_ERROR.name( ), "parameters error" );
		controller.showRKLError( );
	}

	// ******************************************************************
	// INNER CLASSES.
	// ******************************************************************

	// ******************************************************************
	// MAIN.
	// ******************************************************************

}
