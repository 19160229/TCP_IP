package client;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.*;

public class UDPClient {

	public static void client() throws Exception{
		BufferedReader kbr=new BufferedReader(new InputStreamReader(System.in));
		DatagramSocket ds= new DatagramSocket();
		byte[] sendbuf=new byte[1024];
		byte[] recvbuf=new byte[1024];
		String send;
		String recv;
		do{
			send=kbr.readLine();
			sendbuf=send.getBytes();
			DatagramPacket senddp=new DatagramPacket(sendbuf,sendbuf.length,InetAddress.getByName("127.0.0.1"),9999);
			System.out.println("向服务器:127.0.0.1端口9999发送消息:"+send);
			ds.send(senddp);
			
			DatagramPacket recvdp=new DatagramPacket(recvbuf,recvbuf.length);
			ds.receive(recvdp);
			recv=new String(recvdp.getData(),0,recvdp.getLength());
			System.out.println("服务器响应:"+recv);
		}while(!send.equalsIgnoreCase("quit"));
		System.out.println("再见!");
		ds.close();
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
