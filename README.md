# Solar System Simulation (SFML)

Un simulatore orbitale interattivo del Sistema Solare sviluppato in **C++** utilizzando la libreria **SFML**. Il progetto si focalizza sulla rappresentazione dinamica dei moti planetari e sull'implementazione di effetti grafici fluidi.

## Funzionalità Attuali
- **Rendering in Fullscreen:** Adattamento automatico alla risoluzione nativa del monitor.
- **Sistema di Scie (Orbital Trails):** Ogni pianeta lascia una scia sfumata che ne traccia il percorso, con gestione intelligente della trasparenza.
- **Controllo del Tempo:** Possibilità di accelerare o rallentare la simulazione in tempo reale.
- **Pausa Intelligente:** Funzione di pausa (barra spaziatrice) che "congela" la fisica e le scie senza bug visivi al riavvio.
- **Sfondo Stellare Dinamico:** Generazione procedurale di stelle basata sulla risoluzione dello schermo.

## Tecniche Utilizzate
- **OOP (Programmazione a Oggetti):** Architettura pulita con classi dedicate per i corpi celesti.
- **Vettori e Matematica:** Calcolo delle orbite tramite funzioni trigonometriche e gestione dei riferimenti relativi (es. Lune che orbitano attorno ai pianeti).
- **Vertex Arrays:** Utilizzo di `sf::VertexArray` per gestire le scie in modo efficiente dal punto di vista delle prestazioni.

## Obiettivi Futuri (In fase di sviluppo)
- **Interattività Mouse:** Visualizzazione di informazioni dettagliate al passaggio del cursore (Hover) sui pianeti.
- **Focus Camera:** Funzione di zoom e inseguimento automatico dei corpi celesti.
- **Sistema di satelliti:** Implementazione di lune e orbite secondarie per tutti i giganti gassosi.

## Requisiti
- Compilatore C++ (standard 17 o superiore)
- SFML 3.0.0 (o versioni compatibili)