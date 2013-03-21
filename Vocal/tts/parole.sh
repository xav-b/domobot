#!/bin/bash

## Utilisation en français
#pico2wave -l fr-FR -w test.wav "Salut je fais un test de lecture du fichier enregistré. On va voir ce que ça donne mais j'espère que ça va marcher ! Voyons ça."

## Utilisation en anglais
pico2wave -w test.wav "Hi, i would like to introduce myself. I am the director of the biggest company in the world ! So please respect."

## Synthèse vocale du fichier
aplay test.wav

## Nettoyage des fichiers temporaires
rm test.wav


