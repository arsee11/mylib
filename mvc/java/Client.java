// Client.java

import java.net.*;
import java.io.*;
import java.util.Base64;
import java.util.Vector;
import java.util.List;
import mylib.mvc.java.MVCReactor;
import mylib.mvc.java.MVCRequester;
import mylib.mvc.java.JPack;

class Client{
	final static int REMOTE_PORT = 11111;
	final static String REMOTE_IP = "115.29.178.221";
	//final static String REMOTE_IP = "192.168.10.100";
	
	public static void main(String[] args){
		try{
			Socket s = new Socket(REMOTE_IP, REMOTE_PORT);
			MVCReactor reactor = new MVCReactor();
			reactor.register(new MsgView());
			reactor.register(new ErrorView());
			reactor.listen(s);
			
			BufferedReader stdin = new BufferedReader( new InputStreamReader(System.in) );
			MVCRequester rqt = new MVCRequester(new JPack());
			System.out.print("id:");
			String id = stdin.readLine();
			System.out.print("pwd:");
			String pwd = stdin.readLine();
			rqt.setSource("errorview");
			rqt.setTarget("member");
			rqt.setAction("login");
			rqt.addParam("id", id);
			rqt.addParam("key", pwd);
			rqt.appnedParam();
			rqt.post(s);
			
			System.out.print("From:");
			String from = stdin.readLine();
			System.out.print("To:");
			String to = stdin.readLine();
			//System.out.print("Msg:");
			//String msg = stdin.readLine();
			rqt.setSource("errorview");
			rqt.setTarget("msg");
			rqt.setAction("tran_msg");
			rqt.addParam("from", from);
			rqt.addParam("to", to);
			String emsg = new String( Base64.getEncoder().encode(readFromFile()) );
			rqt.addParam("msg", emsg);
			rqt.appnedParam();
			rqt.post(s);
			
		}catch(IOException e){
			e.printStackTrace();
		}
	}
	
	static byte[] readFromFile(){
		try{
			FileReader fr = new FileReader("a.jpg");
			int b;
			List barr = new Vector();
			while( (b=fr.read())!=-1)
			{
				barr.add((byte)b);
			}
			
			byte[] bs = new byte[barr.size()];
			for( int i=0;i<barr.size(); i++)
				bs[i] = (byte)barr.get(i);
				
			return bs;
		}catch(IOException e){
			e.printStackTrace();
			return null;
		}
	}
}
