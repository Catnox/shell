# Réponses aux questions sur les choix techniques Minishell

---

## Focus : Exécution et gestion des signaux

### Exécution des commandes

**Comment l’exécution des commandes est-elle organisée dans Minishell ?**

L’exécution repose sur une séparation claire entre le parsing et l’exécution : après avoir construit la liste des commandes et des redirections, le shell gère chaque commande dans un contexte isolé. Pour chaque commande, un processus enfant est créé via `fork`. Les redirections et pipes sont configurés avant l’appel à `execve` (pour les commandes externes) ou à la fonction builtin correspondante. Le parent attend la fin de chaque enfant avec `waitpid`, récupère le code de retour et gère les erreurs éventuelles. Cette organisation permet de chaîner plusieurs commandes, de gérer les redirections et de garantir l’indépendance de chaque exécution.

**Pourquoi ce découpage ?**

Ce découpage permet de limiter les effets de bord : chaque commande s’exécute dans son propre processus, ce qui évite qu’une erreur ou une modification d’environnement n’affecte le shell principal. Cela facilite aussi la gestion des pipes et des redirections, qui sont propres à chaque commande ou chaîne de commandes.

### Gestion des signaux

**Comment les signaux sont-ils gérés pendant l’exécution ?**

Avant d’exécuter une commande, le shell adapte la gestion des signaux : SIGINT et SIGQUIT sont réinitialisés à leur comportement par défaut dans les processus enfants, pour permettre à l’utilisateur d’interrompre une commande (Ctrl+C) ou de la quitter (Ctrl+\). Dans le shell principal, ces signaux sont capturés ou ignorés pour éviter la fermeture du shell lors d’une interruption. Pour le heredoc, SIGINT est ignoré pour éviter une interruption non désirée pendant la saisie.

**Pourquoi cette gestion contextuelle ?**

La gestion contextuelle des signaux garantit que le shell reste interactif et robuste : l’utilisateur peut interrompre une commande sans fermer le shell, et les signaux sont adaptés au contexte (prompt, exécution, heredoc). Cela évite les comportements inattendus et assure une expérience utilisateur cohérente.

**Quels sont les avantages de cette approche ?**

- Robustesse : le shell ne quitte pas sur un simple Ctrl+C.
- Précision : chaque commande peut être interrompue individuellement.
- Sécurité : les signaux sont adaptés au contexte pour éviter les corruptions ou les fuites de ressources.

---

## 1. Gestion des redirections

**Comment as-tu géré les redirections dans Minishell ?**

La gestion des redirections repose sur une analyse systématique de la ligne de commande lors du parsing. Chaque redirection (entrée, sortie, append, heredoc) est identifiée et stockée dans une structure dédiée. Lors de l’exécution, le shell ouvre les fichiers nécessaires avec les bons flags (O_RDONLY, O_WRONLY, O_APPEND, etc.), puis utilise `dup2` pour rediriger les descripteurs de fichiers vers stdin ou stdout. Les descripteurs inutilisés sont fermés pour éviter les fuites. Cette approche garantit que chaque commande s’exécute dans le contexte de redirection attendu, tout en isolant la logique dans des fonctions spécialisées pour faciliter la maintenance et la robustesse.

**Pourquoi avoir choisi cette approche pour les redirections ?**

J’ai choisi cette approche car elle sépare clairement la détection des redirections (au parsing) de leur gestion technique (à l’exécution). Cela permet de valider la syntaxe en amont, d’éviter les erreurs d’ouverture de fichiers au dernier moment, et de centraliser la gestion des erreurs. L’utilisation de `dup2` est standard en C pour manipuler les descripteurs, et la fermeture systématique des fichiers limite les risques de fuite de ressources. Cette organisation modulaire rend le code plus lisible, testable et évolutif.

---

## 2. Gestion des pipes

**Comment as-tu géré les pipes ?**

Les pipes sont détectés lors du parsing et chaque commande séparée par un pipe est stockée dans une liste. Lors de l’exécution, le shell crée les pipes nécessaires avec la fonction `pipe`, puis fork un processus pour chaque commande. Les entrées et sorties de chaque processus sont redirigées vers les bons descripteurs grâce à `dup2`. Les descripteurs inutilisés sont fermés dans chaque processus pour éviter les fuites. Cette gestion permet d’enchaîner plusieurs commandes en respectant le flux des données.

**Pourquoi cette organisation ?**

Cette organisation permet de gérer chaque commande indépendamment, tout en assurant la synchronisation des processus via les pipes. L’utilisation de listes et de boucles facilite l’extension à un nombre variable de commandes. La séparation entre parsing et exécution rend le code plus clair et évite les effets de bord.

---

## 3. Gestion des signaux

**Comment as-tu géré les signaux (SIGINT, SIGQUIT, etc.) ?**

Le shell initialise des handlers spécifiques pour SIGINT et SIGQUIT lors de l’affichage du prompt. SIGINT permet d’interrompre la saisie utilisateur sans quitter le shell, tandis que SIGQUIT est ignoré pour éviter une fermeture involontaire. Lors de l’exécution de commandes ou de heredoc, les signaux sont réinitialisés ou ignorés selon le contexte pour garantir un comportement cohérent et sécurisé.

**Pourquoi ce choix ?**

Ce choix permet de rendre le shell interactif et robuste : l’utilisateur peut interrompre une commande ou une saisie sans risquer de fermer le shell par erreur. La gestion contextuelle des signaux évite les comportements inattendus lors de l’exécution de commandes externes ou de la gestion du heredoc.

---

## 4. Gestion mémoire

**Comment as-tu géré la mémoire et les ressources ?**

Chaque structure allouée dynamiquement (listes chaînées, tableaux, chaînes) dispose d’une fonction de libération dédiée. Après chaque exécution de commande ou lors de la sortie du shell, toutes les ressources sont systématiquement libérées pour éviter les fuites mémoire. Les erreurs d’allocation sont gérées à chaque étape pour garantir la stabilité du shell.

**Pourquoi cette rigueur ?**

La gestion stricte de la mémoire est indispensable en C pour éviter les fuites et les corruptions. En structurant le code autour de fonctions de libération, on garantit la robustesse et la maintenabilité du projet.

---

## 5. Logique globale et modularité

**Pourquoi avoir structuré le projet en modules indépendants ?**

La modularité permet de séparer chaque fonctionnalité (parsing, exécution, redirections, pipes, builtins, signaux, utilitaires) dans des fichiers dédiés. Cela facilite la lecture, la maintenance et les tests. Chaque module gère ses propres erreurs et embranchements, ce qui rend le shell plus stable et évolutif.

---

## 6. Autres choix techniques

**Pourquoi utiliser la libft ?**

La libft apporte des fonctions utilitaires fiables pour la gestion des chaînes, de la mémoire et des listes chaînées. Cela évite de réécrire des fonctions de base et garantit une meilleure portabilité et robustesse.

---

## 7. Utilisation d'une variable globale

**Pourquoi avoir utilisé une variable globale dans Minishell ?**

L’utilisation d’une variable globale (par exemple pour le code de retour ou l’état du shell) permet de partager facilement des informations essentielles entre les différents modules du projet, sans avoir à passer systématiquement des paramètres dans toutes les fonctions. Cela simplifie la gestion de l’état du shell, notamment pour le code de retour des commandes, la gestion des signaux ou l’arrêt du shell. Ce choix est justifié lorsque l’information doit être accessible partout et modifiée par plusieurs parties du programme, tout en restant limitée à un usage central (et non pour stocker des données arbitraires).

**Pourquoi cette approche peut être pertinente ?**

Dans un shell, le code de retour doit être accessible à tout moment (pour `$?`, pour les conditions, pour l’affichage d’erreurs). Une variable globale évite la duplication et la complexité inutile. Cependant, son usage est limité et encadré pour éviter les effets de bord et garantir la lisibilité du code.

---

## Focus : Pipes et redirections

### Comment as-tu géré les pipes avec redirections dans Minishell ?

La gestion des pipes et des redirections repose sur une organisation précise : chaque commande séparée par un pipe est stockée dans une liste, avec ses propres redirections (entrée, sortie, heredoc, etc.). Lors de l’exécution, le shell crée tous les pipes nécessaires, puis fork un processus pour chaque commande. Avant d’exécuter la commande, il configure les redirections : si une redirection d’entrée est présente, le descripteur stdin est remplacé ; si une redirection de sortie est présente, stdout est remplacé. Les pipes sont ensuite connectés entre les processus via `dup2`, en respectant l’ordre d’enchaînement. Les descripteurs inutilisés sont fermés dans chaque processus pour éviter les fuites et garantir que chaque commande lit/écrit au bon endroit.

### Comment as-tu implémenté cela ?

J’ai séparé la logique en plusieurs fonctions : création des pipes, configuration des redirections, fork des processus, et exécution de la commande. Chaque processus enfant reçoit uniquement les descripteurs nécessaires : stdin, stdout, et éventuellement les pipes. Les redirections sont appliquées avant l’appel à `execve` ou à la fonction builtin. Le parent ferme les descripteurs inutiles et attends la fin de chaque enfant avec `waitpid`.

### Pourquoi avoir fait ces choix ? Quelles erreurs à éviter pour un comportement identique à bash ?

Ce découpage est essentiel pour reproduire le comportement de bash :
- Il faut absolument fermer tous les descripteurs inutilisés dans chaque processus : sinon, les pipes restent ouverts et les commandes peuvent attendre indéfiniment la fin d’un flux.
- Les redirections doivent être appliquées avant l’exécution de la commande, et ne jamais être partagées entre plusieurs commandes.
- Il faut gérer les erreurs d’ouverture de fichiers et de duplication de descripteurs : si une redirection échoue, la commande ne doit pas s’exécuter et le shell doit afficher une erreur.
- Le code de retour doit être récupéré pour chaque commande, et le shell doit restituer le code de la dernière commande de la chaîne (comme bash).
- Il faut éviter de mélanger les redirections et les pipes : chaque commande doit avoir son propre contexte d’exécution.

En respectant ces principes, on garantit que le shell se comporte comme bash, notamment pour les cas complexes (ex : `cat < in.txt | grep foo > out.txt`).

---

Si tu veux d’autres réponses ou des précisions sur un point particulier, indique la question ou le thème !
