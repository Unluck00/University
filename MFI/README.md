# Metodi Formali dell'Informatica (MFI)

## Parte 1
  
### 1. Sematica dei programmi 

#### Introduzione ai concetti fondamentali della semantica formale. Viene spiegato come un programma possa essere descritto matematicamente come una trasformazione degli stati della memoria, fornendo una base rigorosa per l'analisi e la verifica del software.

- Definito da una funzione: `S[⋅]: Act -> (Mem -> Mem)`
  - dove Mem rappresenta il dominio semantico della memoria, formato da degli stati della memoria; rappresentati da `σ` ∈ Mem
  - l'azione mappa la memoria in una nuova memoria: `f: Mem -> Mem`

- Si definisce:
  - una configurazione: `<q,σ> ∈ QxMem`
  - una transizione (o execution step): `<q,σ> =>α <q',σ'>`
    - può essere definita una chiusura riflessiva e transitiva, cioè estende una singola transizione a una sequenza di passi `ω` (anche zero passi)

- Una memoria (`Mem`) più strutturata è data da un set di varibili: `Var = {x1,..,xn}` e una n-tupla di valori `σ = (v_x1,..,v_xn)` tale per cui si ottiene una mappa `σ: Var -> Z`
    - si può estendere anche a un set di array A `σ: (Var U {A[i] | A ∈ Arr, 0≤i<length(A)}) -> Z`
- Quindi si ottiene il seguente set: `Mem = Var -> Z`, definita dalla seguente funzione ausiliaria:
  - aritmetica --> A[a]: Mem -> Z
  - espressione booleana --> B[b]: Mem -> Bool 

### 2. Program Graphs (PG) e Guarded Commands (GC)

#### Descrive la rappresentazione grafica dei programmi tramite grafi di controllo. I Guarded Commands vengono trasformati in Program Graph per visualizzare il flusso di esecuzione e facilitare l'analisi del comportamento del programma.

- E' una tupla `(Q, q_init, q_fin, Act, E)`
- Linguaggio formale del PG: `L(PG) = L(q_init ~> q_fin)(E)`

- Le proprietà del PG sono:
  - sistema deterministico 
    - sistema in cui le stesse azioni di partenza (input) producono sempre lo stesso risultato (output)
    - `<q_init,σ> =>ω' <q_fin,σ'>` e `<q_init,σ> =>ω'' <q_fin,σ''>` implica `σ' = σ''` e `ω' = ω''` 
  - localmente deterministico
    - si comporta in modo prevedibile solo nelle vicinanze di un punto o per brevi periodi, ma diventa imprevedibile nel tempo o su larga scala
    - `(q,α1,q1), (q,α2,q2) ∈ E` e `(α1,q1) ≠ (α2,q2)` implica `dom(S[α1]) ∩ dom(S[α2])= ∅` 
  - se PG,S è localmente deterministico allora è sistema deterministico, ma non sempre è viceversa (esiste un lemma e la prova della proposizione)

- I Commands (C) e i Guarded Commands (GC) sono definiti dalla grammatica, dove `a` e `b` sono rispettivamente aritmetica e espressione booleana

- Si associa il PG a GC tramite la funzione `edges(q_iniz ~> q_fin)(C)`; dove si hanno i seguenti casi:
  - caso di `assignment` o `skip`
  - caso di `sequential composition`
  - caso di `guarded command`
  - caso di `iteration command`

- Definizione della semantica dei GC
  - data la definizione della semantica `S[⋅]: Act -> (Mem -> Mem)`, si associano a tutti i GC
  - si estende anche agli array: `edges(q_iniz ~> q_fin)(A[a1] := a2) = (q_iniz, A[a1] := a2, q_fin)`

### 3. Semantica operazionale

#### Spiega come un programma viene eseguito passo dopo passo mediante configurazioni e regole di transizione, descrivendo l'evoluzione dello stato della memoria durante l'esecuzione.

- Si definisce la composizione della semantica con GC
  - in cui data la configurazione di un comando `c` e di una memoria `σ` si evolve in una nuova configurazione effettuando una serie di azioni `ω di Act*`: `(c,σ) ->ω (c',σ')`
  - quindi se inizia con `α` una sequenza di azioni `(c,σ) ->αω (c',σ')`, allora deve esistere un arco nel grafo che permetta la sua azione aggiornando la memoria `(q,σ) =>α (q'',σ'')` e continuando le restanti azioni `ω`: `(c'',σ'') ->ω (c',σ')`
  - con la semantica del comando `S[c]σ` che restituisce lo stato finale della memoria `σ'` oppure non lo raggiunge


### 4. Programmi annotati

#### Presenta l'uso delle annotazioni con precondizioni, postcondizioni e invarianti. Queste permettono di dimostrare formalmente la correttezza dei programmi, soprattutto in presenza di cicli.

- Si verificano i programmi tramite il suo comportamento in modo che sia conforme alle specifiche (quindi ai predicati detto asserzioni)

- La semantica dei perdicati è definita per induzione strutturale sulla sintassi delle espressioni
  - `_σ_` rappresenta le variabili logiche che sono mappate ai rispettivi valori numerici (interi Z)
  - `E[e](σ,_σ_)` rappresenta la funzione che calcola il valore semantico (il numero intero risultante) dell'espressione `e` sotto gli assegnamenti correnti.

- Il predicato si dice soddisfacibile se: `(σ,_σ_) ⊨ φ`, quindi la coppia `(σ,_σ_)` rende vero `φ` 

- L'assegnazione dei predicati è una mappa: `P: Q -> Pred`
    - c'è un esempio con il calcolo del fattoriale

- Il predicato è corretto (def. di Correttezza) se preserva la validità dei predicati attraverso ogni singola transizione del sistema: `(σ,_σ_) ⊨ P(q_iniz) ∧ S[α]σ = σ' => (σ',_σ_) ⊨ P(q_fin)`
- la parte logica `_σ_` rimane invariata
- la proprietà si estende a sequenza di passi di esecuzione `ω`
- La correttezza parziale di un PG è data da: `(σ,_σ_) ⊨ φ_iniz ∧ <q_iniz,σ> =>ω <q_fin,σ'> => (σ',_σ_) ⊨ φ_fin`
- Se P è corretto allora il PG è parzialmente corretto 
- L'assegnazione del predicato parziale è una mappa parziale `P: Q -> Pred` in cui si dice che P copre PG se: `q_iniz, q_fin ∈ dom(P)` e `per ogni loop in PG contiene un nodo nel dom(P)`

- Un short path fragment (spf) è una porzione di codice che calcola un percorso minimo (il più corto) tra due nodi all'interno di un grafo
- Nel PG è un cammino del tipo: `q_iniz α1 q1 α2 ... qn-1 αn qn` tale che `q_iniz, qn ∈ dom(P)` e `{q1,..,qn-1} ∩ dom(P) = ∅`
- Si ha il seguente lemma: se l'assegnazione P copre PG, allora l'insieme spf(P) è finito.
- Esempio di un algoritmo che computa spf(P)

- Introduzione ai programmi annotati (AP) per trovare un set di nodi che copra PG e per definire l'assegnazione del predicato parziale
- Estensione della funzione `edges` con AP: `edges(q_iniz ~> q_fin)(AP)` per i programmi annotati e restituendo la coppia `(E,P)` dove `E` rappresenta gli archi del PG e `P` rappresenta l'assegnazione del predicato parziale per PG
  - `P1[P2](q)` = `P1(q)` se lo stato `q` appartiene al dominio di `P1` ma non a quello di `P2`; altrimenti è = `P2(q)` se lo stato `q` appartiene al dominio di `P2` 
- `(E1,P1) ⊕ (E2,P2) = (E1∪E2, P1[P2])`
- Sia data la seguente proposizione: Dato un AP sia `edges(q_iniz ~> q_fin)(AP) = (E,P)` con `q_iniz ≠ q_fin`; allora `dom(P)` copre i nodi del PG con gli archi E

- L'analisi dei programmi è l'insieme di tecniche per rilevare le proprietà dei programmi in modo statico (senza esecuzione)
- Si introduce il concetto di memoria `(^σ1,^σ2) ∈ ^Mem` astratta, che rispetto alla memoria concreta `σ ∈ Mem` (in cui si mappa le variabili con i valori numerici, durante l'esecuzione), sostituisce i valori con delle proprietà per l'analisi 
- Esempio con il Rilevamento dei Segni:
  - permette di verificare condizioni di sicurezza critiche (come evitare che il divisore sia astratto come 0) 
  - in cui le variabili non contengono più numeri ma un set di segni `Sign = {-,0,+}` e gli array contengono i sottoinsiemi di questi segni
  - `(^σ1,^σ2) ∈ ^Mem = (Var -> Sign) x (Array -> P(Sign))`
  - si definisce una funzione di estrazione che permette di passare dalla memoria concetta a quella astratta: `η: Mem -> ^Mem con η(σ)=(^σ1,^σ2)`

- L'analisi delle assegnazioni permette di associare ad ogni nodo del PG un collezione di memoria astratta: `A: Q -> P(^Mem)`
- E' semanticamente corretta se:
  - per ognuno dei archi di `E` e per ogni stato di memoria concreta `σ ∈ Mem` si ha che: `η(σ) ∈ A(q_iniz) ∧ S[α](σ) = σ' => η(σ') ∈ A(q_iniz)`
  - `σ ∈ Mem_iniz => η(σ) ∈ A(q_iniz)`
  - la proprietà si estende a sequenza di passi di esecuzione `ω`

- Funzioni di analisi per Aexp (espressioni aritmetiche): `^A[⋅]: Aexp -> ^Mem -> P(Sign)`
- Funzioni di analisi per Bexp (espressioni booleane): `^B[⋅]: Bexp -> ^Mem -> P(Bool)`
- Sia data la seguente proposizione: `per ogni a ∈ Aexp, b ∈ Bexp, σ ∈ Mem` si ha che `sign(A[a](σ)) ∈ ^A[a](η(σ))` e `B[b](σ) ∈ ^B[b](η(σ))`
- Funzioni di analisi per Act: `^S[⋅]: Act -> P(^Mem) -> P(^Mem)`

- Una specifica di un analisi del programma consiste: 
  - nell'analisi del dominio, che è un un powerset `(P(^Mem),⊆)`
  - una funzione di analisi `^S[⋅]: Act -> P(^Mem) -> P(^Mem)` tale che `^S[⋅]` è monotica per tutti gli α ∈ Act: `^M ⊆ ^M' => ^S[α](^M) ⊆ ^S[α](^M')` 
  - un set `^M_init ∈ P(^Mem)` di memoria astratta tenuta inizialmente 

- La correttezza semantica è una proprietà logica e informatica che garantisce la corrispondenza corretta tra la struttura (la sintassi o il codice) e il suo significato effettivo
- Una specifica di un'analisi (`^S[⋅]`, `^M_init`) è corretta semanticamente se 
  - `σ ∈ M_init => η(σ) ∈ ^M_init`
  - `η(σ) ∈ ^M ∧ S[α](σ) = σ' => η(σ') ∈ ^S[α](σ)`
- L'analisi delle assegnazioni `A: Q -> P(^Mem)` è computazionalmente valido o solution se 
  - `(q_init,α,q_fin) ∈ E => ^S[α](A(q_init)) ⊆ A(q_fin)`
  - `^M_init ⊆ A(q_init)` 
- Se `^S[⋅]`, `^M_init` è corretta semanticamente e `A` è computazionalmente valida, allora A è semanticamente corretta
- Esempio di un algoritmo che computa le solutions

- Linguaggio base della sicurezza
- Riservatezza impedisce che i dati privati finiscano nel dominio pubblico
- Integrità impedisce che i dati fidati vengano influenzati o alterati da dati non affidabili

- Un flusso relazionale è una relazione riflessiva e transitiva data da `⊆ (Var ∪ Array)`
- Un flusso esiste tra due insiemi X e Y (X ->> Y) se e solo se ogni elemento di X ha un flusso verso ogni elemento di Y.

- Semantica del Monitor di Riferimento (S1) in cui si descrive un approccio di sicurezza dinamico in cui la semantica viene modificata, quindi se un'istruzione viola le politiche di sicurezza allora l'esecuzione si interrompe
- si indica sia in maniera esplicita che implicita, ma entrambi definiti nel seguente modo `S[x := a](σ)` è uguale al fatto che sia definita `A[a](σ) e fv(a) ->> {x}` oppure sia indefinita
  - dove `fv(a)` indica l'insieme delle variabili libere contenute nell'espressione a, viene indicato il flusso tra queste varibili e l'insieme di x
- Indichiamo dei flussi implici usando la funzione `edges_s` (che ricordiamo che produce un insieme di archi per un PG) associando ad ognuno di essi delle azioni che indicano il flusso richiesto
  - es. `edges_s(q_init ~> q_fin)[x := a](X) = {(q_init, x := a{X}, q_fin)}`
- Introduzione del parametro addizionale `d` (nella funzione `edges_s2`) che permette di tenere traccia dei test booleani precedenti (accumula informazioni)

- Differenza tra Semantica che non implementa il Monitor di Riferimento (S0) e quello invece che lo implementa (S1)
  - S0 l'assegnamento ha successo sempre, purché le espressioni siano valutabili correttamente.
  - S1 il monitor verifica che il flusso di informazioni sia sicuro tramite la relazione di policy ->>

- Analisi della Sicurezza è la parte statica della Semantica del Monitor di Riferimento, usata per verificare l'integrità del flusso di informazioni del programma, basata sulle funzioni implicite `S[⋅](X)` e `S2[⋅](d,X)` che restituiscono le condizioni di flusso
- Esiste un lemma che garantisce le informazioni sulle variabili non vadano perse durante le transizioni del programma e che ogni modifica sia correttamente tracciata.
  - Non dimentica le variabili che facevano già parte dello stato di analisi (X ⊆ X').
  - Qualsiasi variabile o array che subisce una modifica viene "coperto" e giustificato dalle variabili che hanno contribuito a calcolare il suo nuovo valore.
- Esiste una proposizione di correttezza se `S[C](X)` e `<q,σ> =>_0ω <q',σ'>` allora `<q,σ> =>ω <q',σ'>`

- Sicurezza Multi-livello applica il reticolo (lattice), in cui rappresenta un set parzialmente ordinato `(L, ⊆)` in cui ogni coppia di elementi `l,l' ∈ L` ha sempre un estremo inferiore massimo (glb) e un estremo superiore minimo (lub) 
  - l'elemento glb è <= a `l` e `l'`, se c'è un terzo elemento `l''` anch'esso <= a `l` e `l'` allora `l''` dev'essere <= a glb
- Un reticolo viene rappresentato tramite un grafo aciclico diretto (DAG) `(L, ->)`
- Classificazione di sicurezza è data da una mappa `L_sc: (Var ∪ Array) -> L` 
- Estensione per la parte del lattice completo


## Parte 2

### 5. Linguaggio IMP

#### Introduce IMP, un semplice linguaggio imperativo utilizzato come modello teorico. Ne vengono illustrati i costrutti fondamentali, come assegnamenti, sequenze, condizioni e cicli.

- IMP è un linguaggio di comandi generati dalla grammatica `c ∈ C`
- E' un sottoinsieme dei GC se si interpretano gli if e while nel seguente modo:
  - `if b then c1 else c2 == if b -> c1 [] ¬b -> c2 fi`
  - `while b do c == do b -> c od`
- Si introduce una semantica composizionale di IMP in cui un comando `c` dipenda più direttamente dal significato dei suoi componenti (le singole parti definiscono il significato globale)
  
- ATTENZIONE: si andrà a vedere un tipo di struttura dove è formata da premessa (parte sopra) e conclusione (parte sotto), quindi se la premessa è verificata e veritiera allora è verificata la sua conclusione

### 6. Semantica Small-Step

#### Analizza l'esecuzione dei programmi attraverso singole transizioni elementari. Ogni istruzione viene scomposta nei passaggi necessari fino al completamento dell'intero programma.

- La semantica small-step definisce uno step intermedio dato da una relazione con la configurazione `(c,σ)` (con `c` IMP command e `σ` stato della memoria)
- La relazione `(c1,σ1) -> (c2,σ2)` induce in un sistema di transizione `Conf` delle configurazioni: `(Conf, ->)`; il quale è rappresentato da una computazione di singole step da ridurre `(c1,σ1) -> (c2,σ2) -> .. -> (cn,σn)` finchè non si può più ridurre (allora termina) `(cn,σn) -/>`

- Può essere definita una chiusura riflessiva e transitiva, cioè estende una singola transizione a una sequenza di passi `->*` (anche zero passi)

- Si può confontare la semantica small-step (sistema di transizione su configurazioni) e la semantica di GC (sistema di transizione etichettato `(Q,=>,Act)`) definendo un sistema di transizione etichettato tra le due 
- l'etichetta in `Act` è definito come: `Act = {x := a | x ∈ Var ∧ a ∈ Aexp}∪ Bexp ∪ {skip}`
- es. `(x:=a, σ) ->x:=a (skip, σ[x -> A[a]σ)`
- Si definisce `ω` come sequenza di etichette, sia `-ω-` una sequenza ottenuta rimuovendo le occorrenze del comando `skip` da `ω`, allora si dice che le due sequenze `ω e ω'` sono equivalenti a meno di skip `ω ~skip ω'` se `-ω- = -ω-'`
- Esiste un lemma che definisce la correttezza (soundness) o la corrispondenza semantica tra l'esecuzione di un programma basata su PG e la sua semantica small-step. 
  - Stabilisce che se un percorso nel grafo dei controlli permette di passare da uno stato iniziale a uno stato finale con una determinata traccia di azioni, allora lo stesso comportamento può essere replicato passo dopo passo nella semantica del linguaggio di programmazione formale `c`

### 7. Semantica Big-Step

#### Descrive l'esecuzione considerando direttamente il risultato finale del programma. A differenza della Small-Step, non mostra i singoli passaggi ma solo la trasformazione complessiva dello stato.

- Descrive come l'intera esecuzione del programma di un comando `c` a partire da uno stato iniziale `σ` porti ad uno stato finale `τ`: `(c,σ) => τ`
- Dato `D` che è una derivazione di `(c,σ) => τ` si scrive `D:: (c,σ) => τ`, e `⊢(c,σ) => τ` stabilisce che l'esecuzione è valida perché esiste almento una derivazione `D` che la supporta

- Si definisce una proprietà `(=>)` della non-terminazione di un ciclo infinito nella semantica (es, loop = while true do skip), quindi `(loop,σ) => τ` non è derivabile e lo si indica `/⊢(c,σ) => τ`
  - dimostrazione per assurdo l'esistenza di una derivazione `D` del loop

- Definizione della proprietà `(=>)` che è deterministico: `⊢(c,σ) => τ1 ∧ ⊢(c,σ) => τ2 ==> τ1=τ2`, in cui se partiamo da una configurazione iniziale composta da un comando `c` e uno stato `σ`, la valutazione porterà a un unico risultato finale `τ`
  - dimostrazione per induzione strutturale sulle derivazioni `D1 e D2` rispettivamente su `τ1 τ2` (che usano la stessa regola determinata dalla struttura semantica di `c` e `σ`)

- Proprietà semantica equivalente alla small-step per quanto riguarda la terminazione, quindi si ha `(c,σ) => τ <=> (c,σ) ->* (skip, τ)`
  - uso di un lemma1 per dimostrazione dell'equivalenza semantica tra small-step (singolo passo di transizione `->`) e big-step (configurazione successiva valuta a uno stato finale `=>`): `(c,σ) -> (c',σ') ∧ (c',σ') => τ ==> (c,σ) => τ` 
  - uso di un lemma2 per dimostrazione della proprietà di composizionalità sequenziale nella semantica small-step, stabilisce che l'esecuzione di un comando non viene influenzata o interrotta dalla presenza di altri comandi successivi: `(c,σ) ->* (c',σ') => ∀c''. (c,c'',σ) ->* (c';c'',σ')` 
  - teorema della semantica small-big `(c,σ) ->* (skip, τ) => (c,σ) => τ` con lemma 1
  - teorema della semantica big-small `(c,σ) => τ ==> (c,σ) ->* (skip, τ)`
  
- Corollario univoco come quello sopra che dimostra che big-step è deterministico `(c,σ) => τ1 ∧ (c,σ) => τ2 ==> τ1=τ2` provato dalle proprietà e teoremi subito sopra

### 8. Equivalenza dei programmi

#### Spiega quando due programmi possono essere considerati equivalenti, ossia quando producono gli stessi risultati a partire dagli stessi stati iniziali, pur avendo strutture differenti.

- Introduzione `[c]σ` è uguale a `τ se (c,σ) => τ` oppure `⊥` (che rappresenta un valore non definito o la non-terminazione del programma; è una funzione parziale che non è definita per tutti gli elementi del suo dominio (cioè degli stati))

- Si ha la seguente definizione: `c~c' <=> ∀σ ∈ StatiPossibili [c]σ = [c']σ` (quindi determinano la stessa funzione parziale sugli stati (sia elementi che vengono associati che quelli non)) 

### 9. Verifica dei programmi

#### Introduce il problema della verifica formale, cioè la dimostrazione che un programma soddisfi una determinata specifica attraverso metodi matematici.

- Si introducono le pre-condizioni `φ ∈ Pred` e post-condizioni `ψ ∈ Pred`, inoltre `c ∈ Com` è weak correctness se: 
  - per ogni `σ,τ ∈ Mem` si ha `φ` è vero per `σ` e l'esecuzione di `c` in `σ` termina con `τ` allora `ψ` è vero per `τ`
- Definito come tripla di Hoare `{φ}c{ψ}`, se prima dell’esecuzione vale `φ` e il programma termina, allora dopo vale `ψ`

- Si ha la seguente definizione di soddisfacibilità e validità della tripla `(σ,_σ_) ⊨ {φ}c{ψ}`:
  - quindi la coppia `(σ,_σ_)` rende vero (soddisfacibile) `{φ}c{ψ}`, se `(σ,_σ_) ⊨ φ ∧ (c,σ) => τ ==> (τ,_σ_) ⊨ ψ`
  - la tripla `{φ}c{ψ}` è valida `⊨ {φ}c{ψ}` se `∀(σ,_σ_). (σ,_σ_)⊨{φ}c{ψ}`
- Assumendo che `⊥` soddisfi qualsiasi post-condizione di vacuità `(⊥,_σ_) ⊨ ψ` allora si definisce la soddisfacibilità della tripla come segue: `(σ,_σ_) ⊨ φ ==> ([c](σ),_σ_) ⊨ ψ`
  - se il comando `c` non termina `[c](σ) = ⊥` allora `(c,σ) =/> τ` (non converge a nessun stato finale), quindi rende l'antecedente dell'implicazione `(σ,_σ_)` falso (premessa legata alla terminazione corretta fallisce) e quindi (nella logica formale) è sempre vacuamente `{φ}c{ψ}` vera 


### 10. Logica di Hoare

#### Presenta il principale strumento per la verifica della correttezza dei programmi. Attraverso le triple di Hoare e le relative regole è possibile dimostrare formalmente il comportamento corretto dei programmi.

- La logica di Hoare è un sistema composto da regole che invocano la tripla `{φ}c{ψ}` come assioma o per dedurre una conclusione data dalla premessa;
- Le regole sono composizionali in cui la conclusione `c` è fatto da insieme di sotto-comandi `c1,..,cn` nelle premesse
- Si scrive `⊢{φ}c{ψ}` quando esiste una derivazione della tripla `{φ}c{ψ}` usando le regole 

- Diverse regole con dimostrazioni vengono analizzate, inoltre vengono dimostrati i lemmi di sostituzione

### 11. Correttezza parziale e totale

#### Distingue tra correttezza parziale, che garantisce il risultato solo se il programma termina, e correttezza totale, che richiede anche la dimostrazione della terminazione.

- La correttezza è dato da: `⊢{φ}c{ψ} => {φ}c{ψ}` quindi se la tripla è derivabile allora è semanticamente valida `⊨{φ}c{ψ}`
- La correttezza di HL è dato dal seguente teorema: `⊢{φ}c{ψ} => ⊨{φ}c{ψ}`; Lo si prova per induzione sulla derivata `D` della tripla (con i diversi esempi)

### 12. Weakest Liberal Precondition (WLP)

#### Introduce il concetto di precondizione più debole, ovvero la condizione minima che deve essere soddisfatta prima dell'esecuzione affinché, se il programma termina, la postcondizione risulti vera.

- Definizione: `wlp(c, ψ) = {(σ,_σ_) | ∀τ.(c,σ) => τ ==> (τ,_σ_) ⊨ ψ}`; insieme degli stati iniziali tali che se il programma termina, allora dopo vale ψ
- Dimostrazione del lemma per l'espressività che stabilisce che la wlp può essere espressa come un predicato formale: `∀(σ,_σ_). (σ,_σ_) ⊨ φ <==> (σ,_σ_) ∈ wlp(c, ψ)` (equivalenza semantica) e `⊢φ <-> wlp(c, ψ)` (dimostrabilità)
- Dimostrazione di un altro lemma `⊢ {wlp(c, ψ)} c {ψ}` con le varie dimostrazioni per induzione su `c`

- Condizioni di Verifica (per la verifica formale nella logica di Hoare), in cui si vuole dimostrare la validià della tripla `⊨{φ}c{ψ}` calcolando la `wlp(c, ψ)` il problema si riduce a verificare `φ => wlp(c, ψ)` (non induttivo per il caso while)
  - nel caso while si introduce una sintassi estesa `(ACom)` in cui ogni ciclo while viene "annotato" con un'invariante `{X}`
  - dato il comando annotato `C` e una post-condizione `ψ`, si generano due elementi: `pre(C,ψ)` (Una precondizione calcolata) e `vc(C,ψ)` (Le condizioni di verifica)
- Teorema della correttezza della Condizione di Verifica: `vc(C,ψ) => ⊢ {pre(c, ψ)} -c- {ψ}`; dimostrazione per induzione strutturale su `c`

### 13. Analisi statica

#### Descrive le tecniche che permettono di studiare le proprietà di un programma senza eseguirlo, individuando possibili errori o comportamenti indesiderati in fase di compilazione.

### 14. Interpretazione astratta e Sign Analysis

#### Spiega come sia possibile analizzare un programma utilizzando informazioni astratte invece dei valori esatti. La Sign Analysis, ad esempio, considera soltanto il segno delle variabili (positivo, negativo o zero).

### 15. Correttezza dell'analisi astratta

#### Illustra il requisito fondamentale dell'interpretazione astratta: l'analisi deve essere conservativa, cioè includere tutti i comportamenti possibili del programma reale, anche a costo di perdere precisione.

### 16. Language-Based Security

#### Introduce i principi della sicurezza dei linguaggi di programmazione, con particolare attenzione alla protezione delle informazioni riservate e al controllo dei flussi di dati.

### 17. Security Lattices

#### Presenta i reticoli di sicurezza, utilizzati per classificare dati e utenti in diversi livelli di riservatezza o integrità e per regolare i flussi di informazione consentiti.

### 18. Non-Interference

#### Descrive la proprietà secondo cui le informazioni riservate non devono influenzare i dati pubblici osservabili, garantendo così la confidenzialità del sistema.