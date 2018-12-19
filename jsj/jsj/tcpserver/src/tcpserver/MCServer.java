package tcpserver;
import java.net.*;
import java.io.*;

public class MCServer {

	public static void main(String[] args)
	throws Exception
	{
		MulticastSocket ms=new MulticastSocket(9999);
		ms.joinGroup(InetAddress.getByName("226.6.6.6"));
		byte[] rBuf=new byte[64*1024];
		DatagramPacket rdp=new DatagramPacket(rBuf, 64*1024);
		
		boolean again=true;
		while(again)
		{
			ms.receive(rdp);
			String s=new String(rdp.getData(),
					rdp.getOffset(),
					rdp.getLength()
					);
			System.out.println("从组中接收的消息："+s);
		}
ms.leaveGroup(InetAddress.getByName("226.6.6.6"));
ms.close();
	}

}
