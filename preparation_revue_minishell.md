# Préparation à la revue de code Minishell

Ce document regroupe les réponses aux questions techniques et des simulations de questions/réponses pour t’aider à réussir ta revue de code, en particulier sur l’exécution, les pipes, les redirections et la gestion des signaux.

---

## 1. Exécution des commandes

**Q : Comment l’exécution des commandes est-elle organisée dans Minishell ?**

R : Chaque commande est isolée dans un processus enfant (`fork`). Les redirections et pipes sont configurés avant l’exécution (`execve` ou builtin). Le parent attend la fin de chaque enfant avec `waitpid` et récupère le code de retour. Cette organisation permet de chaîner plusieurs commandes et d’éviter les effets de bord.

**Q : Pourquoi utiliser `fork` pour chaque commande ?**

R : Cela garantit que chaque commande s’exécute dans son propre contexte, sans modifier l’environnement du shell principal. C’est indispensable pour gérer les pipes et les redirections comme bash.

**Q : Comment récupères-tu le code de retour ?**

R : Le parent utilise `waitpid` pour chaque enfant et stocke le code de retour, qui est accessible via une variable globale pour `$?`.

---

## 2. Pipes et redirections

**Q : Comment as-tu géré les pipes avec redirections dans Minishell ?**

R : Chaque commande séparée par un pipe est stockée dans une liste, avec ses propres redirections. Le shell crée tous les pipes nécessaires, fork un processus pour chaque commande, configure les redirections (stdin, stdout) et connecte les pipes via `dup2`. Les descripteurs inutilisés sont fermés dans chaque processus.

**Q : Comment as-tu implémenté cela ?**

R : La logique est séparée en fonctions : création des pipes, configuration des redirections, fork, exécution. Chaque enfant reçoit uniquement les descripteurs nécessaires. Les redirections sont appliquées avant l’exécution. Le parent ferme les descripteurs inutiles et attend chaque enfant.

**Q : Quelles erreurs à éviter pour un comportement identique à bash ?**

R :
- Toujours fermer les descripteurs inutilisés dans chaque processus.
- Appliquer les redirections avant l’exécution, sans les partager entre commandes.
- Gérer les erreurs d’ouverture de fichiers et de duplication de descripteurs.
- Restituer le code de retour de la dernière commande de la chaîne.
- Ne pas mélanger les redirections et les pipes : chaque commande a son propre contexte.

**Q : Peux-tu donner un exemple concret ?**

R : Pour `cat < in.txt | grep foo > out.txt`, chaque commande a ses propres redirections et les pipes sont connectés entre elles. Si une redirection échoue, la commande ne s’exécute pas et le shell affiche une erreur.

---

## 3. Gestion des signaux

**Q : Comment les signaux sont-ils gérés pendant l’exécution ?**

R : Avant d’exécuter une commande, SIGINT et SIGQUIT sont réinitialisés à leur comportement par défaut dans les enfants, pour permettre à l’utilisateur d’interrompre ou de quitter une commande. Dans le shell principal, ces signaux sont capturés ou ignorés pour éviter la fermeture du shell. Pour le heredoc, SIGINT est ignoré pour éviter une interruption non désirée.

**Q : Pourquoi cette gestion contextuelle ?**

R : Elle garantit que le shell reste interactif et robuste : l’utilisateur peut interrompre une commande sans fermer le shell, et les signaux sont adaptés au contexte (prompt, exécution, heredoc).

**Q : Que se passe-t-il si l’utilisateur interrompt un heredoc ?**

R : SIGINT est ignoré pendant le heredoc pour éviter une interruption non désirée. À la fin, le comportement des signaux est restauré.

---

## 4. Gestion mémoire

**Q : Comment as-tu géré la mémoire et les ressources ?**

R : Chaque structure allouée dynamiquement dispose d’une fonction de libération dédiée. Après chaque exécution ou à la sortie du shell, toutes les ressources sont libérées pour éviter les fuites mémoire. Les erreurs d’allocation sont gérées à chaque étape.

---

## 5. Logique globale et modularité

**Q : Pourquoi avoir structuré le projet en modules indépendants ?**

R : La modularité facilite la lecture, la maintenance et les tests. Chaque module gère ses propres erreurs et embranchements, ce qui rend le shell plus stable et évolutif.

---

## 6. Utilisation d’une variable globale

**Q : Pourquoi avoir utilisé une variable globale dans Minishell ?**

R : Elle permet de partager facilement des informations essentielles (code de retour, état du shell) entre les modules, sans passer des paramètres partout. Son usage est limité et encadré pour éviter les effets de bord.

---

## 7. Autres choix techniques

**Q : Pourquoi utiliser la libft ?**

R : La libft apporte des fonctions utilitaires fiables pour la gestion des chaînes, de la mémoire et des listes chaînées. Cela évite de réécrire des fonctions de base et garantit une meilleure portabilité et robustesse.

---

## Simulations supplémentaires de questions/réponses

**Q : Que se passe-t-il si tu oublies de fermer un descripteur de pipe dans un processus ?**

R : Si un descripteur de pipe reste ouvert dans un processus où il n’est pas utilisé, la commande qui lit sur ce pipe peut attendre indéfiniment la fin du flux, car le pipe n’est jamais fermé. Cela provoque des blocages et des comportements différents de bash. Il faut donc toujours fermer tous les descripteurs inutilisés dans chaque processus.

**Q : Comment gères-tu les erreurs d’ouverture de fichier pour une redirection ?**

R : Si l’ouverture du fichier échoue (par exemple, fichier inexistant ou permissions insuffisantes), la commande ne s’exécute pas. Le shell affiche une erreur claire et passe à la commande suivante ou restitue le code d’erreur, comme le ferait bash.

**Q : Comment assures-tu que le code de retour est identique à bash pour une chaîne de pipes ?**

R : Le shell récupère le code de retour de chaque commande, mais restitue celui de la dernière commande de la chaîne, comme bash. Cela permet d’utiliser `$?` pour tester le succès ou l’échec de la commande finale.

**Q : Que se passe-t-il si une commande builtin est placée au milieu d’une chaîne de pipes ?**

R : Les builtins sont exécutés dans le contexte du processus enfant, avec les pipes et redirections configurés comme pour une commande externe. Cela garantit que le flux de données et le code de retour sont cohérents avec bash.

**Q : Comment gères-tu les signaux lors de l’exécution d’une commande qui lance un processus long (ex : sleep 10) ?**

R : Les signaux SIGINT et SIGQUIT sont réinitialisés à leur comportement par défaut dans le processus enfant. L’utilisateur peut donc interrompre la commande avec Ctrl+C ou la quitter avec Ctrl+\, sans fermer le shell principal.

**Q : Pourquoi ne pas utiliser une variable globale pour tout ?**

R : Une variable globale doit être réservée aux informations essentielles et partagées (ex : code de retour). En abuser rend le code difficile à maintenir et peut provoquer des effets de bord imprévus. Il vaut mieux passer les paramètres localement quand c’est possible.

**Q : Comment as-tu testé la conformité de ton shell avec bash ?**

R : J’ai comparé le comportement de Minishell avec bash sur des cas complexes (pipes, redirections, erreurs, signaux) et corrigé les différences pour garantir une expérience identique.

---

## Conseils pour la revue de code

- Mets en avant la robustesse, la modularité et la conformité au comportement de bash.
- Illustre tes réponses par des exemples concrets.
- Insiste sur la gestion des erreurs et la fermeture des descripteurs.
- Prépare-toi à expliquer les choix techniques et à les justifier.

---

Si tu veux d’autres simulations ou approfondir un point, précise la question !
