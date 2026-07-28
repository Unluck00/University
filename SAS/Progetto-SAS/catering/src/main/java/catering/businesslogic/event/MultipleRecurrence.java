package catering.businesslogic.event;

import java.sql.Date;
import java.util.ArrayList;

public class MultipleRecurrence implements RecurrenceStrategy {
    private ArrayList<RecurringDate> dates;

    public MultipleRecurrence(ArrayList<RecurringDate> dates) {
        this.dates = dates;
    }

    @Override
    public ArrayList<RecurringDate> getDates() {
        return dates;
    }

    @Override
    public void updateDates(Date dateStart, Date dateEnd, ArrayList<RecurringDate> newDates) {
        // dateStart e dateEnd vengono ignorati —
        // le date ricorrenti sostituiscono la lista
        if (newDates != null && !newDates.isEmpty()) {
            this.dates = newDates;
        }
    }

    @Override
    public void saveDates(int eventId) {
        for (RecurringDate rd : dates) {
            rd.setEventId(eventId);
            rd.saveNewRecurringDate();
        }
    }

    @Override
    public void deleteDates(int eventId) {
        RecurringDate.deleteAllForEvent(eventId);
    }

    @Override
    public RecurrenceStrategy cloneStrategy() {
        // copia profonda dell'ArrayList
        ArrayList<RecurringDate> datesCopy = new ArrayList<>();
        for (RecurringDate rd : this.dates) {
            datesCopy.add(new RecurringDate(rd.getDateStart(), rd.getDateEnd()));
        }
        return new MultipleRecurrence(datesCopy);
    }
}