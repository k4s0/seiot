package smart_cm_gui;

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
							if (MessageFromSerial.isACorrectMessage(msg)) {
								view.printMessage(MessageFromSerial.getCorrectMessage(msg));
								if (msg.equals(MessageFromSerial.NOMORECOFFE.getId())) {
									view.refreshSugar();
									canRefillCoffe = true;
								} else if (msg.equals(MessageFromSerial.REFRESH.getId())
										|| msg.equals(MessageFromSerial.MAKINGCOFFE.getId())) {
									view.refreshSugar();
								}
							} else {
								try {
									view.printSugarLevel(Integer.parseInt(msg));
								} catch (Exception e) {
									System.out.println(msg);
								}
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
