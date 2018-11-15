package smart_cm_gui;

public class Main {

	public static void main(String[] args) throws Exception {
		CoffeMachineComunicator cmc = new CoffeMachineComunicator("", 9600);
		cmc.startCommuncation();
		CoffeMachineViewImpl v = new CoffeMachineViewImpl(cmc);
		cmc.setView(v);
	}

}
