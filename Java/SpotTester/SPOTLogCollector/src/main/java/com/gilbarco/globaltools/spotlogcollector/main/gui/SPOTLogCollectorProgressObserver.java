package com.gilbarco.globaltools.spotlogcollector.main.gui;

public interface SPOTLogCollectorProgressObserver
{
	public void notifyConnected ();
	public void notifyProgress ();
	public void notifySuccess ();
	public void notifyFailed ();
	public void notifyDisconnected ();
}
