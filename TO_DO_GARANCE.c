/* 1. PARENTHESE */
/*
Tests a revoir : 
> hola (ls && pwd) => bash: syntax error near unexpected token `('
> pwd (ls) => bash: syntax error near unexpected token `('
( ) => bash: syntax error near unexpected token `)'
() => bash: syntax error near unexpected token `)'
(()) => exit status 1
(( )) => exit status 1
( ( ) )  => bash: syntax error near unexpected token `)'
( ( ( ( ) ) ) )  => bash: syntax error near unexpected token `)'
(((()))) => exit status 2 a gerer dans exec // doit passer le parsing
export HOLA=bon(jour
export HOLA=bon()jour

"export"'HOLAbon' (jour


bash-5.2$ export HOLA=bon(jour
bash: syntax error near unexpected token `('
bash-5.2$ export HOLA=bon&&(jour
> ^C
bash-5.2$ export && HOLA=bon&&(jour
> export && HOLA=bon&&(jour^C
bash-5.2$ export && HOLA=bon(jour
bash: syntax error near unexpected token `('
bash-5.2$ export && HOLA=bon (jour
bash: syntax error near unexpected token `('
bash-5.2$ export && HOLAbon (jour
bash: syntax error near unexpected token `jour'
bash-5.2$ "export"'HOLAbon' (jour
bash: syntax error near unexpected token `jour'
bash-5.2$ export < infile HOLAbon (jour
bash: syntax error near unexpected token `('
bash-5.2$ export infile < HOLAbon (jour
bash: syntax error near unexpected token `('
bash-5.2$ < HOLAbon (jour
bash: syntax error near unexpected token `('
bash-5.2$ infile < HOLAbon (jour
bash: syntax error near unexpected token `('
bash-5.2$ infile < HOLAbon < (jour
bash: syntax error near unexpected token `('
bash-5.2$ < ( op
bash: syntax error near unexpected token `('
bash-5.2$ > ( op
bash: syntax error near unexpected token `('
bash-5.2$ > > ( po)
bash: syntax error near unexpected token `>'
bash-5.2$ > re (po)
bash: syntax error near unexpected token `('
bash-5.2$ re > (iw
bash: syntax error near unexpected token `('
bash-5.2$ "export"'HOLAbon' (jour
bash: syntax error near unexpected token `jour'
bash-5.2$ "export" 'HOLAbon' (jour
bash: syntax error near unexpected token `('
bash-5.2$ 

ATTENTION A CE TEST : bash-5.1$ echo $USER$LESS$$$$USER
galambey-R834741834741USER
bash-5.1$ echo $USER$LESS$$$$$USER
galambey-R834741834741galambey
bash-5.1$ echo $USER$LESS$$$USER
galambey-R834741galambey



*/


/* 2. TOKEN */
// Si termine par un operateur valide reouvrir un readline => voir parsebis pour l algo + ne pas free minishline avant que token soit termine
// "" et '' doivent avoir le token CMD


// Il n'y a pas de code d'exit status standardisé pour une erreur de mémoire en Bash, car cela dépend de l'implémentation spécifique de la commande ou du programme que vous exécutez.