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

Non è possibile visionare gli imports effettuati dal programma usando pestudio.

### Stringhe

[![Stringhe in pestudio](./Screenshots/pestudio-strings.png)]()

Troviamo una lunga lista di stringhe che pestudio ci segnala come potenziali indicatori di un comportamento malevolo. Quello che possiamo vedere prima di tutto è che il malware, a giudicare da alcune stringhe che costituiscono il nome delle API di Windows e di registri, piò gestire connessioni di rete, recuperare alcune informazioni sull'hardware della macchina infetta, prendere informazioni sui monitor e sulla finestra correntemente visualizzata (e cambiarla), modificare le chiavi del registro di windows, creare/distruggere processi, leggere e scrivere la clipboard. Tra le stringhe ci sono anche diversi riferimenti a "Shell traywnd" ovvero alla barra di windows. Altri riferimenti sono al processo Task Manager, al Prompt dei comandi e a Internet Explorer.
Il malware contiene anche un lungo elenco di tasti della tastiera (ad esempio Left, Right, Shift, [ESC], [F1]...[F8], [DEL], [INS]) che ci può far pensare ad una tastiera virtuale o più probabilmente ad una funzionalità di keylogging.
Un'altra cosa degna di interesse è la presenza di diverse format-strings che fanno pensare ad un codice in C/C++ o Delphi, un ulteriore indizio di ciò è la presenza tra le stringhe di tipi di variabili quali Boolean, Integer, Byte, Word.
E' poi presente una lunga lista di DLL quali kernel32, USER32, uxtheme, DWMAPI.
Compare anche un elenco di tutti i caratteri per la codifica Base64 e ci sono molte stringhe che sembrano pezzi di stringhe codificate.






===================================================
===================================================

# MALWARE
## Analisi statica di base
    In questa sezione dovete riportare i risultati dell’analisi statica del PE del malware utilizzando tool quali PEstudio, ExeInfoPE or PEID. In particolare dovete rispondere alle seguenti domande:
- OK Il malware e’ impacchettato? Se si quale packer e’ stato utilizzato? Quali  sono gli elementi del PE file che indicano che il malware e’ impacchettato?
- MANCA Quali API vengono importate dal malware? Qual’e’ un possibile comportamento del malware in base alle API importate?
- Quali stringhe sono contenute nel malware? Ci sono stringhe che possono che corrispondono a file o cartelle? Oppure sottochiavi del Windows registry? Oppure  URL? oppure indirizzi IP? 
- Alcune delle stringhe sono offuscate o cifrate? Quale codifica o algoritmo di cifratura viene utilizzato?
## Analisi dinamica di base
In questa sezione dovete riportare i risultati dell’analisi dinamica del malware utilizzando tools quali  Regshot, e ProcMon. In particolare dovete rispondere alle seguenti domande:
 
- Il malware crea o modifica chiavi o sottochiavi del Windows registry? Se si quali ?
- Il malware crea o cancella cartelle o file sulla macchina virtuale? Se si quali?
- Il malware crea qualche altro processo?
- Il malware e’ persistente sulla macchina virtuale? Se si, quale tecnica utilizza per raggiungere persistenza?
## Analisi del traffico di rete
In questa sezione dovete riportare i risultati dell’analisi del traffico di rete con Wireshark e Inetsim. In particolare, dovete rispondere alle seguenti domande:
- Il malware inizia connessioni di rete? Se si che tipo di traffico genera?  Quali URL or indirizzi IP tenta di contattare?
 
## Reverse engineering della funzione indicata
In questa sezione dovete spiegare il comportamento della funzione assegnata determinato facendo il reverse engineering del codice con IDA, Ghidra e x32dbg.

# IL DOCUMENTO MALEVOLO
## Analisi statica
In questa sezione dovete spiegare i risultati dell’analisi effettuata con strings, exiftool, e yara Poi presentare i risultati ottenuti con i tool di analisi specifici per la tipologi di documento. Per esempio, se il documento è un pdf riportare i risultati ottenuti con pdfid, pdf-parser e peepdf.
 
## Analisi del codice malevolo contenuto nel documento
 
In questa sezione dovete spiegare il comportamento dell’eventuale codice malevolo contenuto all’interno del documento.