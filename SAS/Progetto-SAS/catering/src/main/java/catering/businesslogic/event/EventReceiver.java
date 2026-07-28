package catering.businesslogic.event;

import catering.businesslogic.menu.Menu;
import catering.businesslogic.user.User;

/**
 * Interface for receiving event-related notifications.
 * Implemented by classes that need to respond to event changes.
 */
public interface EventReceiver {

    void updateEventCreated(Event event);

    void updateEventApproved(Event event);

    void updateEventDeleted(Event event);

    void updateEventModified(Event event);

    void updateServiceCreated(Event event, Service service);

    void updateServiceModified(Service service);

    void updateServiceDeleted(Service service);

    void updateChefAssigned(Event event);

    void updateChefRemoved(Event event);

    void updateMenuAssigned(Service service, Menu menu);

    void updateMenuRemoved(Service service);

    void updateStaffRoleAdded(StaffRole staff);

    void updateStaffRoleRemoved(StaffRole staff);

    void updateCookAdded(Service service, User cook);

    void updateCookRemoved(Service service);

    void updateEventAnnotated(Event event);

    void updateRecurrence(Event event);

    void deleteRecurrence(Event event);

}