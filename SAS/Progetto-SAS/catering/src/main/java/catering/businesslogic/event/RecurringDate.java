package catering.businesslogic.event;

import catering.persistence.PersistenceManager;
import catering.persistence.ResultHandler;
import java.sql.Date;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;

public class RecurringDate {
    private int id;
    private Date dateStart;
    private Date dateEnd;
    private int eventId;

    public RecurringDate(Date dateStart, Date dateEnd) {
        this.dateStart = dateStart;
        this.dateEnd = dateEnd;
    }

    public Date getDateStart() { return dateStart; }
    public Date getDateEnd() { return dateEnd; }
    public int getEventId() { return eventId; }
    public void setEventId(int eventId) { this.eventId = eventId; }

    public void saveNewRecurringDate() {
        String query = "INSERT INTO RecurringDate (date_start, date_end, event_id) VALUES (?, ?, ?)";
        PersistenceManager.executeUpdate(query,
                this.dateStart,
                this.dateEnd,
                this.eventId);
        this.id = PersistenceManager.getLastId();
    }

    public void updateRecurringDate() {
        String query = "UPDATE RecurringDate SET date_start = ?, date_end = ? WHERE id = ?";
        PersistenceManager.executeUpdate(query,
                this.dateStart,
                this.dateEnd,
                this.id);
    }

    public void deleteRecurringDate() {
        String query = "DELETE FROM RecurringDate WHERE id = ?";
        PersistenceManager.executeUpdate(query, this.id);
    }

    public static ArrayList<RecurringDate> loadForEvent(int eventId) {
        ArrayList<RecurringDate> list = new ArrayList<>();
        String query = "SELECT * FROM RecurringDate WHERE event_id = ? ORDER BY date_start";
        PersistenceManager.executeQuery(query, new ResultHandler() {
            @Override
            public void handle(ResultSet rs) throws SQLException {
                Date start = Date.valueOf(rs.getString("date_start"));
                Date end = Date.valueOf(rs.getString("date_end"));
                RecurringDate rd = new RecurringDate(start, end);
                rd.id = rs.getInt("id");
                rd.eventId = rs.getInt("event_id");
                list.add(rd);
            }
        }, eventId);
        return list;
    }

    public static void deleteAllForEvent(int eventId) {
        String query = "DELETE FROM RecurringDate WHERE event_id = ?";
        PersistenceManager.executeUpdate(query, eventId);
    }
}