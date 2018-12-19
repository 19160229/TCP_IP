package mcclient;
import java.net.*;
import java.util.Scanner;
import java.io.*;

public class MCClient {

	public static void main(String[] args)
	throws Exception
	{
		Scanner sc=new Scanner(System.in);
		String line=null;
		MulticastSocket ms=new MulticastSocket();
		do{
			line=sc.nextLine();
			System.out.println("向级中发送："+line);
			DatagramPacket sdp=new DatagramPacket(
					line.getBytes(),
					0,
					line.getBytes().length,
					InetAddress.getByName("226.6.6.6"),
					9999
					);
			ms.send(sdp);
		}while(!line.equalsIgnoreCase("quit"));
ms.close();
	}

}
