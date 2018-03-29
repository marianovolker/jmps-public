/**
 * Copyright © 2013 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file RequestAuthenticationDialog.java
 * @author mgrieco
 * @date Tuesday, August 27, 2013
 * @copyright Copyright © 2013 Gilbarco Inc. Confidential and Proprietary.
 */

package com.gilbarco.globaltools.flexpay.targetunit.authmgnt.challenge.gui;

import java.awt.Color;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

import javax.swing.AbstractAction;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.KeyStroke;

/** @class RequestAuthenticationDialog RequestAuthenticationDialog.java "com.gilbarco.globaltools.flexpay.challenge.gui"
 * @brief This class provides a dialog to request a password for a challenge authentication.
 *
 * @since 1.0.0
 */
public class RequestAuthenticationDialog extends JDialog
{
	/** @brief The Constant serialVersionUID. */
	private static final long  serialVersionUID       = 8755710539831593576L;

	/** @brief The Constant DEFAULT_BUTTON_COLOR. */
	private static final Color DEFAULT_BUTTON_COLOR   = new Color(205, 217, 233);

	/** @brief The Constant DEFAULT_EDITABLE_COLOR. */
	private static final Color DEFAULT_EDITABLE_COLOR = new Color(213, 223, 234);

	/** @brief The button authenticate. */
	private JButton            btnAuthenticate;

	/** @brief The label ppn field. */
	private JLabel             lblPPNField;

	/** @brief The label challenge field. */
	private JLabel             lblChallengeField;

	/** @brief The label password field. */
	private JLabel             lblPasswordField;

	/** @brief The text ppn field. */
	private JTextField         txtPPNField;

	/** @brief The text challenge field. */
	private JTextField         txtChallengeField;

	/** @brief The java password field. */
	private JTextField         jPasswordField;

	/** @brief The button cancel. */
	private JButton            btnCancel;

	/** @brief The boolean value for authenticate. */
	protected boolean          bAuthenticate;

	/** @brief The Constant MAX_PASSWORD_FIELD_LEN. */
	protected static final int MAX_PASSWORD_FIELD_LEN = 10;

	/** @brief The Constant MIN_PASSWORD_FIELD_LEN. */
	protected static final int MIN_PASSWORD_FIELD_LEN = 6;

	/** @brief Default constructor.
	 *
	 * @since 1.0.0
	 */
	public RequestAuthenticationDialog ()
	{
		super();
		this.getContentPane().setBackground(new Color(255, 255, 255));

		setLocation(-223, -44);
		commonConstructor();
	}

	/** @brief Common constructor.
	 *
	 * @since 1.0.0
	 */
	private void commonConstructor ()
	{
		setLayoutSettings();

		addAuthenticationFields();
		addAuthenticationButton();
		addCancelButton();

		pack();
		getContentPane().setLayout(null);
	}

	/** @brief Sets the layout settings.
	 *
	 * @since 1.0.0
	 */
	private void setLayoutSettings ()
	{
		setDefaultCloseOperation(JDialog.HIDE_ON_CLOSE);
		getContentPane().setLayout(null);

		setPreferredSize(new Dimension(350, 200));
		setResizable(false);
	}

	/** @brief Adds the authentication fields.
	 *
	 * @since 1.0.0
	 */
	private void addAuthenticationFields ()
	{
		lblPPNField = new JLabel("PPN" + ":");
		lblPPNField.setFont(new Font("Tahoma", Font.PLAIN, 13));
		lblPPNField.setBounds(50, 15, 87, 18);
		getContentPane().add(lblPPNField);

		txtPPNField = new JTextField("");
		txtPPNField.setBackground(Color.WHITE);
		txtPPNField.setForeground(Color.DARK_GRAY);
		txtPPNField.setEditable(false);
		txtPPNField.setToolTipText((String)null);
		txtPPNField.setBounds(123, 14, 120, 20);
		txtPPNField.setBorder(BorderFactory.createLoweredBevelBorder());
		txtPPNField.setFont(new Font("Tahoma", Font.PLAIN, 13));
		getContentPane().add(txtPPNField);

		lblChallengeField = new JLabel("Challenge" + ":");
		lblChallengeField.setFont(new Font("Tahoma", Font.PLAIN, 13));
		lblChallengeField.setBounds(50, 43, 87, 18);
		getContentPane().add(lblChallengeField);

		txtChallengeField = new JTextField("");
		txtChallengeField.setBackground(Color.WHITE);
		txtChallengeField.setForeground(Color.DARK_GRAY);
		txtChallengeField.setEditable(false);
		txtChallengeField.setToolTipText((String)null);
		txtChallengeField.setBounds(123, 42, 120, 20);
		txtChallengeField.setBorder(BorderFactory.createLoweredBevelBorder());
		txtChallengeField.setFont(new Font("Tahoma", Font.PLAIN, 13));
		getContentPane().add(txtChallengeField);

		lblPasswordField = new JLabel("Password" + ":");
		lblPasswordField.setFont(new Font("Tahoma", Font.PLAIN, 13));
		lblPasswordField.setBounds(50, 72, 87, 18);
		getContentPane().add(lblPasswordField);

		jPasswordField = new JTextField("");
		jPasswordField.setToolTipText((String)null);
		jPasswordField.setBounds(123, 71, 120, 24);
		jPasswordField.setBorder(BorderFactory.createLoweredBevelBorder());
		jPasswordField.setBackground(DEFAULT_EDITABLE_COLOR);
		jPasswordField.setFont(new Font("Tahoma", Font.PLAIN, 13));
		addValidationFieldListeners(jPasswordField);

		checkReadyForAuthentication();		

		getContentPane().add(jPasswordField);
	}

	/** @brief Adds the authentication button.
	 *
	 * @since 1.0.0
	 */
	private void addAuthenticationButton ()
	{
		btnAuthenticate = new JButton("Authenticate");
		btnAuthenticate.addActionListener(new ActionListener()
		{
			public void actionPerformed (ActionEvent e)
			{
				closeDialog(true);
			}
		});
		btnAuthenticate.setBounds(40, 116, 120, 30);
		btnAuthenticate.setFont(new Font("Tahoma", Font.PLAIN, 13));
		btnAuthenticate.setBorderPainted(false);
		btnAuthenticate.setBackground(DEFAULT_BUTTON_COLOR);
		btnAuthenticate.setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));

		checkReadyForAuthentication();		

		getContentPane().add(btnAuthenticate);
	}

	/** @brief Adds the cancel button.
	 *
	 * @since 1.0.0
	 */
	private void addCancelButton ()
	{
		btnCancel = new JButton("Cancel");
		btnCancel.addActionListener(new ActionListener()
		{
			public void actionPerformed (ActionEvent e)
			{
				closeDialog();
			}
		});
		btnCancel.setBounds(202, 116, 112, 30);
		btnCancel.setFont(new Font("Tahoma", Font.PLAIN, 13));
		btnCancel.setBorderPainted(false);
		btnCancel.setBackground(DEFAULT_BUTTON_COLOR);
		btnCancel.setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
		getContentPane().add(btnCancel);

		this.getRootPane().getActionMap().put("ON_EXIT", new AbstractAction()
		{
			private static final long serialVersionUID = 1L;

			@Override
			public void actionPerformed (ActionEvent e)
			{
				closeDialog();
			}
		});
		this.getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke("ESCAPE"), "ON_EXIT");
	}

	/** @brief Adds the validation field listeners.
	 * 
	 * @param txtField [In/Out] Text field to which add key listener 
	 * 
	 * @since 1.0.0
	 */
	private void addValidationFieldListeners (final JTextField txtField)
	{
		txtField.addKeyListener(new KeyAdapter()
		{
			/** @brief Entry point for key typed.
			 * 
			 * @param e [In] Event information regarding the key typed. 
			 * 
			 * @since 1.0.0
			 */
			@Override
			public void keyTyped (KeyEvent e)
			{
				char code = e.getKeyChar();
				String text = ((JTextField)e.getSource()).getText();
				if (((code != KeyEvent.VK_BACK_SPACE) && (code != KeyEvent.VK_CLEAR) && (code != KeyEvent.VK_DELETE)) &&
				    ((!Character.isDigit(code)) || e.isAltDown() || (text.length() > MAX_PASSWORD_FIELD_LEN)))
				{
					e.consume();
					return;
				}
			}

			/** @brief Entry point for key released.
			 * 
			 * @param e [In] Event information regarding the key released. 
			 * 
			 * @since 1.0.0
			 */
			@Override
			public void keyReleased (KeyEvent e)
			{
				String text = ((JTextField)e.getSource()).getText();
				if (!isNumericString(text))
				{
					assert false : "Shouldn't be here !";
					((JTextField)e.getSource()).setText("");
					btnAuthenticate.setEnabled(false);
					return;
				}

				if (text.length() > MAX_PASSWORD_FIELD_LEN)
				{
					((JTextField)e.getSource()).setText(text.substring(0, MAX_PASSWORD_FIELD_LEN));
				}

				checkReadyForAuthentication();
			}
		});

	}

	/** @brief Checks if given string only contains numeric characters.
	 * 
	 * @param text [In] Text which should be validated. 
	 * 
	 * @return True, if it is a numeric string. False otherwise.
	 *
	 * @since 1.0.0
	 */
	private boolean isNumericString (String text)
	{
		for (int i = 0; i < text.length(); ++i)
		{
			if (!Character.isDigit(text.charAt(i)))
				return false;
		}
		return true;
	}

	/** @brief Checks if the information provided by the user is properly formated and could be used for authentication.
	 * 
	 * @return True, if the information provided by the user is properly formated and could be used for authentication. False, otherwise.
	 */
	private boolean checkReadyForAuthentication ()
	{
		boolean bResult = false;
		if ((jPasswordField != null) && (btnAuthenticate != null))
		{
			bResult = (jPasswordField.getText().length() >= MAX_PASSWORD_FIELD_LEN);
			btnAuthenticate.setEnabled(bResult);
		}
		return bResult;
	}

	/** @brief Close dialog.
	 * 
	 * @param bAuthenticationRequested [In] User explicitly requested authentication.
	 * 
	 * @since 1.0.0
	 */
	protected void closeDialog (boolean bAuthenticationRequested)
	{
		bAuthenticate = bAuthenticationRequested;
		this.setVisible(false);// dispose( );
	}

	/** @brief Close dialog.
	 * 
	 * @since 1.0.0
	 */
	protected void closeDialog ()
	{
		closeDialog(false);
	}

	/** @brief Resets the dialog information to request the user a new password.
	 * 
	 * @param sPPN [In] Is the PPN to show the user so he can report it to the TAC.
	 * @param sChallenge [In] Is the Challenge Number to show the user so he can report it to the TAC.
	 * 
	 * @return True, if the new PPN and the new Challenge Number are valid and reset was properly performed. False otherwise.
	 * 
	 * @since 1.0.0
	 */
	public boolean reset (String sPPN, String sChallenge)
	{
		boolean bResult = false;
		if (((sPPN != null) && (sChallenge != null) && ((sPPN.isEmpty() && sChallenge.isEmpty()) || (isNumericString(sPPN) && isNumericString(sChallenge)))) ||
		    ((sPPN == null) && (sChallenge == null)) ||
		    ((sPPN == null) && (sChallenge.isEmpty())) ||
		    ((sPPN.isEmpty()) && (sChallenge == null)))
		{
			bAuthenticate = false;
			txtPPNField.setText((sPPN != null) ? (sPPN) : (""));
			txtChallengeField.setText((sChallenge != null) ? (sChallenge) : (""));
			jPasswordField.setText("");
			btnAuthenticate.setEnabled(false);
			bResult = true;
		}
		return bResult;
	}

	/** @brief Clears the dialog information.
	 * 
	 * @since 1.0.0
	 */
	public void clear ()
	{
		reset(null, null);
	}

	/** @brief Obtains the last valid PPN provided to the dialog.
	 *
	 * @return Current valid PPN stored in the dialog.
	 * 
	 * @since 1.0.0
	 */
	public String getPPN ()
	{
		return txtPPNField.getText();
	}

	/** @brief Obtains the last valid Challenge Number provided to the dialog.
	 *
	 * @return Current valid Challenge Number stored in the dialog.
	 * 
	 * @since 1.0.0
	 */
	public String getChallenge ()
	{
		return txtChallengeField.getText();
	}

	/** @brief Obtains the last retrieved password the user entered into the dialog.
	 *
	 * @return Last entered password provided by the user.
	 * 
	 * @since 1.0.0
	 */
	public String getPassword ()
	{
		return jPasswordField.getText();
	}

	/** @brief Checks if the information provided by the user was confirmed to be used to authenticate.
	 * 
	 * @return True, if the information provided by the user was confirmed to be used to authenticate. False, otherwise.
	 */
	public boolean shouldProceedWithAuthentication ()
	{
		return this.bAuthenticate;
	}

	/** @brief Checks if the information provided by the user is properly formated and ready to be used to authenticate.
	 * 
	 * @return True, if the information provided by the user is properly formated and ready to be used to authenticate. False, otherwise.
	 */
	public boolean isReadyForAuthentication ()
	{
		return checkReadyForAuthentication();
	}
}
