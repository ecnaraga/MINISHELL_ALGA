/* 6/12 : Erreur PARSING: */
/*
echo & USER
*/

echo "echo coucou" | ./minishell

( ( cat | cat )  << L ) | cat << L

ls|infileexistant cat > infileexistant => fail de access

bash-5.1$ echo $eee hola
hola
bash-5.1$ echo "$eee" "hola"
 hola
bash-5.1$

TO DO : ATTENTION A CE TEST : echo $=HOME => Pour l instant traite dans parsing comme si variable d environnement a voir...

TO DO : Renvoyer erreurs pour test : 

, &head_hd<< LIM cat << LAM | cat << LOM



// export a=gaga && < $a

(cat | cat | cat | cat) | ( cat | cat ) | ( ls) > avec des ctrl + c leaks

// bash-5.1$ .
// bash: .: filename argument required
// .: usage: . filename [arguments]
// bash-5.1$ . ...
// bash: .: ...: file not found
// bash-5.1$ . gaga
// bash: .: gaga: file not found
// bash-5.1$ . .
// bash: .: .: is a directory
// bash-5.1$ . ..
// bash: .: ..: is a directory
// bash-5.1$ echo $?
// 1
// bash-5.1$ . gaga
// bash: .: gaga: file not found
// bash-5.1$ echo $?
// 1
// bash-5.1$ 


// bash
// galambey@made-f0Br8s5:~/Documents/MINISHELL_ALGA_BIS$ echo $?
// 0
// galambey@made-f0Br8s5:~/Documents/MINISHELL_ALGA_BIS$ .
// bash: .: filename argument required
// .: usage: . filename [arguments]
// galambey@made-f0Br8s5:~/Documents/MINISHELL_ALGA_BIS$ echo $?
// 2
// galambey@made-f0Br8s5:~/Documents/MINISHELL_ALGA_BIS$ srcs/
// bash: srcs/: Is a directory
// galambey@made-f0Br8s5:~/Documents/MINISHELL_ALGA_BIS$ echo $?
// 126
// galambey@made-f0Br8s5:~/Documents/MINISHELL_ALGA_BIS$ .
// bash: .: filename argument required
// .: usage: . filename [arguments]
// galambey@made-f0Br8s5:~/Documents/MINISHELL_ALGA_BIS$ echo $?
// 2
// galambey@made-f0Br8s5:~/Documents/MINISHELL_ALGA_BIS$ .
// bash: .: filename argument required
// .: usage: . filename [arguments]
// galambey@made-f0Br8s5:~/Documents/MINISHELL_ALGA_BIS$ /gaga
// bash: /gaga: No such file or directory
// galambey@made-f0Br8s5:~/Documents/MINISHELL_ALGA_BIS$ ./gaga
// bash: ./gaga: Is a directory
// galambey@made-f0Br8s5:~/Documents/MINISHELL_ALGA_BIS$ ..
// ..: command not found
// galambey@made-f0Br8s5:~/Documents/MINISHELL_ALGA_BIS$ ./
// bash: ./: Is a directory
// galambey@made-f0Br8s5:~/Documents/MINISHELL_ALGA_BIS$ . 
// bash: .: filename argument required
// .: usage: . filename [arguments]
// galambey@made-f0Br8s5:~/Documents/MINISHELL_ALGA_BIS$ /.
// bash: /.: Is a directory
// galambey@made-f0Br8s5:~/Documents/MINISHELL_ALGA_BIS$ . .
// bash: .: .: is a directory
// galambey@made-f0Br8s5:~/Documents/MINISHELL_ALGA_BIS$ echo $?
// 1
// galambey@made-f0Br8s5:~/Documents/MINISHELL_ALGA_BIS$ .


// POUR ALIX : Minishell$ ((cat)|(ls))

// bash-5.1$ < $b
// bash: $b: ambiguous redirect
// 
// export a=* && echo $a if cmd = export export le wild card si apres =

// bash-5.1$ export a=* && echo $a
// code erreurs.odt Erreurs a gerer dans EXEC ERREUR VALGRIND A GERER fonction inutilisee gaga includes infile Instruction.c L libft Makefile MINI_700_TESTS.ods minishell objs outfile parsing.odg readline.supp srcs TEST_ERROR tests_parsing TO_DO_GARANCE.c
// bash-5.1$ export *=u && echo $a
// bash: export: `*=u': not a valid identifier
// bash-5.1$ *
// bash: code erreurs.odt: command not found
// bash-5.1$ export a=*a && echo $a
// gaga
// bash-5.1$ export a=*b && echo $a
// *b
// bash-5.1$ a
// bash: a: command not found
// bash-5.1$ $a
// bash: *b: command not found
// bash-5.1$ export a=*a && echo $a
// gaga
// bash-5.1$ $*a
// bash: a: command not found
// bash-5.1$ 

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

/*TESTS : 

1- DOIT FONCTIONNER :
( bravo )
(bravo)
( b ravo )
(b ravo)
( bravo ) && ( bravo )
(bravo) && (bravo)
( echo bravo ) && ( bravo )
(echo bravo) && (bravo)
( echo bravo )&&( bravo )
(echo bravo)&&(bravo)
( ( bravo ) )
( ( bravo ))
(( bravo ) )
( (bravo) )
((bravo) )
(( bravo) )
( ( bravo ) ) && ( ( bravo ) )
( ( bravo ) && ( bravo ) )
(( bravo ) && ( bravo ))
((bravo ) && ( bravo))
((bravo) && (bravo))
((bravo)&&(bravo))
( (bravo)&&(bravo) )
( ( echo bravo ) )
((echo bravo) )
( (echo bravo))
( ( echo bravo ))
(( echo bravo ) )
(( bravo ) && bravo )
((bravo) && bravo)
( (bravo) && bravo)
( ( bravo ) && bravo )
( bravo && (bravo))
( bravo && (bravo) )
( bravo && ( bravo ) )
( bravo && ( bravo ))
( ( ls && ls ) )
(( ls && ls ) )
( ( ls && ls ))
(( echo && ( bravo )) && la )
(( echo && ( bravo ) ) && la )
( ( echo && ( bravo ) ) && la )
( ( echo && ( bravo )) && la )
	
2- DOIT PASSER TEST PARSING MAIS EN EXEC : N IMPRIME RIEN + EXITSTATUS = 1 // AGERER DANS L EXEC
(( bravo ))
((bravo))
(( ls && ls ))
((ls && ls))
echo Bravo && ((ls && ls)) && echo hehe
echo Bravo && ((ls && ls)) || echo hehe
( ( (( )) ) )
(())
(( ))
>>>> parenthese collee gerees comme parenthese espacees car on doit gerer parentheses pour priorites

3- NE DOIT PAS PASSER DANS EXEC : bash: syntax error near unexpected token + EXIT STATUS 2
1. 
( bravo ) ( bravo )
(bravo) (bravo)
(bravo)(bravo)
( bravo )( bravo )
	=> minishell: syntax error near unexpected token `('
1. 
( echo bravo ) ( bravo )
(echo bravo) (bravo)
(echo bravo)(bravo)
( echo bravo )( bravo )=> minishell: syntax error near unexpected token `('
3. 
()
( )
(      )
	=> syntax error near unexpected token `)'
3. 
( ( ) ) 
	=> bash: syntax error near unexpected token `)'
3.
( ( ( ( ) ) ) ) 
	=> bash: syntax error near unexpected token `)'
4. 
)
		)
bravo )
=> syntax error near unexpected token `)'

2. 
( echo ( bravo ) )
( echo( bravo ))
( echo(bravo))
(echo (bravo))
( echoo ( bravo ) echo )
( echoo ( bravo ) echo)
(( echo ) bravo )
( ( echo ) bravo )
	=> minishell: syntax error near unexpected token `bravo'
1. 
(( bravo ) ( bravo ))
( (bravo)  (bravo) )
((bravo)  (bravo))
((bravo)(bravo))
((bravo)(bravo) )
( (bravo)(bravo) )
	=> minishell: syntax error near unexpected token `('
1. 
> hola (ls && pwd)
	=> bash: syntax error near unexpected token `('
1. 
> pwd (ls)

4- DOIT ALLER DANS EXEC ET ERREUR DU TYPE : minishell: ((: bravo  bravo: syntax error in expression (error token is "bravo")  + EXIT STATUS 2 // AGERER DANS L EXEC
(( echo bravo ))
((echo bravo))
	=> minishell: ((: echo bravo : syntax error in expression (error token is "bravo ")
( (( ( ) )) )
	=> minishell: ((: ( ) : syntax error: operand expected (error token is ") ")
(((())))
	=> minishell: ((: (()): syntax error: operand expected (error token is "))")
*/

/*
KO : 
export HOLA=bon(jour
export HOLA=bon()jour

bash-5.2$ echo ( bravo )
bash: syntax error near unexpected token `bravo'
bash-5.2$ echo echo ( bravo )
bash: syntax error near unexpected token `('
 
*/


// int	main(void)
// {
// 	int		i;
// 	int		d;
// 	char	*str0 = "H";
// 	char	*str1 = "a b";
// 	char	*str2 = "a      b      ";
// 	char	*str3 = "a   b \"  a  \"";
// 	char	*str4 = "a   b \" \"";
// 	char	*str5 = "a   b \' \'";
// 	char	*str6 = "a   b \'  a  \'";
// 	char	*str7 = "a   b \"\'  a  \'\"";
// 	char	*str8 = "a   b \'\"  a  \"\'";
// 	char	*str9 = "\"\'\"";
// 	char	*str10 = "\"a\"a";
// 	char	*str11 = "\"\'a\'\"a";
// 	char	*str12 = "\'\"a\"\'a";
// 	char	*str13 = "a   b \"\'  a  \'\"\'ab\'\"ab\"";
// 	char	*str14 = "a   b \"\'  a  \'\"\'ab\'\"ab\" a   b \"\'  a  \'\"\'ab\'\"ab\"";
// 	char	*str15 = "echo \"-\"\"nn\'nn\'\" a   b \"\'  a  \'\"\'ab\'\"ab\" a   b \"\'  a  \'\"\'ab\'\"ab\"";
// 	char	*str16 = "echo \"\"";
// 	char	*str17 = "echo \"-n\" Hola";
// 	char	*str18 = "echo \" \" | cat -e";
// 	char	*str19 = "echo \"\" hola";
// 	char	*str20 = "echo\" \" | cat -e";
// 	char	*str21 = "\"    \"echo\"    \" | cat -e";
// 	char	*str22 = "echo \"\"hola";
// 	char	*str23 = "export HOLA=\" -n bonjour \"";
// 	char	*str24 = "export \"\" HOLA=bonjour";
// 	char	*str25 = "echo \"\"\"$HOLA\"\"\" | cat -e";
// 	char	*str26 = "export HOLA=\" hola et $HOLA\"$HOLA";
// 	char	*str27 = "export HOLA=\" bonjour hey \"";
// 	char	*str28 = "export HOLA=\' hola et $HOLA\'";
// 	char	*str29 = "env | \"wc\" -l";
// 	char	*str30 = "export \"\" || ls";
// 	char	*str31 = "export \"\" && ls";
// 	char	*str32 = "export \"\" && unset \"\"";

// 	//TESTS TYPE
// 	char	*str33 = "echo $USER"; // >> Doit etre expand 
// 	char	*str34 = "echo \"$USER\""; // >> Doit etre expand
// 	char	*str35 = "echo \'$USER\'"; // >> Ne doit pas etre expand
// 	char	*str36 = "echo \'\"$USER\"\'"; // >> Ne doit pas etre expand
// 	char	*str37 = "echo \"\'$USER\'\""; // >> Doit etre expand
// 	char	*str38 = "echo \"\'$  USER\'\""; // >> Doit etre expand
// 	char	*str39 = "echo \"\'$$$  $USER\'\""; // >> Doit etre expand
// 	char	*str40 = "echo \"\'$$$  $USER\'$USER\"\'$USER\'$USER\"$US\"ER\"$USER\"";
// 	char	*str41 = "echo $\"USER\""; //le dollar ne devra pas s afficher
// 	char	*str42 = "echo \"$\"\"USER\""; //le dollar doit s afficher
// 	char	*str43 = "echo $ \"USER\""; //le dollar doit s afficher
// 	char	*str44 = "echo \"$\"\"\""; //le dollar doit s afficher
// 	char	*str45 = "echo $\"\""; //le dollar ne devra pas s afficher
// 	char	*str46 = "echo $\'USER\'"; //le dollar ne devra pas s afficher
// 	char	*str47 = "echo \'$\'\'USER\'"; //le dollar doit s afficher
// 	char	*str48 = "echo $ \'USER\'"; //le dollar doit s afficher
// 	char	*str49 = "echo \'$\'\'\'"; //le dollar doit s afficher
// 	char	*str50 = "echo $\'\'"; //le dollar ne devra pas s afficher
// 	char	*str51 = "echo $\"\'USER\'\""; //le dollar ne devra pas s afficher
// 	char	*str52 = "$USER$HELLOYOU"; //les dollars devront s afficher
// 	char	*str53 = "$USER$LESS$$$$USER"; //les dollars devront s afficher
// 	// ""''echo hola""'''' que""'' tal""'' => SPLIT MAUVAIS echo doit avoir type 0 et les autres mots type 5
// 	// echo $"" => SPLIT $ en type 5 au lieu de 0

// 	printf("string = %s\nnb de mot = %d\n", str0, ft_countwords(str0));
// 	t_split *strs = ft_split_msh(str0);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str1, ft_countwords(str1));
// 	strs = ft_split_msh(str1);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str2, ft_countwords(str2));
// 	strs = ft_split_msh(str2);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str3, ft_countwords(str3));
// 	strs = ft_split_msh(str3);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str4, ft_countwords(str4));
// 	strs = ft_split_msh(str4);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str5, ft_countwords(str5));
// 	strs = ft_split_msh(str5);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str6, ft_countwords(str6));
// 	strs = ft_split_msh(str6);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str7, ft_countwords(str7));
// 	strs = ft_split_msh(str7);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str8, ft_countwords(str8));
// 	strs = ft_split_msh(str8);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str9, ft_countwords(str9));
// 	strs = ft_split_msh(str9);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str10, ft_countwords(str10));
// 	strs = ft_split_msh(str10);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str11, ft_countwords(str11));
// 	strs = ft_split_msh(str11);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str12, ft_countwords(str12));
// 	strs = ft_split_msh(str12);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str13, ft_countwords(str13)); //count letter ne passe pas
// 	strs = ft_split_msh(str13);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str14, ft_countwords(str14)); //count letter ne passe pas
// 	strs = ft_split_msh(str14);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str15, ft_countwords(str15)); //count letter ne passe pas
// 	strs = ft_split_msh(str15);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str16, ft_countwords(str16)); //count letter ne passe pas
// 	strs = ft_split_msh(str16);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str17, ft_countwords(str17)); //count letter ne passe pas
// 	strs = ft_split_msh(str17);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str18, ft_countwords(str18)); //count letter ne passe pas
// 	strs = ft_split_msh(str18);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str19, ft_countwords(str19)); //count letter ne passe pas
// 	strs = ft_split_msh(str19);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str20, ft_countwords(str20)); //count letter ne passe pas
// 	strs = ft_split_msh(str20);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str21, ft_countwords(str21)); //count letter ne passe pas
// 	strs = ft_split_msh(str21);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str22, ft_countwords(str22)); //count letter ne passe pas
// 	strs = ft_split_msh(str22);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str23, ft_countwords(str23)); //count letter ne passe pas
// 	strs = ft_split_msh(str23);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str24, ft_countwords(str24)); //count letter ne passe pas
// 	strs = ft_split_msh(str24);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str25, ft_countwords(str25)); //count letter ne passe pas
// 	strs = ft_split_msh(str25);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str26, ft_countwords(str26)); //count letter ne passe pas
// 	strs = ft_split_msh(str26);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str27, ft_countwords(str27)); //count letter ne passe pas
// 	strs = ft_split_msh(str27);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str28, ft_countwords(str28)); //count letter ne passe pas
// 	strs = ft_split_msh(str28);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str29, ft_countwords(str29)); //count letter ne passe pas
// 	strs = ft_split_msh(str29);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str30, ft_countwords(str30)); //count letter ne passe pas
// 	strs = ft_split_msh(str30);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str31, ft_countwords(str31)); //count letter ne passe pas
// 	strs = ft_split_msh(str31);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str32, ft_countwords(str32)); //count letter ne passe pas
// 	strs = ft_split_msh(str32);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str33, ft_countwords(str33)); //count letter ne passe pas
// 	strs = ft_split_msh(str33);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str34, ft_countwords(str34)); //count letter ne passe pas
// 	strs = ft_split_msh(str34);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str35, ft_countwords(str35)); //count letter ne passe pas
// 	strs = ft_split_msh(str35);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str36, ft_countwords(str36)); //count letter ne passe pas
// 	strs = ft_split_msh(str36);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str37, ft_countwords(str37)); //count letter ne passe pas
// 	strs = ft_split_msh(str37);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str38, ft_countwords(str38)); //count letter ne passe pas
// 	strs = ft_split_msh(str38);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str39, ft_countwords(str39)); //count letter ne passe pas
// 	strs = ft_split_msh(str39);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str40, ft_countwords(str40)); //count letter ne passe pas
// 	strs = ft_split_msh(str40);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str41, ft_countwords(str41)); //count letter ne passe pas
// 	strs = ft_split_msh(str41);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str42, ft_countwords(str42)); //count letter ne passe pas
// 	strs = ft_split_msh(str42);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str43, ft_countwords(str43)); //count letter ne passe pas
// 	strs = ft_split_msh(str43);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str44, ft_countwords(str44)); //count letter ne passe pas
// 	strs = ft_split_msh(str44);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str45, ft_countwords(str45)); //count letter ne passe pas
// 	strs = ft_split_msh(str45);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str46, ft_countwords(str46)); //count letter ne passe pas
// 	strs = ft_split_msh(str46);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str47, ft_countwords(str47)); //count letter ne passe pas
// 	strs = ft_split_msh(str47);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str48, ft_countwords(str48)); //count letter ne passe pas
// 	strs = ft_split_msh(str48);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str49, ft_countwords(str49)); //count letter ne passe pas
// 	strs = ft_split_msh(str49);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str50, ft_countwords(str50)); //count letter ne passe pas
// 	strs = ft_split_msh(str50);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str51, ft_countwords(str51)); //count letter ne passe pas
// 	strs = ft_split_msh(str51);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str52, ft_countwords(str52)); //count letter ne passe pas
// 	strs = ft_split_msh(str52);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str53, ft_countwords(str53)); //count letter ne passe pas
// 	strs = ft_split_msh(str53);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// }



// int main(void)
// {
// 	t_split	*strs;
// 	char str0[] = "echo a b";
// 	char str1[] = "echo -n a b";
// 	char	str2[] = "a      b      ";
// 	char	str3[] = {'e', 'c', 'h', 'o', ' ', 'a', ' ', ' ', ' ', 'b', ' ', '"', ' ', ' ', 'a', ' ', ' ', '"', '\0'};
// 	char	str4[] = {'e', 'c', 'h', 'o', ' ', 'a', ' ', ' ', ' ', 'b', ' ', '"', ' ', '"', '\0'};
// 	char	str5[] = {'e', 'c', 'h', 'o', ' ', 'a', ' ', ' ', ' ', 'b', ' ', 39, ' ', 39, '\0'};
// 	char	str6[] = {'e', 'c', 'h', 'o', ' ', 'a', ' ', ' ', ' ', 'b', ' ', 39, ' ', ' ', 'a', ' ', ' ', 39, '\0'};
// 	char	str7[] = {'e', 'c', 'h', 'o', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', '\0'};
// 	char	str8[] = {'e', 'c', 'h', 'o', ' ', 'a', ' ', ' ', ' ', 'b', ' ', 39,'"', ' ', ' ', 'a', ' ', ' ', '"', 39, '\0'};
// 	char	str9[] = {'e', 'c', 'h', 'o', ' ', '"', 39, '"', '\0'};
// 	char	str10[] = {'e', 'c', 'h', 'o', ' ', '"', 'a', '"', 'a','\0'};
// 	char	str11[] = {'e', 'c', 'h', 'o', ' ', '"', 39, 'a', 39, '"', 'a','\0'};
// 	char	str12[] = {'e', 'c', 'h', 'o', ' ', 39, '"', 'a', '"', 39, 'a','\0'};
// 	char	str13[] = {'e', 'c', 'h', 'o', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str14[] = {'e', 'c', 'h', 'o', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str15[] = {'e', 'c', 'h', 'o', ' ', '-', 'n', 'n', 'n',' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str16[] = {'e', 'c', 'h', 'o', ' ', '-', 'n', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str17[] = {'e', 'c', 'h', 'o', ' ', '"', '-', 'n', '"', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str18[] = {'e', 'c', 'h', 'o', ' ', '"', '-', 'n', 'n', 'n', 'n', '"', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str19[] = {'e', 'c', 'h', 'o', ' ', 39, '-', 'n', 39, ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str20[] = {'e', 'c', 'h', 'o', ' ', 39, '-', 'n', 'n', 'n', 'n', 'n', 39, ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str21[] = {'e', 'c', 'h', 'o', ' ', '-', '"', 'n', 'n', 'n', 'n', '"', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str22[] = {'e', 'c', 'h', 'o', ' ', '-', 39, 'n', 'n', 'n', 'n', 39, ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str23[] = {'e', 'c', 'h', 'o', ' ', '-', '"', 'n', '"', 'n', 'n', '"', 'n', '"', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str24[] = {'e', 'c', 'h', 'o', ' ', '-', '"', 'n', '"', 'n', 'n', '"', 'n', '"', 'n', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str25[] = {'e', 'c', 'h', 'o', ' ', '-', 39, 'n', 39, 'n', 'n', 39, 'n', 39, 'n', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str26[] = {'e', 'c', 'h', 'o', ' ', '"', '-', '"', 'n', 'n', 39, 'n', 'n', 39, ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str27[] = {'e', 'c', 'h', 'o', ' ', '"', '-', '"', '"', 'n', 'n', 39, 'n', 'n', 39, '"', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str28[] = {'e', 'c', 'h', 'o', ' ', '"', '"', '\0'};
// 	char	str29[] = {'e', 'c', 'h', 'o', ' ', '"', '-', '"', '"', 'n', 'n', 'n', 'n', '"', ' ', '"', '-', '"', '"', 'n', 'n', 'n', 'n', '"', ' ', '"', '-', '"', '"', 'n', 'n', 'n', 'n', '"', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str30[] = {'e', 'c', 'h', 'o', ' ', '"', '-', '"', '"', 'n', 'n', 'b', 'n', 'n', '"', ' ', '"', '-', '"', '"', 'n', 'n', 'n', 'n', '"', ' ', '"', '-', '"', '"', 'n', 'n', 'n', 'n', '"', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};

// 	printf("string : %s\n", str0);
// 	strs = ft_split_minish(str0);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str0) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str1);
// 	strs = ft_split_minish(str1);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str1) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");
	
// 	printf("string : %s\n", str2);
// 	strs = ft_split_minish(str2);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str2) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str3);
// 	strs = ft_split_minish(str3);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str3) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");
	
// 	printf("string : %s\n", str4);
// 	strs = ft_split_minish(str4);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str4) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str5);
// 	strs = ft_split_minish(str5);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str5) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str6);
// 	strs = ft_split_minish(str6);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str6) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str7);
// 	strs = ft_split_minish(str7);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str7) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str8);
// 	strs = ft_split_minish(str8);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str8) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str9);
// 	strs = ft_split_minish(str9);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str9) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str10);
// 	strs = ft_split_minish(str10);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str10) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str11);
// 	strs = ft_split_minish(str11);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str11) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str12);
// 	strs = ft_split_minish(str12);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str12) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str13);
// 	strs = ft_split_minish(str13);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str13) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str14);
// 	strs = ft_split_minish(str14);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str14) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str15);
// 	strs = ft_split_minish(str15);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str15) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");
	
// 	printf("string : %s\n", str16);
// 	strs = ft_split_minish(str16);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str16) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");
	
// 	printf("string : %s\n", str17);
// 	strs = ft_split_minish(str17);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str17) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str18);
// 	strs = ft_split_minish(str18);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str18) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");
	
// 	printf("string : %s\n", str19);
// 	strs = ft_split_minish(str19);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str19) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");
	
// 	printf("string : %s\n", str20);
// 	strs = ft_split_minish(str20);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str20) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str21);
// 	strs = ft_split_minish(str21);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str21) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str22);
// 	strs = ft_split_minish(str22);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str22) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str23);
// 	strs = ft_split_minish(str23);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str23) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str24);
// 	strs = ft_split_minish(str24);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str24) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str25);
// 	strs = ft_split_minish(str25);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str25) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str26);
// 	strs = ft_split_minish(str26);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str26) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str27);
// 	strs = ft_split_minish(str27);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str27) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str28);
// 	strs = ft_split_minish(str28);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str28) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");
	
// 	printf("string : %s\n", str29);
// 	strs = ft_split_minish(str29);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str29) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");
	
// 	printf("string : %s\n", str30);
// 	strs = ft_split_minish(str30);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str30) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");
// }
