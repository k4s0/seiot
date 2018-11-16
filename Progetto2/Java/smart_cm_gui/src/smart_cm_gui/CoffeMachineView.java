package smart_cm_gui;
/**
 * 
 * Coffe machine view interface.
 *
 */
public interface CoffeMachineView {
	
	/**
	 * Print String message.
	 * @param toPrint string to print
	 */
	void printMessage(String toPrint);
	
	/**
	 * Print sugar level.
	 * @param sugar sugar level
	 */
	void printSugarLevel(int sugar);
	
	/**
	 * Delete the sugar level message from view 
	 */
	void refreshSugar();
	
	/**
	 * Delete text messages from view 
	 */

	void refreshMessage();
	
    /**
     * Start view. 
     */
    void startView();
}
