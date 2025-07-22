# Résumé complet du programme Minishell

Ce document détaille l’ensemble du code du projet Minishell, fonction par fonction et bloc logique par bloc logique, avec tous les embranchements possibles.

---

## Table des matières
1. Introduction générale
2. Initialisation et environnement
3. Boucle principale et gestion du prompt
4. Parsing et tokenisation
5. Expansion des variables
6. Exécution des commandes
7. Gestion des redirections
8. Gestion des pipes
9. Builtins
10. Gestion des signaux
11. Utilitaires
12. Arbre d’exécution global
13. Remarques

---

## 1. Introduction générale
Minishell est un shell minimaliste qui gère l’exécution de commandes, la gestion de l’environnement, les redirections, les pipes, les builtins, l’expansion des variables et la gestion des signaux. Le code est organisé en modules/fichiers spécialisés pour chaque fonctionnalité.

---

## 2. Initialisation et environnement
- Structures : `t_env`, `t_data`, `t_command`, `t_redir`
- Fonctions principales : `init_env`, `create_env_node`, `free_env_list`, `get_env_value`, `set_env_value`, `unset_env_value`, `env_to_array`, etc.
- Embranchements : gestion de la liste chaînée d’environnement, création/suppression/modification de variables, export, unset, etc.

---

## 3. Boucle principale et gestion du prompt
- Fichiers : `main.c`, fonctions : `main`, `minishell_loop`, `process_command_line`, `is_empty_line`, `print_error`
- Embranchements : lecture de la ligne, gestion EOF, gestion des lignes vides, ajout à l’historique, appel au parsing et à l’exécution.

---

## 4. Parsing et tokenisation
- Fichiers : `tokenizer.c`, `tokenizer_utils.c`, `token_utils.c`, etc.
- Fonctions : `tokenize`, `count_tokens`, `extract_token`, `skip_whitespace`, `expand_and_parse_token`, etc.
- Embranchements : découpage de la ligne, gestion des quotes, détection des redirections, gestion des erreurs de syntaxe.

---

## 5. Expansion des variables
- Fichiers : `expand_utils.c`, `expand_token.c`
- Fonctions : `get_variable_value_from_env`, `get_variable_value`, `handle_dollar`, `append_str`, `append_char`, etc.
- Embranchements : expansion des variables d’environnement, gestion des cas particuliers (`$?`, quotes, etc.).

---

## 6. Exécution des commandes
- Fichiers : `execution/*.c`, `command_execution.c`, etc.
- Fonctions : `execute`, `execute_child_process`, `handle_parent_process`, `find_command_path`, etc.
- Embranchements : exécution des commandes externes, gestion des erreurs, gestion du code de retour, fork/exec/wait.

---

## 7. Gestion des redirections
- Fichiers : `redirections/*.c`, `heredoc*.c`, etc.
- Fonctions : `handle_heredoc`, `handle_output_redirect`, `handle_input_redirect`, `process_redirections`, etc.
- Embranchements : gestion des fichiers, création/suppression de fichiers temporaires, gestion des signaux pour heredoc, etc.

---

## 8. Gestion des pipes
- Fichiers : `execution/pipes.c`, `pipe_utils.c`, etc.
- Fonctions : `count_pipes`, `split_all_pipe_commands`, `execute_pipe_chain`, `create_pipes`, etc.
- Embranchements : découpage des commandes, création des pipes, gestion des processus enfants, synchronisation.

---

## 9. Builtins
- Fichiers : `builtins/*.c`
- Fonctions : `handle_builtin`, `is_builtin`, `ft_cd`, `ft_pwd`, `ft_echo`, `ft_env_custom`, `ft_env_sorted`, `handle_export`, `handle_unset`, etc.
- Embranchements : détection des builtins, exécution directe, gestion des variables d’environnement, gestion des erreurs.

---

## 10. Gestion des signaux
- Fichiers : `utils/signal*.c`, etc.
- Fonctions : `init_signals_prompt`, `reset_signals_to_default`, `ignore_sigint`, etc.
- Embranchements : gestion des interruptions, gestion des signaux pour heredoc, restauration des signaux par défaut.

---

## 11. Utilitaires
- Fichiers : `utils/*.c`, `libft/*.c`
- Fonctions : gestion mémoire, affichage, manipulation de chaînes, etc.
- Embranchements : libération de ressources, gestion des erreurs, utilitaires divers.

---

## 12. Arbre d’exécution global
- Schéma logique du shell : initialisation → boucle principale → parsing → expansion → exécution → redirections/pipes → gestion des erreurs → sortie.

---

## 13. Remarques
- Chaque module/fichier est indépendant et spécialisé.
- La gestion mémoire et des erreurs est systématique.
- Les embranchements sont gérés à chaque étape pour garantir la robustesse du shell.

---

## Détail fonction par fonction

### 1. Fichier `src/builtins/builtins.c`

- **get_cd_path**
  - Résout le chemin à utiliser pour la commande `cd`.
  - Embranchements :
    - Si aucun argument ou `~` : récupère la variable HOME.
    - Si `-` : récupère la variable OLDPWD.
    - Sinon : utilise l'argument donné.
    - Si la variable n'existe pas, affiche une erreur et retourne NULL.

- **update_pwd_vars**
  - Met à jour les variables d'environnement PWD et OLDPWD après un changement de répertoire.
  - Embranchements :
    - Si `getcwd` réussit, met à jour les variables.

- **ft_cd**
  - Implémente la commande `cd`.
  - Embranchements :
    - Résout le chemin avec `get_cd_path`.
    - Si le chemin est invalide, retourne 1.
    - Si `chdir` échoue, affiche une erreur et retourne 1.
    - Sinon, met à jour les variables et retourne 0.

- **ft_pwd**
  - Affiche le répertoire courant.
  - Embranchements :
    - Si `getcwd` réussit, affiche le chemin.
    - Sinon, affiche une erreur.

- **handle_builtin**
  - Détecte et exécute la commande builtin appropriée.
  - Embranchements :
    - Pour chaque builtin (`cd`, `pwd`, `echo`, `export`, `unset`, `env`, `exit`), appelle la fonction correspondante.
    - Si la commande n'est pas reconnue, affiche une erreur et retourne 127.

### 2. Fichier `src/builtins/builtin_utils.c`

- **check_multiple_n**
  - Vérifie si l'argument correspond à une option `-n` pour `echo`.
  - Embranchements :
    - Parcourt la chaîne, vérifie la présence de `-n`.
    - Retourne 1 si valide, 0 sinon.

- **ft_echo_n**
  - Affiche les arguments sans retour à la ligne.
  - Embranchements :
    - Parcourt les arguments à partir du 2e, affiche chacun séparé par un espace.

- **is_builtin**
  - Vérifie si une commande est un builtin.
  - Embranchements :
    - Compare la commande à la liste des builtins.
    - Retourne 1 si oui, 0 sinon.

- **ft_echo**
  - Implémente la commande `echo`.
  - Embranchements :
    - Si l'option `-n` est présente, n'affiche pas le retour à la ligne.
    - Affiche tous les arguments séparés par un espace.

- **ft_exit_simple**
  - Implémente la commande `exit`.
  - Embranchements :
    - Si un argument est donné, le convertit en code de sortie.
    - Affiche "exit" puis quitte le shell avec le code donné.

### 3. Fichier `src/builtins/builtin_export.c`

- **ft_env_custom**
  - Affiche les variables d'environnement exportées.
  - Embranchements :
    - Parcourt la liste, affiche chaque variable exportée avec sa valeur.

- **ft_unset**
  - Supprime une variable d'environnement.
  - Embranchements :
    - Parcourt la liste, supprime le nœud correspondant à la clé donnée.
    - Libère la mémoire associée.

- **handle_unset**
  - Gère la commande `unset` pour plusieurs arguments.
  - Embranchements :
    - Pour chaque argument, appelle `ft_unset`.
    - Retourne 0 si tout s'est bien passé.

### 4. Fichier `src/utils/export_utils.c`

- **env_size**
  - Calcule le nombre de variables dans la liste d'environnement.
  - Embranchements :
    - Parcourt la liste chaînée, incrémente un compteur.
    - Retourne le nombre total.

- **ft_sort_str_array**
  - Trie un tableau de chaînes de caractères par ordre alphabétique.
  - Embranchements :
    - Utilise un tri par sélection (double boucle), échange les éléments si nécessaire.

- **export_env_to_array**
  - Construit un tableau de chaînes représentant les variables exportées.
  - Embranchements :
    - Parcourt la liste, ajoute chaque variable exportée au tableau.
    - Alloue dynamiquement chaque chaîne.
    - Retourne le tableau terminé.

- **print_and_free_env_array**
  - Affiche chaque variable exportée et libère la mémoire du tableau.
  - Embranchements :
    - Parcourt le tableau, affiche chaque élément, puis le libère.
    - Libère le tableau principal à la fin.

- **ft_env_sorted**
  - Affiche toutes les variables exportées triées.
  - Embranchements :
    - Construit le tableau avec `export_env_to_array`.
    - Trie le tableau avec `ft_sort_str_array`.
    - Affiche et libère le tableau avec `print_and_free_env_array`.
    - Retourne 0 si tout s'est bien passé, 1 en cas d'erreur d'allocation.

### 5. Fichier `src/execution/exec_utils.c`

- **find_command_path**
  - Trouve le chemin complet d'une commande.
  - Embranchements :
    - Si la commande contient un `/`, considère que c'est un chemin complet.
    - Sinon, parcourt les chemins dans `PATH`, ajoute chaque chemin au nom de la commande et vérifie l'existence.
    - Retourne le chemin complet ou NULL si non trouvé.

- **execute_child_process**
  - Gère l'exécution d'un processus enfant.
  - Embranchements :
    - Si `is_builtin` retourne vrai, exécute la commande builtin correspondante.
    - Sinon, utilise `execve` pour exécuter la commande.
    - Gère les erreurs d'exécution.

- **handle_parent_process**
  - Gère le processus parent après un fork.
  - Embranchements :
    - Attend la fin de l'enfant avec `waitpid`.
    - Récupère et gère le code de retour de l'enfant.

- **execute**
  - Fonction principale pour exécuter une commande.
  - Embranchements :
    - Gère les redirections si présentes.
    - Gère les pipes si présents.
    - Fork un processus et appelle `execute_child_process` dans l'enfant.
    - Gère le processus parent avec `handle_parent_process`.

### 6. Fichier `src/redirections/heredoc_utils.c`

- **heredoc_signal_handler**
  - Gère les signaux pour le processus de heredoc.
  - Embranchements :
    - Ignore le signal SIGINT.
    - Rétablit le comportement par défaut du signal à la fin.

- **open_heredoc_file**
  - Ouvre le fichier temporaire pour le heredoc.
  - Embranchements :
    - Crée un fichier temporaire avec un nom unique.
    - Gère les erreurs d'ouverture.

- **write_heredoc_to_file**
  - Écrit le contenu du heredoc dans le fichier.
  - Embranchements :
    - Boucle de lecture de l'entrée standard jusqu'à ce que le délimiteur soit rencontré.
    - Écrit chaque ligne dans le fichier.
    - Gère les erreurs d'écriture.

- **handle_heredoc**
  - Gère l'ensemble du processus de heredoc.
  - Embranchements :
    - Appelle `open_heredoc_file` pour ouvrir le fichier.
    - Appelle `write_heredoc_to_file` pour écrire le contenu.
    - Gère les signaux avec `heredoc_signal_handler`.
    - Ferme le fichier à la fin.

### 7. Fichier `src/redirections/redirection_utils.c`

- **open_file_for_redirection**
  - Ouvre un fichier pour une redirection.
  - Embranchements :
    - Selon le type de redirection (entrée, sortie, erreur), ouvre le fichier avec les flags appropriés.
    - Gère les erreurs d'ouverture.

- **dup2_redirection**
  - Duplique un descripteur de fichier pour la redirection.
  - Embranchements :
    - Utilise `dup2` pour dupliquer le descripteur.
    - Gère les erreurs de duplication.

- **close_unused_fds**
  - Ferme les descripteurs de fichiers inutilisés.
  - Embranchements :
    - Ferme les descripteurs de fichiers dans le parent après un fork.
    - Ferme les descripteurs de fichiers dans l'enfant après une redirection.

- **handle_output_redirect**
  - Gère la redirection de la sortie standard.
  - Embranchements :
    - Ouvre le fichier de sortie avec `open_file_for_redirection`.
    - Duplique le descripteur de fichier avec `dup2_redirection`.
    - Ferme les descripteurs inutilisés avec `close_unused_fds`.

- **handle_input_redirect**
  - Gère la redirection de l'entrée standard.
  - Embranchements :
    - Ouvre le fichier d'entrée avec `open_file_for_redirection`.
    - Duplique le descripteur de fichier avec `dup2_redirection`.
    - Ferme les descripteurs inutilisés avec `close_unused_fds`.

### 8. Fichier `src/execution/pipes.c`

- **count_pipes**
  - Compte le nombre de pipes dans la commande.
  - Embranchements :
    - Parcourt la commande, incrémente un compteur à chaque pipe trouvé.
    - Retourne le nombre total de pipes.

- **split_all_pipe_commands**
  - Sépare toutes les commandes dans une chaîne de caractères par les pipes.
  - Embranchements :
    - Parcourt la chaîne, utilise `strsep` pour séparer les commandes.
    - Gère les erreurs de mémoire.

- **create_pipes**
  - Crée les pipes nécessaires pour la chaîne de commandes.
  - Embranchements :
    - Utilise `pipe` pour créer chaque pipe.
    - Gère les erreurs de création.

- **execute_pipe_chain**
  - Exécute une chaîne de commandes avec des pipes.
  - Embranchements :
    - Crée les pipes avec `create_pipes`.
    - Pour chaque commande, fork un processus et redirige les entrées/sorties.
    - Gère le processus parent avec `handle_parent_process`.

### 9. Fichier `src/utils/signal_utils.c`

- **init_signals_prompt**
  - Initialise la gestion des signaux pour le prompt.
  - Embranchements :
    - Définit le handler pour SIGINT (Ctrl+C) pour interrompre la saisie utilisateur.
    - Ignore SIGQUIT pour éviter la fermeture du shell.

- **reset_signals_to_default**
  - Restaure le comportement par défaut des signaux.
  - Embranchements :
    - Réinitialise SIGINT et SIGQUIT à leur comportement standard.

- **ignore_sigint**
  - Ignore le signal SIGINT dans certains contextes (exécution de commande, heredoc).
  - Embranchements :
    - Définit le handler SIGINT sur SIG_IGN.

### 10. Fichier `src/utils/free_utils.c`

- **free_env_list**
  - Libère toute la liste chaînée d'environnement.
  - Embranchements :
    - Parcourt la liste, libère chaque nœud et ses champs.

- **free_command_list**
  - Libère la liste chaînée des commandes.
  - Embranchements :
    - Parcourt la liste, libère chaque commande et ses arguments.

- **free_redir_list**
  - Libère la liste chaînée des redirections.
  - Embranchements :
    - Parcourt la liste, libère chaque redirection et ses champs.

- **free_str_array**
  - Libère un tableau de chaînes de caractères.
  - Embranchements :
    - Parcourt le tableau, libère chaque chaîne, puis le tableau principal.

### 11. Fichier `libft/*.c`

- Fonctions utilitaires classiques de la libft : gestion de chaînes (`ft_strlen`, `ft_strdup`, `ft_strjoin`, etc.), gestion mémoire (`ft_calloc`, `ft_bzero`, etc.), manipulation de listes chaînées (`ft_lstnew_bonus`, `ft_lstadd_back_bonus`, etc.), conversion (`ft_atoi`, `ft_itoa`), etc.
  - Embranchements :
    - Chaque fonction gère ses propres cas d’erreur et d’allocation mémoire.

### 12. Arbre d’exécution global

- **Logique générale du shell**
  - Initialisation des structures et de l’environnement.
  - Boucle principale : affichage du prompt, lecture de la ligne, ajout à l’historique.
  - Parsing et tokenisation de la ligne.
  - Expansion des variables.
  - Construction de la liste de commandes et des redirections.
  - Exécution : gestion des pipes, redirections, fork/exec, gestion des signaux.
  - Gestion des erreurs et libération de la mémoire.
  - Sortie du shell sur commande ou EOF.

### 13. Remarques finales

- Chaque module est conçu pour être indépendant et robuste.
- La gestion mémoire et des erreurs est systématique à chaque étape.
- Les embranchements sont explicitement gérés pour garantir la stabilité et la sécurité du shell.
