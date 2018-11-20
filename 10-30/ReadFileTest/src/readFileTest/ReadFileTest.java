package readFileTest;

import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import java.io.*;

public class ReadFileTest {

	public static ArrayList<String> getFiles(String path) {
		ArrayList<String> files = new ArrayList<String>();
		File file = new File(path);
		if (!file.exists()) {
			System.out.println("The path : " + path + " is not exist");
			return null;
		}
		File[] tempList = file.listFiles();
		for (int i = 0; i < tempList.length; i++) {
			files.add(tempList[i].toString());
		}
		return files;
	}

	public static void main(String[] args) {
		// TODO Auto-generated method stub
//		ArrayList<String> files=getFiles("C:\\Data");
//		for(int i=0;i<files.size();i++) {
//			String str=files.get(i);
//			System.out.println(str.substring(str.lastIndexOf("\\")+1));
//		}
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		try {
			String cmd = br.readLine();
			String regEx = "list.*";
			Pattern pattern = Pattern.compile(regEx);
			Matcher matcher = pattern.matcher(cmd);
			boolean rs = matcher.lookingAt();
			if (rs) {
				String[] strs = cmd.split(" ");
				if (strs.length != 2) {
					System.out.println(cmd + " is illegal");
				} else {
					ArrayList<String> files = getFiles(strs[1]);
					if (files != null) {
						for (int i = 0; i < files.size(); i++) {
							String file = files.get(i);
							System.out.println(file.substring(file.lastIndexOf("\\") + 1));
						}
					}
				}
			} else {
				System.out.println(cmd + " is illegal");
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
