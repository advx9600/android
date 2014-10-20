
import android.database.sqlite.SQLiteDatabase;

public class ExtraDaoMater {
	public static void onCreate(SQLiteDatabase db) {
		DaoMaster daoMaster = new DaoMaster(db);
		DaoSession daoSession = daoMaster.newSession();
		TbAppDisDao dao = daoSession.getTbAppDisDao();
		TbAppDis tbAppDis = new TbAppDis();
		tbAppDis.setName("aaaaa");
		tbAppDis.setPkg("bbb");
		dao.insert(tbAppDis);
	}
	/*
	 * add in DaoMaster.java public static abstract class OpenHelper extends
	 * SQLiteOpenHelper {
	 * 
	 * public OpenHelper(Context context, String name, CursorFactory factory) {
	 * super(context, name, factory, SCHEMA_VERSION); }
	 * 
	 * @Override public void onCreate(SQLiteDatabase db) { Log.i("greenDAO",
	 * "Creating tables for schema version " + SCHEMA_VERSION);
	 * createAllTables(db, false); ExtraDaoMater.onCreate(db); // this is added
	 * } }
	 */
}
