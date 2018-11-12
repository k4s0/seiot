package Progetto2;

import msg.jssc.SerialCommChannel;

public class CoffeMachineComunicator {

	private CoffeMachineView view;
	private SerialCommChannel serialChannel;
	private volatile boolean canRefillCoffe;

	public CoffeMachineComunicator(String port, int rate) throws Exception {
		this.serialChannel = new SerialCommChannel("COM9", rate);
	}

	void setView(CoffeMachineViewImpl view) throws Exception {
		this.view = view;
		view.startView();

	}

	public synchronized void coffeRefill() {
		if (this.canRefillCoffe) {
			this.canRefillCoffe = false;
			serialChannel.sendMsg("j");
			view.printMessage("");
			System.out.println("[CoffeMachineComunicator] coffe refilled");
		} else {
			System.out.println("[CoffeMachineComunicator] don't do nothing");
		}
	}

	public void startCommuncation() {
		Thread t = new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					System.out.println("Waiting Arduino for rebooting...");
					Thread.sleep(4000);
					System.out.println("Ready.");

					while (true) {
						if (serialChannel.isMsgAvailable()) {
							String msg = serialChannel.receiveMsg().trim();
							/*msg = MessageFromSerial.getCorrectMessage(msg);
							System.out.println(msg);
							if (!msg.equals("")) {
								view.printMessage(msg);
								if ((msg).equals(MessageFromSerial.NOMORECOFFE.getMessageToPrint())) {
									System.out.println(
											"[CoffeMachineComunicator] " + MessageFromSerial.getCorrectMessage(msg));
									canRefillCoffe = true;
								}
								if((msg).equals(MessageFromSerial.REFRESH.getMessageToPrint())) {
									view.printMessage("");
								}
							}*/
							if(msg.equals("n")) {
								view.printMessage(MessageFromSerial.getCorrectMessage(msg));
							} else if( msg.equals("w")) {

								view.printMessage(MessageFromSerial.getCorrectMessage(msg));
							}

						}
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
		t.start();
	}
}
