
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.ResolveInfo;

public class Util {

	public static ActivityInfo getSelfActivityInfo(Context con) {
		PackageManager packageManager = con.getPackageManager();
		ActivityInfo info = null;
		try {
			info = packageManager.getActivityInfo(
					((Activity) con).getComponentName(), 0);
		} catch (NameNotFoundException e) {
			e.printStackTrace();
			return null;
		}
		return info;
	}

	/*
	 * return AndroidManifest.xml register activities
	 */
	public static ActivityInfo[] getSelfActivityInfos(Context con) {
		Intent i = new Intent(Intent.ACTION_MAIN);
		i.addCategory(Intent.CATEGORY_LAUNCHER);
		PackageManager pm = con.getPackageManager();

		PackageInfo info = null;
		try {

			info = pm.getPackageInfo(con.getPackageName(),
					PackageManager.GET_ACTIVITIES);
		} catch (NameNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return null;
		}

		// ApplicationInfo test = info.applicationInfo;
		ActivityInfo[] list = info.activities;
		return list;
	}

	/*
	 * get Home Activity info
	 */
	public static ActivityInfo getLauncherActivityInfo(Context con) {
		Intent intent = new Intent(Intent.ACTION_MAIN);
		intent.addCategory(Intent.CATEGORY_HOME);
		ResolveInfo resolveInfo = con.getPackageManager().resolveActivity(
				intent, PackageManager.MATCH_DEFAULT_ONLY);
		return resolveInfo.activityInfo;
	}
}
