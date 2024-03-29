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

/*bash-5.1$ << $MDR
> $MDR
bash-5.1$ > $DONTEXISY
bash: $DONTEXISY: ambiguous redirect
bash-5.1$ < $DONTEXUT
bash: $DONTEXUT: ambiguous redirect
bash-5.1$ << $DOT
> $DOT
bash-5.1$ > PATH
bash-5.1$ > $PATH
bash: /mnt/nfs/homes/athiebau/bin:/mnt/nfs/homes/athiebau/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin: No such file or directory
bash-5.1$ export TRUC=hihi
bash-5.1$ > $TRUC
bash-5.1$ > $TRUC'mdr'
bash-5.1$ > $TRUC='mdr'
bash-5.1$ export 3i
bash: export: `3i': not a valid identifier
bash-5.1$ export i3
bash-5.1$ */

/*PIPE*/
/*
echo bravo | < NO_existing_infile echo hello > outfile | < outfile cat
    bash: NO_existing_infile: No such file or directory
    bash: outfile: No such file or directory
*/