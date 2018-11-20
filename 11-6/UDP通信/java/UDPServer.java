package server;

import java.net.*;

public class UDPServer {

	static int count(String s)
	{
		int num = 0;
		for (int i = 0; i<s.length(); i++)
			if (Character.isAlphabetic(s.charAt(i)))
				num++;
		return num;
	}
	
	public static void server() throws Exception{
		DatagramSocket ds =new DatagramSocket(9999);
		System.out.println("UDP服务器在端口9999等待连接...");
		
		byte[] recvbuf=new byte[1024];
		byte[] sendbuf=new byte[1024];
		String recv;
		DatagramPacket recvdp=new DatagramPacket(recvbuf,recvbuf.length);
		do{
			ds.receive(recvdp);
			System.out.println("客户机(IP:"+recvdp.getAddress()+")端口"+recvdp.getPort()+"已连接...");
		
			recv=new String(recvdp.getData(),0,recvdp.getLength());
			System.out.println("客户机数据:"+recv);
		
			int num=count(recv);
			sendbuf=("字母个数:"+num).getBytes();
			DatagramPacket senddp=new DatagramPacket(sendbuf,sendbuf.length,recvdp.getAddress(),recvdp.getPort());
			ds.send(senddp);
		}while(!recv.equalsIgnoreCase("quit"));
		System.out.println("再见!");
		ds.close();
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
