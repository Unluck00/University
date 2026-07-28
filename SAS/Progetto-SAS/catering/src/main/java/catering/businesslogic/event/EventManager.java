package catering.businesslogic.event;

import java.sql.Date;
import java.sql.Time;
import java.util.ArrayList;

import catering.businesslogic.CatERing;
import catering.businesslogic.UseCaseLogicException;
import catering.businesslogic.kitchen.Assignment;
import catering.businesslogic.kitchen.KitchenTask;
import catering.businesslogic.kitchen.SummarySheet;
import catering.businesslogic.menu.Menu;
import catering.businesslogic.user.User;

/**
 * EventManager handles all operations related to events and services in the
 * CatERing system.
 * It manages event creation, modification, and deletion, as well as service
 * management and menu assignments for services.
 */
public class EventManager {
    

    private ArrayList<EventReceiver> eventReceivers;
    private Event selectedEvent;
    private Service currentService;

    /**
     * Constructor initializes the event receivers list
     */
    public EventManager() {
        eventReceivers = new ArrayList<>();
    }

    /**
     * Adds an event receiver to be notified of events changes
     * 
     * @param receiver The event receiver to add
     */
    public void addEventReceiver(EventReceiver receiver) {
        if (receiver != null && !eventReceivers.contains(receiver)) {
            eventReceivers.add(receiver);
        }
    }

    /**
     * Removes an event receiver
     * 
     * @param receiver The event receiver to remove
     */
    public void removeEventReceiver(EventReceiver receiver) {
        eventReceivers.remove(receiver);
    }

    /**
     * Gets all events in the system
     * 
     * @return List of all events
     */
    public ArrayList<Event> getEvents() {
        return Event.loadAllEvents();
    }

    /**
     * Sets the current service directly
     * 
     * @param service Service to set as current
     */
    public void setCurrentService(int serviceId) {
        Service service = findServiceById(serviceId);
        this.currentService = service;
    }

    /**
     * Gets the current service
     * 
     * @return Current service or null if none selected
     */
    public Service getCurrentService() {
        return this.currentService;
    }

    /**
     * Gets the selected event
     * 
     * @return Selected event or null if none selected
     */
    public Event getSelectedEvent() {
        return selectedEvent;
    }

    /**
     * Sets the selected event
     * 
     * @param event Event to select
     */
    public void setSelectedEvent(Event event) {
        this.selectedEvent = event;
    }

    /**
     * Creates a new event with the given details
     * 
     * @param name         Event name
     * @param location     Event location
     * @param dateStart    Start date
     * @param dateEnd      End date (can be null)
     * @param startTime    Start time
     * @param endTime      End time 
     * @param recurrence   Array of recurrence days (can be null)
     * @param participants Number of participants
     * @param eventType    Type of the event
     * @return The newly created event
     */
    public Event createEvent(String name, String location, Date dateStart, Date dateEnd, Time startTime, Time endTime, ArrayList<RecurringDate> recurrence, int participants, Event.EventType eventType) throws UseCaseLogicException {
        User user = CatERing.getInstance().getUserManager().getCurrentUser();
        if(!user.isManager()) {
            throw new UseCaseLogicException("Managers cannot create events");
        }
        
        try {

            if(dateStart.compareTo(dateEnd) <= 0 && startTime.compareTo(endTime) <= 0) {
                Event ev = Event.createEvent(user.getId(), name, location, dateStart, dateEnd, startTime, endTime, participants, recurrence, eventType);
                
                // Notify all receivers (EventPersistence will persist)
                notifyEventCreated(ev);

                // Set as selected event
                this.selectedEvent = ev;
                this.currentService = null;
            
                return ev;
            } else {
                throw new UseCaseLogicException("Error creating event: start date or time must be before or equal to end date or time");
            }
        } catch (Exception e) {
            return null;
        }
    }

    public Event createEvent(String name, String location, Date dateStart, Time startTime, Time endTime, int participants, Event.EventType eventType) throws UseCaseLogicException {
        return createEvent(name, location, dateStart, null, startTime, endTime, null, participants, eventType);
    }

    public Event createEvent(String name, String location, Date dateStart, Date dateEnd, Time startTime, Time endTime, int participants, Event.EventType eventType) throws UseCaseLogicException {
        return createEvent(name, location, dateStart, dateEnd, startTime, endTime, null, participants, eventType);
    }

    public void selectEvent(Event event) {
        this.selectedEvent = event;
        this.currentService = null;
    }

    public Service createService(String name, String serviceType, Time timeStart, Time timeEnd) throws UseCaseLogicException {
        User user = CatERing.getInstance().getUserManager().getCurrentUser();
        if(!user.isManager()) {
            throw new UseCaseLogicException("Managers cannot create events");
        } 

        if (selectedEvent == null) {
            String msg = "Cannot create service: no event selected";
            throw new UseCaseLogicException(msg);
        }

        try {

            if(timeStart.compareTo(timeEnd) <= 0) {
                Service sr = Service.createService(name, serviceType, timeStart, timeEnd, selectedEvent.getId());

                // Add to event and set as current service
                selectedEvent.addService(sr);
                this.currentService = sr;

                // Notify all receivers (EventPersistence will persist)
                notifyServiceCreated(sr);

                return sr;
            } else {
                throw new UseCaseLogicException("Error creating service: start time must be before or equal to end time");
            }

        } catch (Exception e) {
            return null;
        }
    }

    public Event selectEventForCopy(int eventId) throws UseCaseLogicException {
        User user = CatERing.getInstance().getUserManager().getCurrentUser();
        if(!user.isManager()) {
            throw new UseCaseLogicException("Managers cannot create events");
        }

        Event original = Event.loadById(eventId);
        if (original != null) {
            // Create a shallow copy of the original event by copying its basic properties.
            Event copy = Event.copyEvent(original);

            // Services too are copied, but they will be treated as new services when saved, so they won't have the same IDs.
            if (original.getServices() != null) {
                for (Service s : original.getServices()) {
                    Service serviceCopy = new Service();
                    serviceCopy.setName(s.getName());
                    serviceCopy.setTimeStart(s.getTimeStart());
                    serviceCopy.setTimeEnd(s.getTimeEnd());
                    serviceCopy.setServiceType(s.getServiceType());
                    // Menu is not copied, as it will be treated as a new service without an assigned menu.
                    copy.addService(serviceCopy);
                }
            }

            this.selectedEvent = copy;
            this.currentService = null;
            return copy;
        }
        return null;
    } 

    /**
     * Modifies an existing event
     * 
     * @param eventId ID of the event to modify
     * @param name    New name for the event
     * @param date    New date for the event
     */
    public void modifyEvent(int eventId, String name, String location, Date dateStart, Date dateEnd, Time startTime, Time endTime, ArrayList<RecurringDate> recurrence, int participants, Event.EventType eventType) throws UseCaseLogicException {
        User user = CatERing.getInstance().getUserManager().getCurrentUser();
        if(!user.isManager()) {
            throw new UseCaseLogicException("Managers cannot create events");
        }
        
        Event event = Event.loadById(eventId);
        if (event != null && event.getState() != EventServiceState.IN_CORSO) {
            event = event.modifyEvent(event, name, location, dateStart, dateEnd, startTime, endTime, recurrence, participants, eventType);

            // Notify all receivers
            notifyEventModified(event);
            notifyRecurrenceUpdate(event);

            // Update selected event if it's the same one
            if (selectedEvent != null && selectedEvent.getId() == eventId) {
                this.selectedEvent = event;
            }
        } else {
            throw new UseCaseLogicException("Cannot modify event: event not found or event in progress");
        }
    }

    public void modifyEvent(int eventId, String name, String location, Date dateStart, Time startTime, Time endTime, int participants, Event.EventType eventType) throws UseCaseLogicException {
        modifyEvent(eventId, name, location, dateStart, null, startTime, endTime, null, participants, eventType);
    }

    public void modifyEvent(int eventId, String name, String location, Date dateStart, Date dateEnd, Time startTime, Time endTime, int participants, Event.EventType eventType) throws UseCaseLogicException {
        modifyEvent(eventId, name, location, dateStart, dateEnd, startTime, endTime, null, participants, eventType);
    }

    /**
     * Deletes a service by its ID
     * 
     * @param serviceId ID of the service to delete
     * @return true if deleted successfully, false otherwise
     */
    public boolean deleteService(int serviceId) throws UseCaseLogicException {
        User user = CatERing.getInstance().getUserManager().getCurrentUser();
        if(!user.isManager()) {
            throw new UseCaseLogicException("Managers cannot create events");
        }

        if(currentService == null || currentService.getState() == EventServiceState.IN_CORSO) {
            throw new UseCaseLogicException("Cannot delete service: no service selected or event in progress");
        }

        try {
            if (selectedEvent == null) {
                return false;
            }

            Service serviceToDelete = findServiceById(serviceId);
            if (serviceToDelete == null) {
                return false;
            }

            // Clear reference
            if (currentService != null && currentService.getId() == serviceId) {
                currentService = null;
            }

            selectedEvent.removeService(serviceToDelete);

            // Clear current service if it was the one deleted
            if (currentService != null && currentService.getId() == serviceId) {
                currentService = null;
            }

            // Notify all receivers (EventPersistence will delete from DB)
            notifyServiceDeleted(serviceToDelete);

            return true;
        } catch (Exception e) {
            return false;
        }
    }

    /**
     * Deletes an event and all its associated services
     * 
     * @param eventId ID of the event to delete
     * @return true if deleted successfully, false otherwise
     */
    public boolean deleteEvent(int eventId) throws UseCaseLogicException {
        User user = CatERing.getInstance().getUserManager().getCurrentUser();
        if(!user.isManager() || selectedEvent.getId() != eventId) {
            throw new UseCaseLogicException("Managers cannot create events");
        }

        try {
            Event eventToDelete = Event.loadById(eventId);
            if (eventToDelete == null) {
                return false;
            }

            // Clear references if this was the selected event
            if (selectedEvent != null && selectedEvent.getId() == eventId) {
                selectedEvent = null;
                currentService = null;
            }

            eventToDelete.removeEvent(eventToDelete);

            // Notify all receivers (EventPersistence will delete from DB)
            notifyRecurrenceDeleted(eventToDelete);
            notifyEventDeleted(eventToDelete);

            return true;
        } catch (Exception e) {
            return false;
        }
    }


    /**
     * Assigns a chef to the current event
     * 
     * @param chef The chef to assign
     * @throws UseCaseLogicException if no event is selected or the user is not a chef
     */
    public void assignChef(User chef) throws UseCaseLogicException {
        
        if (selectedEvent == null || !chef.isChef()) {
            throw new UseCaseLogicException("Cannot assign chef");
        }

        selectedEvent.setChef(chef);

        // Notify all receivers (EventPersistence will persist)
        notifyChefAssigned(selectedEvent);
    }

    /**
     * Removes the chef assignment from the current event
     * 
     * @param chef The chef to remove
     * @throws UseCaseLogicException if no event is selected or the user is not a chef
     */
    public void removeChefAssignment(User chef) throws UseCaseLogicException {
        
        if (selectedEvent == null || !chef.isChef() || selectedEvent.getChef() == null || selectedEvent.getChef().getId() != chef.getId()) {
            throw new UseCaseLogicException("Cannot remove chef assignment");
        }

        selectedEvent.setChef(null);

        // Notify all receivers (EventPersistence will persist)
        notifyRemovedChefAssigned(selectedEvent);
    }

    /**
     * Assigns a menu to the current service
     * 
     * @param menu The menu to assign
     * @throws UseCaseLogicException if no event or service is selected
     */
    public void assignMenu(int serviceId, Menu menu) throws UseCaseLogicException {
        if (selectedEvent == null || menu == null || !menu.isPublished()) {
            throw new UseCaseLogicException("Cannot assign menu: no event selected or invalid menu");
        }

        Service service = findServiceById(serviceId);
        if (service == null) {
            throw new UseCaseLogicException("Cannot assign menu: service not found");
        }

        this.currentService = service;
        service.setMenu(menu);

        // Notify all receivers (EventPersistence will persist)
        notifyMenuAssigned(service, menu);
    }

    /**
     * Removes the menu from the current service
     * 
     * @param serviceId The ID of the service from which to remove the menu
     * @throws UseCaseLogicException if no event or service is selected, or if the menu is not approved/published
     */
    public void removeMenu(int serviceId) throws UseCaseLogicException {
        Service service = findServiceById(serviceId);
        if (currentService == null || !service.getMenu().isPublished()) {
            throw new UseCaseLogicException("Cannot remove menu: no service selected or menu not approved/published");
        }

        currentService.removeMenu();

        // Notify all receivers
        notifyMenuRemoved(currentService);
    }

    /**
     * Approves the menu assigned to the current service
     * @throws UseCaseLogicException
     */
    public void approveMenu(int serviceId) throws UseCaseLogicException {
        Service service = findServiceById(serviceId);
        if (service == null || currentService != service || service.getMenu() == null) {
            throw new UseCaseLogicException("Cannot approve menu: no service or menu selected");
        }

        service.setMenuApproved(true);

        // Notify all receivers (EventPersistence will persist)
        updateMenuAssigned(service);
    }

    private StaffRole findStaffRoleById(int staffRoleId) {
        if (selectedEvent == null || selectedEvent.getServices() == null) {
            return null;
        }

        for (StaffRole sr : currentService.getAssignedStaff()) {
            if (sr.getId() == staffRoleId) {
                return sr;
            }
        }

        return null;
    }

    /**
     * Adds a staff role to the current service
     * @param staff The staff member to assign the role to
     * @param role The role to assign
     * @throws UseCaseLogicException
     */
    public void addStaffRoleToService(User staff, StaffRole.ServiceRole role) throws UseCaseLogicException {
        if (currentService == null || !staff.isStaff() || currentService.hasRoleAssigned(staff, role)) {
            throw new UseCaseLogicException("Cannot add staff role: no service selected or role already assigned");
        }

        StaffRole staffRole = currentService.addStaffRole(staff, role);

        // Notify all receivers (EventPersistence will persist)
        notifyStaffRoleAdded(staffRole);
    }

    /**
     * Removes a staff role from the current service
     * 
     * @param staff The staff member whose role to remove
     * @param role The role to remove
     */
    public void removeStaffRoleFromService(int staffRoleId, User staff, StaffRole.ServiceRole role) throws UseCaseLogicException {
        if (currentService == null || staff == null || !staff.isStaff() || !currentService.hasRoleAssigned(staff, role)) {
            throw new UseCaseLogicException("Cannot remove staff role: no service selected or role not assigned");
        }

        StaffRole staffRole = findStaffRoleById(staffRoleId);

        currentService.removeStaffRole(staffRoleId);

        // Notify all receivers (EventPersistence will persist)
        notifyStaffRoleRemoved(staffRole);
    }

    /**
     * Gets the staff member whose belongs to the service
     * 
     * @return The staff member assigned to the service, or null if none assigned
     */
    public ArrayList<StaffRole> getStaffRoleForService() {
        if (currentService == null){
            return null;
        }

        return currentService.getAssignedStaff(); 
    }

    /**
     * Adds a cook to the current service for the task to ready
     * @param cook
     * @throws UseCaseLogicException
     */
    public void addCookToService(User cook) throws UseCaseLogicException {
        if (currentService == null || cook == null || !cook.isCook()) {
            throw new UseCaseLogicException("Cannot add cook: no service selected or invalid cook");
        }

        SummarySheet sheet = getSummarySheetForCurrentService();
        if (sheet != null) {
            for (Assignment assignment : sheet.getAssignments()) {
                if (cook.equals(assignment.getCook()) && assignment.getTask() != null && !assignment.getTask().isReady()) {
                    KitchenTask.updateTaskChanged(assignment.getTask());
                }
            }
        }

        cook.setSummoned(true);
        currentService.setCook(cook);

        // Notify all receivers (EventPersistence will persist)
        notifyCookAdded(currentService, cook);
    }

    public void removeCookFromService() throws UseCaseLogicException {
        if (currentService == null) {
            throw new UseCaseLogicException("Cannot remove cook: no service " +
                    "selected");
        }

        currentService.getCook().setSummoned(false);
        currentService.setCook(null);

        notifyCookRemoved(currentService);
    }

    /**
     * 
     * @return The summary sheet associated with the current service, or null if no service selected or no summary sheet found 
     */
    private SummarySheet getSummarySheetForCurrentService() {
        if (currentService == null || currentService.getId() <= 0) {
            return null;
        }

        ArrayList<SummarySheet> sheets = SummarySheet.loadSummarySheetsByServiceId(currentService.getId());
        
        if (sheets.isEmpty()) {
            return null;
        }

        if (sheets.size() > 1) {
            throw new IllegalStateException("Multiple summary sheets found for service" + currentService.getId());
        }

        return sheets.get(0); // Assuming one summary sheet per service
    }
    

    /**
     * Approves the selected event, changing its state to APPROVATO
     * @throws UseCaseLogicException
     */
    public void approveEvent() throws UseCaseLogicException {
        if (selectedEvent == null || selectedEvent.getState() != EventServiceState.DA_DETERMINARE) {
            throw new UseCaseLogicException("Cannot approve event: no event selected or event not in a state that can be approved");
        }

        selectedEvent.setState(EventServiceState.APPROVATO);

        // Notify all receivers (EventPersistence will persist)
        notifyEventApproved(selectedEvent);
    }

    /**
     * Updates the number of participants for the selected event
     * @param participants
     * @throws UseCaseLogicException
     */
    public void updatePartecipants(int participants) throws UseCaseLogicException {
        if (selectedEvent == null || selectedEvent.getState() == EventServiceState.IN_CORSO) {
            throw new UseCaseLogicException("Cannot update participants: no event selected or event in progress");
        }

        selectedEvent.updatePartecipants(participants);

        // Notify all receivers (EventPersistence will persist)
        notifyEventModified(selectedEvent);
    }

    public void updateDate(Date startDate, Date endDate, ArrayList<RecurringDate> recurrence) throws UseCaseLogicException {
        if (selectedEvent == null || selectedEvent.getState() == EventServiceState.IN_CORSO || startDate.compareTo(endDate) > 0) {
            throw new UseCaseLogicException("Cannot update date: no event selected or event in progress");
        }

        selectedEvent.updateDates(startDate, endDate, recurrence);

        // Notify all receivers (EventPersistence will persist)
        notifyEventModified(selectedEvent);
        notifyRecurrenceUpdate(selectedEvent);
    }

    public void updateDate(Date startDate, Date endDate) throws UseCaseLogicException {
        updateDate(startDate, endDate, null);
    }

    public void updateDate(Date startDate) throws UseCaseLogicException {
        updateDate(startDate, null, null);
    }

    /**
     * Modifies the details of a service
     * @param serviceId
     * @param typeService
     * @param timeStart
     * @param timeEnd
     * @throws UseCaseLogicException
     */
    public void modifyService(int serviceId, String name,String typeService, Time timeStart, Time timeEnd) throws UseCaseLogicException {
        User user = CatERing.getInstance().getUserManager().getCurrentUser();
        if(!user.isManager()) {
            throw new UseCaseLogicException("Managers cannot create events");
        }

        Service service = findServiceById(serviceId);
        if (service != null && service.getState() != EventServiceState.IN_CORSO && timeStart.compareTo(timeEnd) <= 0) { // Validate time range

            service = service.modifyService(service, name, typeService, timeStart, timeEnd);

            if(service.getState() == EventServiceState.APPROVATO) {
                service.setFine(true);    
            }

            // Notify all receivers
            notifyServiceModified(service);

        } else {
            throw new UseCaseLogicException("Cannot modify service: service not found or event in progress");
        }
    }

    /**
     * Annotates the event with notes for the menu, personnel allocation, and chef advisory.
     * @param menuNote           Notes related to the menu (can be null)
     * @param personnelAllocation Notes related to personnel allocation (can be null)
     * @param chefAdvisory       Notes related to chef advisory (can be null)
     * @throws UseCaseLogicException if no event is selected or the event is in progress
     */
    public void annotateEvent(int serviceId, String menuNote, String personnelAllocation, String chefAdvisory) throws UseCaseLogicException {
        if (selectedEvent == null || selectedEvent.getState() == EventServiceState.IN_CORSO) {
            throw new UseCaseLogicException("Cannot annotate event: no event selected or event in progress");
        }

        Service service = findServiceById(serviceId);
        if (service == null) {
            throw new UseCaseLogicException("Cannot annotate event: service not found");
        }

        // Update menu note if service has a menu assigned
        if(service.getMenu() != null) {
            service.getMenu().setNote(menuNote);
        }    

        selectedEvent.setPersonnelAllocation(personnelAllocation);
        selectedEvent.setChefAdvisory(chefAdvisory);

        // Notify all receivers (EventPersistence will persist)
        notifyEventAnnotated(selectedEvent);
        Menu.saveNote(service.getMenu());
    }

    /**
     * Helper method to find a service by ID within the selected event
     */
    private Service findServiceById(int serviceId) {
        if (selectedEvent == null || selectedEvent.getServices() == null) {
            return null;
        }

        for (Service s : selectedEvent.getServices()) {
            if (s.getId() == serviceId) {
                return s;
            }
        }

        return null;
    }

    // Notification methods to avoid code duplication
    private void notifyEventCreated(Event event) {
        for (EventReceiver receiver : eventReceivers) {
            receiver.updateEventCreated(event);
        }
    }
    
    private void notifyEventApproved(Event event) {
        for (EventReceiver receiver : eventReceivers) {
            receiver.updateEventApproved(event);
        }
    }

    private void notifyEventDeleted(Event event) {
        for (EventReceiver receiver : eventReceivers) {
            receiver.updateEventDeleted(event);
        }
    }

    private void notifyEventModified(Event event) {
        for (EventReceiver receiver : eventReceivers) {
            receiver.updateEventModified(event);
        }
    }

    private void notifyServiceCreated(Service service) {
        for (EventReceiver receiver : eventReceivers) {
            receiver.updateServiceCreated(selectedEvent, service);
        }
    }

    private void notifyServiceModified(Service service) {
        for (EventReceiver receiver : eventReceivers) {
            receiver.updateServiceModified(service);
        }
    }

    private void notifyServiceDeleted(Service service) {
        for (EventReceiver receiver : eventReceivers) {
            receiver.updateServiceDeleted(service);
        }
    }

    private void notifyChefAssigned(Event selectedEvent) {
        for (EventReceiver receiver : eventReceivers) {
            receiver.updateChefAssigned(selectedEvent);
        }
    }

    private void notifyRemovedChefAssigned(Event selectedEvent) {
        for (EventReceiver receiver : eventReceivers) {
            receiver.updateChefRemoved(selectedEvent);
        }
    }

    private void notifyMenuAssigned(Service service, Menu menu) {
        for (EventReceiver receiver : eventReceivers) {
            receiver.updateMenuAssigned(service, menu);
        }
    }

    private void notifyMenuRemoved(Service service) {
        for (EventReceiver receiver : eventReceivers) {
            receiver.updateMenuRemoved(service);
        }
    }

    private void updateMenuAssigned(Service service) {
        for (EventReceiver receiver : eventReceivers) {
            receiver.updateMenuAssigned(service, service.getMenu());
        }
    }

    private void notifyStaffRoleAdded(StaffRole staffRole) {
        for (EventReceiver receiver : eventReceivers) {
            receiver.updateStaffRoleAdded(staffRole);
        }
    }

    private void notifyStaffRoleRemoved(StaffRole staffRole) {
        for (EventReceiver receiver : eventReceivers) {
            receiver.updateStaffRoleRemoved(staffRole);
        }
    }

    private void notifyCookAdded(Service service, User cook) {
        for (EventReceiver receiver : eventReceivers) {
            receiver.updateCookAdded(service, cook);
        }
    }

    private void notifyCookRemoved(Service service) {
        for (EventReceiver receiver : eventReceivers) {
            receiver.updateCookRemoved(service);
        }
    }

    private void notifyEventAnnotated(Event event) {
        for (EventReceiver receiver : eventReceivers) {
            receiver.updateEventAnnotated(event);
        }
    }

    private void notifyRecurrenceUpdate(Event event) {
        for (EventReceiver receiver : eventReceivers) {
            receiver.updateRecurrence(event);
        }
    }

    private void notifyRecurrenceDeleted(Event event) {
        for (EventReceiver receiver : eventReceivers) {
            receiver.deleteRecurrence(event);
        }
    }
}
