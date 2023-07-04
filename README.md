# codice-malevolo-report
Progetto d'esame per Codice Malevolo AA 2022/2023 - Matricola VR457811

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