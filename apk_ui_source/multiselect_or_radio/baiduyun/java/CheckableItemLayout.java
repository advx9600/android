package com.dafeng.opensource.ui.widget;

import com.example.androidtest.R;
import com.example.androidtest.a;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.view.View;
import android.view.accessibility.AccessibilityEvent;
import android.widget.Checkable;
import android.widget.ListView;
import android.widget.RelativeLayout;

public class CheckableItemLayout extends RelativeLayout implements Checkable {

	private static final int BASE_PADDING = (int) (40);
	private static final int[] CHECKED_STATE_SET = { android.R.attr.state_checked };
	public static final int CHOICE_MODE_MULTIPLE = ListView.CHOICE_MODE_MULTIPLE;
	public static final int CHOICE_MODE_NONE = ListView.CHOICE_MODE_NONE;
	public static final int CHOICE_MODE_SINGLE = ListView.CHOICE_MODE_SINGLE;
	// private static final String TAG = "CheckableItemLayout";

	private int mBasePaddingRight=80;
	private Drawable mCheckMarkDrawable = null;
	private boolean mChecked;
	private int mChoiceMode = CHOICE_MODE_NONE;

	public CheckableItemLayout(Context paramContext) {
		this(paramContext, null);
	}

	public CheckableItemLayout(Context paramContext,
			AttributeSet paramAttributeSet) {
		this(paramContext, paramAttributeSet, 0);
	}

	public CheckableItemLayout(Context paramContext,
			AttributeSet paramAttributeSet, int paramInt) {
		super(paramContext, paramAttributeSet, paramInt);
		setWillNotDraw(false);
	}

	private void setCheckMarkDrawable(Drawable paramDrawable) {
		if (this.mCheckMarkDrawable != null) {
			this.mCheckMarkDrawable.setCallback(null);
			unscheduleDrawable(this.mCheckMarkDrawable);
		}

		this.mCheckMarkDrawable = paramDrawable;

		boolean bool;
		if (paramDrawable != null) {
			paramDrawable.setCallback(this);
			if (getVisibility() == View.VISIBLE) {
				bool = true;
				paramDrawable.setVisible(bool, false);
				paramDrawable.setState(CHECKED_STATE_SET);
				super.setPadding(getPaddingLeft(), getPaddingTop(),
						BASE_PADDING, getPaddingBottom());
				paramDrawable.setState(getDrawableState());
			}
		} else {
			super.setPadding(getPaddingLeft(), getPaddingTop(),
					mBasePaddingRight, getPaddingBottom());
		}
	}

	@Override
	public boolean dispatchPopulateAccessibilityEvent(
			AccessibilityEvent paramAccessibilityEvent) {
		boolean bool = super
				.dispatchPopulateAccessibilityEvent(paramAccessibilityEvent);
		if (!bool) {
			paramAccessibilityEvent.setChecked(this.mChecked);
		}
		return bool;
	}

	protected void drawableStateChanged() {
		super.drawableStateChanged();
		if (this.mCheckMarkDrawable != null) {
			int[] arrayOfInt = getDrawableState();
			this.mCheckMarkDrawable.setState(arrayOfInt);
			invalidate();
		}
	}

	public int getChoiceMode() {
		return this.mChoiceMode;
	}

	@Override
	public boolean isChecked() {
		return this.mChecked;
	}

	@Override
	public void setChecked(boolean checked) {
		// TODO Auto-generated method stub
		if (this.mChecked != checked) {
			this.mChecked = checked;
			refreshDrawableState();
		}
	}

	@Override
	public void toggle() {
		// TODO Auto-generated method stub
		setChecked(!this.mChecked);
	}

	@Override
	protected int[] onCreateDrawableState(int paramInt) {
		int[] arrayOfInt = super.onCreateDrawableState(paramInt + 1);
		if (isChecked()) {
			mergeDrawableStates(arrayOfInt, CHECKED_STATE_SET);
		}
		return arrayOfInt;
	}

	@Override
	protected void onDraw(Canvas paramCanvas) {
		super.onDraw(paramCanvas);
		Drawable localDrawable = this.mCheckMarkDrawable;
		if (localDrawable != null) {
			int i = localDrawable.getIntrinsicHeight();
			int j = localDrawable.getIntrinsicWidth();
			int k = (getHeight() - i) / 2;
			int m = BASE_PADDING;
			int n = getWidth() - m + (m - j) / 2;
			localDrawable.setBounds(n, k, j + n, i + k);
			localDrawable.draw(paramCanvas);
		}
	}

	public void setChoiceMode(int paramInt) {
		if (this.mChoiceMode == paramInt) {
			return;
		}
		this.mChoiceMode = paramInt;
		Drawable localDrawable;
		View localView;
		if (this.mChoiceMode == CHOICE_MODE_SINGLE) {
			localDrawable = getResources().getDrawable(
					R.drawable.rice_btn_radio);
		} else if (this.mChoiceMode == CHOICE_MODE_MULTIPLE) {
			localDrawable = getResources().getDrawable(
					R.drawable.rice_btn_check);
		} else {
			localDrawable = null;
		}

		setCheckMarkDrawable(localDrawable);
		localView = findViewById(android.R.id.button1);
		if (localDrawable != null) {
			if (localView != null) {
				localView.setVisibility(View.GONE);
			}
		} else {
			if (localView != null) {
				localView.setVisibility(View.VISIBLE);
			}
		}
	}

	@Override
	public void setPadding(int paramInt1, int paramInt2, int paramInt3,
			int paramInt4) {
		super.setPadding(paramInt1, paramInt2, paramInt3, paramInt4);
		if (this.mChoiceMode == CHOICE_MODE_NONE) {
			this.mBasePaddingRight = getPaddingRight();
		}
	}

}
