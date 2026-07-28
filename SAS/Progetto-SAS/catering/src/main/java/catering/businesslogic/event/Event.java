package catering.businesslogic.event;

import java.sql.Date;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Time;
import java.util.ArrayList;
import java.util.stream.Stream;

import catering.businesslogic.user.User;
import catering.persistence.PersistenceManager;
import catering.persistence.ResultHandler;
import catering.util.ConvertEnum;

/**
 * Represents an event in the catering system.
 */
public class Event {

    public static enum EventType {
        MATRIMONIO,
        PRIVATO,
        AZIENDALE
    }

    private int id;
    private String name;
    private Date dateStart;
    private Date dateEnd;
    private User chef;
    private User manager;
    private ArrayList<Service> services;
    private String location;
    private Time startTime;
    private Time endTime;
    private int participants;
    private boolean fine;
    private String personnelAllocation;
    private String chefAdvisory;
    private EventServiceState state;
    private EventType type;
    private RecurrenceStrategy recurrence;


    public Event() {
        services = new ArrayList<>();
        fine = false;
        state = EventServiceState.DA_DETERMINARE;
    }

    public Event(String name) {
        this();
        this.name = name;
    }

    // Basic getters and setters
    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Date getDateStart() {
        return dateStart;
    }

    public void setDateStart(Date dateStart) {
        this.dateStart = dateStart;
    }

    public Date getDateEnd() {
        return dateEnd;
    }

    public void setDateEnd(Date dateEnd) {
        this.dateEnd = dateEnd;
    }

    public User getChef() {
        return chef;
    }

    public int getChefId() {
        return chef != null ? chef.getId() : 0;
    }

    public void setChef(User chef) {
        this.chef = chef;
    }

    public void setChefId(int chefId) {
        this.chef = User.load(chefId);
    }

    public ArrayList<Service> getServices() {
        return services;
    }

    public User getManager() {
        return manager;
    }

    public int getManagerId() {
        return manager != null ? manager.getId() : 0;
    }

    public void setManager(User manager) {
        this.manager = manager;
    }

    public void setManagerId(int managerId) {
        this.manager = User.load(managerId);
    }

    public String getLocation() {
        return location;
    }

    public void setLocation(String location) {
        this.location = location;
    }

    public Time getStartTime() {
        return startTime;
    }

    public void setStartTime(Time startTime) {
        this.startTime = startTime;
    }

    public Time getEndTime() {
        return endTime;
    }

    public void setEndTime(Time endTime) {
        this.endTime = endTime;
    }

    public int getParticipants() {
        return participants;
    }

    public void setParticipants(int participants) {
        this.participants = participants;
    }

    public boolean isFine() {
        return fine;
    }

    public void setFine(boolean fine) {
        this.fine = fine;
    }

    public String getPersonnelAllocation() {
        return personnelAllocation;
    }

    public void setPersonnelAllocation(String personnelAllocation) {
        this.personnelAllocation = personnelAllocation;
    }

    public String getChefAdvisory() {
        return chefAdvisory;
    }

    public void setChefAdvisory(String chefAdvisory) {
        this.chefAdvisory = chefAdvisory;
    }

    public EventServiceState getState() {
        return state;
    }

    public void setState(EventServiceState state) {
        this.state = state;
    }

    public EventType getType() {
        return type;
    }

    public void setType(EventType type) {
        this.type = type;
    }

    public ArrayList<RecurringDate> getDates() {
        return recurrence.getDates();
    }

    public RecurrenceStrategy getRecurrence() {
        return recurrence;
    }

    public void setRecurrence(RecurrenceStrategy recurrence) {
        this.recurrence = recurrence;
    }

    public void setServices(ArrayList<Service> services) {
        this.services = services;
    }

    public static Event createEvent(int managerId, String name, String location, Date dateStart, Date dateEnd, Time startTime, Time endTime, int participants, ArrayList<RecurringDate> recurrence, EventType type) {
        Event event = new Event();
        event.setName(name);
        event.setManagerId(managerId);
        event.setLocation(location);
        event.setDateStart(dateStart);
        event.setDateEnd(dateEnd);
        event.setStartTime(startTime);
        event.setEndTime(endTime);
        event.setParticipants(participants);
        if (recurrence == null || recurrence.isEmpty()) {
            event.setRecurrence(new SingleRecurrence(dateStart, dateEnd));
        } else {
            event.setRecurrence(new MultipleRecurrence(recurrence));
        }
        event.setType(type);
        return event;
    }

    public static Event copyEvent(Event original) {
        Event copy = new Event();
        copy.setName(original.getName());
        copy.setLocation(original.getLocation());
        copy.setDateStart(original.getDateStart());
        copy.setDateEnd(original.getDateEnd());
        copy.setStartTime(original.getStartTime());
        copy.setEndTime(original.getEndTime());
        copy.setRecurrence(original.getRecurrence().cloneStrategy());
        copy.setParticipants(original.getParticipants());
        copy.setType(original.getType());
        return copy;
    }

    public Event modifyEvent(Event original, String name, String location, Date dateStart, Date dateEnd,
                            Time startTime, Time endTime, ArrayList<RecurringDate> recurrence, int participants, EventType type) {
        Event event = original;
        event.setName(name);
        event.setLocation(location);
        event.setDateStart(dateStart);
        event.setDateEnd(dateEnd);
        event.setStartTime(startTime);
        event.setEndTime(endTime);
        if (recurrence == null || recurrence.isEmpty()) {
            event.setRecurrence(new SingleRecurrence(dateStart, dateEnd));
        } else {
            event.setRecurrence(new MultipleRecurrence(recurrence));
        }
        event.setParticipants(participants);
        event.setType(type);
        return event;
     }

     public Event removeEvent(Event event) {
        if(getState() == EventServiceState.APPROVATO) {
            setFine(true);    
        }
        setState(EventServiceState.ANNULLATO);
        return event;
     }

     public Event updateDates(Date dateStart, Date dateEnd, ArrayList<RecurringDate> newDates) {
        setDateStart(dateStart);
        
        if (dateEnd != null) {
            setDateEnd(dateEnd);
        } 

        if (newDates == null || newDates.isEmpty()) {
            setRecurrence(new SingleRecurrence(dateStart, dateEnd));
         } else {
            setRecurrence(new MultipleRecurrence(newDates));
         }

         if(getState() == EventServiceState.APPROVATO) {
            setFine(true);
        }

        return this;
     }

     public Event updatePartecipants(int participants) {
        this.setParticipants(participants);

        if(getState() == EventServiceState.APPROVATO || ( (participants >= getParticipants() - ((30 * getParticipants()) / 100)) || (participants <= getParticipants() + ((30 * getParticipants()) / 100)) ) ) {
            setFine(true);
        }

        return this;
     }

    // Event management
    public void updateChef() {
        String query = "UPDATE Events SET chef_id = ? WHERE id = ?";
        PersistenceManager.executeUpdate(query, getChefId(), id);
    }

    public void removeChef(){
        String query = "UPDATE Events SET chef_id = 0 where id = ?";
        PersistenceManager.executeUpdate(query, this.getId());
    }

    public void updateAnnotationEvent(Event event) {
        String query = "UPDATE Events SET personnel_allocation = ?, " +
                "chef_advisory = ? WHERE id = ?";

        PersistenceManager.executeUpdate(query, this.getPersonnelAllocation()
                , this.getChefAdvisory(), this.getId());
    }

    public void updateRecurrence(int event_id) {
        this.getRecurrence().saveDates(event_id);
    }

    // Service management
    public void addService(Service service) {
        if (services == null) {
            services = new ArrayList<>();
        }
        services.add(service);
    }

    public void removeService(Service service) {
        if (services != null) {
            services.remove(service);
        }

        if(getState() == EventServiceState.APPROVATO) {
            setFine(true);    
        }

        setState(EventServiceState.ANNULLATO);
    }

    public boolean containsService(Service service) {
        if (services != null) {
            return services.contains(service);
        }
        return false;
    }

    // Database operations
    public void saveNewEvent() {

        String query = "INSERT INTO Events (name, date_start, date_end, " +
                "chef_id, manager_id, location, time_start, time_end, " +
                "participants, fine, " +
                "state, type) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

        //Date startDate = dateStart;
        //Date endDate = dateEnd;

        PersistenceManager.executeUpdate(query, getName(), getDateStart(),
                getDateEnd(), getChefId(), getManagerId(), getLocation(),
                getStartTime(), getEndTime(), getParticipants(), isFine(),
                ConvertEnum.convertEventServiceState(getState()),
                ConvertEnum.convertEventType(getType()));


        // Get the ID of the newly inserted event
        id = PersistenceManager.getLastId();
        recurrence.saveDates(id);
    }

    public void updateEvent() {
        String query = "UPDATE Events SET name = ?, date_start = ?, date_end = ?, " +
                "manager_id = ?, location = ?, time_start = ?, " +
                "time_end = ?, participants = ?, fine = ?, state = ?, type = ? WHERE id = ?";

        //Long startTimestamp = (dateStart != null) ? dateStart.getTime() : null;
        //Long endTimestamp = (dateEnd != null) ? dateEnd.getTime() : null;

        PersistenceManager.executeUpdate(query, name, dateStart,
                dateEnd, getManagerId(), location,
                startTime, endTime, participants, fine, ConvertEnum.convertEventServiceState(state),
                ConvertEnum.convertEventType(type), id);
    }

    public boolean deleteEvent() {
        // Delete all services first
        for (Service service : services) {
            service.deleteService();
        }
        services.clear();

        // Delete the event
        String query = "DELETE FROM Events WHERE id = ?";
        boolean success = PersistenceManager.executeUpdate(query, id) > 0;

        if (success) {
            //handle delete services and other dependencies
        }

        return success;
    }

    public void updateEventApproved(){
        String query = "UPDATE Events SET state = ? WHERE id = ?";
        PersistenceManager.executeUpdate(query,
                ConvertEnum.convertEventServiceState(this.getState()), this.getId());
    }

    // Static load methods
    public static ArrayList<Event> loadAllEvents() {
        ArrayList<Event> events = new ArrayList<>();
        String query = "SELECT * FROM Events ORDER BY date_start DESC";

        PersistenceManager.executeQuery(query, new ResultHandler() {
            @Override
            public void handle(ResultSet rs) throws SQLException {
                Event e = new Event();
                e.id = rs.getInt("id");
                e.name = rs.getString("name");
                e.dateStart = parseDate(rs, "date_start");
                e.dateEnd = parseDate(rs, "date_end");
                e.chef = User.load(rs.getInt("chef_id"));
                e.manager = User.load(rs.getInt("manager_id"));
                e.location = rs.getString("location");
                e.startTime = parseTime(rs, "time_start");
                e.endTime = parseTime(rs, "time_end");
                e.participants = rs.getInt("participants");
                e.fine = rs.getBoolean("fine");
                e.personnelAllocation = rs.getString("personnel_allocation");
                e.chefAdvisory = rs.getString("chef_advisory");

                int state = rs.getInt("state");
                switch (state) {
                    case 0:
                        e.state = EventServiceState.DA_DETERMINARE;
                        break;

                    case 1:
                        e.state = EventServiceState.APPROVATO;
                        break;

                    case 2:
                        e.state = EventServiceState.IN_CORSO;
                        break;

                    case 3:
                        e.state = EventServiceState.ANNULLATO;
                        break;
                }

                int type = rs.getInt("type");
                switch (type) {
                    case 0:
                        e.type = EventType.MATRIMONIO;
                        break;

                    case 1:
                        e.type = EventType.PRIVATO;
                        break;

                    case 2:
                        e.type = EventType.AZIENDALE;
                        break;
                }

                events.add(e);
            }
        });

        // Load services and recurrence for each event
        ArrayList<RecurringDate> dates;
        for (Event e : events) {
            e.services = Service.loadServicesForEvent(e.id);
            dates = RecurringDate.loadForEvent(e.getId());
            if (dates.isEmpty()) {
                e.recurrence = new SingleRecurrence(e.getDateStart(),
                        e.getDateEnd());
            } else {
                e.recurrence = new MultipleRecurrence(dates);
            }
        }



        return events;
    }

    public static Event loadById(int id) {
        String query = "SELECT * FROM Events WHERE id = ?";
        return loadEventByQuery(query, id);
    }

    public static Event loadByName(String name) {
        String query = "SELECT * FROM Events WHERE name = ?";
        return loadEventByQuery(query, name);
    }

    private static Event loadEventByQuery(String query, Object param) {
        final Event[] eventHolder = new Event[1];
        final boolean[] eventFound = new boolean[1];

        PersistenceManager.executeQuery(query, new ResultHandler() {
            @Override
            public void handle(ResultSet rs) throws SQLException {
                eventFound[0] = true;

                Event e = new Event();

                e.id = rs.getInt("id");
                e.name = rs.getString("name");
                e.dateStart = parseDate(rs, "date_start");
                e.dateEnd = parseDate(rs, "date_end");
                e.location = rs.getString("location");
                e.startTime = parseTime(rs, "time_start");
                e.endTime = parseTime(rs, "time_end");
                e.participants = rs.getInt("participants");
                e.fine = rs.getBoolean("fine");
                e.personnelAllocation = rs.getString("personnel_allocation");
                e.chefAdvisory = rs.getString("chef_advisory");

                int state = rs.getInt("state");
                switch (state) {
                    case 0:
                        e.state = EventServiceState.DA_DETERMINARE;
                        break;

                    case 1:
                        e.state = EventServiceState.APPROVATO;
                        break;

                    case 2:
                        e.state = EventServiceState.IN_CORSO;
                        break;

                    case 3:
                        e.state = EventServiceState.ANNULLATO;
                        break;
                }

                int type = rs.getInt("type");
                switch (type) {
                    case 0:
                        e.type = EventType.MATRIMONIO;
                        break;

                    case 1:
                        e.type = EventType.PRIVATO;
                        break;

                    case 2:
                        e.type = EventType.AZIENDALE;
                        break;
                }

                try {
                    e.chef = User.load(rs.getInt("chef_id"));
                } catch (Exception ex) {
                    e.chef = null;
                }

                try {
                    e.manager = User.load(rs.getInt("manager_id"));
                } catch (Exception ex) {
                    e.manager = null;
                }

                eventHolder[0] = e;
            }
        }, param);

        if (!eventFound[0]) {
            return null;
        }

        Event result = eventHolder[0];
        if (result != null) {
            try {
                result.services = Service.loadServicesForEvent(result.id);
                ArrayList<RecurringDate> dates = RecurringDate.loadForEvent(result.id);
                if (dates.isEmpty()) {
                    result.recurrence = new SingleRecurrence(result.dateStart, result.dateEnd);
                } else {
                    result.recurrence = new MultipleRecurrence(dates);
                }
            } catch (Exception ex) {
                result.services = new ArrayList<>();
            }
        }

        return result;
    }

    // Helper methods to parse dates and times from the database
    private static Date parseDate(ResultSet rs, String column) throws SQLException {
        // primo step normalizza la stringa, rimuovendo spazi e eventuali timestamp
        String raw = rs.getString(column);
        if (raw == null || raw.trim().isEmpty()) {
            return null;
        }
        String normalized = raw.trim();
        int spaceIndex = normalized.indexOf(' ');
        // se c'è uno spazio, prendiamo solo la parte della data
        if (spaceIndex >= 0) {
            normalized = normalized.substring(0, spaceIndex);
        }
        return Date.valueOf(normalized);
    }

    private static Time parseTime(ResultSet rs, String column) throws SQLException {
        String raw = rs.getString(column);
        if (raw == null || raw.trim().isEmpty()) {
            return null;
        }
        // primo step normalizza la stringa, rimuovendo spazi e eventuali timestamp
        String normalized = raw.trim();
        int spaceIndex = normalized.indexOf(' ');
        // se c'è uno spazio, prendiamo solo la parte della data
        if (spaceIndex >= 0) {
            normalized = normalized.substring(0, spaceIndex);
        }
        return Time.valueOf(normalized);
    }

    @Override
    public String toString() {
        return "Event [id=" + id + ", name=" + name + ", dateStart=" + dateStart +
                ", services=" + (services != null ? services.size() : 0) + "]";
    }
}