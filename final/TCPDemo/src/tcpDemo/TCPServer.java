package tcpDemo;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.*;

public class TCPServer {

	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
		ServerSocket ss=new ServerSocket(9999);
		Socket cs=ss.accept();
		BufferedReader bReader=new BufferedReader(new InputStreamReader(cs.getInputStream()));
		PrintWriter pWriter=new PrintWriter(cs.getOutputStream());
		String line=null;
		while((line=bReader.readLine())!=null){
			pWriter.println("´®³¤£º"+line.length());
			pWriter.flush();
			if(line.equalsIgnoreCase("quit")){
				break;
			}
		}
		cs.close();
		ss.close();
		pWriter.close();
		bReader.close();
	}

}
