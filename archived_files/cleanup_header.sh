#!/bin/bash

# Script pour nettoyer le fichier minishell.h en supprimant les fonctions non utilisées
# Date: 19 juillet 2025

echo "Analyse des fonctions non utilisées dans minishell.h..."

# Sauvegarde du header original
cp includes/minishell.h archived_files/minishell_original.h

echo "Header original sauvegardé dans archived_files/minishell_original.h"
echo "Version nettoyée disponible dans archived_files/minishell_clean.h"
echo "Implémentations archivées dans archived_files/unused_functions.c"

echo ""
echo "Fonctions archivées :"
echo "====================="

# Liste des catégories de fonctions archivées
categories=(
    "Fonctions principales non utilisées"
    "Fonctions de parsing non utilisées"
    "Utilitaires de chemin non utilisés"
    "Fonctions d'exécution AST non utilisées"
    "Fonctions AST non utilisées"
    "Fonctions de redirection non utilisées"
    "Fonctions robustes non utilisées"
    "Fonctions heredoc robustes non utilisées"
    "Utilitaires non utilisés"
    "Fonctions export non utilisées"
    "Builtin non utilisé"
)

for category in "${categories[@]}"; do
    echo "- $category"
done

echo ""
echo "Total: ~60 fonctions archivées"
echo ""
echo "Pour restaurer une fonction :"
echo "1. Copier l'implémentation depuis archived_files/unused_functions.c"
echo "2. Ajouter la déclaration dans includes/minishell.h"
echo "3. Implémenter la logique si c'était un stub"

# Optionnel: proposer de remplacer le header actuel
read -p "Voulez-vous remplacer includes/minishell.h par la version nettoyée ? (y/N): " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    cp archived_files/minishell_clean.h includes/minishell.h
    echo "Header remplacé par la version nettoyée"
    echo "L'original est toujours disponible dans archived_files/minishell_original.h"
else
    echo "Header non modifié. La version nettoyée reste dans archived_files/"
fi
