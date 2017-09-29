
package com.jmps.android.pos.abmensuales;

import android.app.Activity;
import android.app.Fragment;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.jmps.android.pos.R;

public class AbMensualesActivity extends Activity
{

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate( savedInstanceState );
		setContentView( R.layout.activity_ab_mensuales );

		if (savedInstanceState == null)
		{
			getFragmentManager( ).beginTransaction( ).add( R.id.container, new PlaceholderFragment( ) ).commit( );
		}
	}

	/**
	 * A placeholder fragment containing a simple view.
	 */
	public static class PlaceholderFragment extends Fragment
	{

		public PlaceholderFragment()
		{
		}

		@Override
		public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
		{
			View rootView = inflater.inflate( R.layout.fragment_ab_mensuales, container, false );
			return rootView;
		}
	}

	public void startAbMensualesAltaActivity(View view)
	{
		this.startActivity( new Intent( this, AbMensualesAltaActivity.class ) );
	}

	public void startAbMensualesEliminarActivity(View view)
	{
		this.startActivity( new Intent( this, AbMensualesEliminarActivity.class ) );
	}

	public void startAbMensualesModificarActivity(View view)
	{
		this.startActivity( new Intent( this, AbMensualesModificarActivity.class ) );
	}

	public void startAbMensualesPagosActivity(View view)
	{
		this.startActivity( new Intent( this, AbMensualesPagosActivity.class ) );
	}

	public void startAbMensualesPendientesActivity(View view)
	{
		this.startActivity( new Intent( this, AbMensualesPendientesActivity.class ) );
	}

	public void startAbMensualesTarifasActivity(View view)
	{
		this.startActivity( new Intent( this, AbMensualesTarifasActivity.class ) );
	}

	public void btnBack_OnClick(View view)
	{
		this.onBackPressed( );
	}
}
