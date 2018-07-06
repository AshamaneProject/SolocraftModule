#include "ScriptMgr.h"
#include "Player.h"
#include "Map.h"
#include "Group.h"

class solocraft_player_instance_handler : public PlayerScript
{
public:
	solocraft_player_instance_handler() : PlayerScript("solocraft_player_instance_handler") { }

	void OnMapChanged(Player* player) override
    {
		Map* map = player->GetMap();
		int difficulty = CalculateDifficulty(map, player);
		int numInGroup = GetNumInGroup(player);
		ApplyBuffs(player, map, difficulty, numInGroup);
	}

private:
	int CalculateDifficulty(Map *map, Player *player)
    {
		int difficulty = 1;
		if (map)
        {
			if (map->Is25ManRaid())
            {
				difficulty = 25;
			}
            else if (map->IsMythic())
            {
				difficulty = 20;
			}
            else if (map->IsHeroic())
            {
				difficulty = 10;
			}
            else if (map->IsRaid())
            {
				difficulty = 40;
			}
            else if (map->IsDungeon())
            {
				difficulty = 5;
			}
		}

		return difficulty;
	}

	int GetNumInGroup(Player *player)
    {
		int numInGroup = 1;

		if (Group* group = player->GetGroup())
            numInGroup = group->GetMemberSlots().size();

		return numInGroup;
	}

	void ApplyBuffs(Player* player, Map* map, int difficulty, int numInGroup)
    {
		ClearBuffs(player, map);
		if (difficulty > 1)
        {
            player->Variables.Set("SoloCraftDifficulty", difficulty);

			for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i)
				player->HandleStatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_PCT, float(difficulty * 100), true);

			player->SetFullHealth();

			if (player->GetPowerType() == POWER_MANA)
				player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));
		}
	}

	void ClearBuffs(Player* player, Map* map)
    {
        if (player->Variables.Exist("SoloCraftDifficulty"))
        {
			int difficulty = player->Variables.GetValue<int>("SoloCraftDifficulty", 1);
			for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i)
				player->HandleStatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_PCT, float(difficulty * 100), false);
        }
	}
};

void AddSC_solocraft()
{
	new solocraft_player_instance_handler();
}
