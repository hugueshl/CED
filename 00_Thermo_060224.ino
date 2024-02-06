//+-------------------------------------------------------------+
//!                                                             !
//!                      Thermomix                              !
//!                                                             !
//+-------------------------------------------------------------+
//!                                                             !
//!  Version: Beta 0.0.0                                        !
//!  Date: 2024/02/06                                           !
//!  Auteurs: Ced/Hug                                           !
//!  Société: APP @peu-près                                     !
//!                                                             !
//+-------------------------------------------------------------+
//!                                                             !
//! Reste @ Programmer:                                         !
//!                                                             !
//! 01 - BP Arrêt   OK                                          !
//! 02 - Csgn de Vitesse Via Téléphone                          !
//! 03 - Wifi Tuya TC: Télécommandes                            !
//! 04 - Voyants Marche/Arrêt/Défauts                           !
//!                                                             !
//+-------------------------------------------------------------+
//!                                                             !
//!  Driver reglé sur oFF/on/on/on ced le 21012024_13h31        !
//!                                                             !
//+-------------------------------------------------------------+


// --- Input ---
const int PIN_NB = 4;
const int PIN_NH = 5;
const int PIN_BP_Bas = 6;
const int PIN_BP_Haut = 7;
const int PIN_BP_Arret = 3;


// Définition des broches utilisées pour le contrôle du moteur
const int ENA_PIN = 8;  // Pin pour activer le driver TB660
const int PULS_PIN = 9; // Pin pour les impulsions du moteur
const int DIR_PIN = 10; // Pin pour la direction du moteur
// const int oscilloscope = 2;


// Sauvegarde Etat des Entrées (état précédent du bouton)
 int Save_I_Det_NH = 1;  // Det_NH
 int Save_I_Det_NB = 1;  // Det_NB
 int Save_I_BP_Bas = 1;  // BP_Bas  
 int Save_I_BP_Haut = 1;  // BP_Haut  
 int Save_I_BP_Arret = 1;  // BP_Arret


// Initialize the pushbutton pin as an input:
 int I_NH = 0;
 int I_NB = 0;
 int I_BP_Bas = 0;
 int I_BP_Haut = 0;
 int I_BP_Arret = 0;


// -----------------------------------------------------------------------------------------------

int Etat_du_boutonCycle_precedent = HIGH;   // Variable pour stocker l'état précédent du bouton
const int Bouton_choix_rampe = 11;          // Définition de la broche utilisée et des variables pour le bouton poussoir choix rampe Acc
int Bouton_choix_rampe_State;               // Variable pour stocker la valeur de passage du bouton choix rampe Acc
int Etat_BoutonChoixRampe_precedent = HIGH; // Variable pour stocker l'état précédent du bouton choix rampe Acc
int memoire_Acc = 1;                        // Creation d'une memoire
int passage = 0;                            // Variable de passage


// Définition pour la broche LED
const int LED_rampe = 3; // LED_rampe sert d'indicateur visuel pour montrer si la rampe d'accélération est activée ou désactivée.

// Définition de la tempo en microsecondes
const int TEMPO = 70;  // old 250    80---->450tr/min   60 ----> 600tr/min   55 ---> 620tr/min   50 ---> 720tr/min  40 ---> 890tr/min  --->20 et max 13 apres bug! 

int nombre_de_pas_a_realiser = 79900;  // old 10000 Variables pour contrôler le mouvement du moteur

unsigned long tempsPrecedent = 0;  // Variable pour stocker le temps précédent


int pasActuels = 0; // Variable pour compter les pas actuels

bool moteurEnMarche = false;  // État du moteur

// Variable pour la rampe
const int rampe_max_initial = 450;  // OLDCED 2000   old 5000    1500 environ 1sec5 / modif a 800 good ced/ mofi a 300 pas mal
int rampe_max = rampe_max_initial;
unsigned long derniere_mise_jour_rampAcc = 0;
const int intervalle = 9;  // Temps en millisecondes entre chaque décrémentation/ modif de 0 a 4 ced/ modif a 5 pas mal

// Cette fonction est exécutée une seule fois au démarrage de l'Arduino.
void setup() {
  // Initialise le port série pour le débogage
  // Serial.begin(115200);

  // Configuration des pattes en sortie
  pinMode(ENA_PIN, OUTPUT);
  pinMode(PULS_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  
//pinMode(oscilloscope, OUTPUT);
 
 // Configuration des pattes en entrée
  pinMode(PIN_NB, INPUT_PULLUP);
  pinMode(PIN_NH, INPUT_PULLUP);
  pinMode(PIN_BP_Bas, INPUT_PULLUP);
  pinMode(PIN_BP_Haut, INPUT_PULLUP);
  pinMode(PIN_BP_Arret, INPUT_PULLUP);
  pinMode(Bouton_choix_rampe, INPUT_PULLUP);
  

  // Mettre la broche ENA a 1
 digitalWrite(ENA_PIN, HIGH);  // mise a 1 pour valider le driver
 digitalWrite(DIR_PIN,LOW);   // High pour changer de Sens  <<<<<<<<<<<<<<<<<<<<<<<<< ICI A TESTER METTRE HIGH
}

// Cette fonction est exécutée en boucle sans fin après le démarrage de l'Arduino.
void loop() {

  // lit l'état actuel de la broche sur laquelle est connecté le bouton de choix de la rampe d'accélération.
 Bouton_choix_rampe_State = digitalRead(Bouton_choix_rampe);         
  if (Bouton_choix_rampe_State != Etat_BoutonChoixRampe_precedent) {  
    if (!Bouton_choix_rampe_State) {                                  
      memoire_Acc = !memoire_Acc;  // Inversion de la valeur de la memoire                                          
    }
  }
  Etat_BoutonChoixRampe_precedent = Bouton_choix_rampe_State; // L'opération consiste à mémoriser l'ancienne valeur.


  // Contrôle de la LED en fonction de memoire_Acc
  if (memoire_Acc == 1) {
    digitalWrite(LED_rampe, HIGH);  // Allume la LED si memoire_Acc est égal à 1
  } else {
    digitalWrite(LED_rampe, HIGH);  // Eteint la LED si memoire_Acc n'est pas égal à 1
  }


  // Lire l'état actuel du bouton depart cycle
   I_BP_Haut = digitalRead(PIN_BP_Haut);
   I_BP_Bas = digitalRead(PIN_BP_Bas);
   I_BP_Arret = digitalRead(PIN_BP_Arret);
   I_NH = digitalRead(PIN_NH);
   I_NB = digitalRead(PIN_NB);
  
    
  
  // ------ CDE DESCENTE ---------
  if (Save_I_BP_Bas == LOW && I_BP_Bas == HIGH && I_NB == 1) { // Vérifier si le BP Bas vient d'être relâché
	digitalWrite(DIR_PIN,LOW);  //  Sens de Rotation Bas
    moteurEnMarche = true;      // Le bouton vient d'être relâché, démarrer le cycle
    pasActuels = 0;
  }
  Save_I_BP_Bas = I_BP_Bas;  // Mettre à jour l'état précédent du bouton depart cycle


 // ------ CDE MONTEE ---------
  if (Save_I_BP_Haut == LOW && I_BP_Haut == HIGH) { // Vérifier si le BP Haut vient d'être relâché
	digitalWrite(DIR_PIN,HIGH);  //  Sens de Rotation Haut
    moteurEnMarche = true;      // Le bouton vient d'être relâché, démarrer le cycle
    pasActuels = 0;
  }
  Save_I_BP_Haut = I_BP_Haut;  // Mettre à jour l'état précédent du bouton depart cycle



 // ------ CDE ARRET BP ---------
   if ((Save_I_BP_Arret != I_BP_Arret) ){   //
      Save_I_BP_Arret = I_BP_Arret; // Sauvegarde l'état du BP
      moteurEnMarche = false;
      rampe_max = rampe_max_initial;      
   }
  

// ------ CDE ARRET NH ---------
  if (moteurEnMarche == true && DIR_PIN == HIGH && I_BP_Haut == LOW) { 
	moteurEnMarche = false;
    rampe_max = rampe_max_initial; 
  }

// ------ CDE ARRET NB ---------
  if (moteurEnMarche == true && DIR_PIN == LOW && I_BP_Bas == LOW) { 
	moteurEnMarche = false;
    rampe_max = rampe_max_initial; 
  }


  // Si le moteur est en marche
  if (moteurEnMarche) {
    // Contrôle de la fonction de memoire_Acc
    if (memoire_Acc == 1) {
      rampe_acc();
    } else {
      rampe_max = 0;
    }

    // Récupération du temps actuel en microsecondes
    unsigned long tempsActuel = micros();

    // Si le temps écoulé depuis la dernière impulsion est supérieur ou égal à la valeur de la tempo
    if (tempsActuel - tempsPrecedent >= TEMPO + rampe_max) {
      tempsPrecedent = tempsActuel;

      // Inversion de l'état de la broche de pulsation pour générer une impulsion
      digitalWrite(PULS_PIN, !digitalRead(PULS_PIN));
     // digitalWrite(oscilloscope, !digitalRead(oscilloscope));
      // Incrémenter le nombre de pas actuels
      pasActuels++;
      // Si le nombre de pas actuels a atteint le nombre de pas à réaliser
      if (pasActuels >= nombre_de_pas_a_realiser) {
        // Arrêter le moteur
        moteurEnMarche = false;
        rampe_max = rampe_max_initial;
      }
    }
  }
}
