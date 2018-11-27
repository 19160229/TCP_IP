package msServer;

import java.net.*;

public class MsServer {

	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
		MulticastSocket mSocket = new MulticastSocket(9999);
		mSocket.joinGroup(InetAddress.getByName("226.6.6.6"));
		byte[] rbuf = new byte[64 * 1024];
		DatagramPacket rdp = new DatagramPacket(rbuf, 64 * 1024);
		boolean again = true;
		while (again) {
			System.out.println("服务器在组226.6.6.6 端口号9999 上接收消息");
			mSocket.receive(rdp);
			String rs = new String(rdp.getData(), rdp.getOffset(), rdp.getLength());
			System.out.println("接收的数据为：" + rs);
		} // while
		mSocket.leaveGroup(InetAddress.getByName("226.6.6.6"));
		mSocket.close();
	}

}
