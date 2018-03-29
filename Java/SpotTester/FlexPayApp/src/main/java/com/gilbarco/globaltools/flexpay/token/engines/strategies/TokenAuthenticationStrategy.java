/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file TokenAuthenticationStrategy.java
 * @author pcantarini
 * @date Tuesday, August 27, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.token.engines.strategies;

import java.awt.Image;
import java.util.List;
import java.util.Observable;
import java.util.Observer;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.gui.app.EdtAppView;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
//import com.gilbarco.globaltools.flexpay.config.FlexPayAppConfig;

import com.gilbarco.globaltools.flexpay.targetunit.authmgnt.api.AuthenticationResult;
import com.gilbarco.globaltools.flexpay.targetunit.authmgnt.api.IAuthenticationBlock;
import com.gilbarco.globaltools.flexpay.targetunit.authmgnt.strategies.IAuthenticationStrategy;
import com.gilbarco.globaltools.flexpay.token.engines.SASTokenManagementEngine;
import com.gilbarco.globaltools.flexpay.token.engines.UPMTokenManagementEngine;
import com.gilbarco.globaltools.flexpay.token.gui.SelectTokenCodeDialog;
import com.gilbarco.globaltools.flexpay.token.gui.TokenManagerModel;
import com.gilbarco.globaltools.flexpay.token.dto.DataTransferForTokenCodeTableRow;
import com.gilbarco.globaltools.sasprotocol.xml.messages.SasXmlErrorCodeMsg.SasErrorCodeType;
import com.gilbarco.globaltools.flexpay.token.gui.TokenManagerView;

/* TODO: TOKEN_COMMENTED_CODE
This class is never used as token management was removed.
*/

/** @class TokenAuthenticationStrategy TokenAuthenticationStrategy.java "com.gilbarco.globaltools.flexpay.token.engines.strategies"
 * @brief This class provides an interface for the authentication strategy.
 *
 * @see IAuthenticationStrategy
 */
public class TokenAuthenticationStrategy implements IAuthenticationStrategy
{
  private static final transient Log logger = LogFactory.getLog( TokenAuthenticationStrategy.class );

  class TokenAuthenticationBlock extends Observable implements IAuthenticationBlock, Observer
  {
      private UPMTokenManagementEngine   upmTokenManagementEngine;
      private SASTokenManagementEngine   sasTokenManagementEngine;
      private SelectTokenCodeDialog      selectTokenCodeDialog;

      public TokenAuthenticationBlock ()
      {
        this.initialize();
      }
      
		@Override
      public void addObserver (Observer o)
      {
        sasTokenManagementEngine.addObserver(o);
        upmTokenManagementEngine.addObserver(o);
      }

		@Override
      public int countObservers ()
      {
        return upmTokenManagementEngine.countObservers();
      }

		@Override
      public void deleteObserver (Observer o)
      {
        sasTokenManagementEngine.deleteObserver(o);
        upmTokenManagementEngine.deleteObserver(o);
      }

		@Override
      public void deleteObservers ()
      {
        sasTokenManagementEngine.deleteObservers();
        upmTokenManagementEngine.deleteObservers();
      }

		@Override
      public boolean hasChanged ()
      {
        return (sasTokenManagementEngine.hasChanged() || upmTokenManagementEngine.hasChanged());
      }

		@Override
      public void notifyObservers ()
      {
        sasTokenManagementEngine.notifyObservers();
        upmTokenManagementEngine.notifyObservers();
      }

		@Override
      public void notifyObservers (Object arg)
      {
        sasTokenManagementEngine.notifyObservers(arg);
        upmTokenManagementEngine.notifyObservers(arg);
      }

		@Override
      public AuthenticationResult authenticate (String uri)
      {
		AuthenticationResult authResult = AuthenticationResult.AUTHENTICATION_DENIED;
		if (RetrieveAuthenticationResource() && ApplyAuthenticationResource())
		{
			authResult = AuthenticationResult.AUTHENTICATION_GRANTED;
		}
        return authResult;
      }

		@Override
      public void setAuthenticated (String uri, boolean authenticated)
      {
      }

		@Override
      public boolean isAuthenticated (String uri)
      {
        return false;
      }

		@Override
      public void forgetAuthentication (String uri)
      {
      }

		@Override
      public boolean isAuthenticating ()
      {
        return false;
      }
		  
	  /** @brief Retrieve an authentication resource corresponding to the token implementation.
	   *
	   * @return true if the authentication resource was successfully retrieved, false otherwise.
	   */
	  protected boolean RetrieveAuthenticationResource ()
	  {
	    logger.debug("TokenAuthenticationStrategy - RetrieveAuthenticationResource()");
	    try
	    {
	      if (!getTargetUnitFromUPM( ))
	      {
	        return false;
	      }
	      if (!getTokenQueryFromSAS( ))
	      {
	        return false;
	      }
	      if (!getTokenFromSAS( ))
	      {
	        return false;
	      }
	    }
	    catch (EdtServiceException | InterruptedException e)
	    {
	      logger.error( e.getMessage( ) );
	    }		

	    return true;
	  }
	  
	  /** @brief Apply an authentication resource corresponding to the token implementation, to the connected unit.
	   *
	   * @return true if the authentication resource was successfully applied, false otherwise.
	   */
	  protected boolean ApplyAuthenticationResource ()
	  {
	    logger.debug("TokenAuthenticationStrategy - ApplyAuthenticationResource()");
	    try
	    {
	      if (!presentingTokenAtUPM( ))
	      {
	        return false;
	      }
	    }
	    catch (EdtServiceException | InterruptedException e)
	    {
	      logger.error( e.getMessage( ) );
	    }		
	    
	    return true;
	  }

	  /** @brief Attend the Observable notification to which this instance is subscribed.
	   *
	   * @param obs is the Observable instance to which this instance is subscribed.
	   * @param arg is the notification variable argument sent in the Observable notification.
	   */
	  @Override
	  public void update(Observable obs, Object arg)
	  {
	    logger.debug("TokenAuthenticationStrategy - update()");
	  }

	  /** @brief Initialize current TokenAuthenticationStrategy instance.
	   */
	  private void initialize ()
	  {
	    logger.debug("TokenAuthenticationStrategy - Initialize()");

	    if (selectTokenCodeDialog == null)
	    {
	      selectTokenCodeDialog = new SelectTokenCodeDialog( );
	      selectTokenCodeDialog.setLocationRelativeTo( null );
	      selectTokenCodeDialog.setAlwaysOnTop( true );
	      selectTokenCodeDialog.setModal( true );
	    }

	    try
	    {
	      if (sasTokenManagementEngine == null)
	      {
	        sasTokenManagementEngine = SASTokenManagementEngine.getInstance( );
	        sasTokenManagementEngine.addObserver( this );
	      }

	      if (upmTokenManagementEngine == null)
	      {
	        upmTokenManagementEngine = UPMTokenManagementEngine.getInstance( );
	        upmTokenManagementEngine.addObserver( this );
	      }
	    }
	    catch (EdtServiceException e)
	    {
	      logger.error( e.getMessage( ) );
	    }		
	  }
	  
	  /** @brief .
	   *
	   * @param .
	   */
	  private void getErrorInformationFromSASEngine(String strSpecificInfo)
	  {
	      String strInfo = "Error: " + strSpecificInfo + ". ";
	      if( sasTokenManagementEngine.getResponseCode( ) == SasErrorCodeType.UNKNOWN_ERROR )
	      {
	        strInfo += "[" + sasTokenManagementEngine.getErrorDescription( ) + "]";
	      }
	      else
	      {
	        strInfo += "[" + sasTokenManagementEngine.getResponseCode( ).getDescription( ) + "]";
	      }
	      
	      logger.error( strInfo );
	  }  

	  /** @brief .
	   *
	   * @param .
	   * @return .
	   */
	  private boolean getTargetUnitFromUPM() throws InterruptedException, EdtServiceException
	  {
	    this.setChanged();
	    this.notifyObservers("Getting PPN from the UPM.");
	    
	    logger.info( "Getting PPN from the UPM." );
	    
	    upmTokenManagementEngine.doObtainThePPN( );
	    
	    if (!upmTokenManagementEngine.hasErrors( ))
	    {
	      logger.info( "Success at getting TargetUnit action from the SPOT. The TargetUnit received is [" + upmTokenManagementEngine.getTargetUnit( ) + "]." );
	      return true;
	    }
	    else
	    {
	      logger.error( "Couldn't get the TargetUnit from the SPOT. The response received is [" + upmTokenManagementEngine.getResponseCode( ).getAckDescription( ) + "]." );
	      return false;
	    }
	  }
	  
	  /** @brief .
	   *
	   * @param .
	   * @return .
	   */
	  private boolean getTokenQueryFromSAS() throws InterruptedException, EdtServiceException
	  {
	    this.setChanged();
	    this.notifyObservers("Getting the list of Tokens for this UPM from SAS Server.");
	    
	    String strInfo = "Getting the TOKEN_QUERY from the SAS";
	    logger.info( strInfo + ", IP ADDRESS [" + sasTokenManagementEngine.getCurrentIPAddress( ) + "] and Port [" + sasTokenManagementEngine.getCurrentPortAddress( ) + "]" );
	    
	    sasTokenManagementEngine.setTargetUnit(upmTokenManagementEngine.getTargetUnit( ));
	    sasTokenManagementEngine.doTokenQueryRequest( );
	    if (!sasTokenManagementEngine.hasErrors( ))
	    {
	      int quantityOfTokens = sasTokenManagementEngine.getTokenQueryResponse( ).getQuantityOfTokens( );
	      strInfo = "Success at Token Query Request action. The quantity of token received was [" + quantityOfTokens + "]";
	      if( quantityOfTokens > 1 )
	      {
	        selectTokenCodeDialog.clearTable( );
	        DataTransferForTokenCodeTableRow dttu = new DataTransferForTokenCodeTableRow();
	        for( int i = 0; i < quantityOfTokens; i++ )
	        {
	          dttu.setTokenCode( sasTokenManagementEngine.getTokenQueryResponse( ).getTokenCode( i ) );
	          dttu.setStartTime( sasTokenManagementEngine.getTokenQueryResponse( ).getStartTime( i ) );
	          dttu.setEndTime  ( sasTokenManagementEngine.getTokenQueryResponse( ).getEndTime   ( i ) );
	          selectTokenCodeDialog.addNewTokenCode( dttu );
	        }
	        selectTokenCodeDialog.setVisible( true );
	        if ( selectTokenCodeDialog.getSelectedToken( ) != null )
	        {
	          sasTokenManagementEngine.setTokenCode( selectTokenCodeDialog.getSelectedToken( ) );
	        }
	        else
	        {
	          logger.error( "Operation canceled by user." );
	          return false;
	        }
	      }
	      else
	      {
	        sasTokenManagementEngine.setTokenCode( sasTokenManagementEngine.getTokenQueryResponse( ).getTokenCode( 0 ) );
	      }
	      logger.info( strInfo );
	      return true;
	    }
	    else
	    {
	      getErrorInformationFromSASEngine(strInfo);
	      return false;
	    }
	  }
	  
	  /** @brief .
	   *
	   * @param .
	   * @return .
	   */
	  private boolean getTokenFromSAS() throws InterruptedException, EdtServiceException
	  {
	    this.setChanged();
	    this.notifyObservers("Getting the real token for this UPM from SAS Server.");
	    
	    String strInfo = "Getting the TOKEN from the SAS";
	    logger.info( strInfo + ", IP ADDRESS [" + sasTokenManagementEngine.getCurrentIPAddress( ) + "] and Port [" + sasTokenManagementEngine.getCurrentPortAddress( ) + "]" );
	    
	    sasTokenManagementEngine.doTokenRequest( );
	    
	    if (!sasTokenManagementEngine.hasErrors( ))
	    {
	      String tokenReceived = sasTokenManagementEngine.getTokenResponse( ).toString( );
	      strInfo = "Success at Token Request Action. The token received was [" + tokenReceived + "]";
	      
	      upmTokenManagementEngine.setXmlToken( tokenReceived.getBytes( ) );

	      logger.info( strInfo );
	      return true;
	    }
	    else
	    {
	      getErrorInformationFromSASEngine(strInfo);
	      return false;
	    }
	  }
	  
	  /** @brief .
	   *
	   * @param .
	   * @return .
	   */
	  private boolean presentingTokenAtUPM() throws InterruptedException, EdtServiceException
	  {
	    this.setChanged();
	    this.notifyObservers("Presenting the token to UPM.");
	    
	    logger.info( "Presenting token to UPM." );
	    
	    upmTokenManagementEngine.doPresentToken( );
	    
	    if (!upmTokenManagementEngine.hasErrors( ))
	    {
	      logger.info( "Success at PresentToken action. The PPN of the SPOT is [" + upmTokenManagementEngine.getTargetUnit( ) + "]." );
	      return true;
	    }
	    else
	    {
	      logger.error( "Couldn't send PresentToken message. The response received was [" + upmTokenManagementEngine.getResponseCode( ).getAckDescription( ) + "]." );
	      return false;
	    }
	  }
  }
  
  /** @brief TokenAuthenticationStrategy default constructor.
   */
  public TokenAuthenticationStrategy()
  {
  }

  /** @brief Retrieve the authentication technician id.
   *
   * @return the authentication technician id.
   */
  public String getTechnicianID()
  {
    return TokenManagerModel.getTechnicianID();
  }

  /** @brief Set the authentication technician id.
   *
   * @param the authentication technician id.
   */
  public void setTechnicianID(String strTechnicianID)
  {
    TokenManagerModel.setTechnicianID(strTechnicianID);
  }

  @Override
  public List<Image> getIconImages ()
  {
  	return null;
  }

  @Override
  public void setIconImages (List<Image> lIcons)
  {
  }

  /** @brief Queries if the corresponding to the token implementation has an authentication manager view.
   *
   * @return always true as the token implementation has an authentication manager view.
   */
  public boolean hasAuthenticationManagerView()
  {
    return true;
  }

  /** @brief Queries if the given view is instance of the token authentication manager view.
   *
   * @return true if the given view is instance of the token authentication manager view, false otherwise.
   */
  public boolean isInstanceOfCurrentAuthenticationManagerView(EdtAppView<?> edtAppView)
  {
    return (edtAppView instanceof TokenManagerView);
  }

  /** @brief Get the authentication manager view.
   *
   * @return the authentication manager view.
   */
  public EdtAppView<?> getAuthenticationManagerView()
  {
    return new TokenManagerView();
  }

  /** @brief Reset the authentication manager view.
   *
   */
  public void resetAuthenticationManagerView()
  {
  }
  
  /** @brief Obtain the current implementation of the ConnectionEngine AuthenticationBlock.
  *
  * @return ConnectionEngine AuthenticationBlock implementation.
  * 
  * @since 1.0.0
  */
  public IAuthenticationBlock getConnectionAuthenticationBlock ()
  {
	 return new TokenAuthenticationBlock();
  }
}
