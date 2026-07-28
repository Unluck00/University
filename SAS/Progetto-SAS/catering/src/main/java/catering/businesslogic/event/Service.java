package catering.businesslogic.event;

import java.sql.*;
import java.util.*;

import catering.businesslogic.menu.Menu;
import catering.businesslogic.menu.MenuItem;
import catering.businesslogic.user.User;
import catering.persistence.PersistenceManager;
import catering.persistence.ResultHandler;
import catering.util.ConvertEnum;

/**
 * Represents a service in an event in the catering system.
 */
public class Service {

    private int id;
    private String name;
    private int serviceDay;
    private Time timeStart;
    private Time timeEnd;
    private String location;
    private int eventId;
    private Menu menu;
    private boolean menuApproved;
    private String serviceType;
    private EventServiceState state;
    private boolean fine;
    private User cook;
    private ArrayList<StaffRole> assignedStaff;

    public Service() {
        assignedStaff = new ArrayList<>();
        menuApproved = false;
        fine = false;
        state = EventServiceState.DA_DETERMINARE;
    }

    public Service(String name) {
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

    // public Date getDate() {
    //     return date;
    // }

    // public void setDate(Date date) {
    //     this.date = date;
    //}

    public Time getTimeStart() {
        return timeStart;
    }

    public void setTimeStart(Time timeStart) {
        this.timeStart = timeStart;
    }

    public Time getTimeEnd() {
        return timeEnd;
    }

    public void setTimeEnd(Time timeEnd) {
        this.timeEnd = timeEnd;
    }

    public String getLocation() {
        return location;
    }

    public void setLocation(String location) {
        this.location = location;
    }

    public int getEventId() {
        return eventId;
    }

    public void setEventId(int eventId) {
        this.eventId = eventId;
    }

    public int getMenuId() {
        return (menu != null) ? menu.getId() : 0;
    }

    public Menu getMenu() {
        return menu;
    }

    public void setMenu(Menu menu) {
        this.menu = menu;
    }

    public boolean isMenuApproved() {
        return menuApproved;
    }

    public void setMenuApproved(boolean menuApproved) {
        this.menuApproved = menuApproved;
    }

    public String getServiceType() {
        return serviceType;
    }

    public void setServiceType(String serviceType) {
        this.serviceType = serviceType;
    }

    public EventServiceState getState() {
        return state;
    }

    public void setState(EventServiceState state) {
        this.state = state;
    }

    public boolean isFine() {
        return fine;
    }

    public void setFine(boolean fine) {
        this.fine = fine;
    }

    public User getCook() {
        return cook;
    }

    public int getCookId() {
        return cook != null ? cook.getId() : 0;
    }

    public void setCook(User cook) {
        this.cook = cook;
    }

    public void setCookId(int cookId) {
        cook = User.load(cookId);
    }

    public ArrayList<StaffRole> getAssignedStaff() {
        return assignedStaff;
    }

    public void setAssignedStaff(ArrayList<StaffRole> assignedStaff) {
        this.assignedStaff = assignedStaff;
    }

    public int getServiceDay() {
        return serviceDay;
    }

    public void setServiceDay(int serviceDay) {
        this.serviceDay = serviceDay;
    }

    public boolean hasRoleAssigned(User staff, StaffRole.ServiceRole role) {
        for (StaffRole sr : assignedStaff) {
            if (sr.getId() == staff.getId() && sr.getServiceRole() == role) {
                return true;
            }
        }
        return false;
    }

    public static Service createService(String name, String serviceType, Time timeStart, Time timeEnd, int eventId) {
        Service service = new Service(name);
        service.setServiceType(serviceType);
        service.setTimeStart(timeStart);
        service.setTimeEnd(timeEnd);
        service.setEventId(eventId);
        return service;
    }

    public Service modifyService(Service original, String name, String serviceType, Time timeStart, Time timeEnd) {
        Service service = original;
        service.setName(name);
        service.setServiceType(serviceType);
        service.setTimeStart(timeStart);
        service.setTimeEnd(timeEnd);
        return service;
    }

    public StaffRole addStaffRole(User staff, StaffRole.ServiceRole role) {
        StaffRole sr = StaffRole.createStaffRole(staff, role, this.getId());
        assignedStaff.add(sr);
        return sr;
    }

    public void removeStaffRole(int staffRoleId) {
        if(assignedStaff != null) {
            assignedStaff.removeIf(sr -> sr.getId() == staffRoleId);
        }
    }

    public void addCook(User cook) {
        String query = "UPDATE Services SET cook_id = ? WHERE id = ?";
        PersistenceManager.executeUpdate(query, this.getCookId(), this.getId());
    }

    public void removeCook() {
        String query = "UPDATE Services SET cook_id = 0 WHERE id = ?";
        PersistenceManager.executeUpdate(query, this.getId());
    }

    public void approveMenu() {
        if (this.menu == null)
            return;

        String query = "UPDATE Services SET approved_menu_id = ? WHERE id = ?";
        PersistenceManager.executeUpdate(query, this.getMenuId(), this.getId());
    }

    public void removeMenu() {
        this.menuApproved = false;
        this.menu = null;
    }

    public ArrayList<MenuItem> getMenuItems() {
        if (this.menu == null) {
            return new ArrayList<>();
        }
        return this.menu.getItems();
    }

    // Database operations
    public void saveNewService() {
        String query = "INSERT INTO Services (event_id, name, " +
                "approved_menu_id, service_day, time_start, time_end, " +
                "location, " +
                "service_type, state, fine) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

        PersistenceManager.executeUpdate(query,
                this.getEventId(),
                this.getName(),
                this.getMenuId(),
                this.getServiceDay(),
                this.getTimeStart(),
                this.getTimeEnd(),
                this.getLocation(),
                this.getServiceType(),
                ConvertEnum.convertEventServiceState(this.getState()),
                this.isFine());


        // Get the ID of the newly inserted service
        this.setId(PersistenceManager.getLastId());
    }

    public void updateService() {
        String query = "UPDATE Services SET " +
                "    name = ?, " +
                "    service_day = ?, " +
                "    time_start = ?, " +
                "    time_end = ?, " +
                "    location = ?, " +
                "    service_type = ?, " +
                "    state = ?, " +
                "    fine = ? " +
                "WHERE id = ?";

        PersistenceManager.executeUpdate(query,
                this.getName(),
                this.getServiceDay(),
                this.getTimeStart(),
                this.getTimeEnd(),
                this.getLocation(),
                this.getServiceType(),
                ConvertEnum.convertEventServiceState(this.getState()),
                this.isFine(),
                this.getId());
    }

    public boolean deleteService() {
        String query = "DELETE FROM Services WHERE id = ?";
        return PersistenceManager.executeUpdate(query, this.getId()) > 0;
    }

    public void assignMenuToService(Menu menu) {
        String query = "UPDATE Services SET approved_menu_id = ? WHERE id = ?";
        PersistenceManager.executeUpdate(query, menu.getId(), this.getId());
    }

    public void removeMenuFromService() {
        String query = "UPDATE Services SET approved_menu_id = 0 WHERE id = ?";
        PersistenceManager.executeUpdate(query, this.getId());
    }

    // Static methods for data loading
    public static ArrayList<Service> loadServicesForEvent(int eventId) {
        ArrayList<Service> services = new ArrayList<>();
        String query = "SELECT * FROM Services WHERE event_id = ? ORDER BY " +
                "service_day, time_start";

        PersistenceManager.executeQuery(query, new ResultHandler() {
            @Override
            public void handle(ResultSet rs) throws SQLException {
                Service s = new Service();
                s.id = rs.getInt("id");
                s.name = rs.getString("name");
                s.serviceDay = rs.getInt("service_day");

                try {
                    s.timeStart = Time.valueOf(rs.getString("time_start"));
                    s.timeEnd = Time.valueOf(rs.getString("time_end"));
                } catch (IllegalArgumentException ex) {
                    // Ignore parsing errors
                }

                s.location = rs.getString("location");
                s.eventId = rs.getInt("event_id");
                s.serviceType = rs.getString("service_type");
                int state = rs.getInt("state");
                switch (state) {
                    case 0:
                        s.state = EventServiceState.DA_DETERMINARE;
                        break;

                    case 1:
                        s.state = EventServiceState.APPROVATO;
                        break;

                    case 2:
                        s.state = EventServiceState.IN_CORSO;
                        break;

                    case 3:
                        s.state = EventServiceState.ANNULLATO;
                        break;
                }
                s.fine = rs.getBoolean("fine");
                s.cook = User.load(rs.getInt("cook_id"));
                int menuId = rs.getInt("approved_menu_id");
                if (menuId > 0)
                    s.menu = Menu.load(menuId);

                s.setAssignedStaff(StaffRole.loadStaffForService(s.getId()));

                services.add(s);
            }
        }, eventId);

        return services;
    }

    public static Service loadById(int id) {
        String query = "SELECT * FROM Services WHERE id = ?";
        return loadServiceByQuery(query, id);
    }

    public static Service loadByName(String name) {
        String query = "SELECT * FROM Services WHERE name = ?";
        return loadServiceByQuery(query, name);
    }

    private static Service loadServiceByQuery(String query, Object param) {
        final Service[] serviceHolder = new Service[1];
        final boolean[] serviceFound = new boolean[1];
        serviceFound[0] = false;

        PersistenceManager.executeQuery(query, new ResultHandler() {
            @Override
            public void handle(ResultSet rs) throws SQLException {
                serviceFound[0] = true;

                Service s = new Service();
                s.id = rs.getInt("id");
                s.name = rs.getString("name");
                s.serviceDay = rs.getInt("service_day");

                try {
                    String startTimeStr = rs.getString("time_start");
                    String endTimeStr = rs.getString("time_end");

                    if (startTimeStr != null && !startTimeStr.isEmpty()) {
                        s.timeStart = Time.valueOf(startTimeStr);
                    }
                    if (endTimeStr != null && !endTimeStr.isEmpty()) {
                        s.timeEnd = Time.valueOf(endTimeStr);
                    }
                } catch (IllegalArgumentException ex) {
                }

                s.location = rs.getString("location");
                s.eventId = rs.getInt("event_id");

                int menuId = rs.getInt("approved_menu_id");
                if (menuId > 0) {
                    try {
                        s.menu = Menu.load(menuId);
                    } catch (Exception e) {
                    }
                }

                serviceHolder[0] = s;
            }
        }, param);

        return serviceFound[0] ? serviceHolder[0] : null;
    }

    @Override
    public String toString() {
        return "Service [id=" + id + ", name=" + name + ", location=" + location +
                ", menu=" + (menu != null ? menu.getTitle() : "none") + "]";
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj)
            return true;
        if (obj == null || getClass() != obj.getClass())
            return false;

        Service other = (Service) obj;

        // If both sections have valid IDs, compare by ID
        if (this.id > 0 && other.id > 0) {
            return this.id == other.id;
        }
        
        // Otherwise, compare by name and items
        boolean nameMatch = (this.name == null && other.name == null) ||
                (this.name != null && this.name.equals(other.name));

        // If names don't match, sections are not equal
        if (!nameMatch)
            return false;

        boolean serviceDayMatch = (this.serviceDay == other.serviceDay);

        if (!serviceDayMatch)
            return false;

        // If times don't match, sections are not equal
        boolean timeStartMatch = (this.timeStart == null && other.timeStart == null) ||
        (this.timeStart != null && this.timeStart.equals(other.timeStart));

        if (!timeStartMatch)
            return false;
        
        boolean timeEndMatch = (this.timeEnd == null && other.timeEnd == null) ||
            (this.timeEnd != null && this.timeEnd.equals(other.timeEnd));
    
        if (!timeEndMatch)
            return false;        

        // If locations don't match, sections are not equal
        boolean locationMatch = (this.location == null && other.location == null) ||
        (this.location != null && this.location.equals(other.location));

        if (!locationMatch)
            return false;

        // If locations don't match, sections are not equal
        boolean menuMatch = (this.menu == null && other.menu == null) ||
        (this.menu != null && this.menu.equals(other.menu));

        if (!menuMatch)
            return false;

        // If events don't match, sections are not equal
        if (this.eventId > 0 && other.eventId > 0) {
            return this.eventId == other.eventId;
        }

        return true;
    }
}
