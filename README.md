                                            Tema PCOM 
                                Web-Client-Communication-with-REST-API-C


Ciupitu Andi Mihai 
321CD

Am inceput tema de la scheletul laboratorului 9.

requests.c
-compute_get_request: am adaugat un char* token pentru a putea compune mesajul 
si cu acel JWT.
-compute_post_request: asemanator functiei de GET, am modificat semnatura 
functiei adaugand tokenul.
-compute_delete_request: este aceeasi functie ca cea de GET, doar ca am 
modificat GET in DELETE pentru a putea sterge cartea.

client.c
Pentru a putea folosi formatul JSON am ales sa folosesc biblioteca
furnizata in cerinta temei parson.c.

Functii ajutatoare folosite au fost:
-bool isNumeric(char* input) = verifica daca input este format doar din cifre
-bool containsSpace(const char *str) = verifica daca str contine spatii

Register: 
-citesc de la tastatura username-ul si parola utilizatorului si le adaug intr-un 
obiect json, trimitandu-le apoi intr-un POST request.
-daca codul actiunii incepe cu 2, inseamna ca actiunea s-a realizat cu succes.

Login:
-citesc de la tastatura username-ul si parola utilizatorului, le adaug intr-un 
obiect json si le trimit intr-o cerere de tip POST pentru logare catre server. 
-daca codul actiunii incepe cu 2, extrag cookie-ul de autentificare si il afisez.

Enter_library:
-pentru a verifica daca avem un utilizator autentificat, se analizeaza continutul
variabilei cookie.
-trimit request de tip GET pentru acces la librarie, iar daca codul actiunii 
incepe cu 2 extrag din mesajul primit de la server token-ul de autentificare si 
il afisez.

Get_books:
-pentru a verifica daca utilizatorul se afla in librarie, se analizaeaza 
continutul variabiei token.
-trimit request de tip GET pentru vizualizarea cartilor, iar daca codul actiunii 
incepe cu 2 extrag raspunsul de la server prelucrand lista cartilor si afisand 
id-ul si titlul pentru fiecare carte.

Get_book:
-pentru a verifica daca utilizatorul se afla in librarie, se analizaeaza 
continutul variabiei token, daca este ok se citeste de la tastatura un ID.
-trimit request de tip GET pentru vizualizarea cartilor, iar daca codul actiunii 
incepe cu 2 extrag raspunsul de la server prelucrand caracteristicile cartii si 
afisandu le in formatul specificat.

Add_book:
-pentru a verifica daca utilizatorul se afla in librarie, se analizaeaza 
continutul variabiei token.
-citesc campurile cartii pe care clientul doreste sa o adauge, verificand daca 
numarul de pagini este format doar din cifre si este numaru pozitiv.
-adaug intr-un obiect de tip json campurile cartii si il trimit printr-un 
request de tip POST catre server.

Delete_book:
-realizez o cerere de tip DELETE pentru cartea specificiata in biblioteca.

Logout:
-trimit o cerere de tip GET, daca codul actiunii incepe cu 2, inseamna ca 
delogarea s-a realizat cu succes.
-tot aici, initializez cookie si token cu "" pentru a stii ca nu este nimeni 
autentificat si in acelasi timp nici in librarie intrat.


Cam atat :=)


Ciupitu Andi Mihai 
321CD