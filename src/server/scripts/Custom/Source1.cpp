#include "ScriptPCH.h"

enum Spells
{
	/*SPELL_LIGHTNINGBOLT = 403,
	SPELL_HEALINGWAVE = 331,
	SPELL_FLAMESHOCK = 8050,*/

	// Moonkin form
	SPELL_WRATH = 67952,
	SPELL_MOONFIRE = 67946,
	SPELL_STARFIRE = 67949,
	SPELL_CYCLONE = 61662,
	SPELL_MOONKIN = 24858,


	// Tree form
	SPELL_TREE = 33891,
	SPELL_LIFEBLOOM = 67959,
	SPELL_LIFEBLOOM_FINAL = 67956,
	SPELL_REGROWTH = 67968,
	SPELL_REJUVENATION = 67971,
	SPELL_HEALINGTOUCH = 69899,

	// Cat form
	SPELL_CATFORM = 768,
	/*SPELL_RAKE = ,
	SPELL_CATMANGLE = ,
	SPELL_RIP = ,

	// Bear form
	SPELL_MAUL = ,
	SPELL_BEARMANGLE = ,
	SPELL_LACERATE = ,

	// Buffs
	SPELL_THRONS = ,
	SPELL_GOTW = ,*/
	SPELL_INNERVATE = 29166,
	SPELL_BARKSKIN = 65860,
	/*SPELL_INSTINCTS = ,
	SPELL_FURY = ,
	SPELL_FRENZY = ,*/
	SPELL_STARFALL = 26540

	// Spawn
	/*ID_TREE = ,
	SPELL_HURRICANE = */
};

enum Cooldowns
{
	cd_wrath = 1200,
	cd_moonfire = 1200,
	cd_starfire = 2700,
	cd_cyclone = 2700,
	cd_starfall = 1200,

	cd_regrowth = 2000,
	cd_healingtouch = 2500,
	tick_lifebloom = 7000
};

class example : public CreatureScript
{
public:
	example() : CreatureScript("example") { }

	struct exampleAI : public BossAI
	{
		exampleAI(Creature* creature) : BossAI(creature, 0)
		{
		}

		uint32 Phase;
		bool beTree;

		void Reset() override
		{
			Phase = 1;
			beTree = false;
			me->SetMaxHealth(8000000);
			me->SetHealth(8000000);
			me->SetMaxPower(POWER_MANA, 500000);
			me->SetPower(POWER_MANA, 500000);
			me->SetDisplayId(3300);
			me->SetObjectScale(2);
			events.Reset();
			me->RemoveAllAuras();
			_Reset();
		}

		void EnterCombat(Unit* /*who*/) override
		{
			events.ScheduleEvent(17, 100, 1);
			events.ScheduleEvent(16, 500, 1);
		}

		void KilledUnit(Unit * /*victim*/) override
		{
		}

		void JustDied(Unit * /*victim*/) override
		{
			me->RemoveAllAuras();
		}

		void UpdateAI(uint32 diff) override
		{
			if (!UpdateVictim())
			return;

			events.Update(diff);

			if (me->HasUnitState(UNIT_STATE_CASTING))
				return;
		
			// Setting the Phases

			if (HealthBelowPct(67) && Phase == 1 && beTree == false)
			{
				events.CancelEventGroup(1);
				me->CastStop();
				Phase = 2;
				beTree = true;
				events.ScheduleEvent(1, 1000, 4);
			}
			
			if (HealthBelowPct(34) && Phase == 2 && beTree == false)
			{
				events.CancelEventGroup(2);
				me->CastStop();
				Phase = 3;
				beTree = true;
				events.ScheduleEvent(1, 1000, 4);
			}

			/****************************************************************************/
			/************************** MOONKIN FORM (Phase 1) **************************/
			/****************************************************************************/

			if (Phase == 1)
			{
				while (uint32 eventId = events.ExecuteEvent())
				{
					switch (eventId)
					{
					case 17:
						DoCast(me, SPELL_MOONKIN);
						me->SetObjectScale(2);
						events.ScheduleEvent(1, 1000, 1);
						break;
					case 1:
						DoCastVictim(SPELL_WRATH);
						events.ScheduleEvent(2, cd_wrath, 1);
						me->MonsterYell("Case 1", LANG_UNIVERSAL, NULL);
						break;
					case 2:
						DoCastVictim(SPELL_WRATH);
						events.ScheduleEvent(3, cd_wrath, 1);
						me->MonsterYell("Case 2", LANG_UNIVERSAL, NULL);
						break;
					case 3:
						if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
						{
							DoCast(pTarget, SPELL_MOONFIRE);
							me->MonsterYell("Case 3", LANG_UNIVERSAL, NULL);
						}
						else
							me->MonsterYell("Skipping Case 3", LANG_UNIVERSAL, NULL);
						events.ScheduleEvent(4, cd_moonfire, 1);
						break;
					case 4:
						DoCastVictim(SPELL_WRATH);
						events.ScheduleEvent(5, cd_wrath, 1);
						me->MonsterYell("Case 4", LANG_UNIVERSAL, NULL);
						break;
					case 5:
						DoCastVictim(SPELL_WRATH);
						events.ScheduleEvent(6, cd_wrath, 1);
						me->MonsterYell("Case 5", LANG_UNIVERSAL, NULL);
						break;
					case 6:
						DoCastVictim(SPELL_WRATH);
						events.ScheduleEvent(7, cd_wrath, 1);
						me->MonsterYell("Case 6", LANG_UNIVERSAL, NULL);
						break;
					case 7:
						if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 6))
						{
							Player *pPlayer = pTarget->ToPlayer();
							if ((pPlayer->getClass() == CLASS_DRUID || pPlayer->getClass() == CLASS_PALADIN || pPlayer->getClass() == CLASS_PRIEST || pPlayer->getClass() == CLASS_SHAMAN) && pTarget != me->GetVictim())
							{
								DoCast(pTarget, SPELL_CYCLONE);
								events.ScheduleEvent(8, cd_cyclone, 1);
								me->MonsterYell("Case 7", LANG_UNIVERSAL, NULL);
							}
							else
								events.ScheduleEvent(7, 100, 1);
						}
						else
						{
							me->MonsterYell("Skipping Case 7", LANG_UNIVERSAL, NULL);
							events.ScheduleEvent(8, 1000, 1);
						}
						break;
					case 8:
						DoCastVictim(SPELL_WRATH);
						events.ScheduleEvent(9, cd_wrath, 1);
						me->MonsterYell("Case 8", LANG_UNIVERSAL, NULL);
						break;
					case 9:
						if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
						{
							DoCast(pTarget, SPELL_STARFALL);
							me->MonsterYell("Case 9", LANG_UNIVERSAL, NULL);
						}
						else
							me->MonsterYell("Skipping Case 9", LANG_UNIVERSAL, NULL);
						events.ScheduleEvent(10, cd_starfall, 1);
						break;
					case 10:
						DoCastVictim(SPELL_WRATH);
						events.ScheduleEvent(11, cd_wrath, 1);
						me->MonsterYell("Case 10", LANG_UNIVERSAL, NULL);
						break;
					case 11:
						if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
						{
							DoCast(pTarget, SPELL_MOONFIRE);
							me->MonsterYell("Case 11", LANG_UNIVERSAL, NULL);
						}
						else
							me->MonsterYell("Skipping Case 11", LANG_UNIVERSAL, NULL);
						events.ScheduleEvent(12, cd_moonfire, 1);
						break;
					case 12:
						DoCastVictim(SPELL_WRATH);
						me->MonsterYell("Case 12", LANG_UNIVERSAL, NULL);
						events.ScheduleEvent(13, cd_wrath, 1);
						break;
					case 13:
						if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
						{
							DoCast(pTarget, SPELL_STARFIRE);
							me->MonsterYell("Case 13", LANG_UNIVERSAL, NULL);
						}
						else
							me->MonsterYell("Skipping Case 13", LANG_UNIVERSAL, NULL);
						events.ScheduleEvent(14, cd_starfire, 1);
						break;
					case 14:
						DoCastVictim(SPELL_WRATH);
						events.ScheduleEvent(15, cd_wrath, 1);
						me->MonsterYell("Case 14", LANG_UNIVERSAL, NULL);
						break;
					case 15:
						if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 6))
						{
							Player *pPlayer = pTarget->ToPlayer();
							if ((pPlayer->getClass() == CLASS_DRUID || pPlayer->getClass() == CLASS_PALADIN || pPlayer->getClass() == CLASS_PRIEST || pPlayer->getClass() == CLASS_SHAMAN) && pTarget != me->GetVictim())
							{
								DoCast(pTarget, SPELL_CYCLONE);
								events.ScheduleEvent(1, cd_cyclone, 1);
								me->MonsterYell("Case 15", LANG_UNIVERSAL, NULL);
							}
							else
								events.ScheduleEvent(15, 100, 1);
						}
						else
						{
							me->MonsterYell("Skipping Case 15", LANG_UNIVERSAL, NULL);
							events.ScheduleEvent(1, 1000, 1);
						}
						me->MonsterYell("Resetting", LANG_UNIVERSAL, NULL);
						break;
					case 16:
						if (me->GetPower(POWER_MANA) * 100 / me->GetMaxPower(POWER_MANA) < 80)
						{
							DoCast(me, SPELL_INNERVATE, true);
							events.ScheduleEvent(16, 12000, 1);
						}
						else
							events.ScheduleEvent(16, 100, 1);
					default:
						break;
					}
				}
			}


			/****************************************************************************/
			/********************* TREE FORM (Phase 2,3 Beginnings) *********************/
			/****************************************************************************/

			if (beTree == true)
			{
				while (uint32 eventId = events.ExecuteEvent())
				{
					switch (eventId)
					{
					case 1:
						//events.Reset();
						DoCast(me, SPELL_TREE);
						me->SetObjectScale(2);
						events.ScheduleEvent(2, 1000, 4);
						events.ScheduleEvent(10, 17000, 4);
						break;
					case 2:
						DoCast(me, SPELL_LIFEBLOOM);
						events.ScheduleEvent(3, 1000, 4);
						break;
					case 3:
						DoCast(me, SPELL_LIFEBLOOM);
						events.ScheduleEvent(4, 1000, 4);
						break;
					case 4:
						DoCast(me, SPELL_LIFEBLOOM);
						events.ScheduleEvent(5, 1000, 4);
						events.ScheduleEvent(9, tick_lifebloom, 4);
						break;
					case 5:
						DoCast(me, SPELL_REJUVENATION);
						events.ScheduleEvent(6, 1000, 4);
						break;
					case 6:
						DoCast(me, SPELL_BARKSKIN);
						events.ScheduleEvent(7, 1000, 4);
						break;
					case 7:
						DoCast(me, SPELL_REGROWTH);
						events.ScheduleEvent(8, cd_regrowth, 4);
						break;
					case 8:
						DoCast(me, SPELL_HEALINGTOUCH);
						me->SetHealth(me->GetHealth() + me->GetMaxHealth() * 0.04);
						events.ScheduleEvent(8, cd_healingtouch, 4);
						break;
					case 9: //Lifebloom Final
						DoCast(me, SPELL_LIFEBLOOM_FINAL, true);
						DoCast(me, SPELL_LIFEBLOOM_FINAL, true);
						DoCast(me, SPELL_LIFEBLOOM_FINAL, true);
						break;
					case 10:
						events.CancelEventGroup(4);
						me->CastStop();
						beTree = false;
						events.ScheduleEvent(1, 1000, Phase);
						break;
					default:
						break;
					}
				}
			}

			if (Phase == 2 && beTree == false)
			{
				while (uint32 eventId = events.ExecuteEvent())
				{
					switch (eventId)
					{
					case 1:
						DoCast(me, SPELL_CATFORM);
						me->SetObjectScale(2);
						me->MonsterYell("Script Finished!", LANG_UNIVERSAL, NULL);
						break;
					
					default:
						break;
					}
				}
			}

			DoMeleeAttackIfReady();
		}
	};

	CreatureAI* GetAI(Creature* creature) const override
	{
		return new exampleAI(creature);
	}
};

void AddSC_example()
{
	new example();
}