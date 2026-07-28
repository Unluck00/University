package catering.businesslogic.event;

import java.sql.Date;
import java.util.List;
import java.util.logging.Logger;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Nested;
import org.junit.jupiter.api.Test;

import catering.businesslogic.CatERing;
import catering.businesslogic.user.User;
import catering.persistence.PersistenceManager;
import catering.util.LogManager;


/**
 * Tests for {@link Event}: in-memory aggregate behaviour and the static
 * loaders against the seeded SQLite database.
 */
class EventTest {

    private static final Logger LOGGER = LogManager.getLogger(EventTest.class);

    private static CatERing app;
    //private static User organizer;

    @BeforeAll
    static void initializeDatabase() {
        PersistenceManager.initializeDatabase("database/catering_init_sqlite.sql");
    }

    @BeforeAll 
    static void initializeApp() {
        app = CatERing.getInstance();
        LOGGER.info("Initialized CatERing instance for testing");
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

    @Nested
    class Aggregate {

        private Event event;
        private Service first;
        private Service second;

        @BeforeEach
        void setUp() {
            event = new Event("Test Event");
            first = new Service();
            first.setId(1);
            second = new Service();
            second.setId(2);
        }

        @Test
        void testName_SetInConstructor_IsReadable() {
            assertEquals("Test Event", event.getName());
        }

        @Test
        void testId_SetExplicitly_IsReadable() {
            event.setId(42);
            assertEquals(42, event.getId());
        }

        @Test
        void testChef_SetExplicitly_IsReadableAndExposesId() {
            User chef = new User();
            chef.setId(7);

            event.setChef(chef);

            assertEquals(chef, event.getChef());
            assertEquals(7, event.getChefId());
        }

        @Test
        void testServices_FreshEvent_IsEmpty() {
            assertTrue(event.getServices().isEmpty());
        }

        @Test
        void testAddService_RecordsContainment() {
            event.addService(first);

            assertTrue(event.containsService(first));
            assertFalse(event.containsService(second));
        }

        @Test
        void testRemoveService_DropsOnlyTheTargetedService() {
            event.addService(first);
            event.addService(second);

            event.removeService(first);

            assertFalse(event.containsService(first));
            assertTrue(event.containsService(second));
        }

        @Test
        void testDates_SetExplicitly_AreReadable() {
            Date day = Date.valueOf("2024-05-29");

            event.setDateStart(day);
            event.setDateEnd(day);

            assertEquals(day, event.getDateStart());
            assertEquals(day, event.getDateEnd());
        }
    }

    @Nested
    class StaticLoaders {

        @Test
        void testLoadAllEvents_ReturnsSeededEvents() {
            List<Event> events = Event.loadAllEvents();

            assertNotNull(events);
            assertFalse(events.isEmpty(), "the seed script must populate at least one event");

            Event sample = events.get(0);
            assertNotNull(sample.getName());
            assertNotNull(sample.getDateStart());
            assertNotNull(sample.getChef());
            assertNotNull(sample.getServices());
        }

        @Test
        void testLoadById_RoundTripsTheSameEvent() {
            Event sample = Event.loadAllEvents().get(0);

            Event loaded = Event.loadById(sample.getId());

            assertNotNull(loaded);
            assertEquals(sample.getId(), loaded.getId());
            assertEquals(sample.getName(), loaded.getName());
        }

        @Test
        void testLoadByName_FindsEventByExactName() {
            Event sample = Event.loadAllEvents().get(0);

            Event loaded = Event.loadByName(sample.getName());

            assertNotNull(loaded);
            assertEquals(sample.getName(), loaded.getName());
        }
    }
}
