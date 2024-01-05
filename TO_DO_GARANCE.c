/* 6/12 : Erreur PARSING: */
/*
echo & USER
*/

bash-5.1$ echo $eee hola
hola
bash-5.1$ echo "$eee" "hola"
 hola
bash-5.1$

(cat | cat | cat | cat) | ( cat | cat ) | ( ls) > avec des ctrl + c leaks

POUR ALIX : Minishell$ ((cat)|(ls))

export a=* && echo $a if cmd = export export le wild card si apres =

bash-5.1$ export a=* && echo $a
code erreurs.odt Erreurs a gerer dans EXEC ERREUR VALGRIND A GERER fonction inutilisee gaga includes infile Instruction.c L libft Makefile MINI_700_TESTS.ods minishell objs outfile parsing.odg readline.supp srcs TEST_ERROR tests_parsing TO_DO_GARANCE.c
bash-5.1$ export *=u && echo $a
bash: export: `*=u': not a valid identifier
bash-5.1$ *
bash: code erreurs.odt: command not found
bash-5.1$ export a=*a && echo $a
gaga
bash-5.1$ export a=*b && echo $a
*b
bash-5.1$ a
bash: a: command not found
bash-5.1$ $a
bash: *b: command not found
bash-5.1$ export a=*a && echo $a
gaga
bash-5.1$ $*a
bash: a: command not found
bash-5.1$ 

// >> est ce que l entree standard est redefinie dans le pipe avant de creer le subshell et donc de fork a nouveau?
// ( cat ) | ( ls )
// ( cat | cat | cat) | ls 

// << L cat && << L cat | cat && ( cat | cat | cat ) << L && ( echo bravo && (echo bravo && export alix=love && echo $alix) && echo $alix)

// ( cat | cat ) << L && cat | cat << L
// ( cat | cat << L ) << L && cat | cat << L

// << L cat && ( cat | cat << L && ( cat | cat ) << L && cat | cat << L ) && cat | cat << L
// << L cat && ( cat | cat << L && ( cat | cat ) << L && cat | cat << L ) && cat | cat << L
// valide key dans expand
// ft_expand dans heredoc 
// wildcard potentiellement dans heredoc // PAS DE WILDCARD DANS HDOC
// conditional jump avec wildcard

// bash-5.2$ < innnn
// bash: innnn: No such file or directory
// bash-5.2$ echo $?
// 1
// bash-5.2$ 

// ( cat | cat ) < infile < infile << L < infile

// Minishell$ ^C
// Minishell$ ^C
// Minishell$ ^C
// Minishell$ ^C
// Minishell$ ( cat | cat << L ) << L && cat | cat << L
// Sortie ft_parse_line : _( cat | cat << L ) << L && cat | cat << L_
// PARSE_TER
// FT_EXEC
// > ^C
// Minishell$ ^C
// Minishell$ ^C
// Minishell$ ^C
// Minishell$ ^C
// Minishell$ ( cat | cat << L ) << L && cat | cat << L
// Sortie ft_parse_line : _( cat | cat << L ) << L && cat | cat << L_
// PARSE_TER
// FT_EXEC
// > a
// Minishell$

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


// echo coucou && << LEM echo bravo | cat | < outfile cat && echo salut << LIM
// cat << LAM | cat && cat << LAM //OK

// ((echo bravo && echo salut ) >> bravo && echo tada) && echo hello
// ((echo bravo && echo salut ) >> bravo || echo tada) && echo hello