# codice-malevolo-report
Progetto d'esame per Codice Malevolo AA 2022/2023 - Matricola VR457811

# Malware

## Analisi statica di base

### Informazioni generali ed impacchettamento

[![Remnux file comand](./Screenshots/remnux-file.png)]()

```
remnux@remnux:~/Desktop$ file '/home/remnux/Desktop/malware/malware' 
/home/remnux/Desktop/malware/malware: PE32 executable (GUI) Intel 80386, for MS Windows, UPX compressed
```

Vediamo innanzitutto che si tratta di un eseguibile per Windows a 32 bit dotato di interfaccia grafica e impacchettato con UPX.
Possiamo vedere l'impacchettamento tramite UPX anche aprendo il file con pestudio e verificando il campo signature.

[![Prima apertura con pestudio](./Screenshots/prima-apertura-pestudio.png)]()

Se vogliamo un'ulteriore conferma la possiamo trovare guardando il nome delle sezioni dell'eseguibile ed osservando nomi come UPX0 e UPX1:

[![sezioni file in pestudio](./Screenshots/pestudio-sections.png)]()

Spacchettiamo ora l'eseguibile utilizzando UPX e apriamo la versione spacchettata utilizzando nuovamente pestudio:

[![Seconda apertura con pestudio](./Screenshots/seconda-apertura-pestudio.png)]()

Vediamo che il campo signature è vuoto. La data di compilazione sembra essere domenica 15 aprile 2012. Utilizzando il programma ExeInfo PE riusciamo a vedere che la signature riporta [DA FARE]

Nella lista delle sezioni troviamo la sezione tls, la sua presenza potrebbe indicare che il malware ha incluso un controllo di anti-debug, questa cosa merita controlli approfonditi che faremo in seguito. E' presente anche una sezione risorse.

### Risorse

Tra le risorse troviamo alcuni file Delphi.

### Imports

Quello che possiamo vedere prima di tutto è che il malware include diversi import di API di Windows che gli consentono di gestire connessioni di rete, recuperare alcune informazioni sull'hardware della macchina infetta, prendere informazioni sui monitor e sulla finestra correntemente visualizzata (e cambiarla), modificare le chiavi del registro di windows, creare/distruggere processi, leggere e scrivere la clipboard.
Un import da notare è GetCapture, funzione che consente di catturare una porzione dello/gli schermo/i, questo è un indicatore del fatto che il malware potrebbe catturare degli screenshot ed inviarli ad un attaccante. Contiene anche una serie di API legate alla gestione delle risorse e questo può essere legato al fatto che nelle risorse abbiamo dei file Delphi. Non mancano API per la lettura e scrittura di chiavi del registro di Windows, così come GetTickCount che può risultare utile per tecniche di antidebugging visto che consente di accorgersi se l'esecuzione è step-by-step anziché "in tempo reale". Un'altra API utile al malware per proteggersi è Sleep che aiuta ad evadere il rilevamento da parte dell'antivirus.
Ci sono anche API utili per le connessioni di rete e per l'allocazione della memoria, per la cattura dello stato della tastiera (il che può essere utile per un keylogger ad esempio) e possibilità di aggiungere, rimuovere e ricercare stringhe nella Atom Table (per salvare l'elenco di tasti premuti?), per la gestione di file su disco (ricerca, eliminazione, eccetera).

### Stringhe

[![Stringhe in pestudio](./Screenshots/pestudio-strings.png)]()

Oltre alle stringhe legate alle API di cui abbiamo appena parlato troviamo una lunga lista di stringhe che pestudio ci segnala come potenziali indicatori di un comportamento malevolo. Tra le stringhe ci sono chiavi di registro ma anche diversi riferimenti a "Shell traywnd" ovvero alla barra di windows, al processo Task Manager, al Prompt dei comandi, a Internet Explorer e ad altre applicazioni.
Il malware contiene anche un lungo elenco di tasti della tastiera (ad esempio Left, Right, Shift, [ESC], [F1]...[F8], [DEL], [INS]) che ci può far pensare ad una tastiera virtuale o più probabilmente ad una funzionalità di keylogging, soprattutto quest'ultimo considerando che contiene anche OnKeyDown, OnKeyPressed, OnKeyUp.
Un'altra cosa degna di interesse è la presenza di diverse format-strings che fanno pensare ad un codice in C/C++ o Delphi, un ulteriore indizio di ciò è la presenza tra le stringhe di tipi di variabili quali Boolean, Integer, Byte, Word.
E' poi presente una lunga lista di DLL quali kernel32, USER32, uxtheme, DWMAPI.
Compare anche un elenco di tutti i caratteri per la codifica Base64 e ci sono molte stringhe che sembrano pezzi di stringhe codificate.
Tra le stringhe vediamo anche una lista di registri di sistema (o pezzi di essi) tra i quali SOFTWARE\Microsoft\Windows\CurrentVersion\Run che può essere usato per l'avvio automatico al boot di qualche risorsa creata dal malware o del malware stesso.
L'unico indirizzo IP che compare tra le stringhe sembra essere 127.0.0.1, quindi localhost, e non sono stati notati percorsi a file se non quelli ad applicazioni di Windows citati prima.
Non è presente tra le stringhe alcun URL completo ma compaiono sia "http://" che "www.", indicatori che degli url vengono costruiti ed utilizzati durante il funzionamento.

### Conclusioni dell'analisi statica

Il malware potrebbe essere un keylogger capace di impostarsi in avvio automatico, creare copie di sé stesso, eseguire screenshots e mandare il tutto ad un endpoint.

## Analisi dinamica di base

Eseguamo ora il malware dopo aver fatto un'istantanea dei registri di Windows con RegShot ed avviando Process Monitor.

[![ProcMon example](./Screenshots/procmon.png)]()

### Persistenza (Mitre ATT&CK Matrix)

[![Persistenza raggiunta](./Screenshots/persistenza.png)]()

Come vediamo dallo screenshot qui sopra riportato la persistenza viene raggiunta dal malware creando una copia di sé chiamata _Taskmgrk.exe_ al percorso _C:\Users\admin\AppData\Local\Temp\Temp_ ed aggiungendola al registro di Windows tra le applicazioni da avviare automaticamente all'avvio, fingendosi quindi il task manager di Windows per qualcuno che naviga nel file system o nel registro. Infine il malware avvia questa nuova copia di sé, lo vediamo nella penultima riga dello screenshot in cui vi è l'operazione di _Process Create_.

### Registro di Windows

Durante l'analisi statica sono state viste le seguenti chiavi di registro:

Chiavi di registro in strings:
- SOFTWARE\Borland\Delphi\RTL
- Software\Borland\Locales
- Software\Borland\Delphi\Locales
- SOFTWARE\Microsoft\Windows NT\CurrentVersion\FontSubstitutes
- SOFTWARE\Microsoft\Shared Tools\MSConfig\startupreg
- SOFTWARE\Microsoft\Shared Tools\MSConfig\startupfolder
- Software\Microsoft\Windows NT\CurrentVersion\Winlogon
- HARDWARE\DESCRIPTION\System\CentralProcessor\0
- SOFTWARE\Microsoft\Windows\CurrentVersion\Run

Sono stati trovati anche dei frammenti di chiavi di registro:
- HKLM
- HKCU
- CurrentVersion
- System\CurrentControlSet\Services\

Esseguendo l'analisi dinamica con _Process Monitor_ si è visto che in realtà il malware accede ad una quantità immensa di registri, possiamo vederne una piccola parte nel seguente screenshot:

[![RegOpenKey](./Screenshots/procmon-regopenkey.png)]()

La lista delle chiavi create è per fortuna molto più breve:

[![RegCreateKey](./Screenshots/procmon-regcreatekey.png)]()

Nessuna chiave esistente è invece stata rinominata.

### Registro di Windows - Privilege escalation (Mitre ATT&CK Matrix)
Una chiave che compare nell'elenco e che può destare preoccupazioni è _HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon_ con tipo di accesso _All access_: il malware può quindi modificare le procedure di login/logout dal sistema.

### Registro di Windows - Discovery (Mitre ATT&CK Matrix)
Un'altra operazione fatta riguarda 
_HKCU\SOFTWARE\MICROSOFT\WINDOWS\CURRENTVERSION\INTERNET SETTINGS\ZONES\1_ (e 2, 3, 4, 0), significa che il malware accede alle impostazioni internet.
Vengono cercate anche le chiavi _HKLM\SOFTWARE\MICROSOFT\CRYPTOGRAPHY\CONFIGURATION_ e _HKLM\SYSTEM\CONTROLSET001\CONTROL\COMPUTERNAME\ACTIVECOMPUTERNAME_ per leggere il guid ed il nome della macchina infettata.
L'ultima chiave del registro da notare è _
HKLM\System\CurrentControlSet\Control\NLS\Language_ per controllare la lingua del sistema.

### Creazione processi

Il malware non avvia processi oltre a quello visto per la persistenza.

### Creazione file

Il malware non scrive files oltre a quello visto per la persistenza.

## Analisi del traffico di rete

E' stato catturato molto traffico di rete dalla macchina in esame. Filtrando per cercare le chiamate ai DNS troviamo un url ventoclima.hopto.org che potrebbe essere sospetto e cercando a fondo si scopre che è riportato in diverse blacklist, ad esempio su https://github.com/stamparm/blackbook . Questo indirizzo è stato cercato ogni pochi secondi per tutta la durata dell'esecuzione del malware.

[![DNS](./Screenshots/wireshark-dns.png)]()

Non vengono effettuate connessioni di tipo SMTP.

[TODO: ricontrollare la configurazione di inetsim e verificare se è corretto che non effettui altre richieste]

## Reverse engineering della funzione sub_4835DC

[TODO]

# IL DOCUMENTO MALEVOLO

# Verifica del formato del file

[![Verifica del formato](./Screenshots/docx-file.png)]()

## Analisi statica

### Stringhe

[![strings](./Screenshots/docx-strings.png)]()

Tra le stringhe non troviamo informazioni particolarmente interessanti, in chiaro troviamo solo i nomi e percorsi dei vari file contenuti del docx, gli stessi che vedremo quando elencheremo il contenuto del pacchetto, oltre ad una lunga lista di stringhe incomprensibili.

### exiftool

[![exiftool](./Screenshots/docx-exiftool.png)]()

Una prima cosa da notare è che il file è stato realizzato partendo dal template Normal.dotm, tale estensione ci fa pensare che il file contenga una macro. Il nostro file è però effettivamente un docx e non un dotm.
Il creatore del file è un anonimo "Windows User".
Vediamo anche che il file è stato creato (o modificato) nel settembre 2018 ed abbiamo conferma del fatto che si tratta realmente di un docx. La data di modifica dello Zip, invece, non va presa in considerazione in quanto la mezzanotte del primo gennaio 1980 è sicuramente l'interpretazione come data di un dato assente.
Non abbiamo alcuna indicazione relativa alla localizzazione.

### Ricerca di codice VBA

[![zipdump](./Screenshots/docx-zipdump.png)]()
[![tree](./Screenshots/docx-tree.png)]()
[![olevba](./Screenshots/docx-olevba.png)]()
Come possiamo vedere con i comandi zipdump, tree (dopo la scompattazione) e olevba il docx in esame non contiene macro.

### Altro - Dynamic Data Exchange

Si potrebbe pensare che il documento non sia quindi malevolo, ma contiene invece un pericolo di tipo diverso. Esiste una vecchia tecnologia Microsoft che consente di eseguire del codice direttamente dall'interno di un documento Office, chiamata Dynamic Data Exchange (DDE). DDE è documentato in MITRE ATT&CK® T1559 ed è un protocollo nato per lo scambio di dati tra applicazioni del pacchetto Microsoft Office. Con le versioni più recenti di Office l'esecuzione di comandi DDE avviene solo dopo una richiesta di conferma all'utente, attenuando il problema legato alla sicurezza, tuttavia l'attaccante può spingere l'utente ad ignorare i messaggi di avviso e lasciare che venga eseguito il codice.

Procediamo quindi a verificare l'eventuale presenza di comandi DDE con un apposito comando preinstallato in REMnux: msodde.

[![msodde](./Screenshots/docx-msodde.png)]()

Vediamo così un comando che effettua il download di un software malevolo. L'url _trt.doe.louisiana.gov_ non è purtroppo più raggiungibile e non possiamo quindi analizzare il malware che era previsto dover essere scaricato ed eseguito sulla macchina.

===================================================
===================================================

# MALWARE
## OK Analisi statica di base
    In questa sezione dovete riportare i risultati dell’analisi statica del PE del malware utilizzando tool quali PEstudio, ExeInfoPE or PEID. In particolare dovete rispondere alle seguenti domande:
- OK Il malware e’ impacchettato? Se si quale packer e’ stato utilizzato? Quali  sono gli elementi del PE file che indicano che il malware e’ impacchettato?
- OK Quali API vengono importate dal malware? Qual’e’ un possibile comportamento del malware in base alle API importate?
- OK Quali stringhe sono contenute nel malware? Ci sono stringhe che possono che corrispondono a file o cartelle? Oppure sottochiavi del Windows registry? Oppure  URL? oppure indirizzi IP? 
- OK? Alcune delle stringhe sono offuscate o cifrate? Quale codifica o algoritmo di cifratura viene utilizzato?
## OK Analisi dinamica di base
In questa sezione dovete riportare i risultati dell’analisi dinamica del malware utilizzando tools quali  Regshot, e ProcMon. In particolare dovete rispondere alle seguenti domande:
 
- OK Il malware crea o modifica chiavi o sottochiavi del Windows registry? Se si quali ?
- OK Il malware crea o cancella cartelle o file sulla macchina virtuale? Se si quali?
- OK Il malware crea qualche altro processo?
- OK Il malware e’ persistente sulla macchina virtuale? Se si, quale tecnica utilizza per raggiungere persistenza?
## Analisi del traffico di rete
In questa sezione dovete riportare i risultati dell’analisi del traffico di rete con Wireshark e Inetsim. In particolare, dovete rispondere alle seguenti domande:
OK - Il malware inizia connessioni di rete? Se si che tipo di traffico genera?  Quali URL or indirizzi IP tenta di contattare?
 
## Reverse engineering della funzione indicata
In questa sezione dovete spiegare il comportamento della funzione assegnata determinato facendo il reverse engineering del codice con IDA, Ghidra e x32dbg.

# IL DOCUMENTO MALEVOLO
## Analisi statica
In questa sezione dovete spiegare i risultati dell’analisi effettuata con strings (OK) , exiftool (OK), e yara (MANCA, fatto con msodde) Poi presentare i risultati ottenuti con i tool di analisi specifici per la tipologia di documento. Per esempio, se il documento è un pdf riportare i risultati ottenuti con pdfid, pdf-parser e peepdf.
 
## OK Analisi del codice malevolo contenuto nel documento
 
In questa sezione dovete spiegare il comportamento dell’eventuale codice malevolo contenuto all’interno del documento.