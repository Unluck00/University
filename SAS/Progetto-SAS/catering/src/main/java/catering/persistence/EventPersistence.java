package catering.persistence;

import catering.businesslogic.event.Event;
import catering.businesslogic.event.EventReceiver;
import catering.businesslogic.event.Service;
import catering.businesslogic.event.StaffRole;
import catering.businesslogic.menu.Menu;
import catering.businesslogic.user.User;

/**
 * Persistence class for Event operations.
 * Delegates to Event and Service classes for actual persistence.
 */
public class EventPersistence implements EventReceiver {

    @Override
    public void updateEventCreated(Event event) {
        event.saveNewEvent();
    }

    @Override
    public void updateEventApproved(Event event){
        event.updateEventApproved();
    }

    @Override
    public void updateEventModified(Event event) {
        event.updateEvent();
    }

    @Override
    public void updateEventDeleted(Event event) {
        event.deleteEvent();
    }

    @Override
    public void updateServiceCreated(Event event, Service service) {
        service.saveNewService();
    }

    @Override
    public void updateServiceModified(Service service) {
        service.updateService();
    }

    @Override
    public void updateServiceDeleted(Service service) {
        service.deleteService();
    }

    @Override
    public void updateChefAssigned(Event event){
        event.updateChef();
    }

    @Override
    public void updateChefRemoved(Event event){
        event.removeChef();
    }

    @Override
    public void updateMenuAssigned(Service service, Menu menu) {
        service.assignMenuToService(menu);
    }

    @Override
    public void updateMenuRemoved(Service service) {
        service.removeMenuFromService();
    }

    @Override
    public void updateStaffRoleAdded(StaffRole staffRole) {
        // Implementation for adding staff role
        staffRole.saveNewStaffRole();
    }

    @Override
    public void updateStaffRoleRemoved(StaffRole staffRole) {
        // Implementation for removing staff role
        staffRole.deleteStaffRole();
    }

    @Override
    public void updateCookAdded(Service service, User cook){
        service.addCook(cook);
    }

    @Override
    public void updateCookRemoved(Service service){
        service.removeCook();
    }

    @Override
    public void updateEventAnnotated(Event event){
        event.updateAnnotationEvent(event);
    }

    @Override
    public void updateRecurrence(Event event) {
        event.updateRecurrence(event.getId());
    }

    @Override
    public void deleteRecurrence(Event event) {
        event.getRecurrence().deleteDates(event.getId());
    }
    
}