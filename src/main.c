#include <gb/gb.h>
#include <stdio.h>
#include "../sprites/link.c"
#include "GameCharacter.c"

struct GameCharacter link;
UBYTE spriteSize = 8;
UBYTE flipWalkCycle;
UBYTE spriteMirrored;
UBYTE movementSpeed = 5;

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

void sleep(UINT8 numloops)
{
    UINT8 i;
    for (i = 0; i < numloops; i++)
    {
        wait_vbl_done();
    }
}

void mirrorSprite()
{
    if (!spriteMirrored)
    {
        spriteMirrored = 1;
        set_sprite_prop(0, S_FLIPX);
        set_sprite_prop(1, S_FLIPX);
        set_sprite_prop(2, S_FLIPX);
        set_sprite_prop(3, S_FLIPX);
    }
}

void unmirrorSprite()
{
    if (spriteMirrored)
    {
        spriteMirrored = 0;
        set_sprite_prop(0, get_sprite_prop(0) & ~S_FLIPX);
        set_sprite_prop(1, get_sprite_prop(1) & ~S_FLIPX);
        set_sprite_prop(2, get_sprite_prop(2) & ~S_FLIPX);
        set_sprite_prop(3, get_sprite_prop(3) & ~S_FLIPX);
    }
}

void walkDown()
{
    unmirrorSprite();

    if (!flipWalkCycle)
    {
        flipWalkCycle = 1;
        set_sprite_tile(0, 4);
        set_sprite_tile(1, 5);
        set_sprite_tile(2, 6);
        set_sprite_tile(3, 7);
    }
    else
    {
        flipWalkCycle = 0;
        set_sprite_tile(0, 0);
        set_sprite_tile(1, 1);
        set_sprite_tile(2, 2);
        set_sprite_tile(3, 3);
    }
}

void walkUp()
{
    unmirrorSprite();

    if (!flipWalkCycle)
    {
        flipWalkCycle = 1;
        set_sprite_tile(0, 12);
        set_sprite_tile(1, 13);
        set_sprite_tile(2, 14);
        set_sprite_tile(3, 15);
    }
    else
    {
        flipWalkCycle = 0;
        set_sprite_tile(0, 8);
        set_sprite_tile(1, 9);
        set_sprite_tile(2, 10);
        set_sprite_tile(3, 11);
    }
}

void walkRight()
{
    unmirrorSprite();

    if (!flipWalkCycle)
    {
        flipWalkCycle = 1;
        set_sprite_tile(0, 20);
        set_sprite_tile(1, 21);
        set_sprite_tile(2, 22);
        set_sprite_tile(3, 23);
    }
    else
    {
        flipWalkCycle = 0;
        set_sprite_tile(0, 16);
        set_sprite_tile(1, 17);
        set_sprite_tile(2, 18);
        set_sprite_tile(3, 19);
    }
}

void walkLeft()
{
    mirrorSprite();

    if (!flipWalkCycle)
    {
        flipWalkCycle = 1;
        set_sprite_tile(0, 21);
        set_sprite_tile(1, 20);
        set_sprite_tile(2, 23);
        set_sprite_tile(3, 22);
    }
    else
    {
        flipWalkCycle = 0;
        set_sprite_tile(0, 17);
        set_sprite_tile(1, 16);
        set_sprite_tile(2, 19);
        set_sprite_tile(3, 18);
    }
}

void processInput()
{
    if (joypad() & J_UP)
    {
        walkUp();
        link.y -= movementSpeed;
        moveCharacter(&link, link.x, link.y);
    }
    if (joypad() & J_DOWN)
    {
        walkDown();
        link.y += movementSpeed;
        moveCharacter(&link, link.x, link.y);
    }
    if (joypad() & J_LEFT)
    {
        walkLeft();
        link.x -= movementSpeed;
        moveCharacter(&link, link.x, link.y);
    }
    if (joypad() & J_RIGHT)
    {
        walkRight();
        link.x += movementSpeed;
        moveCharacter(&link, link.x, link.y);
    }
}

void main()
{
    // sprite data contains 8 8x8 sprites
    // tiles 0-3 are of link facing down with his right foot forwards
    // tiles 4-7 are of link facing down with his left foot forwards
    // tiles 8-11 are of link facing up with his left foot forwards
    // tiles 12-15 are of link facing up with his right foot forwards
    // tiles 16-19 are of link facing right with his right foot forwards
    // tiles 20-23 are of link facing right with his left foot forwards
    set_sprite_data(0, 24, LinkSpriteData);
    setupLink();
    SHOW_SPRITES;
    DISPLAY_ON;

    while (1)
    {
        processInput();
        sleep(10);
    }
}
