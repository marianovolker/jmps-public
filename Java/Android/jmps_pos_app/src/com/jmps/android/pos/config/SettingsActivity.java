package com.jmps.android.pos.config;

import java.util.List;

import android.annotation.TargetApi;
import android.os.Build;
import android.os.Bundle;
import android.preference.ListPreference;
import android.preference.Preference;
import android.preference.PreferenceActivity;
import android.preference.PreferenceFragment;
import android.preference.PreferenceManager;

import com.jmps.android.pos.R;

/**
 * A {@link PreferenceActivity} that presents a set of application settings.
 * <p>
 * See <a href="http://developer.android.com/design/patterns/settings.html"> Android Design: Settings</a> for design guidelines and the <a
 * href="http://developer.android.com/guide/topics/ui/settings.html">Settings API Guide</a> for more information on developing a Settings UI.
 */
public class SettingsActivity extends PreferenceActivity
{
   /** {@inheritDoc} */
   @Override
   @TargetApi(Build.VERSION_CODES.HONEYCOMB)
   public void onBuildHeaders(List<Header> target)
   {
      loadHeadersFromResource(R.xml.pref_headers, target);
   }
   
   /**
    * A preference value change listener that updates the preference's summary to reflect its new value.
    */
   private static Preference.OnPreferenceChangeListener sBindPreferenceSummaryToValueListener = new Preference.OnPreferenceChangeListener()
   {
      @Override
      public boolean onPreferenceChange(Preference preference, Object value)
      {
         String stringValue = value.toString();
         
         if (preference instanceof ListPreference)
         {
            // For list preferences, look up the correct display value in
            // the preference's 'entries' list.
            ListPreference listPreference = (ListPreference) preference;
            int index = listPreference.findIndexOfValue(stringValue);
            
            // Set the summary to reflect the new value.
            preference.setSummary(index >= 0 ? listPreference.getEntries()[index] : null);
            
         }
         else
         {
            // For all other preferences, set the summary to the value's
            // simple string representation.
            preference.setSummary(stringValue);
         }
         return true;
      }
   };
   
   /**
    * Binds a preference's summary to its value. More specifically, when the preference's value is changed, its summary (line of text below the preference title) is updated to reflect the value. The
    * summary is also immediately updated upon calling this method. The exact display format is dependent on the type of preference.
    * 
    * @see #sBindPreferenceSummaryToValueListener
    */
   private static void bindPreferenceSummaryToValue(Preference preference)
   {
      // Set the listener to watch for value changes.
      preference.setOnPreferenceChangeListener(sBindPreferenceSummaryToValueListener);
      
      // Trigger the listener immediately with the preference's
      // current value.
      sBindPreferenceSummaryToValueListener.onPreferenceChange(preference,
         PreferenceManager.getDefaultSharedPreferences(preference.getContext()).getString(preference.getKey(), ""));
   }
   
   /**
    * This fragment shows general preferences only. It is used when the activity is showing a two-pane settings UI.
    */
   @TargetApi(Build.VERSION_CODES.HONEYCOMB)
   public static class PrinterSettingsFragment extends PreferenceFragment
   {
      @Override
      public void onCreate(Bundle savedInstanceState)
      {
         super.onCreate(savedInstanceState);
         addPreferencesFromResource(R.xml.pref_printer_settings);
         
         // Bind the summaries of EditText preferences
         // to their values. When their values change, their summaries are
         // updated to reflect the new value, per the Android Design
         // guidelines.
         bindPreferenceSummaryToValue(findPreference("ppsAddress"));
         bindPreferenceSummaryToValue(findPreference("ppsPhone"));
      }
   }
}