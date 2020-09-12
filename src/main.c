#include <gb/gb.h>
#include <stdio.h>
#include "../sprites/link.c"
#include "GameCharacter.c"

struct GameCharacter link;
UBYTE spriteSize = 8;

void moveCharacter(struct GameCharacter *character, UINT8 x, UINT8 y)
{
    move_sprite(character->spriteIds[0], x, y);
    move_sprite(character->spriteIds[1], x + spriteSize, y);
    move_sprite(character->spriteIds[2], x, y + spriteSize);
    move_sprite(character->spriteIds[3], x + spriteSize, y + spriteSize);
}

void setupLink()
{
    link.x = 80;
    link.y = 130;
    link.width = 16;
    link.height = 16;

    set_sprite_tile(0, 0);
    link.spriteIds[0] = 0;
    set_sprite_tile(1, 1);
    link.spriteIds[1] = 1;
    set_sprite_tile(2, 2);
    link.spriteIds[2] = 2;
    set_sprite_tile(3, 3);
    link.spriteIds[3] = 3;

    moveCharacter(&link, link.x, link.y);
}

void performantDelay(UINT8 numloops)
{
    UINT8 i;
    for (i = 0; i < numloops; i++)
    {
        wait_vbl_done();
    }
}

void main() {
    set_sprite_data(0, 4, LinkSpriteData);
    setupLink();

    SHOW_SPRITES;
    DISPLAY_ON;
}
