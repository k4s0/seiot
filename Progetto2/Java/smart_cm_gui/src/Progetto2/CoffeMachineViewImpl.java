package Progetto2;

import javafx.application.Application;
import javafx.application.Platform;

public class CoffeMachineViewImpl implements CoffeMachineView {

    private static CoffeMachineComunicator CMC;
    private static MainScene mainScene;

    
    public CoffeMachineViewImpl(final CoffeMachineComunicator cmc) {
        this.setComunicator(cmc);
    }

    
    private synchronized void setComunicator(final CoffeMachineComunicator cmc) {
    	System.out.println("[CoffeMachineViewImpl] i have the CMC");
        CoffeMachineViewImpl.CMC = cmc;
    }

    @Override
    public void startView() {
        Application.launch(MainWindow.class);
    }

    static void setMainScene(final MainScene scene) {
    	System.out.println("[CoffeMachineViewImpl] i have the MainScene");
        CoffeMachineViewImpl.mainScene = scene;
    }

    static CoffeMachineComunicator getComunicator() {
        return CoffeMachineViewImpl.CMC;
    }


	@Override
	public void printMessage(String toPrint) {
		Platform.runLater(()->mainScene.printString(toPrint));
		
	}


	@Override
	public void enableCoffeRefill() {
		// TODO Auto-generated method stub
		
	}


	@Override
	public void disableCoffeRefill() {
		// TODO Auto-generated method stub
		
	}
}
