package smart_cm_gui;

public interface CoffeMachineView {
	void printMessage(String toPrint);
	void printSugarLevel(int sugar);
	void refreshSugar();
	void refreshMessage();
    void startView();
}