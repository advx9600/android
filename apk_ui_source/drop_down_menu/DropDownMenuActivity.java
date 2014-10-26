/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.example.android.customchoicelist;

import java.util.ArrayList;
import java.util.List;

import com.s5pv210.system.test.R;

import android.app.Activity;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.PopupWindow;
import android.widget.TextView;

/**
 * This sample demonstrates how to create custom single- or multi-choice
 * {@link android.widget.ListView} UIs. The most interesting bits are in the
 * <code>res/layout/</code> directory of this sample.
 */
public class DropDownMenuActivity extends Activity {
	private LayoutInflater inflater;

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.sample_main);
		inflater = LayoutInflater.from(this);
	}

	/**
	 * A simple array adapter that creates a list of cheeses.
	 */
	protected class DropDownListItem {
		public int icon;
		public String description;
		public Runnable action;

		public DropDownListItem(int icon, String description, Runnable action) {
			super();
			this.icon = icon;
			this.description = description;
			this.action = action;
		}

		@Override
		public String toString() {
			return description;
		}
	}

	public void test(View v) {
		LinearLayout list = new LinearLayout(v.getContext());
		list.setOrientation(LinearLayout.VERTICAL);

		final PopupWindow dropDown = new PopupWindow(list,
				LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT, true);
		List<DropDownListItem> items = new ArrayList<DropDownListItem>();
		items.add(new DropDownListItem(
				android.R.drawable.ic_menu_close_clear_cancel, "test",
				new Runnable() {
					@Override
					public void run() {
						android.util.Log.i("aaaaaaaa", "aaaaaaaaaaaa");
					}
				}));

		for (final DropDownListItem item : items) {
			ViewGroup listItem = (ViewGroup) inflater.inflate(
					R.layout.drop_down_list_item, null);
			list.addView(listItem);

			ImageView icon = (ImageView) listItem.findViewById(R.id.icon);
			icon.setImageResource(item.icon);

			TextView description = (TextView) listItem
					.findViewById(R.id.description);
			description.setText(item.description);

			listItem.setOnClickListener(new OnClickListener() {

				@Override
				public void onClick(View v) {
					item.action.run();
					dropDown.dismiss();
				}
			});
		}

		Drawable background = getResources().getDrawable(
				android.R.drawable.editbox_dropdown_dark_frame);
		dropDown.setBackgroundDrawable(background);
		dropDown.showAsDropDown(v);
	}
}
