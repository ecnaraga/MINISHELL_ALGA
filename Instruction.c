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

/*EXPAND
>> ex : un double pipe stockee dans 1 variable d environnement n est pas considere comme un operateur
bash-5.1$ (ls $OPERATOR ls) || ls
ls: cannot access '||': No such file or directory
ls: cannot access 'ls': No such file or directory
'code erreurs.odt'	      Instruction.c   MINI_700_TESTS.ods   parsing.odg
'Erreurs a gerer dans EXEC'   libft	      minishell		   srcs
 includes		      Makefile	      objs		   tests_parsing
*/