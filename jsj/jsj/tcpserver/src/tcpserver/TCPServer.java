package tcpserver;
import java.net.*;
import java.io.*;

public class TCPServer {

	public static void server()
	throws Exception
	{
		System.out.println(
				"服务器在9999上等待连接...");
		
		ServerSocket ss=new ServerSocket(9999);
		Socket cs=ss.accept();
		System.out.println("客户机IP:"+
		cs.getInetAddress()+
		" port:"+cs.getPort()+
		"已连接进来..."
				);
		BufferedReader br=new BufferedReader(
			new InputStreamReader(
			cs.getInputStream()	
					)	
				);
		PrintWriter pw=new PrintWriter(
		cs.getOutputStream()
				);
		String line=null;
		while((line=br.readLine())!=null)
		{
			pw.println("串长："+line.length());
			pw.flush();
			if("quit".equalsIgnoreCase(line))
			{
				break;
			}
		}//while
		System.out.println("再见！");
		pw.close();
		br.close();
		cs.close();
		ss.close();
		
	}
	
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		try {
			server();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
