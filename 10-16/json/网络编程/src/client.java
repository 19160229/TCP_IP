import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import net.sf.json.*;
public class client {
    public static void main(String args[])throws Exception {
        String host = "127.0.0.1";
        int port = 55533;
        Socket socket = new Socket(host,port);
        OutputStream outputStream = socket.getOutputStream();
        String message = "你好，wumi";
        socket.getOutputStream().write(message.getBytes("UTF-8"));

        socket.shutdownOutput();
        InputStream inputStream = socket.getInputStream();
        byte[] bytes = new byte[1024];
        int len;
        StringBuilder sb = new StringBuilder();
        while((len = inputStream.read(bytes))!=-1)
        {
            sb.append(new String(bytes,0,len,"UTF-8"));
        }
        String json = sb.toString();
        JSONObject obj = JSONObject.fromObject(json);
        System.out.println("get message from server: " + sb);
        inputStream.close();
        outputStream.close();
        socket.close();
    }
}
