package udpserver;
import java.net.*;
import java.io.*;

public class UDPServer {

	public static void main(String[] args)
	throws Exception
	{
		DatagramSocket ds=
	new DatagramSocket(9999);
byte[] rBuf=new  byte[64*1024];
DatagramPacket rdp=
new DatagramPacket(rBuf,64*1024);
boolean again=true;
while(again)
{
System.out.println
("UDP��������9999�ȴ�UDP��...");
ds.receive(rdp);
String s=new String(
rdp.getData(),
rdp.getOffset(),
rdp.getLength()
		);
System.out.println("�ͻ�����IP�� "+
rdp.getAddress()+" �˿ںţ�"+
rdp.getPort()+" �������ݣ�"+s
		);
String res="����:"+s.length();
DatagramPacket sdp=
new DatagramPacket(
		res.getBytes(),
		res.getBytes().length,
		rdp.getAddress(),
		rdp.getPort()
		);
ds.send(sdp);

}//while
ds.close();
	}

}
