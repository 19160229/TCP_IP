package uspclient;
import java.net.*;
import java.util.Scanner;
import java.io.*;
public class UDPClient {

	public static void main(String[] args) 
	throws Exception
	{
		Scanner sc=new Scanner(System.in);
		DatagramSocket ds=new DatagramSocket();
		byte[] rBuf=new byte[5];
		DatagramPacket rdp=new DatagramPacket(rBuf, 5);
		
		String line=null;
		do{
			line=sc.nextLine();
			DatagramPacket sdp=new DatagramPacket(
					line.getBytes(),
					0,
					line.getBytes().length,					
					InetAddress.getByName("127.0.0.1"),
					9999
					);
			ds.send(sdp);
			ds.receive(rdp);
			String res=new String(rdp.getData(),rdp.getOffset(),
					rdp.getLength()
					);
			System.out.println("接收到的服务器方响应："+res
			);
		}while(!line.equalsIgnoreCase("quit"));
		ds.close();

	}

}
