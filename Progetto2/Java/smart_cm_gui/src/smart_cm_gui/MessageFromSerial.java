package smart_cm_gui;

import java.util.Arrays;
import java.util.Optional;

public enum MessageFromSerial {
	WELCOME {

		@Override
		public String getId() {
			return "w";
		}

		@Override
		public String getMessageToPrint() {
			return "WELCOME";
		}

	},
	MAKINGCOFFE {

		@Override
		public String getId() {
			return "m";
		}

		@Override
		public String getMessageToPrint() {
			return "Making Coffe...";
		}

	},
	COFFEREADY {

		@Override
		public String getId() {
			return "r";
		}

		@Override
		public String getMessageToPrint() {
			return "Coffe is Ready";
		}

	},
	NOMORECOFFE {

		@Override
		public String getId() {
			return "n";
		}

		@Override
		public String getMessageToPrint() {
			return "No more coffe MAINTENANCE";
		}
	},
	
	REFRESH{

		@Override
		public String getId() {
			return "t";
		}

		@Override
		public String getMessageToPrint() {
			return "";
		}
		
	};

	abstract public String getId();

	abstract public String getMessageToPrint();

	public static boolean isACorrectMessage(String c) {
		return Arrays.asList(MessageFromSerial.values()).stream().anyMatch(i -> i.getId().equals(c));
	}

	public static String getCorrectMessage(String c) {
		Optional<MessageFromSerial> m = Arrays.asList(MessageFromSerial.values()).stream()
				.filter(i -> i.getId().equals(c)).findFirst();
		if (m.isPresent()) {
			return m.get().getMessageToPrint();
		} else {
			return "";
		}
	}
}
