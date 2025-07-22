# Résumé complet du code `main.c` de Minishell

Ce document détaille le fonctionnement du fichier `main.c` du projet Minishell, fonction par fonction, avec tous les embranchements et la logique générale.

---

## 1. Variable globale
```c
int g_exit_status = 0;
```
- Stocke le code de sortie du shell (utilisé pour la gestion des erreurs et la sortie du programme).

---

## 2. Fonction `print_error`
```c
void print_error(char *cmd, char *msg)
```
- Affiche une erreur formatée sur la sortie d’erreur standard.
- Format : `minishell: <cmd>: <msg>`
- Embranchements :
  - Si `cmd` est non NULL, affiche le nom de la commande.
  - Si `msg` est non NULL, affiche le message d’erreur.

---

## 3. Fonction `is_empty_line`
```c
int is_empty_line(char *line)
```
- Vérifie si une ligne ne contient que des espaces ou tabulations, ou est NULL.
- Retourne 1 si la ligne est vide, sinon 0.
- Embranchements :
  - Si `line` est NULL, retourne 1.
  - Si tous les caractères sont des espaces/tabulations, retourne 1.
  - Sinon, retourne 0.

---

## 4. Fonction `process_command_line`
```c
static void process_command_line(t_data *data)
```
- Ajoute la ligne à l’historique.
- Découpe la ligne en tokens (arguments) avec `tokenize`.
- Si des arguments existent :
  - Pour chaque argument, effectue l’expansion (variables, etc.) avec `expand_and_parse_token`.
  - Remplace l’ancien token par le nouveau.
  - Exécute la commande avec `execute(data)`.
  - Libère le tableau d’arguments avec `ft_free_tab`.
- Embranchements :
  - Si `data->args` est NULL, rien n’est exécuté.

---

## 5. Fonction `minishell_loop`
```c
void minishell_loop(t_env *env)
```
- Initialise la structure de données et les signaux.
- Boucle principale du shell :
  - Affiche le prompt et lit la ligne avec `readline`.
  - Si EOF (Ctrl+D), affiche "exit" et quitte la boucle.
  - Si la ligne est vide (`is_empty_line`), libère la ligne et continue la boucle.
  - Sinon, traite la ligne avec `process_command_line` puis libère la ligne.

---

## 6. Fonction `main`
```c
int main(int argc, char *argv[], char *env[])
```
- Initialise la liste d’environnement à partir de `env` avec `init_env`.
- Si l’environnement échoue, affiche une erreur et quitte avec code 1.
- Lance la boucle principale du shell avec `minishell_loop`.
- Libère la liste d’environnement à la fin avec `free_env_list`.
- Retourne le code de sortie global `g_exit_status`.

---

## Logique générale et embranchements
- **Initialisation** : Prépare l’environnement et les signaux.
- **Boucle shell** : Affiche le prompt, lit la ligne, ignore les lignes vides, traite et exécute la commande.
- **Traitement** : Découpe la ligne, expanse les tokens, exécute la commande, gère l’historique.
- **Gestion mémoire** : Libère les ressources à chaque étape.
- **Sortie** : Quitte proprement sur EOF ou erreur d’environnement.

---

## Arbre d’exécution simplifié
1. `main`
    - Initialise l’environnement
    - Si erreur : affiche et quitte
    - Appelle `minishell_loop`
        - Boucle :
            - Lit la ligne
            - Si EOF : quitte
            - Si vide : ignore
            - Sinon :
                - Appelle `process_command_line`
                    - Tokenize
                    - Expansion
                    - Exécution
                    - Libération
    - Libère l’environnement
    - Retourne le code de sortie

---

## Remarques
- Les fonctions comme `tokenize`, `expand_and_parse_token`, `execute`, `init_env`, `free_env_list` sont appelées mais leur logique dépend des autres fichiers du projet.
- La gestion des signaux et de l’historique est assurée par des fonctions utilitaires.
- Le shell gère proprement la mémoire et les erreurs à chaque étape.

---

**Ce résumé couvre toute la logique du fichier `main.c` et détaille chaque fonction et branchement possible.**
