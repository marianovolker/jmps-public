/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file IAuthenticationStrategy.java
 * @author pcantarini
 * @date Tuesday, August 27, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.authmgnt.strategies;

import java.awt.Image;
import java.util.List;

import com.gilbarco.globaltools.edt.gui.app.EdtAppView;
import com.gilbarco.globaltools.flexpay.targetunit.authmgnt.api.IAuthenticationBlock;

/** @class IAuthenticationStrategy IAuthenticationStrategy.java "com.gilbarco.globaltools.flexpay.authentication.engines.strategies"
 * @brief This class provides an interface for the authentication strategy.
 *
 */
public interface IAuthenticationStrategy
{
  /** @brief Retrieve the authentication technician id.
   *
   * @return the authentication technician id.
   */
  public String getTechnicianID();

  /** @brief Set the authentication technician id.
   *
   * @param the authentication technician id.
   */
  public void setTechnicianID(String strTechnicianID);

  /** @brief Retrieve the authentication icon images.
   *
   * @return the authentication icon images.
   */
  public List<Image> getIconImages ();

  /** @brief Set the authentication icon images.
   *
   * @param the authentication icon images.
   */
  public void setIconImages (List<Image> lIcons);
	
  /** @brief Queries if the corresponding to the authentication implementation has an authentication manager view.
   *
   * @return true if the underlying authentication implementation has an authentication manager view.
   */
  public boolean hasAuthenticationManagerView();

  /** @brief Queries if the given view is instance of the underlying authentication manager view.
   *
   * @return true if the given view is instance of the underlying authentication manager view, false otherwise.
   */
  public boolean isInstanceOfCurrentAuthenticationManagerView(EdtAppView<?> edtAppView);

  /** @brief Get the corresponding authentication manager view.
   *
   * @return the corresponding authentication manager view, null otherwise.
   */
  public EdtAppView<?> getAuthenticationManagerView();

  /** @brief Reset the authentication manager view.
   *
   */
  public void resetAuthenticationManagerView();
  
  /** @brief Obtain the current implementation of the ConnectionEngine AuthenticationBlock.
   *
   * @return ConnectionEngine AuthenticationBlock implementation.
   * 
   * @since 1.0.0
   */
  public IAuthenticationBlock getConnectionAuthenticationBlock ();

}
