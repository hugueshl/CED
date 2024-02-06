
void rampe_acc() {

  unsigned long tempsActuel1 = millis();

  if (tempsActuel1 - derniere_mise_jour_rampAcc >= intervalle && rampe_max > 0) {
   
    // Cette ligne décrémente la valeur de rampe_max d'une unité à chaque mise à jour de la rampe. Cela permet 
    //de réduire progressivement la valeur de la rampe d'accélération.
    rampe_max--;
    
    // Cette ligne met à jour la variable avec le temps actuel, afin de mesurer le temps écoulé 
    //depuis la dernière mise à jour lors de la prochaine exécution de la fonction.   
    derniere_mise_jour_rampAcc = tempsActuel1;
     
     // Serial.println(rampe_max); : Cette ligne est actuellement commentée, mais si elle était décommentée,
     // elle enverrait la valeur de rampe_max à la sortie série pour le débogage. Cela peut être utile pour
     // suivre la diminution de la valeur de la rampe d'accélération au fil du temps.
          
     // Serial.println(rampe_max);
  }
}
