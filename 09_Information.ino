/*
Guide Pratique pour une Accélération Maîtrisée d'un Moteur Pas-à-Pas 

Ce code est destiné à contrôler un moteur pas à pas avec un Arduino, en utilisant un driver TB6600.
Le code contrôle trois aspects du moteur : l'activation, la direction et la vitesse.

Le contrôle du moteur est réalisé via les broches ENA (Enable), PULS (Pulse) et DIR (Direction). ENA sert à activer
ou désactiver le moteur, PULS contrôle le mouvement du moteur en générant des impulsions, et DIR contrôle la direction du mouvement.

L'état d'activation du moteur est contrôlé par l'état d'un bouton physique connecté à la broche Bouton_depart_cycle. 
Lorsque ce bouton est relâché, le moteur est activé et commence à se déplacer.

Le code utilise aussi une rampe d'accélération, qui est une façon d'augmenter progressivement la vitesse du moteur au démarrage.
Lorsqu'elle est active, la rampe d'accélération modifie le délai entre les impulsions envoyées à la broche PULS, ce qui fait accélérer 
le moteur progressivement. L'activation de la rampe d'accélération est contrôlée par un autre bouton connecté à la broche Bouton_choix_rampe.
L'état de la rampe d'accélération est aussi indiqué par une LED.

En résumé, le fonctionnement du moteur est largement contrôlé par l'état des deux boutons. Lorsqu'ils sont relâchés, le moteur
est activé et commence à accélérer progressivement, si la rampe d'accélération est active. La vitesse du moteur et la durée pendant
laquelle il est en mouvement dépendent des valeurs de nombre_de_pas_a_realiser et TEMPO. Une fois que le moteur a effectué le nombre
de pas requis, il s'arrête jusqu'à ce que le bouton de départ du cycle soit à nouveau relâché.

*/
