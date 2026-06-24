genre la en gros on a :

récuperer la touche appuyer
on la transforme en Input(direction,coup)
on stock l'Input dans le Buffer

Roadmap : 

### court terme:
- convention de code
- cicd
- build system


- Struct character : nom, maxhp ,movespeed, hitboxes ,  moveset
- Struct Player : nom, character, hp, coord(x,y) , ring 


- basic front en terminal 
raylib : arene + character
- Placement des personnages dans l'Arene

- basic actions ( movement avant / arriere / sauts  , coup classique ) 
- auto turn 
- Revers Input pour coupspecial
- Struct terrain : fond ( juste avoir des terrain avec des fond de couleur different atm )

moyen terme:
- gestion hit boxes coup / hitbox  
ajouter hitbox sur les coup / coupspecial et character

 (normalement la c'est jouable en tour par tour)

long terme:
- 60 fps en  2 joueurs ( aucune idée perso )
