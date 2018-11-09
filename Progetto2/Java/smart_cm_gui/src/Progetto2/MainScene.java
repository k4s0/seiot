package Progetto2;

import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.StackPane;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.stage.Stage;

/**
 * This class is responsible for showing to the user the Main Menu. It extends
 * the current Scene.
 */
public final class MainScene extends Scene {

	private static final double WIDTH = 800;
	private static final double HEIGHT = 800;
	private static final double BUTTON_WIDTH = 750;
	private static final MainScene MAINSCENE = new MainScene();

	private static Stage mainStage;
	private final StackPane layout = new StackPane();
	final StackPane printingSpace = new StackPane();
	private final Button refillCoffe = new Button("REFILL COFFE");

	private MainScene() {
		super(new StackPane(), WIDTH, HEIGHT);
		layout.getChildren().addAll(printingSpace,refillCoffe);
		printingSpace.setId("Comunication");
		printingSpace.setAlignment(Pos.BOTTOM_CENTER);
		this.refillCoffe.setPrefWidth(BUTTON_WIDTH);
		this.refillCoffe.setOnAction(e -> {
			CoffeMachineViewImpl.getComunicator().coffeRefill();
		});
		this.setRoot(layout);

	}

	void printString(final String toPrint) {
		final Label text = new Label();

		text.setFont(Font.font(null, FontWeight.BOLD, 72));
		text.setVisible(true);
		text.setTextFill(Color.BLACK);
		text.setText(toPrint);
		printingSpace.getChildren().removeIf(o -> o instanceof Label);
		printingSpace.getChildren().add(text);
	}

	static MainScene get(final Stage mainWindow) {
		mainStage = mainWindow;
		mainStage.setFullScreen(false);
		mainStage.setWidth(WIDTH);
		mainStage.setHeight(HEIGHT);
		mainStage.centerOnScreen();
		CoffeMachineViewImpl.setMainScene(MAINSCENE);
		return MAINSCENE;
	}

}
