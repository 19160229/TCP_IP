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
		System.out.println("UDP�������ڶ˿�9999�ȴ�����...");
		
		byte[] recvbuf=new byte[1024];
		byte[] sendbuf=new byte[1024];
		String recv;
		DatagramPacket recvdp=new DatagramPacket(recvbuf,recvbuf.length);
		do{
			ds.receive(recvdp);
			System.out.println("�ͻ���(IP:"+recvdp.getAddress()+")�˿�"+recvdp.getPort()+"������...");
		
			recv=new String(recvdp.getData(),0,recvdp.getLength());
			System.out.println("�ͻ�������:"+recv);
		
			int num=count(recv);
			sendbuf=("��ĸ����:"+num).getBytes();
			DatagramPacket senddp=new DatagramPacket(sendbuf,sendbuf.length,recvdp.getAddress(),recvdp.getPort());
			ds.send(senddp);
		}while(!recv.equalsIgnoreCase("quit"));
		System.out.println("�ټ�!");
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
