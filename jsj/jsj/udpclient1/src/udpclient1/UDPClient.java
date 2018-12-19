package udpclient1;
import java.net.*;
import java.util.Scanner;
import java.io.*;

public class UDPClient {

	public static void main(String[] args) 
	throws Exception
	{
		DatagramSocket ds=
	new DatagramSocket();
Scanner sc=
new Scanner(System.in);
byte[] rBuf=new  byte[64*1024];
DatagramPacket rdp=
new DatagramPacket(rBuf,64*1024);
String line=null;
do{
	line=sc.nextLine();
DatagramPacket sdp=
new DatagramPacket(
		line.getBytes(), 
		line.getBytes().length, 
		InetAddress
		//.getByName("127.0.0.1"), 
		.getByName("226.6.6.6"), 
		9999);
ds.send(sdp);
/*
ds.receive(rdp);
String res=new String(
rdp.getData(),
rdp.getOffset(),
rdp.getLength()
		);
System.out.println
("·þÎñÆ÷ÏìÓ¦£º"+
		res);

	*/
}while(!line
		.equalsIgnoreCase("quit"));
ds.close();


	}

}
