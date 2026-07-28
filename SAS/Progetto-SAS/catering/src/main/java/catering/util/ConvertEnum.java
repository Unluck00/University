package catering.util;

import catering.businesslogic.event.Event;
import catering.businesslogic.event.EventServiceState;
import catering.businesslogic.event.StaffRole;

public class ConvertEnum {

    public static int convertEventServiceState(EventServiceState state) {
        switch (state) {
            case DA_DETERMINARE:
                return 0;

            case APPROVATO:
                return 1;

            case IN_CORSO:
                return 2;

            case ANNULLATO:
                return 3;

            default:
                return 0;
        }
    }

    public static int convertEventType(Event.EventType type) {
        switch (type) {
            case MATRIMONIO:
                return 0;

            case PRIVATO:
                return 1;

            case AZIENDALE:
                return 2;

            default:
                return -1;
        }
    }
}
