package com.nui;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;

import android.content.Context;
import android.content.res.AssetManager;

/**
 * [internal] 初始化时操作数据
 *
 */
public class DataOperator {
	// [internal] 初始化拷贝数据的信息比对；是发布配置数据时间
    // [internal]  如果Data目录下配置文件发生变化，修改此处
	static final byte[] VERSION = { 2, 0, 1, 9, 0, 1, 1, 6, 0 };

	static String s_file_dir = "";
	static String s_cache_dir = "";

	static String[] ASSET_RES = {};

	// 扩展数组长度
	private static String[] copyOf(String[] rets, int length) {
		String[] values = new String[length];
		for (int i = 0; i < rets.length; i++) {
			values[i] = rets[i];
		}
		return values;
	}

	private static String[] mergeData(String[]... datas) {
		String[] rets = new String[] {};
		for (int i = 0; i < datas.length; i++) {
			int length1 = rets.length;
			int length2 = datas[i].length;
			rets = copyOf(rets, length1 + length2);
			System.arraycopy(datas[i], 0, rets, length1, length2);
		}
		return rets;
	}

	private static String[] addPath(String[] datas, String path) {
		for (int i = 0; i < datas.length; i++) {
			datas[i] = path + datas[i];
		}
		return datas;
	}

	public static void initialize(Context context) {
		s_file_dir = context.getExternalFilesDir("").getAbsolutePath() + "/";
		s_cache_dir = context.getExternalCacheDir().getAbsolutePath() + "/";
		try {
			File file = new File(s_file_dir);
			if (!file.exists()) {
				file.mkdirs();
			}
		} catch (Exception e) {
			e.printStackTrace();
			return;
		}

		int i_init = 1;
		InputStream is;
		// [internal] 文件输出流
		FileOutputStream os_file;
		// [internal] 文件输入流
		FileInputStream is_file;
		// [internal] 数据流
		byte[] b_data;
		// [internal] 文件
		File file;
		try {
			file = new File(s_file_dir + "ver.dat");
			if (file.exists()) {
				// [internal] 比对数据版本文件
				is_file = new FileInputStream(file);
				b_data = new byte[is_file.available()];
				is_file.read(b_data);
				is_file.close();

				if (Arrays.equals(b_data, VERSION)) {
					i_init = 0;
					return;
				}
			}

			if (i_init != 0) {
				// [internal] 创建数据版本文件
				if (file.exists()) {
					file.delete();
				}
				file.createNewFile();
				os_file = new FileOutputStream(file);
				os_file.write(VERSION);
				os_file.close();

				// [internal] img文件夹
				file = new File(s_file_dir + "img");
				if (!file.exists()) {
					file.mkdirs();
				}
				
				// [internal] img中的table文件夹
				file = new File(s_file_dir + "img/table");
				if (!file.exists()) {
					file.mkdirs();
				}
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}

		if (i_init != 0) {
			AssetManager assetManager = context.getAssets();
			try {
				String[] ASSET_PHOTO = addPath(assetManager.list("img"), "img/");
				String[] ASSET_TABLE = addPath(assetManager.list("img/table"), "img/table/");
				ASSET_RES = mergeData(ASSET_PHOTO, ASSET_TABLE);
				// [internal]拷贝img数据到file文件夹中
				for (int i = 0; i < ASSET_RES.length; i++) {
					file = new File(s_file_dir + ASSET_RES[i]);
					if (file.exists()) {
						if (file.isDirectory())
							continue;
						file.delete();
					}
					is = assetManager.open(ASSET_RES[i]);
					b_data = new byte[is.available()];
					is.read(b_data);
					is.close();

					file.createNewFile();
					os_file = new FileOutputStream(file);
					os_file.write(b_data);
					os_file.close();
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	public static String getFileDir() {
		return s_file_dir;
	}
}
