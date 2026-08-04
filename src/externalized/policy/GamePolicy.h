#pragma once

#include <stdint.h>

enum UIMode
{
	UI_Tactical,
	UI_Map
};

enum HotkeyModifier
{
	HKMOD_None,
	HKMOD_CTRL,
	HKMOD_SHIFT,
	HKMOD_ALT,
	HKMOD_CTRL_SHIFT
};

#define gamepolicy(element) (GCM->getGamePolicy()->element)

class GamePolicy
{
public:
	/** Check if a hotkey is enabled. */
	virtual bool isHotkeyEnabled(UIMode mode, HotkeyModifier modifier, uint32_t key) const = 0;
	// this could be defaulted in C++11
	virtual ~GamePolicy() {}

	bool f_draw_item_shadow;              /**< Draw shadows from the inventory items. */

	int32_t target_fps;
	double game_durations_multiplier;

	int32_t starting_cash_easy;
	int32_t starting_cash_medium;
	int32_t starting_cash_hard;

	uint16_t squad_size;

	/* Battle */
	bool f_drop_everything;               /**< Enemy drop all equipment. */
	bool f_all_dropped_visible;           /**< All dropped equipment is visible right away. */

	bool hide_bullets;                    // don't draw bullets in flight; resolve shots faster (revives the old TOPTION_HIDE_BULLETS option)

	bool prone_random_hit_location;       // prone targets can be hit in head/torso/legs (5/80/15) instead of always the torso

	bool multiple_interrupts;             // can interrupt more than once per turn

	bool allow_overwatch_interrupt;       // a player merc who already sees an enemy that does not see him gets an interrupt duel the moment that enemy spots him (the merc enters the enemy's opplist as SEEN_CURRENTLY). Independent of the enemy's alert status

	bool fixed_cost_to_shoot;    // Changes the formula for APs to shoot

	int8_t enemy_weapon_minimal_status;   /**< Minimal status of the enemy weapon (0 - 100). */

	int8_t gun_jam_chance_minimum;        // Minimum % chance for a gun to jam even in perfect condition (0 = vanilla, never jams above status 80)

	bool gui_extras;                      /* graphical user interface cosmetic mod */
	bool hide_terrorist_names;            // don't reveal a terrorist's name on the tactical selection UI
	bool informative_tooltips;            /* Reveal modifiers in hover boxes */
	bool extra_attachments;               // allow more item attachments options
	bool skip_sleep_explanation;          // skip annoying popups

	bool middle_mouse_look;               // Look cursor with middle mouse button
	bool extra_mousewheel_actions;        // Bind tactical actions to the mouse wheel; currently raising and lowering aim in turn-based confirm-action mode
	bool can_enter_turnbased;             // 'd' can start turnbased if in real-time

	bool ai_better_aiming_choice;         // decide where to shoot depending on to-hit probability if random choice is being made
	bool ai_go_prone_more_often;          // especially when already facing the right direction
	int8_t threshold_cth_head;            // threshold AI always take head shots, increase game difficulty
	int8_t threshold_cth_legs;            // threshold AI switch to leg shots from torso

	bool avoid_ambushes;                  // AI able to recognize and avoid ambushes on seeing friendlies' corpses
	bool stay_on_rooftop;                 // AI on guard on rooftop are disallowed to go down
	bool ai_avoid_lit_tiles_at_night;     // at night, generic enemy soldiers won't path across tiles that are lit AND visible to a player merc
	int8_t ai_night_swat_chance;          // at night, % chance a generic enemy that would RUN instead SWATs (0 = vanilla always-run, 100 = always)

	bool ai_always_has_keys;              // AI-controlled soldiers can open any locked door, so locks don't shut them out of buildings; lock and trap are left intact (false = vanilla)

	int8_t ai_cover_building_bonus;       // % bonus to cover value for tiles inside a building, biasing AI to hide indoors (0 = vanilla)
	int8_t ai_cover_search_wisdom;        // treat AI as having at least this Wisdom when sizing the cover search radius (0 = use actual Wisdom)
	int8_t ai_cover_search_turns;         // how many turns of movement the AI may spend reaching cover; lets it head for distant cover over several turns (1 = vanilla, this-turn only)

	int8_t enemy_elite_minimum_level;     // increase challenge: minimum experience level for enemy elite soldier
	int8_t enemy_elite_maximum_level;     // maximum experience level for enemy elite soldier

	bool imp_load_saved_merc_by_nickname; // IMP merc is saved and can be loaded at IMP creation if has same nickname
	bool imp_load_keep_inventory;         // IMP merc gets inventory from last save game
	bool pablo_wont_steal;                // Packages not stolen
	bool reinforce_all_sam_sites;         // capturing any SAM reinforces all three town SAMs; their all-elite reinforcement groups sneak in at night without sleeping

	float critical_damage_head_multiplier;//Head damage multiplier. Vanilla 1.5
	float critical_damage_legs_multiplier;//Legs damage multiplier. Vanilla 0.5
	int8_t chance_to_hit_minimum;         //Minimum chance to hit (0 - chance_to_hit_maximum) vanilla 1
	int8_t chance_to_hit_maximum;         //Maximum chance to hit (chance_to_hit_minimum - 100) vanilla 99

	bool burst_penalty_after_cth_cap;     // subtract the per-shot burst penalty after clamping chance-to-hit instead of before, so it is not swallowed by the cap/floor (false = vanilla)

	bool nonlinear_range_modifier;        // curve the chance-to-hit range modifier instead of a flat 3%/tile line: bigger bonus up close, accelerating penalty far out (false = vanilla)
	int16_t range_bonus_point_blank;      // chance-to-hit bonus at range 0, tapering to 0 at NORMAL_RANGE along a concave curve
	int16_t range_penalty_far_linear;     // per-tile chance-to-hit penalty beyond NORMAL_RANGE, in tenths of a percent
	int16_t range_penalty_far_quadratic;  // accelerating part of the penalty beyond NORMAL_RANGE, in tenths of a percent; scaled by (tiles beyond)^2 / NORMAL_RANGE in tiles

	int8_t aim_bonus_per_std_ap;          // Aim bonus % for first 4 AP (aim clicks) spent
	int8_t aim_bonus_sniperscope;         // Flat bonus after at suitable range
	int8_t aim_bonus_laserscope;          // Aim bonus in the dark
	int16_t range_penalty_silencer;        // Absolute penalty to range from silencer
	int16_t range_bonus_barrel_extender;   // Aim bonus from extender

	int16_t thrown_range_modifier;        // % modifier to the maximum range of hand-thrown items (100 = vanilla); does not affect launchers or unaerodynamic items

	bool always_show_cursor_in_tactical;  // Always show mouse cursor during tactical view (if false, no mourse cursor is shown when moving in real-time mode, selecting a merc, etc)
	bool show_hit_chance;                 // Show chance-to-hit when pressing 'F' and next to mouse cursor when preparing an attack

	float website_loading_time_scale;     // Scales the loading time of websites on the laptop. Lower value means faster loading. Setting this to 0.0 removes the loading entirely.

	bool diagonally_interactable_doors;   // Open doors without exposing your mercs too much. Also affects switches.

	bool locksmith_kit_wear;              // botched lock picks wear the locksmith kit down, and the merc's spare kits are pooled into the one being used (false = vanilla, kits never wear out)

	/* IMP */
	int8_t imp_attribute_max;             // IMP character attribute maximum 0 to 100, vanilla 85
	int8_t imp_attribute_min;             // IMP character attribute minimum 0 to imp_attribute_max, vanilla 35
	int32_t imp_attribute_bonus;          // IMP character attribute unallocated bonus points, vanilla 40
	int32_t imp_attribute_zero_bonus;     // IMP character attribute points given instead of imp_attribute_min, vanilla 15
	bool imp_pick_skills_directly;        // Use the IMP_SkillTrait selection screen from JA2.5, skipping the personality quiz, vanilla falase

	/* M.E.R.C. */
	uint8_t merc_online_min_days;         // The earliest day on or after which M.E.R.C. goes online
	uint8_t merc_online_max_days;         // The latest day on or before which M.E.R.C. goes online

	// Difficulty / Campaign Progress
	float progress_weight_kills;         // Weight of kill count on campaign progress
	float progress_weight_control;       // Weight of area control on campaign progress
	float progress_weight_income;        // Weight of income on campaign progress
	int8_t kills_per_point_easy;             // Kills per point for difficulty Easy
	int8_t kills_per_point_medium;             // Kills per point for difficulty Medium
	int8_t kills_per_point_hard;             // Kills per point for difficulty Hard
	int8_t progress_event_madlab_min;     // Minimum first progress to trigger event Quest Madlab
	int8_t progress_event_mike_min;       // Minimum first progress to trigger event Mike
	int8_t progress_event_iggy_min;       // Minimum first progress to trigger event Iggy

	int8_t unhired_merc_deaths_easy;       // Maximum unhired mercs KIA difficulty Easy
	int8_t unhired_merc_deaths_medium;       // Maximum unhired mercs KIA difficulty Medium
	int8_t unhired_merc_deaths_hard;       // Maximum unhired mercs KIA difficulty Hard

	bool enable_stat_healing;		// Enable ability to heal stats with doctoring

	bool progressive_weight_penalties;	// true: carried weight penalizes APs/agility/breath/strategic movement from any load, scaling non-linearly. false: vanilla penalties (only above 100% capacity).

	bool quest_experience_all_mercs;      // true: quest experience rewards go to every merc on the team. false: vanilla, only the squad present when the quest is closed

	int8_t auto_sleep_breath_threshold;   // max breath below which a merc who is not on a squad or in a vehicle turns in on his own during the hourly update (vanilla 50)

	uint8_t enemy_autoresolve_retreat_health_percent; // Health (in percent of maximum) below which enemy soldiers flee an autoresolve battle; 0 disables enemy retreating

	bool skyrider_explores_sectors;       // true: Skyrider maps every sector the helicopter flies over, revealing it on the strategic map. false: vanilla, only sectors where mercs actually set foot count as explored.

	uint16_t start_sector;        // Starting sector
	bool reveal_start_sector;     // Should the start sector radar map be shown at start

	uint8_t suppression_fire_modifier; // Scales AP loss from suppression (numerator of the AP-loss formula); vanilla 6, 0 disables AP loss
	uint16_t suppression_fire_reaction_threshold; // Numerator of the stance-reaction threshold; vanilla 130, 0 = always react (1.13 behaviour), higher = more resistant

	////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////
};
