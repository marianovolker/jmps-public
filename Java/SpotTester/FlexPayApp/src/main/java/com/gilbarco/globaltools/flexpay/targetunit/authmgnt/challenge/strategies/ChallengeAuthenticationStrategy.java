/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file ChallengeAuthenticationStrategy.java
 * @author pcantarini
 * @date Tuesday, August 27, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.authmgnt.challenge.strategies;

import java.awt.Image;
import java.io.IOException;
import java.net.URISyntaxException;
import java.nio.charset.StandardCharsets;
import java.nio.file.DirectoryStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Observer;

import javax.swing.JDialog;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;

import org.apache.commons.lang3.tuple.Pair;
import org.apache.commons.lang3.tuple.MutablePair;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.gilbarco.globaltools.edt.gui.app.EdtAppView;
import com.gilbarco.globaltools.edt.services.EdtServiceException;
import com.gilbarco.globaltools.flexpay.targetunit.authmgnt.api.AuthenticationResult;
import com.gilbarco.globaltools.flexpay.targetunit.authmgnt.api.IAuthenticationBlock;
import com.gilbarco.globaltools.flexpay.targetunit.authmgnt.challenge.engines.ChallengeAuthenticationEngine;
import com.gilbarco.globaltools.flexpay.targetunit.authmgnt.challenge.engines.ChallengeRequestEngine;
import com.gilbarco.globaltools.flexpay.targetunit.authmgnt.challenge.gui.RequestAuthenticationDialog;
import com.gilbarco.globaltools.flexpay.targetunit.authmgnt.strategies.IAuthenticationStrategy;
import com.gilbarco.globaltools.flexpay.targetunit.utilities.IWaitCondition;
import com.gilbarco.globaltools.flexpay.targetunit.utilities.WaitCondition;

/** @class ChallengeAuthenticationStrategy ChallengeAuthenticationStrategy.java "com.gilbarco.globaltools.flexpay.token.engines.strategies"
 * @brief This class provides an interface for the authentication strategy.
 *
 * @see IAuthenticationStrategy
 * 
 * @since 1.0.0
 */
public class ChallengeAuthenticationStrategy implements IAuthenticationStrategy
{
	/** @brief Logger instance for this class type */
	private static final transient Log logger = LogFactory.getLog(ChallengeAuthenticationStrategy.class);

	/** @brief Instance synchronization monitor */ 
	private Object  monitor;

	/** @brief List of title bar icons to be used by the helper dialogs */ 
	private List<Image> icons;
	
	/** @brief Current technician Id used for authentication */ 
	private String technicianId;
	
	/** @brief Default constructor.
	 * 
	 * @param the authentication icon images.
	 * 
	 * @since 1.0.0
	 */
	public ChallengeAuthenticationStrategy (List<Image> lIcons)
	{
		this.monitor = new Object();
		this.icons = lIcons;
		this.technicianId = "";
	}

	/** @brief Retrieve the authentication technician id.
	 *
	 * @return the authentication technician id.
	 * 
	 * @since 1.0.0
	 */
	@Override
	public String getTechnicianID ()
	{
		String sResult = "";
		synchronized (monitor)
		{
			sResult = technicianId;
		}
		return sResult;
	}

	/** @brief Set the authentication technician id.
	 *
	 * @param the authentication technician id.
	 * 
	 * @since 1.0.0
	 */
	@Override
	public void setTechnicianID (String strTechnicianID)
	{
		synchronized (monitor)
		{
			technicianId = strTechnicianID;
		}
	}

	/** @brief Retrieve the authentication icon images.
	 *
	 * @return the authentication icon images.
	 * 
	 * @since 1.0.0
	 */
	@Override
	public List<Image> getIconImages ()
	{
		List<Image> result = null;
		synchronized (monitor)
		{
			result = icons;
		}
		return result;
	}

	/** @brief Set the authentication icon images.
	 *
	 * @param the authentication icon images.
	 * 
	 * @since 1.0.0
	 */
	@Override
	public void setIconImages (List<Image> lIcons)
	{
		synchronized (monitor)
		{
			icons = lIcons;
		}
	}

	/** @brief Queries if the corresponding to the challenge implementation has an authentication manager view.
	 *
	 * @return always false as the challenge implementation does not have an authentication manager view.
	 * 
	 * @since 1.0.0
	 */
	@Override
	public boolean hasAuthenticationManagerView ()
	{
		return false;
	}

	/** @brief Queries if the given view is instance of the challenge authentication manager view.
	 *
	 * @param edtAppView [In] The EdtAppView which should be inspected in order to confirm or not if the current authentication manager view is present.
	 * 
	 * @return true if the given view is instance of the challenge authentication manager view, false otherwise.
	 * 
	 * @since 1.0.0
	 */
	@Override
	public boolean isInstanceOfCurrentAuthenticationManagerView (EdtAppView<?> edtAppView)
	{
		return false;
	}

	/** @brief Get the authentication manager view.
	 *
	 * @return the authentication manager view.
	 * 
	 * @since 1.0.0
	 */
	@Override
	public EdtAppView<?> getAuthenticationManagerView ()
	{
		return null;
	}

	/** @brief Reset the authentication manager view.
	 *
	 * @since 1.0.0
	 */
	@Override
	public void resetAuthenticationManagerView ()
	{
	}

	//----------------------------------------------------
	//TODO: Remove when correct implementation is done. --
	private static String getTokenHackDirectoryPath ()
	{
		String sPath = "";
		try
        {
			sPath = Paths.get(Paths.get(ClassLoader.getSystemClassLoader().getResource(".").toURI()).toString(),"token_hack").toString();
        }
        catch (URISyntaxException e)
        {
            logger.error("Error. Exception thrown while obtaining the token hack directory. " + e.getMessage(),e);
        }
        logger.info("Info. Token Hack path is: '" + sPath + "'.");
		return sPath;
	}
	private static final String TOKEN_HACK_DIRECTORY_PATHNAME = getTokenHackDirectoryPath();
	private static final String TOKEN_HACK_FILE_EXTENSION_MATCHES_REGEX = "^.*\\.token.*$";
	private static final String TOKEN_HACK_FILE_EXTENSION_REPLACE_REGEX = "\\.token.*$";
	private static final int    TOKEN_HACK_FILE_MAX_SIZE = 1048576;
	//----------------------------------------------------

	/** @brief Obtain the current implementation of the ConnectionEngine AuthenticationBlock.
	 *
	 * @return ConnectionEngine AuthenticationBlock implementation.
	 * 
	 * @since 1.0.0
	 */
	@Override
	public IAuthenticationBlock getConnectionAuthenticationBlock ()
	{
		IAuthenticationBlock authenticationBlock = null;
		try
		{
			synchronized (monitor)
			{
				/** @class @class ChallengeAuthenticationStrategy.$IAuthenticationBlock ChallengeAuthenticationStrategy.java "com.gilbarco.globaltools.flexpay.token.engines.strategies"
				 * @brief This is Authentication wrapper implemented for the challenge authentication.
				 *
				 * @since 1.0.0
				 */
    			authenticationBlock = new IAuthenticationBlock()
        		{
    				/** @class @class ChallengeAuthenticationStrategy.$IAuthenticationBlock.GuiChallengePasswordRequest ChallengeAuthenticationStrategy.java "com.gilbarco.globaltools.flexpay.token.engines.strategies"
    				 * @brief This is an Authentication helper class which facilitates synch GIU operations through runnables that can be sent to the Swing Event Dispatcher.
    				 *
    				 * @since 1.0.0
    				 */
        			class GuiChallengePasswordRequest
        			{
        				/** @brief Instance synchronization monitor */ 
        				private Object monitor;
        				/** @brief Keeps track if the GUI request is still in progress */ 
        				private boolean bInProgress;

        				/** @brief Keeps track if the GUI showing constrains informarion should be shown again */ 
        				private boolean bDontShowConstrainInformation;
        				
        				/** @brief List of title bar icons to be used by the helper dialogs */ 
        				private List<Image> icons;
        
        				/** @brief PPN number to be shown for the authentication dialog in order to request the challenge password */ 
        				private String ppn;
        				/** @brief Challenge Number to be shown for the authentication dialog in order to request the challenge password */ 
        				private String challengeNumber;
        				/** @brief The Challenge Pasword obtained from the authentication dialog */ 
        				private String challengePassword;
        				
        				/** @brief Constructor
        				 *
        				 * @param lIcons [In] List of title bar icons to be used by the helper dialogs.
        				 * 
        				 * @since 1.0.0
        				 */ 
        				public GuiChallengePasswordRequest (List<Image> lIcons)
        				{
        					this.monitor = new Object();
        					this.bInProgress = false;
        					this.bDontShowConstrainInformation = false;
        					this.icons = lIcons;
        					this.ppn = "";
        					this.challengeNumber= "";
        					this.challengePassword= "";
        				}
        				
                        /** @brief Set the list of title bar icons to be used by the helper dialogs
                         *
                         * @param lIcons [In] List of title bar icons to be used by the helper dialogs.
                         * 
                         * @since 1.0.0
                         */ 
        				public boolean setIconImages (List<Image> lIcons)
        				{
        					boolean bResult = false;
        					synchronized (monitor)
        					{
            					if (bInProgress == false)
            					{
            						this.icons = lIcons;
            						bResult = true;
            					}
        					}
        					return bResult;
        				}
        				
        				/** @brief Check if the GUI request is still in progress.
        				 * 
        				 * @return True if the GUI request is still in progress. False otherwise.
        				 * 
        				 * @since 1.0.0
        				 */	
        				public boolean isInProgress ()
        				{
        					boolean bResult = false;
        					synchronized (monitor)
        					{
           						bResult = bInProgress;
        					}
        					return bResult;
        				}
        				
        				/** @brief Gets the PPN number set for the dialogs to be shown.
        				 * 
        				 * @return PPN number.
        				 * 
        				 * @since 1.0.0
        				 */	
        				public String getPPN ()
        				{
        					String sResult = "";
        					synchronized (monitor)
        					{
        						sResult = ppn;
        					}
        					return sResult;
        				}
        				
        				/** @brief Gets the Challenge Number set for the dialogs to be shown.
        				 * 
        				 * @return Challenge Number.
        				 * 
        				 * @since 1.0.0
        				 */	
        				public String getChallengeNumber ()
        				{
        					String sResult = "";
        					synchronized (monitor)
        					{
        						sResult = challengeNumber;
        					}
        					return sResult;
        				}
        				
        				/** @brief Gets the Challenge Password retrieved from the user.
        				 * 
        				 * @return Challenge Password.
        				 * 
        				 * @since 1.0.0
        				 */	
        				public String getChallengePassword ()
        				{
        					String sResult = "";
        					synchronized (monitor)
        					{
        						sResult = challengePassword;
        					}
        					return sResult;
        				}
        				
        				/** @brief Gets a runnable block that can be used in the Swing Event Dispatcher to request user information for the Challenge Authentication.
        				 * 
        				 * @return Runnable block to be executed on the Swing Event Dispatcher.
        				 * 
        				 * @since 1.0.0
        				 */	
        				public Runnable getGuiChallengePasswordRequestBlock (String sPPN, String sChallengeNumber, Object notifyMonitor)
        				{
        					Runnable rnBlock = null;
        					synchronized (monitor)
        					{
        						if (bInProgress == false)
        						{
            						this.ppn = sPPN;
            						this.challengeNumber = sChallengeNumber;
            						this.challengePassword = "";
        							bInProgress = true;
        							rnBlock = new Runnable ()
        							{
        								private Object notifyMonitor;
        								
        								public Runnable initialize (Object notifyMonitor)
        								{
        									this.notifyMonitor = notifyMonitor;
        									return this;
        								}
        								
        								@Override
        				                public void run ()
        				                {
        									Object notifyMonitorAux = null;
        									synchronized (monitor)
        									{
        										if (bInProgress)
        										{
    				        						RequestAuthenticationDialog requestAuthenticationDialog = new RequestAuthenticationDialog();
    				        						requestAuthenticationDialog.setLocationRelativeTo( null );
    				        						requestAuthenticationDialog.setAlwaysOnTop( true );
    				        						requestAuthenticationDialog.setModal( true );

        				        					requestAuthenticationDialog.setIconImages(icons);
        				        					requestAuthenticationDialog.setTitle("Authentication");
        				        					requestAuthenticationDialog.reset(ppn,challengeNumber);
        				        
        				        					// Finally here the control is transfered to the dialog
        				        					requestAuthenticationDialog.setVisible( true );
        				        
        				        					if (requestAuthenticationDialog.shouldProceedWithAuthentication())
        				        					{
        				        						challengePassword = requestAuthenticationDialog.getPassword();
        				        					}

        				        					requestAuthenticationDialog.dispose( );
        				        					bInProgress = false;
        				        					notifyMonitorAux = notifyMonitor;
        										}
        									}
				        					if (notifyMonitorAux != null)
				        					{
				        						synchronized (notifyMonitorAux)
				        						{
				        							notifyMonitorAux.notifyAll();
				        						}
				        					}
        				                }
        							}.initialize(notifyMonitor);
        						}
        					}
        					return rnBlock;
        				}
        
        				/** @brief Gets a runnable block that can be used in the Swing Event Dispatcher to inform the user regarding the Challenge Authentication constrains.
        				 * 
        				 * @return Runnable block to be executed on the Swing Event Dispatcher.
        				 * 
        				 * @since 1.0.0
        				 */	
        				public Runnable getGuiAuthenticatedInformationBlock (Object notifyMonitor)
        				{
        					Runnable rnBlock = null;
        					synchronized (monitor)
        					{
        						if (bDontShowConstrainInformation == false)
        						{
        							rnBlock = new Runnable ()
        							{
        		        				private List<Image> icons;
        								
        								public Runnable initialize (List<Image> icons)
        								{
        									this.icons = icons;
        									return this;
        								}
        								
        								@Override
        				                public void run ()
        				                {
        									Object[] options = {"Ok","Don't show again"};
    		        						String message = "" +
    		        								"You have logged into Secure Maintenance Mode.\n" +
    		        								"Per Security regulations, you will automatically be logged out of Secure Maintenance Mode if 15 minutes elapse.\n" +
    		        								"To log back in, you will have to repeat the challenge authentication procedure.\n" +
    		        								"Thank you.";
    		        						JOptionPane pane = new JOptionPane(message,JOptionPane.INFORMATION_MESSAGE,JOptionPane.DEFAULT_OPTION,null,options,options[0]);
    		        						JDialog informationDialog = pane.createDialog("Information");
    		        						informationDialog.setAlwaysOnTop(true);

    										informationDialog.setIconImages(icons);

    										informationDialog.setVisible(true);

    										Object userSelectedOption = pane.getValue();
    										if ((userSelectedOption != null) && (userSelectedOption.equals(options[1])))  //Don't show again
											{
        			        					synchronized (monitor)
        			        					{
        			        						bDontShowConstrainInformation = true;
        			        					}										
											}

    										informationDialog.dispose();
        				                }
        							}.initialize(icons);
        						}
        					}
        					return rnBlock;
        				}

        				/** @brief Gets a runnable block that can be used in the Swing Event Dispatcher to inform the user regarding the Challenge Authentication error.
        				 * 
        				 * @return Runnable block to be executed on the Swing Event Dispatcher.
        				 * 
        				 * @since 1.0.0
        				 */	
        				public Runnable getGuiNotAuthenticatedErrorBlock (Object notifyMonitor)
        				{
        					Runnable rnBlock = null;
        					synchronized (monitor)
        					{
        						if (bInProgress == false)
        						{
        							bInProgress = true;
        							rnBlock = new Runnable ()
        							{
        								private Object notifyMonitor;
        								
        								public Runnable initialize (Object notifyMonitor)
        								{
        									this.notifyMonitor = notifyMonitor;
        									return this;
        								}
        								
        								@Override
        				                public void run ()
        				                {
        									Object notifyMonitorAux = null;
        									synchronized (monitor)
        									{
        										if (bInProgress)
        										{
    				        						String message = "" +
    				        								"Authentication failed. Please try again...";
    				        						JOptionPane pane = new JOptionPane(message,JOptionPane.ERROR_MESSAGE);
    				        						JDialog informationDialog = pane.createDialog("Information");
    				        						informationDialog.setAlwaysOnTop(true);

        											informationDialog.setIconImages(icons);

        											informationDialog.setVisible(true);
        											informationDialog.dispose();
        				        					bInProgress = false;
        				        					notifyMonitorAux = notifyMonitor;
        										}
        									}
				        					if (notifyMonitorAux != null)
				        					{
				        						synchronized (notifyMonitorAux)
				        						{
				        							notifyMonitorAux.notifyAll();
				        						}
				        					}
        				                }
        							}.initialize(notifyMonitor);
        						}
        					}
        					return rnBlock;
        				}
        			}

    				/** @brief Instance synchronization monitor */ 
        			private Object  monitor;
        
        			/** @brief Expression which track the connection proper initialization */ 
        			private IWaitCondition waitConditionExpression;
        			
        			/** @brief Keeps track of the authentication in progress state */ 
        	        boolean bAuthenticating;
        
        			/** @brief Current technician Id used for authentication */ 
        	        private String technicianId;
        			/** @brief Engine used to request authentication requirements */ 
        	        private ChallengeRequestEngine challengeRequestEngine;
        			/** @brief Engine used to perform authentication */ 
        	        private ChallengeAuthenticationEngine challengeAuthenticationEngine;
        			/** @brief Map that keeps track of the authenticated units */ 
        	        private Map<String,Pair<String,Boolean>> uriToChallengePasswordAuthenticatedMap;
        			/** @brief Helper object which facilitates synch GUI operations through Swing Events */ 
        	        private GuiChallengePasswordRequest guiChallengePasswordRequest;
        
        	    	//----------------------------------------------------
					//TODO: Remove when correct implementation is done. --
        			final Map<String,String> tokenHackMap = new HashMap<String,String>();
        			
        			private void iterateTokenDirectory (Path directoryPath)
        			{
        				if (directoryPath != null)
        				{
            				if (Files.exists(directoryPath))
            				{
            					if (Files.isDirectory(directoryPath))
            					{
                                    try (DirectoryStream<Path> directoryStream = Files.newDirectoryStream(directoryPath))
                                    {
                                    	if (directoryStream != null)
                                    	{
                                        	for (Path entry: directoryStream)
                                        	{
                                        		if (Files.isDirectory(entry))
                                        		{
                                        			iterateTokenDirectory(entry);
                                        		}
                                        		else
                                        		{
                                        			String fileName = entry.getFileName().toString();
                                        			if (fileName.matches(TOKEN_HACK_FILE_EXTENSION_MATCHES_REGEX))
                                        			{
                                        				String tokenPPN = fileName.replaceFirst(TOKEN_HACK_FILE_EXTENSION_REPLACE_REGEX,"");
                            	                        if (Files.size(entry) < TOKEN_HACK_FILE_MAX_SIZE)
                            	                        {
                                                            String tokenValid = new String(Files.readAllBytes(entry),StandardCharsets.UTF_8);
                                            				tokenHackMap.put(tokenPPN,tokenValid);
                            	                        }
                                        			}
                                        		}
                                        	}
                                    	}
                                    }
                                    catch (IOException e)
                                    {
	                                    logger.error("Error. Exception thrown while iterating token hack directory '" + directoryPath.toAbsolutePath().toString() + "'. " + e.getMessage(),e);
                                    }
            					}
            				}        					
        				}
        			}
        			
        			private void initializeChallengePasswordTokenHack ()
        			{
        				tokenHackMap.clear();
        				iterateTokenDirectory(Paths.get(TOKEN_HACK_DIRECTORY_PATHNAME));
        				if (tokenHackMap.isEmpty())
        				{
        					logger.error("Error. No tokens for hack have been loaded.");
        				}
        			}
        			
        			private String challengePasswordTokenHack (String sUri)
        			{
        				initializeChallengePasswordTokenHack(); //Refresh token hack from the file system before returning a value.
        				return tokenHackMap.get(sUri);
        			}
					//----------------------------------------------------
        
        			/** @brief Internal representation of the successful condition.
        			 * 
        			 * @since 1.0.0
        			 */	
        			private boolean successfulExpression ()
        			{
        				return (guiChallengePasswordRequest.isInProgress() == false);
        			}
        
        			/** @brief Anonymous initializer
        			 * 
        			 * @return Current anonymous instance.
        			 *
        			 * @since 1.0.0
        			 */
        			public IAuthenticationBlock initialize (List<Image> lIcons, String sTechnicianId) throws EdtServiceException
        			{
						//----------------------------------------------------
						//TODO: Remove when correct implementation is done. --
        				this.initializeChallengePasswordTokenHack();
						//----------------------------------------------------
        				
        				this.monitor = new Object();
        
        				this.waitConditionExpression = new IWaitCondition()
        				{
        					@Override
        					public boolean expression()
        					{
        						return successfulExpression();
        					}
        				};
        				
        				bAuthenticating = false;
        
        				technicianId = sTechnicianId;
        				challengeRequestEngine = new ChallengeRequestEngine();
        				challengeAuthenticationEngine = new ChallengeAuthenticationEngine();
        				uriToChallengePasswordAuthenticatedMap = new HashMap<String,Pair<String,Boolean>>();
        				guiChallengePasswordRequest = new GuiChallengePasswordRequest(lIcons);
        				return this;
        			}
        
        			/** @brief Adds an observer to the set of observers for this object, provided that it is not the same as some observer already in the set.
        			 *
        			 * @param o [In] Observer to be notified upon updates.
        			 * 
        			 * @since 1.0.0
        			 */ 
        			@Override
                    public void addObserver (Observer o)
                    {
        				synchronized (monitor)
        				{
        					if (bAuthenticating == false)
        					{
                				challengeRequestEngine.addObserver(o);
                				challengeAuthenticationEngine.addObserver(o);
        					}
        				}
                    }
        
        			/** @brief Returns the number of observers of this Observable object.
        			 *
        			 * @return Number of observers of this Observable object.
        			 * 
        			 * @since 1.0.0
        			 */
        			@Override
                    public int countObservers ()
                    {
        				int iResult = -1;
        				synchronized (monitor)
        				{
        					if (bAuthenticating == false)
        					{
        						iResult = challengeRequestEngine.countObservers(); // challengeAuthenticationEngine.countObservers();
        					}
        				}
        				return iResult;
                    }
        
        			/** @brief Deletes an observer from the set of observers of this object.
        			 *
        			 * @param o [In] Observer to be notified upon updates.
        			 * 
        			 * @since 1.0.0
        			 */
        			@Override
                    public void deleteObserver (Observer o)
                    {
        				synchronized (monitor)
        				{
        					if (bAuthenticating == false)
        					{
                				challengeRequestEngine.deleteObserver(o);
                				challengeAuthenticationEngine.deleteObserver(o);
        					}
        				}
                    }
        
        			/** @brief Clears the observer list so that this object no longer has any observers.
        			 *
        			 * @since 1.0.0
        			 */
        			@Override
                    public void deleteObservers ()
                    {
        				synchronized (monitor)
        				{
        					if (bAuthenticating == false)
        					{
                				challengeRequestEngine.deleteObservers();
                				challengeAuthenticationEngine.deleteObservers();
        					}
        				}
                    }
        
        			/** @brief Tests if this object has changed.
        			 *
        			 * @return True if the object has been marked as changed. False otherwise.
        			 *
        			 * @since 1.0.0
        			 */
        			@Override
                    public boolean hasChanged ()
                    {
        				boolean bResult = false;
        				synchronized (monitor)
        				{
        					if (bAuthenticating == false)
        					{
        						bResult = challengeRequestEngine.hasChanged() || challengeAuthenticationEngine.hasChanged();
        					}
        				}
        				return bResult;
                    }
        
        			/** @brief If this object has changed, as indicated by the hasChanged method, then notify all of its observers and then call the clearChanged method to indicate that this object has no longer changed.
        			 *
        			 * @since 1.0.0
        			 */
        			@Override
                    public void notifyObservers ()
                    {
        				synchronized (monitor)
        				{
        					if (bAuthenticating == false)
        					{
                				challengeRequestEngine.notifyObservers();
                				challengeAuthenticationEngine.notifyObservers();
        					}
        				}
                    }
        
        			/** @brief If this object has changed, as indicated by the hasChanged method, then notify all of its observers and then call the clearChanged method to indicate that this object has no longer changed.
        			 *
        			 * @param arg [In] Object to be sent along with notification.
        			 * 
        			 * @since 1.0.0
        			 */
        			@Override
                    public void notifyObservers (Object arg)
                    {
        				synchronized (monitor)
        				{
        					if (bAuthenticating == false)
        					{
                				challengeRequestEngine.notifyObservers(arg);
                				challengeAuthenticationEngine.notifyObservers(arg);
        					}
        				}
                    }
        
        			/** @brief Perform authentication when commanded.
        			 * 
        			 * @param uri [In] Uri address to be matched for authentication.
        			 * 
        			 * @return         AuthenticationResult.AUTHENTICATION_GRANTED if successfully authenticated.
        			 *                 AuthenticationResult.AUTHENTICATION_DENIED if authentication failed.
        			 *                 AuthenticationResult.AUTHENTICATION_CANCELED if authentication was not even attempted.
        			 *                 AuthenticationResult.AUTHENTICATION_INTERRUPTED if connection was lost during authentication.
        			 *
        			 * @since 1.0.0
        			 */ 
        			@Override
                    public AuthenticationResult authenticate (String uri)
                    {
        				AuthenticationResult authResult = AuthenticationResult.AUTHENTICATION_DENIED;
        				synchronized (monitor)
        				{
        					if (bAuthenticating == false)
        					{
        						bAuthenticating = true;
        	    				if ((uri != null) && (uri.isEmpty() == false))
        	    				{
        	    					Pair<String,Boolean> uriEntry = uriToChallengePasswordAuthenticatedMap.get(uri);
        	    					if (uriEntry != null)
        	    					{
        	    						String sChallengePassword = uriEntry.getLeft();
        	    						//TODO: Evaluate if besides uriEntry.getLeft() has a valid password, uriEntry.getRight() is true there is still a need to re-authenticate.
        	    						if (sChallengePassword != null)
        	    						{
        	    							authResult = challengeAuthenticationEngine.authenticate(null,sChallengePassword);
        	    							if (authResult == AuthenticationResult.AUTHENTICATION_GRANTED)
        	    							{
        	    								uriEntry.setValue(true);
        	    							}
        	    						}
        	    					}
        	    
        	    					if (authResult == AuthenticationResult.AUTHENTICATION_DENIED)
        	    					{
        	    						uriToChallengePasswordAuthenticatedMap.remove(uri);
        	    						StringBuilder ppn = new StringBuilder();
        	    						StringBuilder challengeNumber = new StringBuilder();
        	    						if (challengeRequestEngine.requestChallengeNumber(null,technicianId,ppn,challengeNumber))
        	    						{
    	    								final int iMaxRetries = 3;
    	    								for (int i = iMaxRetries; (i > 0) && (authResult == AuthenticationResult.AUTHENTICATION_DENIED); --i)
    	    								{
            	    							Runnable block = guiChallengePasswordRequest.getGuiChallengePasswordRequestBlock(ppn.toString(),challengeNumber.toString(),monitor);
            	    							if (block != null)
            	    							{
            	    								SwingUtilities.invokeLater(block);
            	    								if (WaitCondition.waitConditionExpression(monitor,waitConditionExpression))
            	    								{
            	    									String sChallengePassword = guiChallengePasswordRequest.getChallengePassword();

            	    									//----------------------------------------------------
            	    									//TODO: Remove when correct implementation is done. --
            	    									if ((sChallengePassword != null) && (sChallengePassword.isEmpty() == false))
            	    									{
            	    										sChallengePassword = challengePasswordTokenHack(((sChallengePassword.equals("0000000000"))?(sChallengePassword):(uri)));
            	    									}
            	    									//----------------------------------------------------
            	    									
            	    									if ((sChallengePassword != null) && (sChallengePassword.isEmpty() == false))
            	    									{
            	    										authResult = challengeAuthenticationEngine.authenticate(null,sChallengePassword);
            	    										
            	    										if (authResult == AuthenticationResult.AUTHENTICATION_GRANTED)
            	    										{
            	    											uriToChallengePasswordAuthenticatedMap.put(uri,MutablePair.of(sChallengePassword,true));
            	    										}
            	    										else if (authResult == AuthenticationResult.AUTHENTICATION_INTERRUPTED)
            	    										{
            	    											i = 1; //Just stop trying because connection was lost.
            	    										}
            	    										else if (i > 1)
            	    										{
            	    	        	    						Runnable blockAux = guiChallengePasswordRequest.getGuiNotAuthenticatedErrorBlock(monitor);
            	    	        	    						if (blockAux != null)
            	    	        	    						{
            	    	        	    							SwingUtilities.invokeLater(blockAux);
            	    	        	    							WaitCondition.waitConditionExpression(monitor,waitConditionExpression);
            	    	        	    						}
            	    										}
            	    									}
            	    									else
            	    									{
        	    											authResult = AuthenticationResult.AUTHENTICATION_CANCELED;
            	    									}
            	    								}
        	    								}
        	    							}
        	    						}
        	    					}
        	    					
        	    					if (authResult == AuthenticationResult.AUTHENTICATION_GRANTED)
        	    					{
        	    						Runnable block = guiChallengePasswordRequest.getGuiAuthenticatedInformationBlock(monitor);
        	    						if (block != null)
        	    						{
        	    							SwingUtilities.invokeLater(block);
        	    						}
        	    					}
        	    				}
        						bAuthenticating = false;
        					}
        				}
        	            return authResult;
                    }
        
        			/** @brief Hard reset authentication flag without performing any kind of authentication.
        			 * 
        			 * @param uri           [In] Uri address to be matched for authentication.
        			 * @param authenticated [In] flag value to set for authentication.
        			 * 
        			 * @since 1.0.0
        			 */ 
        			@Override
                    public void setAuthenticated (String uri, boolean authenticated)
                    {
        				synchronized (monitor)
        				{
        					if (bAuthenticating == false)
        					{
            					Pair<String,Boolean> uriEntry = uriToChallengePasswordAuthenticatedMap.get(uri);
            					if (uriEntry != null)
            					{
            						uriEntry.setValue(authenticated);
            					}
        					}
        				}
                    }
        
        			/** @brief Check if authentication was commanded at least once and keep track of it.
        			 * 
        			 * @param uri [In] Uri address to be matched for authentication.
        			 * 
        			 * @return         True if was authenticated. False otherwise.
        			 *
        			 * @since 1.0.0
        			 */ 
        			@Override
                    public boolean isAuthenticated (String uri)
                    {
        				boolean bResult = false;
        				synchronized (monitor)
        				{
        					if (bAuthenticating == false)
        					{
            					Pair<String,Boolean> uriEntry = uriToChallengePasswordAuthenticatedMap.get(uri);
            					if (uriEntry != null)
            					{
            						bResult = uriEntry.getValue().booleanValue();
            					}
        					}
        				}
        	            return bResult;
                    }
        
        			/** @brief Perform an explicit call to forget authentication.
        			 * 
        			 * @param uri [In] Uri address to be matched for authentication.
        			 *
        			 * @since 1.0.0
        			 */ 
        			@Override
                    public void forgetAuthentication (String uri)
                    {
        				synchronized (monitor)
        				{
        					if (bAuthenticating == false)
        					{
        						uriToChallengePasswordAuthenticatedMap.remove(uri);
        					}
        				}
                    }
        
        			/** @brief Check if authentication is in progress.
        			 * 
        			 * @return True if is currently authenticating. False otherwise.
        			 *
        			 * @since 1.0.0
        			 */ 
        			@Override
                    public boolean isAuthenticating ()
                    {
        				boolean bResult = false;
        				synchronized (monitor)
        				{
        					bResult = bAuthenticating;
        				}
        	            return bResult;
                    }
        		}.initialize(icons,technicianId);
			}
		}
		catch (EdtServiceException e)
		{
			logger.error("Error. Exception thrown while getting a new authentication handler: " + e.getMessage(),e);
		}
		return authenticationBlock;
	}
}
