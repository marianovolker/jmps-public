/**
 * 
 */
package com.jmps.android.pos;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.GridView;
import android.widget.TextView;

import com.jmps.android.pos.R;

public class GridViewDemo extends Activity implements AdapterView.OnItemClickListener
{
   
   TextView selection;
   String[] items = { "this", "is", "a", "really", "really2", "really3", "really4", "really5", "silly", "list" };
   
   @Override
   public void onCreate(Bundle icicle)
   {
      super.onCreate(icicle);
      setContentView(R.layout.grid_view_demo);
      selection = (TextView) findViewById(R.id.selection);
      
      GridView gv = (GridView) findViewById(R.id.grid);
      
      ArrayAdapter<String> aa = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, items);
      
      gv.setAdapter(aa);
      gv.setOnItemClickListener(this);
   }
   
   public void onItemClick(AdapterView<?> parent, View v, int position, long id)
   {
      selection.setText(items[position]);
   }
   
}
