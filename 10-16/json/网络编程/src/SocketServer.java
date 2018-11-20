import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import net.sf.json.*;
public class SocketServer {
    public static void main(String[] args)throws Exception{
        int port=55533;
        TreeNode root = new TreeNode("0", "0", "000000", "00JJJ");
        root.add(new TreeNode("1", "0", "111111", "11AAA"));
        root.add(new TreeNode("2", "0", "222222", "11BBB"));
        root.add(new TreeNode("3", "2", "333333", "11CCC"));
        JSONArray obj = JSONArray.fromObject(root);
        String jsonString ="";
        jsonString = obj.toString();
        System.out.println(jsonString);
        byte[] jsonByte = jsonString.getBytes();

        ServerSocket server = new ServerSocket(port);
        System.out.println("Server将一直等待连接的到来");
        Socket socket = server.accept();
        InputStream inputStream = socket.getInputStream();
        byte[] bytes = new byte[1024];
        int len;
        StringBuilder sb = new StringBuilder();
        while((len=inputStream.read(bytes))!=-1)
        {
            sb.append(new String(bytes,0,len,"UTF-8"));
        }
        System.out.println("get message from client:"+sb);
        OutputStream outputStream = socket.getOutputStream();
        outputStream.write(jsonByte);
        outputStream.flush();

        outputStream.close();
        inputStream.close();
        socket.close();
        server.close();
    }
}
