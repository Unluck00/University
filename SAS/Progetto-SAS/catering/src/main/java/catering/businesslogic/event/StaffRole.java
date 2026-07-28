package catering.businesslogic.event;

import catering.businesslogic.user.User;
import catering.persistence.PersistenceManager;
import catering.persistence.ResultHandler;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;

public class StaffRole {

    public static enum ServiceRole {
        FINGER_FOOD,
        BEVERAGE
    }

    private int id;
    private User user;
    private ServiceRole serviceRole;
    private int serviceId;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public User getUser() {
        return user;
    }

    public int getUserId() {
        return user != null ? user.getId() : 0;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public void setUserId(int userId) {
        user = User.load(userId);
    }

    public ServiceRole getServiceRole() {
        return serviceRole;
    }

    public int getServiceId() {
        return serviceId;
    }

    public void setServiceId(int serviceId) {
        this.serviceId = serviceId;
    }

    public void setServiceRole(ServiceRole serviceRole) {
        this.serviceRole = serviceRole;
    }

    public static StaffRole createStaffRole(User user, ServiceRole serviceRole, int serviceId) {
        StaffRole sr = new StaffRole();
        sr.setUser(user);
        sr.setServiceRole(serviceRole);
        sr.serviceId = serviceId;
        return sr;
    }

    public void saveNewStaffRole() {
        String query = "INSERT INTO StaffRole (user_id, service_role, " +
                "service_id) VALUES (?, ?, ?)";
        PersistenceManager.executeUpdate(query,
                this.getUserId(),
                this.serviceRole.name(),
                this.serviceId);
        this.id = PersistenceManager.getLastId();
    }

    public void updateStaffRole() {
        String query = "UPDATE StaffRole SET user_id = ?, service_role = ?, " +
                "service_id = ? WHERE id = ?";
        PersistenceManager.executeUpdate(query,
                this.getUserId(),
                this.serviceRole.name(),
                this.serviceId,
                this.id);
    }

    public void deleteStaffRole() {
        String query = "DELETE FROM StaffRole WHERE id = ?";
        PersistenceManager.executeUpdate(query, this.id);
    }

    public static ArrayList<StaffRole> loadStaffForService(int serviceId) {
        ArrayList<StaffRole> list = new ArrayList<>();
        String query = "SELECT * FROM StaffRole WHERE service_id = ?";
        PersistenceManager.executeQuery(query, new ResultHandler() {
            @Override
            public void handle(ResultSet rs) throws SQLException {
                StaffRole sr = new StaffRole();
                sr.id = rs.getInt("id");
                sr.serviceId = rs.getInt("service_id");
                sr.setUserId(rs.getInt("user_id"));
                sr.serviceRole = ServiceRole.valueOf(rs.getString("service_role"));
                list.add(sr);
            }
        }, serviceId);
        return list;
    }
}
