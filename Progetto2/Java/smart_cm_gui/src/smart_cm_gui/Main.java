package smart_cm_gui;

public class Main {

	public static void main(String[] args) throws Exception {
		CoffeMachineComunicator cmc = new CoffeMachineComunicator("COM9", 9600);
		cmc.startCommuncation();
		CoffeMachineViewImpl v = new CoffeMachineViewImpl(cmc);
		cmc.setView(v);
	}

}
