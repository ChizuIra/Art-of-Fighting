#include "la.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>




typedef enum{
    NEUTRE = 5,
    HAUTG = 7,
    HAUT = 8,
    HAUTD = 9,
    DROIT = 6,
    BASD = 3,
    BAS = 2,
    BASG = 1,
    GAUCHE = 4,
}Direction;

typedef enum{
    NONE,
    POING,
    PIED,
    GPOING,
    GPIED,
} Coup;

typedef struct {
    Direction direction;
    Coup coup;
}Input;


typedef struct {
    Input array[8];
    size_t size;
    char name[256];
}CoupSpecial;

typedef struct {
    CoupSpecial array[256];
    size_t size;
}Moveset;


typedef struct{
    Input array[120];
    size_t start;    
    size_t end;
}Ring;


typedef struct {
    char name[256];
    int max_health;
    Moveset moveset;
}Character;

typedef struct {
    char name[256];
    Character character;
    int health;
    V2i coords;
    Ring ring;
}Player;

Ring create_ring(){
    Ring ring;
    ring.start = 0;
    ring.end = 0;
    return ring;
}

Player create_player(char* name,Character selected_character,V2i spawn_coords){
    Player player;
    strcpy(player.name,name);
    player.ring = create_ring();
    player.character = selected_character;
    player.health = player.character.max_health;
    player.coords.x = spawn_coords.x;
    player.coords.y = spawn_coords.y;
    return player;
}

size_t get_diff(Ring ring){
    if(ring.end >= ring.start){
        return ring.end - ring.start;
    }else{
        return (ring.end+120)-ring.start;
    }
}
void ring_offset(Ring *ring){
    size_t diff = get_diff(*ring);    
    if(diff >= 60){
        ring->start = (ring->start+1)%120;
    }
    ring->end = (ring->end+1)%120;
}
Input create_input(Direction direction,Coup coup){
    Input input;
    input.direction = direction;
    input.coup = coup;
    return input;
} 

void add_input(Ring *ring,Input input){
    ring->array[ring->end] = input;
    ring_offset(ring);
}


const Input NO_INPUT= {.direction = NEUTRE,.coup=NONE};

Input get_pressed_input(char pressed){
    switch(pressed){
        case '1':
            return create_input(BASG,NONE);
        case '2':
            return create_input(BAS,NONE);
        case '3':
            return create_input(BASD,NONE);
        case '4':
            return create_input(GAUCHE,NONE);
        case '5':
            return create_input(NEUTRE,NONE);
        case '6':
            return create_input(DROIT,NONE);
        case '7':
            return create_input(HAUTG,NONE);
        case '8':
            return create_input(HAUT,NONE);
        case '9':
            return create_input(HAUTD,NONE);
        case 'a':
            return create_input(NEUTRE,POING);
        case 'z':
            return create_input(NEUTRE,GPOING);
        case 'e':
            return create_input(NEUTRE,PIED);
        case 'r':
            return create_input(NEUTRE,GPIED);
        default:
            return NO_INPUT;
    }

}

// crée les CoupSpecial
// CHARACTER RYU
const CoupSpecial HADOKEN = {
    .array = {
        {BAS,NONE},
        {BASD,NONE},
        {DROIT,NONE},
        {NEUTRE,POING}
    },
    .size = 4,
    .name = "Haadoken!" 
};

const CoupSpecial SHORYUKEN = {
    .array = {
        {DROIT,NONE},
        {BAS,NONE},
        {BASD,NONE},
        {NEUTRE,POING}

    },
    .size = 4,
    .name = "Shooryuken!" 
};

const CoupSpecial TATSUMAKI= {
    .array = {
        {BAS,NONE},
        {BASG,NONE},
        {GAUCHE,NONE},
        {NEUTRE,PIED}
    },
    .size = 4,
    .name = "Tatsumaki Senpukyakui!" 
};
// CHARACTER ..

// crée les Moveset
const Moveset RYU_MOVESET = {
    .array = {
        HADOKEN,
        SHORYUKEN,
        TATSUMAKI
    },
    .size = 3
}; 

// crée les Character
const Character RYU = {
    .name = "Ryu",
    .max_health = 1000,
    .moveset = RYU_MOVESET,
    };

Input get_input(Ring ring,size_t i){
    return ring.array[i+ring.start%120];
}

void print_ring(Ring ring){
    size_t diff=get_diff(ring);
    for(size_t i=0;i<diff;i++){
        printf("{%d,%d} ",get_input(ring,i).direction,get_input(ring,i).coup);
    }    
    printf("\n");
}
int compare_input(Input i1,Input i2){
    return i1.coup == i2.coup && i1.direction == i2.direction;
}

int interpretator(Ring ring,CoupSpecial coup_special){
    int j = coup_special.size-1;
    size_t diff=get_diff(ring);
    for(size_t i=diff-1;i<diff;i--){
        Input current_input = get_input(ring,i);
        if(compare_input(current_input,coup_special.array[j])){
            if(j==0){
                return 1;
            }
            j--;
        }else if(compare_input(current_input, NO_INPUT)&& i<diff-1){
            continue;
        }else{
            break;
        }

    }  
    return 0;  
}


int  find_coup_special(Ring ring,Moveset moveset){
    int finded = 0;
    for(size_t i = 0;i < moveset.size;i++){
        finded = interpretator(ring,moveset.array[i]);
        if(finded == 1){
            return i;
        }
    }
    return -1;
}


const V2i SPAWN_COORDS_J1 = {
    .x = 300,
    .y = 0
};

const V2i SPAWN_COORDS_J2 = {
    .x = 500,
    .y = 0
};
int main(){
    
    Player player1 = create_player("J1",RYU,SPAWN_COORDS_J1);
    while(1){
        char pressed = getchar(); 
        if(pressed == '\n'){continue;}
        Input input = get_pressed_input(pressed);
        add_input(&player1.ring,input);
        int id_coup_special = find_coup_special(player1.ring,player1.character.moveset);
        if(id_coup_special > -1){
            printf("%s\n",player1.character.moveset.array[id_coup_special].name);
        }
    }
    return 0;
}
