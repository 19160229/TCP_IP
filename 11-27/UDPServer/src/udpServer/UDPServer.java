package udpServer;

import java.net.*;
//import java.io.*;

public class UDPServer {

	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
		DatagramSocket ds = new DatagramSocket(9999);
		System.out.println("�������ڶ˿ں�9999�ȴ�����...");
		byte[] rbuf = new byte[16 * 1024];
		DatagramPacket rdp = new DatagramPacket(rbuf, 16 * 1024);
		boolean again = true;
		while (again) {
			ds.receive(rdp);
			String recvString = new String(rbuf, rdp.getOffset(), rdp.getLength());
			System.out.println("�ͻ���ip: " + rdp.getAddress() + " �˿ں�: " + rdp.getPort() + "����: " + recvString);
			String res = "����:" + recvString.length();
			DatagramPacket sdp = new DatagramPacket(res.getBytes(), res.getBytes().length, rdp.getAddress(),
					rdp.getPort());
			ds.send(sdp);
		}
		ds.close();
	}

}
