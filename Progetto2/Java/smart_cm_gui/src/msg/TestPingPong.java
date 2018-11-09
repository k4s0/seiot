package msg;


import msg.*;
//import seiot.modulo_lab_2_2.msg.rxtx.SerialCommChannel;
import msg.jssc.*;

/**
 * Testing simple message passing.
 * 
 * To be used with an Arduino connected via Serial Port
 * running modulo-lab-2.2/pingpong.ino
 * 
 * @author aricci
 *
 */
public class TestPingPong {

	public static void main(String[] args) throws Exception {
		SerialCommChannel channel = new SerialCommChannel("COM6",9600);	
		
		/* attesa necessaria per fare in modo che Arduino completi il reboot */
		System.out.println("Waiting Arduino for rebooting...");		
		Thread.sleep(4000);
		System.out.println("Ready.");		

		
		while (true){
			System.out.println("Sending ping");
			channel.sendMsg("ping");
			if(channel.isMsgAvailable()) {
				String msg = channel.receiveMsg();
				System.out.println("Received: "+msg);
			}
			Thread.sleep(800);
		}
	}

}
