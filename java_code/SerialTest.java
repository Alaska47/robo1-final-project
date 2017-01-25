import java.util.Scanner;

import arduino.*;

public class SerialTest {

	public static void main(String[] args) throws Exception{
		
		Scanner ob = new Scanner(System.in);
		Arduino arduino = new Arduino("COM13", 9600); //enter the port name here, and ensure that Arduino is connected, otherwise exception will be thrown.
		arduino.openConnection();
      Thread.sleep(5000);
      String mess = "1010f";
      System.out.println("sending: " + mess);
      arduino.serialWrite(mess, 1, 100);
      Thread t=new Thread() {
			public void run() {
				//the following line will keep this app alive for 1000 seconds,
				//waiting for events to occur and responding to them (printing incoming messages to console).
				try {Thread.sleep(1000000);} catch (InterruptedException ie) {}
			}
		};
		t.start();
	}

}