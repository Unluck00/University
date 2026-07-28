package catering.businesslogic.event;

import java.sql.Date;
import java.util.ArrayList;

public interface RecurrenceStrategy {

    ArrayList<RecurringDate> getDates();

    void updateDates(Date dateStart, Date dateEnd, ArrayList<RecurringDate> newDates);

    void saveDates(int eventId);

    void deleteDates(int eventId);

    RecurrenceStrategy cloneStrategy();

}