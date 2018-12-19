package mcDemo;

import java.net.*;
import java.util.Scanner;

public class MCClient {

	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
		Scanner sc = new Scanner(System.in);
		String line = null;
		MulticastSocket ms = new MulticastSocket();
		do {
			line = sc.nextLine();
			DatagramPacket sdp = new DatagramPacket(line.getBytes(), 0, line.getBytes().length,
					InetAddress.getByName("226.6.6.6"), 9999);
			ms.send(sdp);
		} while (!"quit".equals(line));
		ms.close();
		sc.close();
	}

}
