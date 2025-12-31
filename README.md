# **FixItNow — Gestiune și Simulare Service Electrocasnice**

**FixItNow** este o aplicație complexă dezvoltată în C++20, concepută pentru administrarea unui service autorizat de electrocasnice. Aceasta gestionează administrarea personalului, ciclul de viață al cererilor de reparație și simularea în timp real a procesului de reparație, respectând cu strictețe principiile OOP și SOLID.

## **🏗️ Arhitectura Proiectului**

Proiectul este structurat pe trei straturi principale pentru a asigura o separare clară a responsabilităților:

* **Stratul Core**: Conține entitățile domeniului (Angajați, Electrocasnice, Cereri de Reparație) și logica de business.  
* **Stratul Factory**: Gestionează instanțierea controlată a obiectelor, administrând ID-urile unice și inițializările complexe.  
* **Stratul Management**: Conține logica centrală de orchestrare (ServiceManager) și interfața cu utilizatorul (SimpleMenu).

### **Structura Directoarelor**

* include/: Fișiere header (.hpp)  
* src/: Fișiere sursă (.cpp)  
  * core/: Entități de bază și derivate.  
  * factory/: Implementarea șablonului de proiectare Factory.  
  * manager/: Managerul de tip Singleton și logica UI.  
* tests/: Scenarii de testare și date CSV pentru validare.  
* build/: Fișiere obiect compilate.

## **🧬 Ierarhii de Clase**

### **1\. Angajați (Clasa de bază Employee)**

Sistemul de angajați calculează salariile dinamic pe baza salariului de bază (4000 RON), fidelității (5% la fiecare 3 ani lucrați) și locației (bonus de transport de 400 RON pentru cei cu domiciliul în afara Bucureștiului).

* **Tehnician**: Poate repara tipuri/mărci specifice de electrocasnice. Primește un bonus de 2% din valoarea reparațiilor finalizate.  
* **Recepționer**: Înregistrează și gestionează o listă cu ID-urile cererilor de reparație create.  
* **Supervizor**: Primește un spor de conducere de 20% aplicat salariului de bază. Se ocupă de raportări.

### **2\. Electrocasnice (Clasa de bază Appliance)**

* **Frigider**: Monitorizează dacă include sau nu un congelator.  
* **TV**: Reține dimensiunea diagonalei în cm sau inci.  
* **Mașină de spălat**: Monitorizează capacitatea de încărcare în kg.

### **3\. Cereri de Reparație**

Unitatea centrală a simulării. Face legătura între un anumit obiect Appliance, un Receptionist (creator) și un Technician (alocat). Calculează durata reparației în funcție de complexitate și vechimea aparatului.

## **🛠️ Șabloane de Proiectare (Design Patterns)**

### **Singleton (ServiceManager)**

Clasa ServiceManager este implementată ca un Singleton. Acționează ca baza de date centrală a aplicației, deținând toate colecțiile de tip unique\_ptr de angajați și cereri. Acest lucru asigură consistența datelor pe parcursul simulării.

### **Șablonul Factory**

Pentru a menține integritatea și a gestiona constructorii privați, obiectele sunt create exclusiv prin fabrici specializate:

* EmployeeFactory: Gestionează ID-urile unice pentru personal.  
* ApplianceFactory: Asigură specificațiile tehnice corecte pentru fiecare tip de dispozitiv.  
* RepairRequestFactory: Gestionează logica pentru prețul și durata estimată.  
* AddressFactory: Simplifică crearea adreselor de tip Casă sau Apartament.

## **⚙️ Aspecte Tehnice Importante**

### **Validare CNP**

CNPValidator implementează specificațiile oficiale românești:

* **Lungime și Cifre**: Verificare strictă pentru 13 caractere numerice.  
* **Cifra de Control**: Utilizează ponderile constante 279146358279 pentru verificarea sumei de control.  
* **Extracție**: Derivă automat anul nașterii, luna și sexul pentru a valida vârsta de angajare (minim 16 ani).

### **Sistemul de Simulare și "Tick"**

Simularea progresează în unități discrete numite "ticuri" (ticks).

1. **Auto-Alocare**: ServiceManager verifică cererile în stare Pending și le alocă către Tehnicienii disponibili care au competența necesară și mai puțin de 3 sarcini active.  
2. **Progres**: Fiecare tic de timp decrementează remaining\_time (timpul rămas) pentru toate cererile aflate InProgress.  
3. **Finalizare**: Când timpul ajunge la zero, statusul se schimbă în Completed, iar tehnicianul este eliberat.

### **Persistența Datelor**

Aplicația suportă salvarea și încărcarea întregii stări prin fișiere CSV. Include un parser CSV personalizat care gestionează câmpurile escapate și șirurile complexe (ex: listele de competențe ale tehnicienilor).

## **🚀 Primii Pași**

### **Cerințe Prealabile**

* Un compilator C++ care suportă standardul **C++20** (g++ pentru a folosit în Makefile).  
* Utilitarul de build make.

### **Compilare**

Rulați următoarea comandă în directorul rădăcină:

make

Aceasta va compila toate sursele în directorul build/ și le va lega într-un executabil numit app.

### **Rulare**

./app

## **📊 Raportare**

Aplicația generează trei tipuri de rapoarte CSV:

1. top3\_salaries.csv: Top 3 angajați cu cele mai mari salarii, sortați după nume.  
2. longest\_repair.csv: Date detaliate despre tehnicianul care se ocupă de cea mai complexă sarcină.  
3. pending\_requests.csv: Listă grupată și sortată a lucrărilor care așteaptă în coadă.

## **🧪 Testare**

Fișiere de test detaliate se găsesc în tests/. Acestea acoperă:

* **CNP-uri Valide/Invalide** pentru logica de angajare.  
* **Cazuri limită pentru Alocarea Cererilor** (încărcare maximă, lipsă competențe).  
* **Persistență** (Salvare și încărcare completă prin CSV).