/*
INFILE

Il peut se trouver avant 1 cmd apres 1 cmd et ses arguments ou en plein milieu
Il peut y en avoir plusieurs
*/

/*
1 seul groupe de cmd x 1 infile/outfile mais plusieurs infile/outfile x 1 groupe de cmd
exemple : 
(echo Bravo ) > hola > baba       => OK car un groupe de cmd entre parenthese et deux outfile
echo Bravo > hola > baba		  => OK car un groupe de cmd (echo Bravo) et deux outfile
echo Bravo > hola > baba coucou	  =>  => OK car un groupe de cmd (echo Bravo coucou) et deux outfile
(echo Bravo) > hola > baba cat	  =>  => PAS OK car DEUX groupe de cmd ((echo Bravo) et cat) et deux outfile
=> erreur de parsing a gerer dans l arbre
Les groupe de cmd doivent etre separes par des operateurs
*/