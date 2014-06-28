#include "ScriptPCH.h"

enum Spells
{
	SPELL_WRATH = 67952,
	SPELL_MOONFIRE = 67946,
	SPELL_STARFIRE = 67949,
	SPELL_CYCLONE = 61662,
	SPELL_MOONKIN = 24858,

	SPELL_INNERVATE = 29166,
	SPELL_STARFALL = 26540
};

enum Events
{
};

class boss_morthrox : public CreatureScript
{
public:
	boss_morthrox() : CreatureScript("boss_morthrox") { }

	struct boss_morthroxAI : public BossAI
	{
		boss_morthroxAI(Creature* creature) : BossAI(creature, 0)
		{
		}

		uint32 Phase;
		bool beTree;


		void Reset() override
		{
			_Reset();
		}

		void EnterCombat(Unit* /*who*/) override
		{
		}

		void KilledUnit(Unit * /*victim*/) override
		{
		}

		void JustDied(Unit * /*victim*/) override
		{
		}

		void UpdateAI(uint32 diff) override
		{
			if (!UpdateVictim())
			return;

			events.Update(diff);

			if (me->HasUnitState(UNIT_STATE_CASTING))
				return;

			while (uint32 eventId = events.ExecuteEvent())
			{
				switch (eventId)
				{
				case 1:
					break;
				default:
					break;
				}
			}

			DoMeleeAttackIfReady();
		}
	};

	CreatureAI* GetAI(Creature* creature) const override
	{
		return new boss_morthroxAI(creature);
	}
};

void AddSC_boss_morthrox()
{
	new boss_morthrox();
}