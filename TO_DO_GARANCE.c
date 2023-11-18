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



*/


/* 2. TOKEN */
// Si termine par un operateur valide reouvrir un readline => voir parsebis pour l algo + ne pas free minishline avant que token soit termine
// "" et '' doivent avoir le token CMD