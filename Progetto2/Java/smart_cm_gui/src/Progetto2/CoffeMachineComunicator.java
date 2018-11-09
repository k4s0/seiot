package Progetto2;

import msg.jssc.SerialCommChannel;

public class CoffeMachineComunicator {

	private CoffeMachineView view;
	private SerialCommChannel serialChannel;
	private volatile boolean canRefillCoffe;

	public CoffeMachineComunicator(String port, int rate) throws Exception {
		this.serialChannel = new SerialCommChannel("COM6", rate);
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
							view.printMessage(MessageFromSerial.getCorrectMessage(msg));
							if ((msg).equals(MessageFromSerial.NOMORECOFFE.getId())) {
								System.out.println(
										"[CoffeMachineComunicator] " + MessageFromSerial.getCorrectMessage(msg));
								canRefillCoffe = true;
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