package mcDemo;

import java.net.*;

public class MCServer {

	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
		MulticastSocket ms = new MulticastSocket(9999);
		ms.joinGroup(InetAddress.getByName("226.6.6.6"));
		byte[] rBuf = new byte[64 * 1024];
		DatagramPacket rdp = new DatagramPacket(rBuf, rBuf.length);
		boolean again = true;
		while (again) {
			ms.receive(rdp);
			String s = new String(rdp.getData(), rdp.getOffset(), rdp.getLength());
			System.out.println("�����н�����Ϣ:" + s);
		}
		ms.leaveGroup(InetAddress.getByName("226.6.6.6"));
		ms.close();
	}

}
