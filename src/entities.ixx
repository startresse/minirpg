module;

#include <inttypes.h>

#include <iostream> // debug
#include <vector>

export module entities;

namespace entities
{
    export
    {
        void init();
        void update();
    }

    using uint32 = uint32_t;

    struct Statblock
    {
        int m_health;
        int m_attack;
        int m_defense;
        Statblock(const Statblock& other) : Statblock(other.m_health, other.m_attack, other.m_defense) {}
        Statblock(int health, int attack, int defense) : m_health(health), m_attack(attack), m_defense(defense)
        {
            printf("Created StatBlock: %s\n", generate_printable());
        }
        std::string generate_printable() {
            // char printable[256];
            // sprintf(printable, "HP:%d, ATK:%d, DEF:%d", m_health, m_attack, m_defense);
            // return printable;
        }
    };

    std::vector<Statblock> statblock_array;
    class Character
    {
        std::vector<Statblock>::iterator m_statblock_it; // is this better than storing a unique_ptr?

    public:
        Character() = delete;
        Character(const Statblock& statblock) {
            for (auto& it : statblock_array)
            {
            }
        }
        ~Character()
        {
            statblock_array.erase(m_statblock_it);
        }

        int get_health() const { return m_statblock_it->m_health; }
    };

    void init()
    {
        printf("Init entities\n");

        Character chara0{ {100, 10, 10} };

        //StatBlock stats(150, 20, 20);
        //Character chara1(stats);

        printf("Character0 health: %d\n", chara0.get_health());
        //printf("Character1 health: %d\n", chara1.get_health());

        printf("\nStatnlockList:\n");
        for (const Statblock& statblock : statblock_array)
        {
            printf("HP: %d, ATK: %d, DEF: %d\n", statblock.m_health, statblock.m_attack, statblock.m_defense);
        }
        printf("\n");
    }
}
