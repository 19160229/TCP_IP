package client;
import java.net.*;
import java.io.*;

public class TCPClient {

	public static void Client() throws Exception {
		String addr="127.0.0.1";
		int port=9999;
		Socket cs=new Socket(InetAddress.getByName(addr), port);
		System.out.println("客户端向服务器发起连接...");
		BufferedReader br=new BufferedReader(new InputStreamReader(cs.getInputStream()));
		PrintWriter pw=new PrintWriter(cs.getOutputStream());
		BufferedReader kbr=new BufferedReader(new InputStreamReader(System.in));
		String line=null;
		while (true){
			line=kbr.readLine();
			if("quit".equals(line)){
				pw.println(-1);
				break;
			}
			int cmd=analyseCmd(line);
			String serverFilePath,clientFilePath;
			String[] strs=line.split(" ");
			FileTrans fileTrans;
			File file;
			switch (cmd) {
			case 1:
				serverFilePath=strs[1];
				pw.println(1);
				pw.println(serverFilePath);
				pw.flush();
				if(Boolean.valueOf(br.readLine())){
					int len=Integer.valueOf(br.readLine());
					for(int i=0;i<len;i++){
						System.out.println(br.readLine());
					}
				}else {
					System.out.println("非法命令");
				}
				break;
			case 2:
				serverFilePath=strs[1];
				clientFilePath=strs[2];
				pw.println(2);
				pw.println(serverFilePath);
				pw.flush();
				if(Boolean.valueOf(br.readLine())){
					fileTrans=new FileTrans(clientFilePath,cs);
					fileTrans.DownloadFile();
					System.out.println("文件传输成功");
				}else {
					System.out.println("非法命令");
				}
				break;
			case 3:
				clientFilePath=strs[1];
				file=new File(clientFilePath);
				if(file.exists()&&file.isFile()){
					serverFilePath=strs[2];
					pw.println(3);
					pw.println(serverFilePath);
					pw.flush();
					fileTrans=new FileTrans(clientFilePath,cs);
					fileTrans.FileUp();
					System.out.println("文件上传完成");
				}else {
					System.out.println("非法命令");
				}
				break;
			default:
				break;
			}
		} 
		System.out.println("再见！");	
		pw.close();
		br.close();
		cs.close();
	}
	
	public static int analyseCmd(String string) {
		String[] temps = string.split(" ");
		if (temps.length <= 1 || temps.length > 3) {
			return 0;
		}
		String cmd = temps[0];
		if ("list".equals(cmd)) {
			return 1;
		} else if ("get".equals(cmd)) {
			return 2;
		} else if ("put".equals(cmd)) {
			return 3;
		}
		return 0;
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		try {
			Client();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
