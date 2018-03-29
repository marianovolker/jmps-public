package com.gilbarco.globaltools.spotdecommissioning.main.gui;

public interface SPOTDecommissioningProgressObserver
{
	public void notifyConnected ();
	public void notifyProgress ();
	public void notifySuccess ();
	public void notifyFailed (String error);
	public void notifyDisconnected ();
}
