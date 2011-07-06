#ifndef TRACK_H
#define TRACK_H


#include "vector.h"
#include "map.h"
#include "list.h"
#include "audio_effect.h"
#include "value_stream.h"
#include "audio_lock.h"


#define TICKS_PER_BEAT 192
typedef uint64_t Tick;


class Automation {
public:

	enum DisplayMode {
		DISPLAY_ROWS,
		DISPLAY_SMALL,
		DISPLAY_LARGE
	};

private:
	AudioEffect *owner;
	ControlPort *port;
	DisplayMode display_mode;
	bool visible;

	Map<int,ValueStream<Tick,float> > data;
public:

	void set_point(int p_pattern, Tick p_offset, float p_value);
	bool has_point(int p_pattern, Tick p_offset) const;
	float get_point(int p_pattern, Tick p_offset) const;
	void remove_point(int p_pattern, Tick p_offset);

	Tick get_point_tick_by_index(int p_pattern,int p_index) const;
	float get_point_by_index(int p_pattern,int p_index) const;
	int get_point_count(int p_pattern) const;
	void get_points_in_range(int p_pattern, Tick p_from, Tick p_to, int &r_from_idx, int& r_to_idx) const;

	ControlPort *get_control_port();
	AudioEffect *get_owner();


	bool is_visible() const;
	DisplayMode get_display_mode() const;

	Automation(ControlPort *p_port, AudioEffect *p_owner=NULL);

};

class Track : public AudioEffect {
public:
	struct Note {

		enum {
			EMPTY=0xFF,
			OFF=0xFE,
			MAX_VOLUME=99
		};

		uint8_t note;
		uint8_t volume;
		inline bool is_empty() const { return (note==EMPTY && volume==EMPTY); }
		bool operator==(Note p_note) const { return note==p_note.note && volume==p_note.volume; }

		Note() { note=EMPTY; volume=EMPTY; }
	};

	struct Pos {

		Tick tick;
		int column;

		bool operator<(const Pos& p_rval) const { return (tick==p_rval.tick)?(column<p_rval.column):(tick<p_rval.tick); }
		bool operator>(const Pos& p_rval) const { return (tick==p_rval.tick)?(column>p_rval.column):(tick>p_rval.tick); }
		bool operator==(const Pos& p_rval) const { return (tick==p_rval.tick) && (column==p_rval.column); }

		Pos() { tick=0; column=0; }
	};

	struct PosNote {
		Pos pos;
		Note note;
	};
private:

	Map<int, ValueStream<Pos, Note > > note_data;
	int note_columns;

	float swing;
	int swing_step;

	Vector<AudioEffect*> effects;
	Vector<Automation*> automations;

public:

	/* audio effects */

	int get_audio_effect_count() const;
	void add_audio_effect(AudioEffect *p_effect,int p_pos=-1);
	void remove_audio_effect(int p_pos);
	AudioEffect *get_audio_effect(int p_pos);

	/* automations */

	int get_automation_count() const;
	void add_automation(Automation *p_automation,int p_pos=-1);
	void remove_automation(int p_pos);
	Automation *get_automation(int p_pos);

	virtual int get_internal_automation_count() const=0;
	virtual Automation *internal_automation_get(int p_pos)=0;

	/* notes */

	void set_note_columns(int p_columns);
	int get_note_columns() const;

	void set_note(int p_pattern, Pos p_pos, Note p_note);
	Note get_note(int p_pattern,Pos p_pos) const;

	void get_notes_in_range(int p_pattern,const Pos& p_from,const Pos& p_to,List<PosNote> *r_notes ) const;

	void set_swing(float p_swing);
	float get_swing() const;

	void set_swing_step(int p_swing_step);
	int get_swing_step() const;


	Track();
};

#endif // TRACK_H