package smart_cm_gui;

import javafx.application.Application;
import javafx.application.Platform;

public class CoffeMachineViewImpl implements CoffeMachineView {

	private static CoffeMachineComunicator CMC;
	private static MainScene mainScene;

	public CoffeMachineViewImpl(final CoffeMachineComunicator cmc) {
		this.setComunicator(cmc);
	}

	private synchronized void setComunicator(final CoffeMachineComunicator cmc) {
		CoffeMachineViewImpl.CMC = cmc;
	}

	@Override
	public void startView() {
		Application.launch(MainWindow.class);
	}

	static void setMainScene(final MainScene scene) {
		CoffeMachineViewImpl.mainScene = scene;
	}

	static CoffeMachineComunicator getComunicator() {
		return CoffeMachineViewImpl.CMC;
	}

	@Override
	public void printMessage(String toPrint) {
		Platform.runLater(() -> mainScene.printString(toPrint));

	}

	@Override
	public void printSugarLevel(int sugar) {
		Platform.runLater(() -> mainScene.printSugar(sugar));

	}

	@Override
	public void refreshSugar() {
		Platform.runLater(() -> mainScene.refreshSugar());
		
	}

	@Override
	public void refreshMessage() {
		Platform.runLater(() -> mainScene.refreshMessage());
		
	}
}
