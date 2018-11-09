package Progetto2;

import java.util.Arrays;

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
	MACKINGCOFFE {

		@Override
		public String getId() {
			return "m";
		}

		@Override
		public String getMessageToPrint() {
			return "Macking Coffe...";
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
			return "No more coffe MANTEINANCE";
		}
	};

	abstract public String getId();

	abstract public String getMessageToPrint();

	public static boolean isACorrectMessage(String c) {
		return Arrays.asList(MessageFromSerial.values()).stream().anyMatch(i -> i.getId().equals(c));
	}

	public static String getCorrectMessage(String c) {
		return Arrays.asList(MessageFromSerial.values()).stream().filter(i -> i.getId().equals(c)).findFirst()
				.get().getMessageToPrint();
	}
}
