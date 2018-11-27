package udpClient;

import java.net.*;
import java.util.Scanner;
//import java.io.*;

public class UDPClient {

	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
		DatagramSocket ds = new DatagramSocket();
		byte[] rbuf = new byte[16 * 1024];
		DatagramPacket rdp = new DatagramPacket(rbuf, 16 * 1024);
		Scanner sc = new Scanner(System.in);
		String line = null;
		do {
			line = sc.nextLine();
			DatagramPacket sdp = new DatagramPacket(line.getBytes(), line.getBytes().length,
					InetAddress.getByName("127.0.0.1"), 9999);
			ds.send(sdp);
			ds.receive(rdp);
			String res = new String(rbuf, rdp.getOffset(), rdp.getLength());
			System.out.println("·þÎñÆ÷Ó¦´ð£º" + res);
		} while (!line.equalsIgnoreCase("quit"));
		sc.close();
		ds.close();
	}

}
