package catering.businesslogic.event;

import java.sql.Date;
import java.sql.Time;
import java.util.ArrayList;
import java.util.logging.Logger;

import org.junit.jupiter.api.AfterEach;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import catering.businesslogic.CatERing;
import catering.businesslogic.menu.Menu;
import catering.businesslogic.user.User;
import catering.persistence.PersistenceManager;
import catering.util.LogManager;

public class EventManagerTest {

    private static final Logger LOGGER = LogManager.getLogger(EventTest.class);

    private static CatERing app;
    //private static User organizer;
    private static User chef;
    private static User staff;
    private static Menu menu;
    
    @BeforeAll
    static void initializeDatabase() {
        PersistenceManager.initializeDatabase("database/catering_init_sqlite.sql");
        app = CatERing.getInstance();
    }

    // Aggiungere un organizzatore e un menu già presente nel DB per i test con @BeforeEach
    @BeforeEach
    void setUpOrganizer() throws Exception {
        // Simula il login dell'organizzatore per i test che richiedono un utente autenticato
        app.getUserManager().fakeLogin("Giovanni");

        // chef = User.load("Antonio");
        // staff = User.load("Marco");
        // menu = Menu.load(1);
    }

    @Test
    @DisplayName("createEvent: happy path with valid name, verifies default values and persistence")
    void createEvent_and_ApproveEvent_VerifyValues() throws Exception {

        LOGGER.info("Starting test: createEvent_and_ApproveEvent_VerifyValues");

        EventManager eventManager = app.getEventManager();

        LOGGER.info("Creating recurring dates for the event");

        ArrayList<RecurringDate> recurrence = new ArrayList<>();

        RecurringDate recurringDate1 = new RecurringDate(
            Date.valueOf("2024-06-13"),
            Date.valueOf("2024-06-14")
        );
        recurrence.add(recurringDate1);

        RecurringDate recurringDate2 = new RecurringDate(
            Date.valueOf("2024-07-11"),
            Date.valueOf("2024-07-12")
        );
        recurrence.add(recurringDate2);

        RecurringDate recurringDate3 = new RecurringDate(
            Date.valueOf("2024-08-15"),
            Date.valueOf("2024-08-16")
        );
        recurrence.add(recurringDate3);

        LOGGER.info("Creating event with name, location, dates, recurrence, participants and type");

        Event event = eventManager.createEvent(
            "Matrimonio Pino e Pina",
            "Castello di Rivoli, Torino",
            Date.valueOf("2024-06-13"),
            Date.valueOf("2024-08-16"),
            Time.valueOf("18:00:00"),
            Time.valueOf("23:00:00"),
            recurrence,
            120,
            Event.EventType.MATRIMONIO
        );

        LOGGER.info("Verifying created event's properties");

        assertNotNull(event);
        assertEquals("Matrimonio Pino e Pina", event.getName());
        assertEquals("Castello di Rivoli, Torino", event.getLocation());
        assertEquals(120, event.getParticipants());
        assertEquals(Event.EventType.MATRIMONIO, event.getType());
        assertEquals(3, event.getRecurrence().getDates().size());

        LOGGER.info("Assigning chef, menu and staff to the event");

        chef = User.load("Antonio");
        eventManager.assignChef(chef);

        assertEquals(chef, event.getChef());
        assertEquals(chef.getId(), event.getChefId());

        Service service = eventManager.createService(
            "Apericena",
            "Offire bevande e stuzzichini", 
            Time.valueOf("18:00:00"),
            Time.valueOf("21:00:00")    
        );

        assertNotNull(service);
        assertEquals("Apericena", service.getName());

        menu = Menu.load(1);
        eventManager.assignMenu(service.getId(), menu);

        assertEquals(menu, service.getMenu());

        LOGGER.info("Approving the assigned menu");
        eventManager.approveMenu(service.getId());

        assertTrue(service.isMenuApproved(), "the menu should be approved after approval");

        LOGGER.info("Loading staff and assigning roles");
        staff = User.load("Marco");
        eventManager.addStaffRoleToService(staff, StaffRole.ServiceRole.BEVERAGE);

        // Verify that the staff role is correctly added to the service
        assertTrue(service.getAssignedStaff().stream().anyMatch(role -> role.getUser().equals(staff) && role.getServiceRole() == StaffRole.ServiceRole.BEVERAGE),
                "the staff should have the assigned role in the service");

        // Verify that the staff role is correctly associated with the service
        StaffRole assignedRole = service.getAssignedStaff().stream()
                .filter(role -> role.getUser().equals(staff) && role.getServiceRole() == StaffRole.ServiceRole.BEVERAGE)
                .findFirst() // should find the role we just added
                .orElse(null); // if null, the role was not found, which would be a failure

        assertNotNull(assignedRole, "the staff role should be associated with the correct service");
        assertEquals(1, service.getAssignedStaff().size(), "the service should contain exactly one assigned staff role");
        assertEquals(staff, assignedRole.getUser(), "the assigned role should reference the correct staff member");
        assertEquals(staff.getId(), assignedRole.getUserId(), "the assigned role should store the staff member id");
        assertEquals(StaffRole.ServiceRole.BEVERAGE, assignedRole.getServiceRole(), "the assigned role should be BEVERAGE");
        assertEquals(service.getId(), assignedRole.getServiceId(), "the staff role should be associated with the correct service");

        LOGGER.info("Approving the event");
        eventManager.approveEvent();
        assertTrue(event.getState() == EventServiceState.APPROVATO);
    }

    @Test
    @DisplayName("removeChef: removes assigned chef and verifies nullification")
    void removeChef_RemovesAssignedChefAndVerifiesNullification() throws Exception {
        EventManager eventManager = app.getEventManager();

        LOGGER.info("Creating event and assigning chef for removal test");



        Event event = eventManager.createEvent(
            "Evento per test removeChef",
            "Location di test",
            Date.valueOf("2024-09-01"),
            Date.valueOf("2024-09-01"),
            Time.valueOf("19:00:00"),
            Time.valueOf("22:00:00"),
            new ArrayList<>(),
            50,
            Event.EventType.PRIVATO
        );

        chef = User.load("Antonio");
        eventManager.assignChef(chef);

        assertEquals(chef, event.getChef(), "chef should be assigned before removal");

        LOGGER.info("Removing assigned chef");
        eventManager.removeChefAssignment(chef);

        assertEquals(null, event.getChef(), "chef should be null after removal");
    }

    @Test
    @DisplayName("modifyDates: updates event dates and verifies changes")
    void modifyDates_UpdatesEventDatesAndVerifiesChanges() throws Exception {
        EventManager eventManager = app.getEventManager();

        LOGGER.info("Creating event for date modification test");

        ArrayList<RecurringDate> recurrence = new ArrayList<>();

        RecurringDate recurringDate1 = new RecurringDate(
            Date.valueOf("2024-10-07"),
            Date.valueOf("2024-10-08")
        );
        recurrence.add(recurringDate1);

        RecurringDate recurringDate2 = new RecurringDate(
            Date.valueOf("2024-10-14"),
            Date.valueOf("2024-10-15")
        );
        recurrence.add(recurringDate2);

        RecurringDate recurringDate3 = new RecurringDate(
            Date.valueOf("2024-10-21"),
            Date.valueOf("2024-10-22")
        );
        recurrence.add(recurringDate3);

        Event event = eventManager.createEvent(
            "Evento per test modifyDates",
            "Location di test",
            Date.valueOf("2024-10-01"),
            Date.valueOf("2024-11-01"),
            Time.valueOf("19:00:00"),
            Time.valueOf("22:00:00"),
            recurrence,
            50,
            Event.EventType.PRIVATO
        );

        LOGGER.info("Modifying event dates");
        //eventManager.modifyEventDates(Date.valueOf("2024-11-01"), Date.valueOf("2024-11-01"));
        Date startDate = Date.valueOf("2024-11-01");
        Date endDate = Date.valueOf("2024-12-01");
        ArrayList<RecurringDate> updatedRecurrence = new ArrayList<>();

        RecurringDate updateRecurringDate1 = new RecurringDate(
            Date.valueOf("2024-11-01"),
            Date.valueOf("2024-11-05")
        );
        updatedRecurrence.add(updateRecurringDate1);

        RecurringDate updateRecurringDate2 = new RecurringDate(
            Date.valueOf("2024-11-08"),
            Date.valueOf("2024-11-12")
        );
        updatedRecurrence.add(updateRecurringDate2);

        eventManager.updateDate(startDate, endDate, updatedRecurrence);

        assertEquals(Date.valueOf("2024-11-01"), event.getDateStart(), "start date should be updated");
        assertEquals(Date.valueOf("2024-12-01"), event.getDateEnd(), "end date should be updated");
        assertEquals(2, event.getRecurrence().getDates().size(), "recurrence list " +
                "should be updated with new dates");
    }

    // Cleanup after any test the organizer's fake login to avoid side effects on other tests
    @AfterEach
    void tearDown() throws Exception {
        app.getUserManager().fakeLogin(null); // Simula il logout
        LOGGER.info("Cleaned up after test, logged out organizer");
    }
}
