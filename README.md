# numLock
Static programming language whose code can be written with the use of only nupad and nothing else.  
  
Syntax analisator -> done  
Semantic analisator -> done  
Abstract tree -> done 
Code generator -> done

# Općenito
NumLock je kompajlerski jezik. On koristi jedino tipke koje mogu biti pronađne na NumPad sekciji tastature. Ti dozvoljeni karakteri u ovom jeziku su:  
- Cifre od 0 do 9  
- Znak +  
- Znak –  
- Znak *  
- Znak /  
- Znak ,  
- Space  
Znakovi koje ignoriše NumLock su znakovi za prelazak u novi red i tab.  
Znak koji označava završetak neke naredbe jeste znak „-“.  
Komentari u NumLocku počinju sa „ , , “ (,,) i to je jednolinijski komentar.  
U ovom jeziku postoje samo numeričke vrijednosti, varijable su kodirane na način da počinju sa dvije nule, pa onda neki broj različit od nule, pa onda po želji više različitih brojeva, u zavisnosti od potrebe kodiranja.  
Ostale vrijednosti su obični brojevi koji ne smiju počinjati sa nulom.  
Komande kojima se kompajlira i pokreće kod u našem jeziku su: 1. cococpp numlock.atg -frames /usr/share/coco-cpp/ 2. g++ main.cpp Scanner.cpp Parser.cpp -o numl 3. ./numl main.numl 4. nasm -f elf test.asm 5. ld -m elf_i386 -o test test.o 6. ./test
Imati na umu da je main.numl ime file-a u kojem se pohranjuje NumLock kod te prilikom pokretanja ovih naredbi treba biti lociran u folderu gdje se on nalazi. Samo se ime tog file-a može mijenjati u ovim komandama.  

# Operatori
Operatori u NumLock-u su kodirani kao brojevi koji počinju sa jednom cifrom 0. Operatori koje podržava NumLock jezik su:
- Jednakost (dodjela, =) koje je kodirano kao „01“ i ono služi da bi se u deklarisanu varijablu mogla dodijeliti određena vrijednost.  
- Sabiranje (+) koje je kodirano kao naredba „02“ i ono sabira 2 strane ovog izraza. To omogućava da sa izraza sa obe strane može biti neki drugi izraz, a ne samo varijabla ili broj.  
- Oduzimanje (-) koje je kodirano kao naredba „03“ i ono oduzima 2 strane ovog izraza. To omogućava da sa izraza sa obe strane može biti neki drugi izraz, a ne samo varijabla ili broj.  
- Množenje (*) koje je kodirano kao naredba „04“ i ono množi 2 strane ovog izraza. To omogućava da sa izraza sa obe strane može biti neki drugi izraz, a ne samo varijabla ili broj.  
- Dijeljenje (/) koje je kodirano kao naredba „05“ i ono dijel lijvu stranu sa desnom. To omogućava da sa izraza sa obe strane može biti neki drugi izraz, a ne samo varijabla ili broj.  
- Manje (<) koje je kodirano kao naredba „06“ i ono poredi lijevu stranu sa desnom. Omogućeno je da se sa obe strane izraza može biti neki drugi izraz, a ne samo varijabla ili broj.  
- Veće (>) koje je kodirano kao naredba „07“ i ono poredi lijevu stranu sa desnom. Omogućeno je da se sa obe strane izraza može biti neki drugi izraz, a ne samo varijabla ili broj.  
- And (&&) koje je kodirano kao naredba „09“ i ono uzima istinitosne vrijednosti sa obe strane ovog operatora. Omogućeno je da se sa obe strane izraza može biti neki drugi izraz, a ne samo varijabla ili broj.  
- Poređenje na jednakost (==) koje je kodirano kao naredba „011“ i ono uzima istinitosne vrijednosti sa obe strane ovog operatora te daje rezultat istinitosti te komparacije. Omogućeno je da se sa obe strane izraza može biti neki drugi izraz, a ne samo varijabla ili broj.  
- Sabiranje i dodjela sabrane vrijednosti (=+) koje je kodirano kao naredba „012“ (kao kombinacija operatora „01“ (=) i „02“ (+)) je operator koji sabira vrijednost varijable sa lijeve strane sa proračunom izraza sa desne strane. S desne strane treba biti varijabla, dok s desne može biti neki konkretan izraz koji može rezultirati nekom vrijednošću.  
- Sabiranje i dodjela sabrane vrijednosti (=+) koje je kodirano kao naredba „012“ (kao kombinacija operatora „01“ (=) i „02“ (+)) i ovaj operator sabira vrijednost varijable sa lijeve strane sa proračunom izraza sa desne strane. S lijeve strane treba biti varijabla, dok s desne može biti neki konkretan izraz koji može rezultirati nekom vrijednošću.  
- Oduzimanje i dodjela oduzete vrijednosti (=-) koje je kodirano kao naredba „013“ (kao kombinacija operatora „01“ (=) i „03“ (-)) je operator koji oduzima vrijednost varijable sa lijeve strane sa proračunom izraza sa desne strane. S lijeve strane treba biti varijabla, dok s desne može biti neki konkretan izraz koji može rezultirati nekom vrijednošću.  
- Množenje i dodjela pomnožene vrijednosti (=*) koje je kodirano kao naredba „014“ (kao kombinacija operatora „01“ (=) i „04“ (*)) je operator koji množi vrijednost varijable sa lijeve strane sa proračunom izraza sa desne strane. S lijeve strane treba biti varijabla, dok s desne može biti neki konkretan izraz koji može rezultirati nekom vrijednošću.  
- Dijeljenje i dodjela podjenjene vrijednosti (=/) koje je kodirano kao naredba „015“ (kao kombinacija operatora „01“ (=) i „05“ (/)) je operator koji dijeli vrijednost varijable sa lijeve strane sa proračunom izraza sa desne strane. S lijeve strane treba biti varijabla, dok s desne može biti neki konkretan izraz koji može rezultirati nekom vrijednošću.  
- Manje ili jednako (=<) koje je kodirano kao naredba „016“ (kao kombinacija operatora „01“ (=) i „06“ (<)) je operator koji poredi vrijednost izraza sa lijeve strane sa proračunom izraza sa desne strane na način da vraća vrijednost istinitosti u slučaju da je lijeva strana manja ili jednaka od desne. Omogućeno je da se sa obe strane izraza može biti neki drugi izraz, a ne samo varijabla ili broj.  
- Veće ili jednako (=>) koje je kodirano kao naredba „017“ (kao kombinacija operatora „01“ (=) i „07“ (>)) je operator koji poredi vrijednost izraza sa lijeve strane sa proračunom izraza sa desne strane na način da vraća vrijednost istinitosti u slučaju da je desna strana manja ili jednaka od lijeve. Omogućeno je da se sa obe strane izraza može biti neki drugi izraz, a ne samo varijabla ili broj.  
- Poređenje na različitost (=!) koje je kodirano kao naredba „018“ (kao kombinacija operatora „01“ (=) i „08“ (!)) je operator koji poredi vrijednost izraza sa lijeve strane sa proračunom izraza sa desne strane na način da vraća vrijednost istinitosti u slučaju da je desna strana različita od lijeve. Omogućeno je da se sa obe strane izraza može biti neki drugi izraz, a ne samo varijabla ili broj.  
- Ispis na ekran je kodirano kao 000 i ono na ekran ispisuje varijablu koja se nađe sa desne strane ovog operatora. U slučaju potrebe ispisa varijable, preporučuje se spremanje rezultata u novu varijablu zbog toga što ispis može direktno da utiče na vrijednost varijable.  

  
# Petlje  
Petlje u jeziku NumLock su definisane na idući način:  
- Petlja počinje sa znakom *  
- Poslije znaka * ide broj iteracija koda petlje koja slijedi. Taj broj može biti vrijednost neke varijable, ili neki konkretan broj.  
- Idući znak poslije određenja broja varijabli jeste znak „/“ koji označava početak bloka kojeg programer želi da ponavlja u ovoj petlji.  
- Sada ide tijelo petlje koje se može sastojati od proizvoljnog broja iskaza.  
- Tijelo petlje zatvara se znakom „/“ i ovim petlja završava.  
Primjer petlje u jeziku NumLock:  
005-  
*5/  
005 012 1-  
/  
005 -> deklaracija varijable 005  
* -> početak petlje  
5 -> broj iteracija  
/ -> početak bloka  
005 012 1- -> iskaz koji uvećava vrijednost varijable 005 za 1  
/ -> kraj bloka  
  
# Uslovi  
Uslovi (if-ovi) u jeziku NumLock su definisani na idući način:  
- Uslov počinje sa znakom „+“  
- Poslije znaka „+“ ide uslov  
- Poslije uslova ide znak „/“  
- Poslije ide tijelo u koje se treba uću u slučaju da uslov bude ispunjen  
- Tijelo je zatvoreno sa „/“  
- Poslije ovoga može po želji da ide else  
- Else ide na način da se kuca „++“ i onda direktno piše „/“ što označava početak bloka. Razlika između if-a i else if-a te else-a leži u tome što se kod else nakon „++“ ne navodi uslov, nego se direktno piše tijelo uslova.  
Primjer uslova u NumLock-u:  
001- ,,deklaracija varijable 001  
001 01 1- ,,dodjeljivanje vrijednosti 1 varijabli 001  
+001 011 1 / ,,+ označava if; provjera da li je 001 jednako 1; otvaranje bloka sa „/“  
001 013 1- ,,oduzmi 1 od varijabl1 001  
/ ,,zatvori blok  
++/ ,,else sa ++ i sa odmah otvorenim blokom sa / (bez uslova)  
001 014 2- ,,pomnoži varijablu 001 sa 2  
/ ,,zatvori blok  
