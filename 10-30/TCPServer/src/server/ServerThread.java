package server;

import java.io.*;
import java.net.Socket;

public class ServerThread extends Thread {

	private Socket cs;

	public ServerThread(Socket cs) {
		// TODO Auto-generated constructor stub
		this.cs = cs;
	}

	public void run() {
		System.out.println("客户机IP：" + cs.getInetAddress() + " port：" + cs.getPort() + "已经连接进来");
		try {
			PrintWriter pw = new PrintWriter(cs.getOutputStream());
			BufferedReader br = new BufferedReader(new InputStreamReader(cs.getInputStream()));
			String inLine = null;
			while (true){
				inLine=br.readLine();
				int intCmd=Integer.parseInt(inLine);
				if(intCmd==-1){
					break;
				}
				String serverFilePath;
				File file;
				FileTrans fileTrans;
				switch (intCmd) {
				case 1:
					serverFilePath=br.readLine();
					file=new File(serverFilePath);
					if(file.exists()){
						pw.println(String.valueOf(true));
						pw.flush();
						fileTrans=new FileTrans(serverFilePath,cs);
						fileTrans.ReadFile();
					}else {
						pw.println(String.valueOf(false));
						pw.flush();
					}
					break;
				case 2:
					serverFilePath=br.readLine();
					file=new File(serverFilePath);
					if(file.exists()&&file.isFile()){
						pw.println(String.valueOf(true));
						pw.flush();
						fileTrans=new FileTrans(serverFilePath,cs);
						fileTrans.FileUp();
					}else {
						pw.println(String.valueOf(false));
						pw.flush();
					}
					break;
				case 3:
					serverFilePath=br.readLine();
					fileTrans=new FileTrans(serverFilePath,cs);
					fileTrans.DownloadFile();
					break;
				default:
					break;
				}
			}
			System.out.println("再见！");
			pw.close();
			br.close();
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		try {
			cs.close();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
