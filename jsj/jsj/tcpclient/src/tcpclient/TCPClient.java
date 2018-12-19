package tcpclient;
import java.net.*;
import java.io.*;
public class TCPClient {

	public static void client()
	throws Exception
	{
		System.out.println("向服务器发起TCP连接...");
		Socket cs=new Socket(
		InetAddress.getByName("127.0.0.1"),
		9999
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
		BufferedReader kbr=
		new BufferedReader(
		new InputStreamReader(System.in)
		
				);
		do{
			line=kbr.readLine();
			pw.println(line);
			pw.flush();
			String res=br.readLine();
			System.out.println("服务器返回结果："+
			res
					);
		}while(!"quit".equalsIgnoreCase(line));
		System.out.println("再见！ ");
		pw.close();
		br.close();
		cs.close();
			
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		try {
			client();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
