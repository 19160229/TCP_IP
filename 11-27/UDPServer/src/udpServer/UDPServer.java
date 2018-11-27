package udpServer;

import java.net.*;
//import java.io.*;

public class UDPServer {

	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
		DatagramSocket ds = new DatagramSocket(9999);
		System.out.println("服务器在端口号9999等待连接...");
		byte[] rbuf = new byte[16 * 1024];
		DatagramPacket rdp = new DatagramPacket(rbuf, 16 * 1024);
		boolean again = true;
		while (again) {
			ds.receive(rdp);
			String recvString = new String(rbuf, rdp.getOffset(), rdp.getLength());
			System.out.println("客户端ip: " + rdp.getAddress() + " 端口号: " + rdp.getPort() + "数据: " + recvString);
			String res = "长度:" + recvString.length();
			DatagramPacket sdp = new DatagramPacket(res.getBytes(), res.getBytes().length, rdp.getAddress(),
					rdp.getPort());
			ds.send(sdp);
		}
		ds.close();
	}

}
