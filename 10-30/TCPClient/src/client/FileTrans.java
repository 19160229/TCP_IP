package client;

import java.io.*;
import java.net.*;

public class FileTrans {
	private String filePath;
	private Socket socket;

	public FileTrans() {
		// TODO Auto-generated constructor stub
	}

	public FileTrans(String filePath, Socket socket) {
		this.filePath = filePath;
		this.socket = socket;
	}

	public void ReadFile() {
		File file = new File(filePath);
		File[] tempList = file.listFiles();
		PrintWriter pw;
		try {
			pw = new PrintWriter(socket.getOutputStream());
			pw.println(tempList.length);
			pw.flush();
			for (int i = 0; i < tempList.length; i++) {
				pw.println(tempList[i].toString());
				pw.flush();
			}
			// pw.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void DownloadFile() {
		try {
			DataInputStream dis = new DataInputStream(socket.getInputStream());
			// long fileLength=dis.readLong();
			File file = new File(filePath);
			FileOutputStream fos = new FileOutputStream(file);
			long fileLength=dis.readLong();
			int length=(int)fileLength;
			int passedLen=0;
			byte[] bytes = new byte[1024];
			while(passedLen!=length){
				int read=dis.read(bytes,0,bytes.length);
				fos.write(bytes, 0, read);
				passedLen+=read;
				fos.flush();
			}
			fos.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void FileUp() {
		try {
			File file=new File(filePath);
			FileInputStream fis = new FileInputStream(filePath);
			DataOutputStream dos = new DataOutputStream(socket.getOutputStream());
			dos.writeLong(file.length());
			dos.flush();
			// dos.flush();
			byte[] bytes = new byte[1024];
			int length = 0;
			while ((length = fis.read(bytes, 0, bytes.length)) != -1) {
				dos.write(bytes, 0, length);
				dos.flush();
			}
			fis.close();
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
	}
}
