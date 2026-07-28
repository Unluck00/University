# Progetto-SAS 25-26

## 1.0 Testo Progetto

## 2.0 UC Brevi

Dagli `Attori` agli `UC Brevi`

### 2.1 Glossario

Elenco di `Attori` e delle loro rispettive definizioni del progetto

### 2.2 Elenco Azioni

Creare lista azioni ordinate per `Attore`, organizzazione e uniforme

### 2.3 Attori e Azioni

Raggruppamneto per `Attore` e `“Complemento Oggetto”`

### 2.4 Attori e UC brevi

Rielaborazione di ogni macro gruppo con titolo e descrizione, Descrizioni di `UC brevi`

## 3.0 UC Dettagliati 

Dalle `User Story` agli `UC Dettagliati`

Si hanno 3 UC da fare: 
- UC di Esempio: Gestire i menù
- UC di Esercitazione: Gestire i compiti
- UC d' Esame: Gestire gli eventi

### 3.1 Rivedere UC Brevi

Verifica degli `Attori` coinvolti e i limiti del UC

### 3.2 Comprensioni di processi esistenti

Interviste con `Attori` coinvolti (o `User Stories`)
Il processo racconta una sessione tipo e da lì si estraggono i passi

### 3.3 Creazione di processo combinato

Creazione di scenario combinato con US simili 

### 3.4 Organizzazione di Scenari

Gli `scenari` descrivono le intenzioni (sequenza di azioni) dell'attore in una sessione di lavoro

Tra questi scenari si avrà lo `scenario principale di successo`, cioè uno scenario comune di attraversamento del Caso d’Uso, di successo e incondizionato. E' costituito da una sequenza di passi, che può contenere passi da ripetere, ma che
non comprende nessuna diramazione

## 4.0 Modello di Dominio e Diagrammi di Sequenza di Sistema (SSD)

Il Modello di Dominio è una rappresentazione visuale delle classi concettuali (oggetti del dominio). Include:
- ✓ Oggetti di dominio.
- ✓ Associazioni tra classi concettuali.
- ✓ Attributi di classi concettuali.
- X Operazioni

Il Diagramma di Sequenza del Sistema è un elaborato della disciplina dei requisitiche illustra eventi di input e di output relativi ai sistemi in discussione.

Il Sistema reagisce a: `Eventi esterni`, `Eventi temporali` e `Guasti o eccezioni`

Un SSD mostra: 
- L’attore primario del Caso d’Uso;
- Il sistema in discussione;
- I passi che rappresentano le interazioni tra il sistema e l’attore.

## 5.0 Contratti 

I contratti usano pre-condizioni e post-condizioni per descrivere nel dettaglio i cambiamenti agli oggetti in un Modello di Dominio dopo ogni operazione

Si crea un contratto per le operazioni complesse o i cui effetti sono sottili (non chiari dai Casi d’Uso)
Si scrivono le pre-condizioni e le post-condizioni:
- creazione o cancellazione di oggetti;
- formazione o rottura di collegamenti;
- modifica di attributi.


## 6.0 DCD e DSD

## Parole Chiavi

- `UP o Unified Process`: è un processo iterativo ed evolutivo (incrementale) per lo sviluppo del software per la costruzione di sistemi orientati agli oggetti. Le iterazioni iniziali sono guidate dal rischio, dal cliente e dall’architettura.
Divide lo sviluppo in 4 fasi, in ogni fase si costituisce una o più iterazioni e ogni iterazione cicla su 6 discipline.
Fasi: Ideazione, Elaborazione, Costruzione e Transazione

- `UC o Casi d'Uso (Users Case)`: I casi d’uso sono il modo migliore per definire i requisiti: il cliente racconta una storia e il programmatore la traduce in un caso d’uso.

- `Storie Utente (User Story)`: scenari d’uso in cui potrebbe trovarsi un utente. Il cliente lavora a stretto contatto con il team di sviluppo e discute di possibili scenari;

- `Discipline`: si ripetono in ogni iterazione, è un insieme di attività e dei relativi elaborati in una determinata area, come le attività relative all’analisi dei requisiti.

- `Artefatto (o elaborato)`:  è il termine generico che indica un qualsiasi prodotto di lavoro: codice, schemi di basi di dati, documenti di testo, diagrammi, modelli, etc.

- `Requisito`: è una capacità o una condizione a cui il sistema deve essere conforme.
Derivano da richieste degli utenti del sistema per risolvere dei problemi e raggiungere degli obiettivi.