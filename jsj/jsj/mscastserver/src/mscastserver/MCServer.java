package mscastserver;
import java.net.*;
import java.io.*;

public class MCServer {

	public static void main(String[] args) 
	throws  Exception
	{
		MulticastSocket ms=
		new MulticastSocket(9999);
		ms.joinGroup(
		InetAddress.getByName("226.6.6.6"));
		boolean again=true;
		byte[] rBuf=new byte[64*1024];
		DatagramPacket rdp=
	new DatagramPacket(rBuf, 64*1024);
		while(again)
		{
			//����226.6.6.6 9999�Ͻ��հ�
			System.out.println(
		"����226.6.6.6�˿ڣ�9999�Ͻ��հ�");
			ms.receive(rdp);
			String rs=
	new String(
			rdp.getData(),
			rdp.getOffset(),
			rdp.getLength()			
			);
	System.out.println("���յ����ݣ�"+rs);
		}//while
      ms.leaveGroup(
    InetAddress.getByName("226.6.6.6"));
      ms.close();
	}

}
