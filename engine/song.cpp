#include "song.h"


void Song::_check_delete_pattern_config(int p_pattern) {

	if (!pattern_config.has(p_pattern))
		return;
	if (pattern_config[p_pattern].beats_per_bar==DEFAULT_BEATS_PER_BAR && pattern_config[p_pattern].bars==DEFAULT_PATTERN_BARS ) {

		pattern_config.erase(p_pattern);
	}
}

void Song::pattern_set_beats_per_bar(int p_pattern, int p_beats_per_bar) {

	_AUDIO_LOCK_

	if (!pattern_config.has(p_pattern))
		pattern_config[p_pattern]=PatternConfig();


	pattern_config[p_pattern].beats_per_bar=p_beats_per_bar;

	_check_delete_pattern_config(p_pattern);
}

int Song::pattern_get_beats_per_bar(int p_pattern) const {

	if (!pattern_config.has(p_pattern))
		return DEFAULT_BEATS_PER_BAR;

	return pattern_config[p_pattern].beats_per_bar;

}
void Song::pattern_set_bars(int p_pattern, int p_bars) {

	_AUDIO_LOCK_

	if (!pattern_config.has(p_pattern))
		pattern_config[p_pattern]=PatternConfig();


	pattern_config[p_pattern].bars=p_bars;

	_check_delete_pattern_config(p_pattern);
}
int Song::pattern_get_bars(int p_pattern) const {

	if (!pattern_config.has(p_pattern))
		return DEFAULT_PATTERN_BARS;

	return pattern_config[p_pattern].bars;

}

void Song::order_set(int p_order, int p_pattern) {

	_AUDIO_LOCK_

	ERR_FAIL_COND(p_order<0 || (p_order>ORDER_MAX && p_order!=ORDER_EMPTY && p_order!=ORDER_SKIP));

	if (p_order==ORDER_EMPTY)
		order_list.erase(p_order);
	else
		order_list.insert(p_order,p_pattern);	
}
int Song::order_get(int p_order) const {

	if (order_list.has(p_order))
		return order_list[p_order];
	else
		return ORDER_EMPTY;
}

int Song::get_track_count() const {

	return tracks.size();
}
void Song::add_track(Track *p_track,int p_pos) {

	_AUDIO_LOCK_

	if (p_pos<0)
		p_pos=tracks.size();
	ERR_FAIL_COND(p_pos>tracks.size());
	tracks.insert(p_pos,p_track);
}
Track *Song::get_track(int p_pos) {

	ERR_FAIL_INDEX_V(p_pos,tracks.size(),NULL);
	return tracks[p_pos];
}
void Song::remove_track(int p_pos) {

	_AUDIO_LOCK_

	ERR_FAIL_INDEX(p_pos,tracks.size());
	tracks.remove(p_pos);

}

Song::~Song() {

	for(int i=0;i<tracks.size();i++)
		delete tracks[i];
}

Song::Song()
{
}
