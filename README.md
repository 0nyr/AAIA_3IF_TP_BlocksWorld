# AAIA - TP Monde des Blocks (Blocks World Problem)

![Blocks World illustration](./img/blocks_world_front_image.jpg)

> This project is configured for Linux, NixOS

Le problème du monde des blocs (Blocks World) est un casse-tête classique en intelligence artificielle qui se concentre sur la planification et le raisonnement spatial. Le cadre est simple : plusieurs blocs sont disposés sur une table, et l'objectif est de déplacer ces blocs pour atteindre une configuration spécifique, en utilisant un minimum de mouvements. Chaque mouvement consiste à prendre un bloc et à le déplacer vers un nouvel emplacement, soit sur la table soit sur un autre bloc. Ce problème est utilisé pour tester et comparer des algorithmes de planification et de recherche, tels que A* et AW* (Anytime Weighted A*), car il offre une représentation claire des défis de planification spatiale et de manipulation d'objets tout en restant suffisamment abstrait pour une étude théorique.

## commands

#### Manual compilation

Time to enjoy a makefile...

#### Make compilation

`make dirs`: Before doing anything, create output directories for Make rules.

`make build`: Build by compiling project.

`make rebuild`: Rebuild (recompile) the project.

`make run`: Run compiled executable.
