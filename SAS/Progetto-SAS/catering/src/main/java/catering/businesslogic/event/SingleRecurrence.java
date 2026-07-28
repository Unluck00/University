package catering.businesslogic.event;

import java.sql.Date;
import java.util.ArrayList;

public class SingleRecurrence implements RecurrenceStrategy {
    private Date dateStart;
    private Date dateEnd;

    public SingleRecurrence(Date dateStart, Date dateEnd) {
        this.dateStart = dateStart;
        this.dateEnd = dateEnd;
    }

    @Override
    public ArrayList<RecurringDate> getDates() {
        // un evento singolo ha solo una data
        ArrayList<RecurringDate> list = new ArrayList<>();
        list.add(new RecurringDate(dateStart, dateEnd));
        return list;
    }

    @Override
    public void updateDates(Date dateStart, Date dateEnd, ArrayList<RecurringDate> newDates) {
        // ignora newDates, aggiorna solo start e end
        this.dateStart = dateStart;
        this.dateEnd = dateEnd;
    }

    @Override
    public void saveDates(int eventId) {
        // niente da salvare — le date stanno già in Events
    }

    @Override
    public void deleteDates(int eventId) {
        // niente da eliminare
    }

    @Override
    public RecurrenceStrategy cloneStrategy() {
        return new SingleRecurrence(this.dateStart, this.dateEnd);
    }

    public Date getDateStart() { return dateStart; }
    public Date getDateEnd() { return dateEnd; }
}