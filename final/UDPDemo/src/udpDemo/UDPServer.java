package udpDemo;

import java.net.*;

public class UDPServer {

	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
		DatagramSocket ds = new DatagramSocket(9999);
		byte[] rBuf = new byte[64 * 1024];
		DatagramPacket rdp = new DatagramPacket(rBuf, 64*1024);
		boolean again = true;
		while (again) {
			ds.receive(rdp);
			String s = new String(rdp.getData(), rdp.getOffset(), rdp.getLength());
			System.out.println("�ͻ���IP:" + rdp.getAddress() + " �˿ں�:" + rdp.getPort() + " ��������:" + s);
			String res = "����" + s.length();
			DatagramPacket sdp = new DatagramPacket(res.getBytes(), res.getBytes().length, rdp.getAddress(),
					rdp.getPort());
			ds.send(sdp);
		}
		ds.close();
	}

}
