GUIDE COMPLET D'UTILISATION
Création d'Interfaces GTK avec Syntaxe HTML/XML
TABLE DES MATIÈRES
Introduction

Installation et Démarrage

Structure de Base d'un Document

Widgets Standards

Système de Conteneurs

Système de Menus

Règles d'Écriture

Bonnes Pratiques

Exemples Complets

Dépannage

1. INTRODUCTION
1.1 Qu'est-ce que ce parseur?
Ce parseur permet de décrire des interfaces graphiques GTK en utilisant une syntaxe similaire à HTML. Au lieu d'écrire des centaines de lignes de code C, vous écrivez un fichier HTML simple qui est automatiquement converti en une application GTK fonctionnelle.

1.2 Avantages
Aspect	Bénéfice
Simplicité	Syntaxe intuitive comme HTML
Rapidité	Prototypage en quelques minutes
Lisibilité	La structure visuelle est explicite
Maintenance	Modifications faciles
Réutilisabilité	Mêmes fichiers HTML pour différentes applications
1.3 Comment ça marche
text
Votre fichier HTML      Parseur              Application GTK
┌─────────────────┐    ┌─────────┐          ┌─────────────────┐
│ <window>        │───▶│ Lecture │─────────▶│ Fenêtre avec    │
│   <vbox>        │    │ Analyse │          │ boutons, menus, │
│     <button/>   │    │ Génération         │ labels, etc.    │
│   </vbox>       │    └─────────┘          └─────────────────┘
│ </window>       │
└─────────────────┘
1.4 Prérequis
Système Linux avec GTK3 installé

Compilateur GCC

Connaissances de base en HTML

2. INSTALLATION ET DÉMARRAGE
2.1 Fichiers Nécessaires
Pour utiliser le parseur, vous devez avoir ces trois fichiers dans le même répertoire:

text
votre_projet/
├── parser.c          # Le parseur (fourni)
├── structures.h      # Les structures GTK (fourni)
├── structures.c      # L'implémentation GTK (fourni)
└── mon_interface.html # Votre interface (à créer)
2.2 Compilation du Parseur
bash
gcc -o parser parser.c structures.c `pkg-config --cflags --libs gtk+-3.0` -lm
2.3 Utilisation
bash
# Syntaxe de base
./parser mon_fichier.html

# Avec nom personnalisé pour l'exécutable
./parser mon_fichier.html mon_app

# Exemple concret
./parser mon_interface.html mon_application
2.4 Premier Exemple
Créez un fichier hello.html:

html
<window title="Ma Première App" width="400" height="300">
    <vbox spacing="10">
        <label text="Bonjour le monde!" color="blue" style="bold"/>
        <button label="Cliquez-moi" bgcolor="green"/>
    </vbox>
</window>
Exécutez:

bash
./parser hello.html
Résultat: Une fenêtre GTK avec un label bleu et un bouton vert!

3. STRUCTURE DE BASE D'UN DOCUMENT
3.1 La Balise <window>
Toute interface DOIT commencer par une balise <window>.

html
<window title="Titre" width="largeur" height="hauteur" bgcolor="couleur" resizable="true/false">
    <!-- Contenu ici -->
</window>
Attributs de <window>
Attribut	Type	Description	Exemple
title	texte	Titre de la fenêtre	title="Mon App"
width	nombre	Largeur en pixels	width="800"
height	nombre	Hauteur en pixels	height="600"
bgcolor	couleur	Couleur de fond	bgcolor="#f0f0f0"
resizable	booléen	Redimensionnable?	resizable="true"
Règles IMPORTANTES
Une seule balise <window> par fichier

Tout le contenu doit être À L'INTÉRIEUR de <window>

La balise doit être fermée: </window>

3.2 Indentation
L'indentation n'est PAS obligatoire mais vivement recommandée:

html
<!-- BON - Lisible -->
<window>
    <vbox>
        <button/>
    </vbox>
</window>

<!-- AUSSI BON - Mais moins lisible -->
<window>
<vbox>
<button/>
</vbox>
</window>
3.3 Commentaires
html
<!-- Ceci est un commentaire - ignoré par le parseur -->
<window>
    <!-- Un autre commentaire -->
    <vbox>
        <label text="Hello"/>
    </vbox>
</window>
4. WIDGETS STANDARDS
4.1 Label (Étiquette)
Affiche du texte.

html
<label text="Mon texte" color="rouge" bgcolor="fond" style="bold" position="center" font="Arial"/>
Attributs
Attribut	Valeurs possibles	Description
text	texte quelconque	Le texte à afficher
color	couleur CSS	Couleur du texte
bgcolor	couleur CSS	Couleur de fond
style	bold, italic, underline	Style du texte
position	left, center, right	Alignement
font	nom de police	Police à utiliser
Exemples
html
<!-- Label simple -->
<label text="Nom: "/>

<!-- Label stylisé -->
<label text="Titre Principal" color="#2c3e50" style="bold" position="center"/>

<!-- Label avec fond -->
<label text="Alerte!" bgcolor="red" color="white" style="bold"/>
Contenu entre balises
html
<label>
    Ceci est un texte
    sur plusieurs lignes
</label>
4.2 Button (Bouton)
Bouton cliquable.

html
<button label="Texte" bgcolor="couleur" width="largeur" height="hauteur"/>
Attributs
Attribut	Description	Exemple
label	Texte du bouton	label="OK"
bgcolor	Couleur de fond	bgcolor="#4CAF50"
width	Largeur en pixels	width="120"
height	Hauteur en pixels	height="40"
Exemples
html
<!-- Bouton simple -->
<button label="Cliquez ici"/>

<!-- Bouton vert stylisé -->
<button label="Valider" bgcolor="green" width="100" height="35"/>

<!-- Bouton rouge d'avertissement -->
<button label="Supprimer" bgcolor="red"/>
Contenu entre balises
html
<button>Annuler</button>  <!-- Identique à label="Annuler" -->
4.3 Entry (Champ de saisie)
Zone de texte modifiable.

html
<entry placeholder="Texte indicatif" width="largeur" bgcolor="couleur"/>
Attributs
Attribut	Description	Exemple
placeholder	Texte grisé d'exemple	placeholder="Entrez votre nom"
width	Largeur	width="300"
bgcolor	Couleur de fond	bgcolor="white"
Exemples
html
<!-- Champ texte simple -->
<entry placeholder="Nom d'utilisateur"/>

<!-- Champ mot de passe (texte caché) -->
<entry placeholder="Mot de passe"/>

<!-- Champ large -->
<entry placeholder="Description" width="400"/>
4.4 Checkbox (Case à cocher)
Case à cocher binaire.

html
<checkbox label="Texte" active="true/false"/>
Attributs
Attribut	Description	Exemple
label	Texte à côté	label="J'accepte"
active	État initial	active="true"
Exemples
html
<!-- Non coché par défaut -->
<checkbox label="Se souvenir de moi"/>

<!-- Pré-coché -->
<checkbox label="Recevoir la newsletter" active="true"/>
Contenu entre balises
html
<checkbox active="true">Accepter les conditions</checkbox>
4.5 Radiobutton (Bouton radio)
Sélection unique dans un groupe.

html
<radiobutton group="nom_du_groupe" label="Texte" active="true/false"/>
Attributs
Attribut	Description	Exemple
group	Nom du groupe (OBLIGATOIRE)	group="couleur"
label	Texte affiché	label="Rouge"
active	Sélectionné par défaut?	active="true"
Règle CRITIQUE
Tous les radiobuttons du même groupe doivent avoir le MÊME group.

html
<!-- BON - Même groupe -->
<radiobutton group="taille" label="Petit" active="true"/>
<radiobutton group="taille" label="Moyen"/>
<radiobutton group="taille" label="Grand"/>

<!-- MAUVAIS - Groupes différents -->
<radiobutton group="taille1" label="Petit"/>
<radiobutton group="taille2" label="Moyen"/>  <!-- Pas le même groupe! -->
Exemple complet
html
<vbox spacing="5">
    <label text="Choisissez une couleur:"/>
    <radiobutton group="couleur" label="Rouge" active="true"/>
    <radiobutton group="couleur" label="Vert"/>
    <radiobutton group="couleur" label="Bleu"/>
</vbox>
4.6 Image
Affiche une image.

html
<image file="chemin/fichier.png" width="largeur" height="hauteur"/>
Attributs
Attribut	Description	Exemple
file	Chemin vers l'image	file="logo.png"
width	Largeur d'affichage	width="100"
height	Hauteur d'affichage	height="100"
Exemple
html
<image file="icone.png" width="64" height="64"/>
4.7 ProgressBar (Barre de progression)
Affiche une progression.

html
<progressbar width="largeur" height="hauteur"/>
Attributs
Attribut	Description	Exemple
width	Largeur	width="400"
height	Hauteur	height="25"
Exemple
html
<progressbar width="300" height="20"/>
4.8 Spinner (Animation d'attente)
Indique une opération en cours.

html
<spinner text="Message" active="true/false"/>
Attributs
Attribut	Description	Exemple
text	Texte à côté	text="Chargement..."
active	Animation active	active="true"
Exemple
html
<spinner text="Traitement en cours..." active="true"/>
4.9 HeaderBar (Barre d'en-tête)
Barre supérieure personnalisée (remplace la barre de titre système).

html
<headerbar title="Titre" subtitle="Sous-titre" showclose="true/false"/>
Attributs
Attribut	Description	Exemple
title	Titre principal	title="Application"
subtitle	Sous-titre	subtitle="Version 1.0"
showclose	Afficher bouton X?	showclose="true"
Exemple
html
<headerbar title="Mon Éditeur" subtitle="Document.txt" showclose="true"/>
5. SYSTÈME DE CONTENEURS
5.1 VBox (Vertical Box)
Disposition VERTICALE (haut → bas).

html
<vbox spacing="espace" margin="marge" bgcolor="couleur">
    <!-- Enfants ici -->
</vbox>
Attributs
Attribut	Description	Exemple
spacing	Espace entre enfants (px)	spacing="10"
margin	Marge autour (px)	margin="20"
bgcolor	Couleur de fond	bgcolor="#f0f0f0"
Exemple
html
<vbox spacing="15" margin="10">
    <label text="Ligne 1"/>
    <label text="Ligne 2"/>
    <label text="Ligne 3"/>
</vbox>
5.2 HBox (Horizontal Box)
Disposition HORIZONTALE (gauche → droite).

html
<hbox spacing="espace" margin="marge">
    <!-- Enfants ici -->
</hbox>
Attributs
Attribut	Description	Exemple
spacing	Espace entre enfants (px)	spacing="5"
margin	Marge autour (px)	margin="10"
bgcolor	Couleur de fond	bgcolor="#e0e0e0"
Exemple
html
<hbox spacing="10">
    <button label="OK"/>
    <button label="Annuler"/>
    <button label="Appliquer"/>
</hbox>
5.3 Combinaison VBox + HBox
Pour des interfaces complexes, imbriquez les conteneurs:

html
<window title="Formulaire" width="500" height="400">
    <vbox spacing="20" margin="15">
        
        <!-- Section 1: Titre -->
        <label text="Informations Personnelles" style="bold"/>
        
        <!-- Section 2: Formulaire en lignes -->
        <vbox spacing="10">
            <hbox spacing="10">
                <label text="Nom:" width="80"/>
                <entry placeholder="Votre nom"/>
            </hbox>
            
            <hbox spacing="10">
                <label text="Email:" width="80"/>
                <entry placeholder="email@exemple.com"/>
            </hbox>
        </vbox>
        
        <!-- Section 3: Boutons en bas -->
        <hbox spacing="10">
            <button label="Valider" bgcolor="green"/>
            <button label="Annuler" bgcolor="red"/>
        </hbox>
        
    </vbox>
</window>
6. SYSTÈME DE MENUS
6.1 Menu Button (Bouton déroulant)
Un bouton qui affiche un menu au clic.

html
<menu_button label="Texte" icon="nom_icone" bgcolor="couleur" width="largeur">
    <menu orientation="vertical">
        <!-- menu_items ici -->
    </menu>
</menu_button>
Attributs
Attribut	Description	Exemple
label	Texte du bouton	label="Fichier"
icon	Icône système	icon="document-open"
bgcolor	Couleur du bouton	bgcolor="#3498db"
width	Largeur	width="100"
height	Hauteur	height="35"
Icônes système disponibles
Icône	Utilisation
document-new	Nouveau fichier
document-open	Ouvrir fichier
document-save	Sauvegarder
edit-cut	Couper
edit-copy	Copier
edit-paste	Coller
help-about	À propos
application-exit	Quitter
preferences-system	Paramètres
6.2 Menu
Conteneur pour les items de menu.

html
<menu orientation="vertical">
    <!-- menu_item, menu_separator ici -->
</menu>
Attributs
Attribut	Description	Exemple
orientation	vertical ou horizontal	orientation="vertical"
6.3 Menu Item
Item cliquable dans un menu.

html
<menu_item label="Texte"/>
Attributs
Attribut	Description	Exemple
label	Texte affiché	label="Nouveau"
Contenu entre balises
html
<menu_item>Ouvrir</menu_item>
6.4 Menu Separator
Ligne de séparation dans un menu.

html
<menu_separator/>
6.5 Menu Item avec Sous-menu
Un item peut contenir un sous-menu (imbrication).

html
<menu_item label="Ouvrir">
    <menu orientation="vertical">
        <menu_item label="Fichier 1"/>
        <menu_item label="Fichier 2"/>
        <menu_separator/>
        <menu_item label="Fichier récent"/>
    </menu>
</menu_item>
6.6 Exemple Complet de Menu
html
<window title="Application avec Menus" width="600" height="400">
    <vbox spacing="0">
        
        <!-- Barre d'outils avec menu buttons -->
        <hbox spacing="5" bgcolor="#e0e0e0" margin="5">
            
            <menu_button label="Fichier" bgcolor="#3498db" width="80">
                <menu orientation="vertical">
                    <menu_item label="Nouveau"/>
                    <menu_item label="Ouvrir"/>
                    <menu_separator/>
                    <menu_item label="Sauvegarder"/>
                    <menu_separator/>
                    <menu_item label="Quitter"/>
                </menu>
            </menu_button>
            
            <menu_button label="Édition" bgcolor="#3498db" width="80">
                <menu orientation="vertical">
                    <menu_item label="Copier"/>
                    <menu_item label="Coller"/>
                    <menu_separator/>
                    <menu_item label="Tout sélectionner"/>
                </menu>
            </menu_button>
            
            <menu_button label="Ouvrir" bgcolor="#2ecc71" width="80">
                <menu orientation="vertical">
                    <menu_item label="Ouvrir récent">
                        <menu orientation="vertical">
                            <menu_item label="Projet 1"/>
                            <menu_item label="Projet 2"/>
                            <menu_item label="Projet 3"/>
                        </menu>
                    </menu_item>
                    <menu_item label="Ouvrir depuis">
                        <menu orientation="vertical">
                            <menu_item label="Disque local"/>
                            <menu_item label="Cloud"/>
                            <menu_item label="Réseau"/>
                        </menu>
                    </menu_item>
                </menu>
            </menu_button>
            
        </hbox>
        
        <!-- Contenu principal -->
        <vbox margin="20">
            <label text="Bienvenue dans l'application" style="bold" position="center"/>
            <label text="Utilisez les menus ci-dessus pour naviguer" position="center"/>
        </vbox>
        
    </vbox>
</window>
7. RÈGLES D'ÉCRITURE
7.1 Syntaxe des Balises
Balise ouvrante + fermante
html
<vbox>
    <!-- contenu -->
</vbox>
Balise auto-fermante
html
<button label="OK"/>
<label text="Texte"/>
<menu_separator/>
Balise avec contenu texte
html
<button>Cliquez ici</button>
<label>Texte du label</label>
7.2 Écriture des Attributs
html
<!-- RÈGLE 1: Toujours entre guillemets -->
<button label="Mon Bouton"/>     <!-- BON -->
<button label=Mon Bouton/>       <!-- MAUVAIS -->

<!-- RÈGLE 2: Séparés par des espaces -->
<button label="OK" width="100"/>  <!-- BON -->
<button label="OK"width="100"/>    <!-- MAUVAIS -->

<!-- RÈGLE 3: Guillemets simples ou doubles -->
<button label="OK"/>   <!-- BON -->
<button label='OK'/>   <!-- BON -->
7.3 Valeurs Booléennes
html
<!-- Pour "vrai" -->
resizable="true"
active="true"
showclose="true"

<!-- Pour "faux" -->
resizable="false"
active="false"
showclose="false"
7.4 Valeurs de Couleur
html
<!-- Format hexadécimal -->
bgcolor="#FF0000"    <!-- Rouge -->
bgcolor="#00FF00"    <!-- Vert -->
bgcolor="#0000FF"    <!-- Bleu -->
bgcolor="#333333"    <!-- Gris foncé -->

<!-- Noms de couleurs -->
bgcolor="red"
bgcolor="blue"
bgcolor="green"
bgcolor="white"
bgcolor="black"

<!-- RGB -->
bgcolor="rgb(255,0,0)"
7.5 Hiérarchie Correcte
html
<!-- BON - Hiérarchie logique -->
<window>
    <vbox>
        <hbox>
            <button/>
            <button/>
        </hbox>
        <label/>
    </vbox>
</window>

<!-- MAUVAIS - Widgets flottants -->
<window>
    <button/>
    <vbox>          <!-- À l'intérieur de window, OK -->
        <label/>
    </vbox>
</window>
<!-- Ceci fonctionne mais peut être ambigu -->

<!-- MAUVAIS - Structure incohérente -->
<window>
    <button>        <!-- Ouverture button -->
        <label/>    <!-- label DANS button - interdit! -->
    </button>
</window>
7.6 Widgets Qui PEUVENT Contenir des Enfants
Widget	Peut contenir
window	vbox, hbox, headerbar
vbox	n'importe quel widget
hbox	n'importe quel widget
headerbar	rien directement (via attributs)
menu	menu_item, menu_separator
menu_button	menu
menu_item	menu (pour sous-menu)
7.7 Widgets Qui NE PEUVENT PAS Contenir des Enfants
label

button

entry

checkbox

radiobutton

progressbar

spinner

image

menu_separator

8. BONNES PRATIQUES
8.1 Organisation du Fichier
html
<!-- Structure recommandée -->
<window title="Titre" width="800" height="600">
    
    <!-- 1. Header bar (optionnelle) -->
    <headerbar title="Mon App" showclose="true"/>
    
    <!-- 2. Conteneur principal -->
    <vbox spacing="0">
        
        <!-- 3. Barre d'outils (optionnelle) -->
        <hbox spacing="5" margin="5" bgcolor="#e0e0e0">
            <!-- menu_buttons -->
        </hbox>
        
        <!-- 4. Contenu principal -->
        <vbox margin="20" spacing="15">
            <!-- widgets principaux -->
        </vbox>
        
        <!-- 5. Barre de statut (optionnelle) -->
        <hbox margin="5" bgcolor="#f0f0f0">
            <label text="Prêt"/>
        </hbox>
        
    </vbox>
    
</window>
8.2 Nommage des Groupes Radio
html
<!-- Utilisez des noms explicites -->
<radiobutton group="option_theme" label="Clair"/>
<radiobutton group="option_theme" label="Sombre"/>

<radiobutton group="option_taille" label="Petit"/>
<radiobutton group="option_taille" label="Grand"/>

<!-- Évitez les noms vagues -->
<radiobutton group="g1" label="A"/>    <!-- MAUVAIS -->
<radiobutton group="groupe1" label="A"/>  <!-- MIEUX -->
<radiobutton group="theme_couleur" label="A"/>  <!-- BON -->
8.3 Espacement et Marges
html
<!-- Utilisez spacing pour l'espacement ENTRE enfants -->
<vbox spacing="15">
    <label text="Ligne 1"/>
    <label text="Ligne 2"/>  <!-- 15px entre Ligne 1 et Ligne 2 -->
</vbox>

<!-- Utilisez margin pour l'espacement AUTOUR du conteneur -->
<vbox margin="20">
    <!-- 20px de marge sur tous les côtés -->
</vbox>
8.4 Combinaison de Conteneurs
html
<!-- Exemple: Formulaire à deux colonnes -->
<window title="Formulaire" width="600" height="400">
    <vbox margin="20" spacing="15">
        
        <!-- Ligne 1 -->
        <hbox spacing="10">
            <label text="Nom:" width="100"/>
            <entry placeholder="Entrez votre nom" width="300"/>
        </hbox>
        
        <!-- Ligne 2 -->
        <hbox spacing="10">
            <label text="Email:" width="100"/>
            <entry placeholder="email@exemple.com" width="300"/>
        </hbox>
        
        <!-- Ligne 3: Multi-colonnes -->
        <hbox spacing="10">
            <label text="Pays:" width="100"/>
            <entry placeholder="Pays" width="140"/>
            <label text="Ville:" width="60"/>
            <entry placeholder="Ville" width="140"/>
        </hbox>
        
        <!-- Boutons -->
        <hbox spacing="10">
            <button label="Valider" bgcolor="green" width="100"/>
            <button label="Annuler" bgcolor="red" width="100"/>
        </hbox>
        
    </vbox>
</window>
8.5 Utilisation des Marges pour l'Alignement
html
<!-- Centrer un widget -->
<vbox>
    <hbox>
        <label text="Texte centré"/>
    </hbox>
</vbox>

<!-- Ajouter de l'espace à gauche -->
<hbox>
    <label text="Colonne de gauche" width="200"/>
    <label text="Colonne de droite"/>
</hbox>
9. EXEMPLES COMPLETS
9.1 Application de Connexion
html
<window title="Connexion" width="400" height="350" bgcolor="#f5f5f5">
    
    <vbox margin="30" spacing="20">
        
        <!-- Logo/Titre -->
        <label text="🔐 Connexion" style="bold" color="#2c3e50" position="center" font="Arial, 18"/>
        
        <!-- Champs -->
        <vbox spacing="15">
            <entry placeholder="Nom d'utilisateur" width="300" bgcolor="white"/>
            <entry placeholder="Mot de passe" width="300" bgcolor="white"/>
        </vbox>
        
        <!-- Options -->
        <hbox spacing="10">
            <checkbox label="Se souvenir de moi"/>
        </hbox>
        
        <!-- Boutons -->
        <vbox spacing="10">
            <button label="Se connecter" bgcolor="#3498db" width="200" height="40"/>
            <button label="Créer un compte" bgcolor="#95a5a6" width="200"/>
        </vbox>
        
        <!-- Lien -->
        <label text="Mot de passe oublié?" color="#3498db" position="center"/>
        
    </vbox>
    
</window>
9.2 Éditeur de Texte Simple
html
<window title="Éditeur de Texte" width="800" height="600">
    
    <vbox spacing="0">
        
        <!-- Barre de menu -->
        <hbox bgcolor="#e0e0e0" margin="5" spacing="5">
            
            <menu_button label="Fichier" bgcolor="#34495e" width="80">
                <menu orientation="vertical">
                    <menu_item label="Nouveau"/>
                    <menu_item label="Ouvrir"/>
                    <menu_item label="Sauvegarder"/>
                    <menu_separator/>
                    <menu_item label="Quitter"/>
                </menu>
            </menu_button>
            
            <menu_button label="Édition" bgcolor="#34495e" width="80">
                <menu orientation="vertical">
                    <menu_item label="Copier"/>
                    <menu_item label="Coller"/>
                    <menu_separator/>
                    <menu_item label="Rechercher"/>
                </menu>
            </menu_button>
            
            <menu_button label="Format" bgcolor="#34495e" width="80">
                <menu orientation="vertical">
                    <menu_item label="Gras"/>
                    <menu_item label="Italique"/>
                    <menu_separator/>
                    <menu_item label="Couleur">
                        <menu orientation="vertical">
                            <menu_item label="Rouge"/>
                            <menu_item label="Vert"/>
                            <menu_item label="Bleu"/>
                        </menu>
                    </menu_item>
                </menu>
            </menu_button>
            
        </hbox>
        
        <!-- Zone d'édition -->
        <vbox margin="20" spacing="15">
            <entry placeholder="Titre du document" width="500" bgcolor="white"/>
            
            <vbox margin="10">
                <entry placeholder="Écrivez votre texte ici..." width="700" height="400"/>
            </vbox>
            
            <!-- Barre de progression -->
            <progressbar width="500" height="15"/>
            <spinner text="Vérification orthographique..." active="true"/>
            
            <!-- Boutons d'action -->
            <hbox spacing="10">
                <button label="Enregistrer" bgcolor="#27ae60" width="120"/>
                <button label="Annuler" bgcolor="#e74c3c" width="120"/>
            </hbox>
        </vbox>
        
    </vbox>
    
</window>
9.3 Interface de Paramètres
html
<window title="Paramètres" width="550" height="500" bgcolor="#f8f9fa">
    
    <vbox margin="20" spacing="20">
        
        <label text="⚙️ Paramètres de l'Application" style="bold" position="center" font="Arial, 16"/>
        
        <!-- Section Apparence -->
        <vbox spacing="10">
            <label text="Apparence" style="bold" color="#2c3e50"/>
            
            <hbox spacing="10">
                <label text="Thème:" width="120"/>
                <radiobutton group="theme" label="Clair" active="true"/>
                <radiobutton group="theme" label="Sombre"/>
                <radiobutton group="theme" label="Système"/>
            </hbox>
            
            <hbox spacing="10">
                <label text="Taille du texte:" width="120"/>
                <radiobutton group="size" label="Petite"/>
                <radiobutton group="size" label="Moyenne" active="true"/>
                <radiobutton group="size" label="Grande"/>
            </hbox>
            
            <checkbox label="Activer les animations" active="true"/>
            <checkbox label="Mode haute performance"/>
            
        </vbox>
        
        <menu_separator/>
        
        <!-- Section Notifications -->
        <vbox spacing="10">
            <label text="Notifications" style="bold" color="#2c3e50"/>
            
            <checkbox label="Notifications bureau" active="true"/>
            <checkbox label="Sons de notification"/>
            <checkbox label="Email quotidien"/>
            
            <hbox spacing="10">
                <label text="Langue:" width="100"/>
                <radiobutton group="lang" label="Français" active="true"/>
                <radiobutton group="lang" label="English"/>
                <radiobutton group="lang" label="Español"/>
            </hbox>
            
        </vbox>
        
        <menu_separator/>
        
        <!-- Section Stockage -->
        <vbox spacing="10">
            <label text="Stockage" style="bold" color="#2c3e50"/>
            
            <label text="Espace utilisé: 2.3 GB / 10 GB"/>
            <progressbar width="400" height="20"/>
            
            <checkbox label="Nettoyer automatiquement"/>
            
        </vbox>
        
        <!-- Boutons -->
        <hbox spacing="10">
            <button label="Appliquer" bgcolor="#3498db" width="100" height="35"/>
            <button label="Annuler" bgcolor="#95a5a6" width="100" height="35"/>
        </hbox>
        
    </vbox>
    
</window>
9.4 Application Multimédia (Lecteur Audio)
html
<window title="Lecteur Audio" width="600" height="400" bgcolor="#1a1a2e">
    
    <vbox spacing="0">
        
        <!-- Header bar -->
        <headerbar title="Mon Lecteur" subtitle="Musique" showclose="true"/>
        
        <!-- Barre d'outils -->
        <hbox spacing="10" margin="15">
            <menu_button icon="media-playback-start" bgcolor="#e74c3c">
                <menu orientation="vertical">
                    <menu_item label="Lire"/>
                    <menu_item label="Pause"/>
                    <menu_item label="Stop"/>
                </menu>
            </menu_button>
            
            <menu_button icon="media-skip-backward" bgcolor="#2c3e50">
                <menu orientation="vertical">
                    <menu_item label="Précédent"/>
                    <menu_item label="Retour 10s"/>
                </menu>
            </menu_button>
            
            <menu_button icon="media-skip-forward" bgcolor="#2c3e50">
                <menu orientation="vertical">
                    <menu_item label="Suivant"/>
                    <menu_item label="Avancer 10s"/>
                </menu>
            </menu_button>
            
            <menu_button label="Playlist" bgcolor="#27ae60">
                <menu orientation="vertical">
                    <menu_item label="Chanson 1"/>
                    <menu_item label="Chanson 2"/>
                    <menu_item label="Chanson 3"/>
                    <menu_separator/>
                    <menu_item label="Ajouter à la playlist"/>
                </menu>
            </menu_button>
            
        </hbox>
        
        <!-- Information chanson -->
        <vbox margin="20" spacing="10" bgcolor="#16213e">
            <label text="🎵 Chanson en cours" color="#e94560" style="bold" position="center"/>
            <label text="Titre: Bohemian Rhapsody" color="white" position="center"/>
            <label text="Artiste: Queen" color="#aaa" position="center"/>
            
            <!-- Progression -->
            <label text="Progression" color="white"/>
            <progressbar width="500" height="10"/>
            
            <hbox spacing="10">
                <label text="01:23" color="white"/>
                <label text="───────────●───────────" color="white" position="center"/>
                <label text="05:45" color="white"/>
            </hbox>
            
            <spinner text="Tamponnage..." active="false"/>
            
        </vbox>
        
    </vbox>
    
</window>
10. DÉPANNAGE
10.1 Erreurs Fréquentes et Solutions
Erreur: "No valid widgets found"
Cause: Aucune balise reconnue dans le fichier.

Solutions:

Vérifiez que vous avez une balise <window>

Vérifiez l'orthographe des balises (<vbox> et non <vboxx>)

Vérifiez que les balises sont bien fermées

Erreur: "Cannot open file"
Cause: Le fichier n'existe pas.

Solution:

bash
# Vérifiez que le fichier existe
ls -la mon_fichier.html

# Vérifiez le chemin
./parser ../autre_dossier/fichier.html
Erreur: "structures.c not found"
Cause: Le fichier structures.c n'est pas dans le répertoire.

Solution:

bash
# Copiez les fichiers nécessaires
cp /chemin/vers/structures.c .
cp /chemin/vers/structures.h .
Compilation Failed
Cause: Erreur de syntaxe dans le HTML.

Solutions courantes:

Vérifiez les guillemets autour des valeurs d'attributs

Vérifiez que toutes les balises sont fermées

Vérifiez que les balises auto-fermantes ont />

html
<!-- Problèmes courants -->
<button label=OK/>      <!-- Manque guillemets -->
<button label="OK">     <!-- Pas fermé -->
<button label="OK"/>    <!-- BON -->

<!-- Problèmes de hiérarchie -->
<button>
    <label/>            <!-- label DANS button - interdit! -->
</button>
10.2 Problèmes d'Affichage
Les boutons radio ne fonctionnent pas en groupe
Vérifiez l'attribut group:

html
<!-- BON -->
<radiobutton group="options" label="Option 1"/>
<radiobutton group="options" label="Option 2"/>

<!-- MAUVAIS - groupes différents -->
<radiobutton group="opt1" label="Option 1"/>
<radiobutton group="opt2" label="Option 2"/>
Le menu ne s'affiche pas
Vérifiez la structure:

html
<!-- BON -->
<menu_button label="Menu">
    <menu orientation="vertical">   <!-- menu DANS menu_button -->
        <menu_item label="Item"/>
    </menu>
</menu_button>

<!-- MAUVAIS - menu en dehors -->
<menu_button label="Menu"/>
<menu>                               <!-- menu hors du button -->
    <menu_item label="Item"/>
</menu>
Les marges ne s'appliquent pas
Vérifiez l'ordre:

html
<!-- BON - margin sur le conteneur -->
<vbox margin="20">
    <button label="Bouton"/>  <!-- bouton a 20px de marge -->
</vbox>

<!-- margin sur un widget qui ne le supporte pas -->
<button margin="20" label="Bouton"/>  <!-- ignoré -->
10.3 Vérification de la Hiérarchie
Le parseur affiche la hiérarchie trouvée:

text
Widget hierarchy:
  window_0 (type: window, parent: none)
  vbox_1 (type: vbox, parent: window_0)
  button_2 (type: button, parent: vbox_1)
À vérifier:

Le parent du widget est-il celui attendu?

Y a-t-il des widgets avec parent "none" (sauf window)?

L'ordre est-il logique?

10.4 Conseils de Débogage
Commencez petit

html
<!-- Testez d'abord une fenêtre simple -->
<window title="Test" width="300" height="200">
    <label text="Ça marche!"/>
</window>
Ajoutez progressivement

html
<!-- Ajoutez un widget à la fois -->
<window title="Test" width="300" height="200">
    <vbox>
        <label text="Test 1"/>
        <!-- <button label="Test 2"/> -->  <!-- Décommentez après test -->
    </vbox>
</window>
Vérifiez les messages du parseur

text
Parsing file...
  Found tag: window at line 1
    Pushed to stack: window_0, stack_top=0
  Found tag: vbox at line 2
    Pushed to stack: vbox_1, stack_top=1
  Found tag: label at line 3
Utilisez des couleurs vives pour tester

html
<!-- Facilite le repérage visuel -->
<window bgcolor="#ffcccc">   <!-- Fond rouge clair -->
    <vbox bgcolor="#ccffcc">  <!-- Fond vert clair -->
        <button bgcolor="#ccccff" label="Bouton bleu clair"/>
    </vbox>
</window>
RÉSUMÉ RAPIDE
Liste des Widgets
Widget	Utilisation
window	Conteneur principal (OBLIGATOIRE)
vbox	Disposition verticale
hbox	Disposition horizontale
label	Texte
button	Bouton cliquable
entry	Champ de saisie
checkbox	Case à cocher
radiobutton	Bouton radio (nécessite group)
progressbar	Barre de progression
spinner	Animation d'attente
image	Image
headerbar	Barre d'en-tête
menu_button	Bouton avec menu déroulant
menu	Conteneur de menu
menu_item	Élément de menu
menu_separator	Ligne de séparation
Attributs Essentiels
Attribut	Widgets	Description
label	button, menu_item, checkbox	Texte affiché
text	label	Texte du label
placeholder	entry	Texte indicatif
group	radiobutton	Groupe (OBLIGATOIRE)
spacing	vbox, hbox	Espacement interne
margin	vbox, hbox	Marge externe
bgcolor	presque tous	Couleur de fond
width	plusieurs	Largeur
height	plusieurs	Hauteur
Points Clés à Retenir
Une seule <window> par fichier

Tout est contenu dans <window>

Les groupes radio doivent avoir le même group

Les menus doivent être DANS un menu_button

Les sous-menus se font par imbrication

Les conteneurs (vbox, hbox) peuvent être imbriqués

Les attributs doivent être entre guillemets

Bonne création d'interfaces! 🚀

